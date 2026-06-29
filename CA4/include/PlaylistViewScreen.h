#pragma once

#include <string>
#include <vector>

#include "Screen.h"

class Player;
class Song;
class Playlist;

// Browse view for the active playlist. Supports playing a song, sorting,
// searching and filtering. Sort/search/filter affect ONLY this view and are
// reset whenever the screen is (re)entered or the active playlist changes.
class PlaylistViewScreen : public Screen {
public:
    PlaylistViewScreen(UIRenderer& ui, InputHandler& input, Player& player);

    void render() override;
    ScreenId handleInput() override;

private:
    enum class SortKey { None, Title, Artist, Album, Year, Duration };

    void resetView();
    void rebuild();             // recompute display list from base + filters
    void ensureFresh();         // reset if active playlist changed / first entry

    void doSort();
    void doSearch();
    void doFilter();

    std::string statusText() const;

    Player& player_;
    Playlist* boundPlaylist_ = nullptr;     // playlist the view was built from
    bool initialised_ = false;

    std::vector<Song*> base_;               // full active-playlist snapshot
    std::vector<Song*> display_;            // what is currently shown

    SortKey sortKey_ = SortKey::None;
    bool sortDescending_ = false;
    std::string searchTerm_;
    std::string filterType_;                // "Artist" / "Album" / ""
    std::string filterValue_;
};
