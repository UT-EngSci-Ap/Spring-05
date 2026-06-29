#include "Playlist.h"

#include <utility>

#include "Song.h"

Playlist::Playlist(std::string name) : name_(std::move(name)) {}

void Playlist::addSong(Song* song) {
    if (song != nullptr) {
        songs_.push_back(song);
    }
}

void Playlist::clear() { songs_.clear(); }

const std::string& Playlist::name() const { return name_; }
const std::vector<Song*>& Playlist::songs() const { return songs_; }
std::size_t Playlist::size() const { return songs_.size(); }
bool Playlist::empty() const { return songs_.empty(); }

Song* Playlist::at(std::size_t index) const {
    if (index >= songs_.size()) {
        return nullptr;
    }
    return songs_[index];
}

int Playlist::indexOf(const Song* song) const {
    for (std::size_t i = 0; i < songs_.size(); ++i) {
        if (songs_[i] == song) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
