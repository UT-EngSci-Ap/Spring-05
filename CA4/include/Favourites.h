#pragma once

#include <set>
#include <string>
#include <vector>

class Song;
class MusicLibrary;

// Tracks the user's favourite songs (the ♥ system). Identity is the song's
// file path, so the set survives restarts and is independent of playlist order.
// Persisted as a plain list of paths (one per line), the same simple format as
// an .m3u file.
class Favourites {
public:
    explicit Favourites(std::string path);

    void load();
    void save() const;

    bool contains(const Song* song) const;
    void toggle(const Song* song);   // add if absent, remove if present

    // Favourite songs resolved against the library, returned in library order
    // for a stable, predictable listing.
    std::vector<Song*> resolve(const MusicLibrary& library) const;

    std::size_t size() const;

private:
    std::string path_;
    std::set<std::string> paths_;
};
