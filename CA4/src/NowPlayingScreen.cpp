#include "NowPlayingScreen.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <utility>

#include "Favourites.h"
#include "InputHandler.h"
#include "Player.h"
#include "Playlist.h"
#include "Song.h"
#include "UIRenderer.h"

namespace {

constexpr int kRefreshMs = 250;     // redraw cadence: keeps the timer live
constexpr int kSeekStep = 10;       // seconds per ←/→ press
constexpr int kBarWidth = 30;       // progress-bar character cells

std::string stateBadge(PlayerState state) {
    switch (state) {
        case PlayerState::PLAYING: return "▶  PLAYING";
        case PlayerState::PAUSED:  return "∥  PAUSED";
        case PlayerState::STOPPED: return "■  STOPPED";
    }
    return "STOPPED";
}

// Formats a non-negative number of seconds as MM:SS.
std::string mmss(int seconds) {
    if (seconds < 0) {
        seconds = 0;
    }
    std::ostringstream out;
    out << std::setw(2) << std::setfill('0') << (seconds / 60) << ':'
        << std::setw(2) << std::setfill('0') << (seconds % 60);
    return out.str();
}

// Renders a [====>     ] style bar filled to elapsed/total.
std::string progressBar(int elapsed, int total) {
    std::string bar = "[";
    int filled = 0;
    if (total > 0) {
        double frac = std::min(1.0, static_cast<double>(elapsed) / total);
        filled = static_cast<int>(frac * kBarWidth);
    }
    for (int i = 0; i < kBarWidth; ++i) {
        if (i < filled) {
            bar += '=';
        } else if (i == filled) {
            bar += '>';
        } else {
            bar += ' ';
        }
    }
    bar += ']';
    return bar;
}

}  // namespace

NowPlayingScreen::NowPlayingScreen(UIRenderer& ui, InputHandler& input,
                                   Player& player, Favourites& favourites,
                                   std::function<void()> onFavouritesChanged)
    : Screen(ui, input),
      player_(player),
      favourites_(favourites),
      onFavouritesChanged_(std::move(onFavouritesChanged)) {}

ScreenId NowPlayingScreen::run() {
    RawMode raw;                 // single-keypress input; restored on return
    ui_.clearScreen();           // one full clear; refreshes redraw in place
    ui_.hideCursor();

    ScreenId next = ScreenId::None;
    while (next == ScreenId::None) {
        render();
        int keyCode = input_.readKey(kRefreshMs);
        if (keyCode == key::None) {
            player_.tick();      // no key: just auto-advance if the song ended
            continue;
        }
        next = dispatchKey(keyCode);
    }

    ui_.showCursor();
    return next;
}

void NowPlayingScreen::render() {
    ui_.home();                  // redraw over the existing frame (no flicker)
    ui_.boxTop();
    ui_.boxCentered("♫  Terminal Music Player  ♫", Color::White);
    ui_.boxSep();
    ui_.boxLine("  Now Playing", Color::White);

    Song* song = player_.currentSong();
    if (song == nullptr) {
        ui_.boxLine("  Nothing loaded yet.", Color::Gray);
        ui_.boxSep();
        ui_.boxLine("  [q] back to menu", Color::Dim);
        ui_.boxBottom();
        ui_.flush();
        return;
    }

    bool fav = favourites_.contains(song);
    ui_.boxLine(std::string("  Title  : ") + (fav ? "♥ " : "") + song->title(),
                fav ? Color::Magenta : Color::Green);
    ui_.boxLine("  Artist : " + song->artist(), Color::Gray);

    std::ostringstream album;
    album << "  Album  : " << song->album() << "   [" << song->year() << "]";
    ui_.boxLine(album.str(), Color::Gray);
    ui_.boxLine("  Genre  : " + song->genre(), Color::Gray);
    ui_.boxSep();

    int elapsed = static_cast<int>(player_.cursorSeconds());
    int total = player_.totalSeconds();
    ui_.boxLine("  " + mmss(elapsed) + " / " + mmss(total) + "  " +
                    progressBar(elapsed, total),
                Color::Cyan);
    ui_.boxSep();

    std::string playlistName =
        player_.playlist() ? player_.playlist()->name() : "(none)";
    ui_.boxLine("  " + stateBadge(player_.state()) +
                    "      Playlist: " + playlistName,
                Color::Yellow);
    ui_.boxLine("  Mode: " + toString(player_.mode()), Color::Gray);
    ui_.boxSep();
    ui_.boxLine("  [space] play/pause   [n] next   [b] prev   [s] stop",
                Color::Dim);
    ui_.boxLine("  [←/→] seek 10s   [f] favourite ♥   [q] menu", Color::Dim);
    ui_.boxBottom();
    ui_.flush();
}

ScreenId NowPlayingScreen::dispatchKey(int keyCode) {
    switch (keyCode) {
        case ' ':
        case 'p':
            switch (player_.state()) {
                case PlayerState::PLAYING: player_.pause();  break;
                case PlayerState::PAUSED:  player_.resume(); break;
                case PlayerState::STOPPED: player_.play();   break;
            }
            break;
        case 'n': player_.next();             break;
        case 'b': player_.previous();         break;
        case 's': player_.stop();             break;
        case key::Left:  player_.seekBy(-kSeekStep); break;
        case key::Right: player_.seekBy(kSeekStep);  break;
        case 'f':
            favourites_.toggle(player_.currentSong());
            if (onFavouritesChanged_) {
                onFavouritesChanged_();       // persist + rebuild ♥ playlist
            }
            break;
        case 'q':
        case 'Q':
        case key::Esc:
            return ScreenId::MainMenu;
        default:
            break;                            // unknown key ignored
    }
    return ScreenId::None;
}

ScreenId NowPlayingScreen::handleInput() {
    // Provided to satisfy the Screen interface; the live loop in run() drives
    // input directly so it can refresh the timer between keypresses.
    return dispatchKey(input_.readKey(kRefreshMs));
}
