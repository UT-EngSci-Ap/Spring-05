#include "MusicLibrary.h"

#include "Song.h"

MusicLibrary::~MusicLibrary() {
    for (Song* song : songs_) {
        delete song;            // sole owner releases every Song here
    }
    songs_.clear();
}

void MusicLibrary::addSong(Song* song) {
    if (song != nullptr) {
        songs_.push_back(song);
    }
}

const std::vector<Song*>& MusicLibrary::songs() const { return songs_; }
std::size_t MusicLibrary::size() const { return songs_.size(); }

Song* MusicLibrary::findByPath(const std::string& path) const {
    for (Song* song : songs_) {
        if (song->filePath() == path) {
            return song;
        }
    }
    return nullptr;
}

std::vector<Song*> MusicLibrary::filterByArtist(const std::string& artist) const {
    std::vector<Song*> result;
    for (Song* song : songs_) {
        if (song->artist() == artist) {
            result.push_back(song);
        }
    }
    return result;
}

std::vector<Song*> MusicLibrary::filterByAlbum(const std::string& album) const {
    std::vector<Song*> result;
    for (Song* song : songs_) {
        if (song->album() == album) {
            result.push_back(song);
        }
    }
    return result;
}
