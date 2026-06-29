#include "PlaylistViewScreen.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <vector>

#include "InputHandler.h"
#include "Player.h"
#include "Playlist.h"
#include "Song.h"
#include "UIRenderer.h"

namespace {

std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

bool contains(const std::string& haystack, const std::string& needle) {
    return toLower(haystack).find(toLower(needle)) != std::string::npos;
}

}  // namespace

PlaylistViewScreen::PlaylistViewScreen(UIRenderer& ui, InputHandler& input,
                                       Player& player)
    : Screen(ui, input), player_(player) {}

void PlaylistViewScreen::resetView() {
    boundPlaylist_ = player_.playlist();
    base_.clear();
    if (boundPlaylist_ != nullptr) {
        base_ = boundPlaylist_->songs();
    }
    sortKey_ = SortKey::None;
    sortDescending_ = false;
    searchTerm_.clear();
    filterType_.clear();
    filterValue_.clear();
    rebuild();
    initialised_ = true;
}

void PlaylistViewScreen::ensureFresh() {
    // Re-enter from the menu, or the active playlist was switched.
    if (!initialised_ || boundPlaylist_ != player_.playlist()) {
        resetView();
    }
}

void PlaylistViewScreen::rebuild() {
    display_.clear();
    for (Song* s : base_) {
        if (!filterType_.empty()) {
            if (filterType_ == "Artist" && s->artist() != filterValue_) continue;
            if (filterType_ == "Album"  && s->album()  != filterValue_) continue;
        }
        if (!searchTerm_.empty()) {
            if (!contains(s->title(), searchTerm_) &&
                !contains(s->artist(), searchTerm_) &&
                !contains(s->album(), searchTerm_)) {
                continue;
            }
        }
        display_.push_back(s);
    }

    auto cmp = [this](Song* a, Song* b) {
        switch (sortKey_) {
            case SortKey::Title:    return toLower(a->title())  < toLower(b->title());
            case SortKey::Artist:   return toLower(a->artist()) < toLower(b->artist());
            case SortKey::Album:    return toLower(a->album())  < toLower(b->album());
            case SortKey::Year:     return a->year()     < b->year();
            case SortKey::Duration: return a->duration() < b->duration();
            case SortKey::None:     return false;
        }
        return false;
    };

    if (sortKey_ != SortKey::None) {
        std::stable_sort(display_.begin(), display_.end(), cmp);
        if (sortDescending_) {
            std::reverse(display_.begin(), display_.end());
        }
    }
}

std::string PlaylistViewScreen::statusText() const {
    std::ostringstream out;
    if (!searchTerm_.empty()) {
        out << "Search: \"" << searchTerm_ << "\"";
    } else if (!filterType_.empty()) {
        out << filterType_ << ": " << filterValue_;
    } else {
        const char* keys[] = {"-", "Title", "Artist", "Album", "Year", "Dur"};
        out << "Sort: " << keys[static_cast<int>(sortKey_)]
            << (sortKey_ == SortKey::None ? ""
                                          : (sortDescending_ ? " v" : " ^"));
    }
    return out.str();
}

void PlaylistViewScreen::render() {
    ensureFresh();

    std::string name = boundPlaylist_ ? boundPlaylist_->name() : "(none)";

    ui_.clearScreen();
    ui_.boxTop();
    {
        std::ostringstream header;
        header << "  " << name << "  (" << display_.size() << " songs)   "
               << statusText();
        ui_.boxCentered(header.str(), Color::White);
    }
    ui_.boxSep();
    ui_.boxLine("  #   Title                    Artist         Dur",
                Color::White);
    ui_.boxSep();

    Song* current = player_.currentSong();
    if (display_.empty()) {
        ui_.boxLine("  (no songs to show)", Color::Gray);
    } else {
        for (std::size_t i = 0; i < display_.size(); ++i) {
            Song* s = display_[i];
            bool isCurrent = (s == current);
            std::ostringstream row;
            row << "  " << std::left << std::setw(3) << (i + 1)
                << (isCurrent ? "▶ " : "  ")
                << std::setw(24) << s->title().substr(0, 23)
                << std::setw(15) << s->artist().substr(0, 14)
                << s->durationString();
            ui_.boxLine(row.str(), isCurrent ? Color::Yellow : Color::Gray);
        }
    }

    ui_.boxSep();
    ui_.boxLine("  [num] play  [s] sort  [f] filter  [/] search", Color::Dim);
    bool filtered = !searchTerm_.empty() || !filterType_.empty();
    ui_.boxLine(filtered ? "  [0] clear filter/search"
                         : "  [0] back to menu",
                Color::Dim);
    ui_.boxBottom();
}

