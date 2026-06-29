#pragma once

#include <cstddef>
#include <string>
#include <vector>

class Song;

// The catalogue of every Song. This is the ONLY owner of Song objects:
// it allocates nothing itself but takes ownership of heap-allocated Songs
// handed to it and deletes them all in its destructor.
class MusicLibrary {
public:
    MusicLibrary() = default;
    ~MusicLibrary();

    MusicLibrary(const MusicLibrary&) = delete;
    MusicLibrary& operator=(const MusicLibrary&) = delete;

    // Takes ownership of a heap-allocated Song.
    void addSong(Song* song);

    const std::vector<Song*>& songs() const;
    std::size_t size() const;

    Song* findByPath(const std::string& path) const;
    std::vector<Song*> filterByArtist(const std::string& artist) const;
    std::vector<Song*> filterByAlbum(const std::string& album) const;

private:
    std::vector<Song*> songs_;   // owning
};
