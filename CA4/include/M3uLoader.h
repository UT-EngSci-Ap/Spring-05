#pragma once

#include <memory>
#include <string>
#include <vector>

class MusicLibrary;
class Playlist;

// Loads every *.m3u file from a directory. Each file becomes one Playlist
// whose entries are Song pointers resolved against the library by file path.
// Paths present in an .m3u but absent from the library are ignored.
class M3uLoader {
public:
    std::vector<std::unique_ptr<Playlist>> loadAll(
        const std::string& dir, const MusicLibrary& library) const;
};
