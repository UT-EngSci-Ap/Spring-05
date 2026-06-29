#include "PlayHistory.h"

#include <algorithm>
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

PlayHistory::PlayHistory(std::string path) : path_(std::move(path)) {}

void PlayHistory::load() {
    std::ifstream in(path_);
    if (!in.is_open()) {
        return;   // no history yet
    }
    std::string line;
    while (std::getline(in, line)) {
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }
        std::size_t comma = trimmed.find(',');
        if (comma == std::string::npos) {
            continue;
        }
        // Path is everything after the first comma, so commas inside a path
        // (unlikely, but safe) do not corrupt parsing.
        std::string countText = trim(trimmed.substr(0, comma));
        std::string path = trim(trimmed.substr(comma + 1));
        try {
            int value = std::stoi(countText);
            if (value > 0 && !path.empty()) {
                counts_[path] = value;
            }
        } catch (const std::exception&) {
            // skip malformed line
        }
    }
}

void PlayHistory::save() const {
    std::ofstream out(path_);
    if (!out.is_open()) {
        std::cerr << "[PlayHistory] Could not write " << path_ << '\n';
        return;
    }
    for (const auto& [path, value] : counts_) {
        out << value << ',' << path << '\n';
    }
}

void PlayHistory::record(const Song* song) {
    if (song != nullptr) {
        ++counts_[song->filePath()];
    }
}

int PlayHistory::count(const Song* song) const {
    if (song == nullptr) {
        return 0;
    }
    auto it = counts_.find(song->filePath());
    return it != counts_.end() ? it->second : 0;
}

std::vector<Song*> PlayHistory::mostPlayed(const MusicLibrary& library,
                                           std::size_t limit) const {
    std::vector<Song*> played;
    for (Song* song : library.songs()) {
        if (count(song) > 0) {
            played.push_back(song);
        }
    }

    std::stable_sort(played.begin(), played.end(),
                     [this](Song* a, Song* b) {
                         int ca = count(a);
                         int cb = count(b);
                         if (ca != cb) {
                             return ca > cb;   // most plays first
                         }
                         return a->title() < b->title();
                     });

    if (played.size() > limit) {
        played.resize(limit);
    }
    return played;
}
