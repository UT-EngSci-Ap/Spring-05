#include "M3uLoader.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "MusicLibrary.h"
#include "Playlist.h"
#include "Song.h"

namespace fs = std::filesystem;

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

std::unique_ptr<Playlist> loadOne(const fs::path& file,
                                  const MusicLibrary& library) {
    std::ifstream in(file);
    if (!in.is_open()) {
        std::cerr << "[M3uLoader] Could not open " << file << '\n';
        return nullptr;
    }

    auto playlist = std::make_unique<Playlist>(file.stem().string());

    std::string line;
    while (std::getline(in, line)) {
        std::string path = trim(line);
        if (path.empty() || path[0] == '#') {
            continue;   // blank line or comment
        }
        Song* song = library.findByPath(path);
        if (song != nullptr) {
            playlist->addSong(song);
        } else {
            std::cerr << "[M3uLoader] " << file.filename().string()
                      << ": path not in library, ignored -> " << path << '\n';
        }
    }
    return playlist;
}

}  // namespace

std::vector<std::unique_ptr<Playlist>> M3uLoader::loadAll(
    const std::string& dir, const MusicLibrary& library) const {
    std::vector<std::unique_ptr<Playlist>> playlists;

    std::error_code ec;
    if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) {
        std::cerr << "[M3uLoader] Playlist directory not found: " << dir << '\n';
        return playlists;
    }

    // Collect .m3u paths first so we can sort for a stable, predictable order.
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (ext == ".m3u") {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end());

    for (const auto& file : files) {
        auto pl = loadOne(file, library);
        if (pl) {
            playlists.push_back(std::move(pl));
        }
    }
    return playlists;
}
