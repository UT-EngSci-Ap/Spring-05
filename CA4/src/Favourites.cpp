#include "Favourites.h"

#include <fstream>
#include <iostream>
#include <utility>

#include "MusicLibrary.h"
#include "Song.h"

namespace {

std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    std::size_t begin = s.find_first_not_of(ws);
    if (begin == std::string::npos) {
        return "";
    }
    std::size_t end = s.find_last_not_of(ws);
    return s.substr(begin, end - begin + 1);
}

}  // namespace

Favourites::Favourites(std::string path) : path_(std::move(path)) {}

void Favourites::load() {
    std::ifstream in(path_);
    if (!in.is_open()) {
        return;   // no favourites yet
    }
    std::string line;
    while (std::getline(in, line)) {
        std::string path = trim(line);
        if (!path.empty() && path[0] != '#') {
            paths_.insert(path);
        }
    }
}

void Favourites::save() const {
    std::ofstream out(path_);
    if (!out.is_open()) {
        std::cerr << "[Favourites] Could not write " << path_ << '\n';
        return;
    }
    for (const std::string& path : paths_) {
        out << path << '\n';
    }
}

bool Favourites::contains(const Song* song) const {
    return song != nullptr && paths_.count(song->filePath()) > 0;
}

void Favourites::toggle(const Song* song) {
    if (song == nullptr) {
        return;
    }
    auto it = paths_.find(song->filePath());
    if (it == paths_.end()) {
        paths_.insert(song->filePath());
    } else {
        paths_.erase(it);
    }
}

std::vector<Song*> Favourites::resolve(const MusicLibrary& library) const {
    std::vector<Song*> result;
    for (Song* song : library.songs()) {
        if (paths_.count(song->filePath()) > 0) {
            result.push_back(song);
        }
    }
    return result;
}

std::size_t Favourites::size() const { return paths_.size(); }
