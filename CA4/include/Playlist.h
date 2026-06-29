#pragma once

#include <cstddef>
#include <string>
#include <vector>

class Song;

// An ordered, read-only collection of Song pointers.
// IMPORTANT: a Playlist does NOT own its songs and must never delete them.
// The single owner of every Song is MusicLibrary.
class Playlist {
public:
    explicit Playlist(std::string name);

    void addSong(Song* song);
    void clear();                                 // drops all entries (non-owning)

    const std::string& name() const;
    const std::vector<Song*>& songs() const;
    std::size_t size() const;
    bool empty() const;
    Song* at(std::size_t index) const;          // nullptr if out of range
    int indexOf(const Song* song) const;         // -1 if not present

private:
    std::string name_;
    std::vector<Song*> songs_;                    // non-owning
};