void PlaylistViewScreen::doSort() {
    ui_.println("");
    ui_.println("  Sort by: 1.Title  2.Artist  3.Album  4.Year  5.Dur",
                Color::White);
    ui_.println("  Add + for descending (e.g. 4+ for Year desc)", Color::Dim);
    std::string in = input_.readLine("Sort choice: ");
    if (in.empty()) {
        return;
    }
    bool desc = in.find('+') != std::string::npos;
    switch (in[0]) {
        case '1': sortKey_ = SortKey::Title;    break;
        case '2': sortKey_ = SortKey::Artist;   break;
        case '3': sortKey_ = SortKey::Album;    break;
        case '4': sortKey_ = SortKey::Year;     break;
        case '5': sortKey_ = SortKey::Duration; break;
        default:  return;
    }
    sortDescending_ = desc;
    rebuild();
}

void PlaylistViewScreen::doSearch() {
    std::string term = input_.readLine("Search (empty to clear): ");
    searchTerm_ = term;          // empty term clears the search
    rebuild();
}

void PlaylistViewScreen::doFilter() {
    ui_.println("");
    ui_.println("  Filter by:  1.Artist  2.Album   [0] cancel", Color::White);
    std::string in = input_.readLine("Filter choice: ");
    if (in.empty() || in[0] == '0') {
        return;
    }
    std::string type;
    if (in[0] == '1') type = "Artist";
    else if (in[0] == '2') type = "Album";
    else return;

    // Build the distinct list of values present in the playlist.
    std::vector<std::string> values;
    for (Song* s : base_) {
        const std::string& v = (type == "Artist") ? s->artist() : s->album();
        if (std::find(values.begin(), values.end(), v) == values.end()) {
            values.push_back(v);
        }
    }
    std::sort(values.begin(), values.end());

    ui_.println("");
    ui_.println("  " + type + "s in " +
                    (boundPlaylist_ ? boundPlaylist_->name() : ""),
                Color::White);
    for (std::size_t i = 0; i < values.size(); ++i) {
        int count = 0;
        for (Song* s : base_) {
            const std::string& v = (type == "Artist") ? s->artist() : s->album();
            if (v == values[i]) ++count;
        }
        std::ostringstream line;
        line << "  " << (i + 1) << ".  " << values[i] << "  (" << count
             << " songs)";
        ui_.println(line.str(), Color::Gray);
    }
    ui_.println("  0.  Back", Color::Dim);

    int choice = input_.readInt("Choice: ", 0,
                                static_cast<int>(values.size()));
    if (choice == 0) {
        return;
    }
    filterType_ = type;
    filterValue_ = values[static_cast<std::size_t>(choice - 1)];
    searchTerm_.clear();   // a filter replaces any active search
    rebuild();
}

ScreenId PlaylistViewScreen::handleInput() {
    std::string cmd = input_.readLine("Choice: ");
    if (cmd.empty()) {
        return ScreenId::None;
    }

    if (cmd == "0") {
        if (!searchTerm_.empty() || !filterType_.empty()) {
            searchTerm_.clear();
            filterType_.clear();
            filterValue_.clear();
            rebuild();                 // back to the full playlist view
            return ScreenId::None;
        }
        initialised_ = false;          // reset next time we enter
        return ScreenId::MainMenu;
    }

    switch (cmd[0]) {
        case 's': doSort();   return ScreenId::None;
        case 'f': doFilter(); return ScreenId::None;
        case '/': doSearch(); return ScreenId::None;
        default: break;
    }

    // Otherwise interpret as a song number.
    try {
        std::size_t pos = 0;
        int n = std::stoi(cmd, &pos);
        if (pos == cmd.size() && n >= 1 &&
            n <= static_cast<int>(display_.size())) {
            Song* chosen = display_[static_cast<std::size_t>(n - 1)];
            player_.playSong(chosen);
            initialised_ = false;      // fresh view when we come back
            return ScreenId::NowPlaying;
        }
    } catch (const std::exception&) {
        // not a number; fall through
    }

    ui_.println("No song at this index.", Color::Red);
    return ScreenId::None;
}
