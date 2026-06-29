#pragma once

#include <string>

class MusicLibrary;

// Reads library.csv and populates a MusicLibrary with heap-allocated Songs.
// The CSV has no header row; every line is a record:
//   title,artist,album,genre,year,duration_sec,file_path
class CsvLoader {
public:
    // Returns the number of songs successfully loaded.
    // Malformed rows are skipped with a warning; loading continues.
    int load(const std::string& path, MusicLibrary& library) const;
};
