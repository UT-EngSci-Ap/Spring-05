#include "CsvLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

// Splits a CSV line into fields. Supports double-quoted fields so values
// containing commas are handled correctly.
std::vector<std::string> parseLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';   // escaped quote
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                field += c;
            }
        } else {
            if (c == '"') {
                inQuotes = true;
            } else if (c == ',') {
                fields.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
    }
    fields.push_back(field);
    return fields;
}

}  // namespace

int CsvLoader::load(const std::string& path, MusicLibrary& library) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[CsvLoader] Could not open " << path << '\n';
        return 0;
    }

    int loaded = 0;
    int lineNo = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++lineNo;
        if (trim(line).empty()) {
            continue;
        }

        std::vector<std::string> f = parseLine(line);
        if (f.size() < 7) {
            std::cerr << "[CsvLoader] Skipping malformed row " << lineNo
                      << " (expected 7 fields).\n";
            continue;
        }

        std::string title    = trim(f[0]);
        std::string artist   = trim(f[1]);
        std::string album    = trim(f[2]);
        std::string genre    = trim(f[3]);
        std::string yearStr  = trim(f[4]);
        std::string durStr   = trim(f[5]);
        std::string filePath = trim(f[6]);

        if (title.empty() || filePath.empty()) {
            std::cerr << "[CsvLoader] Skipping row " << lineNo
                      << " (empty title or path).\n";
            continue;
        }

        int year = 0;
        int duration = 0;
        try {
            year = std::stoi(yearStr);
            duration = std::stoi(durStr);
        } catch (const std::exception&) {
            std::cerr << "[CsvLoader] Skipping row " << lineNo
                      << " (invalid year/duration).\n";
            continue;
        }

        library.addSong(new Song(title, artist, album, genre, year,
                                 duration, filePath));
        ++loaded;
    }

    return loaded;
}
