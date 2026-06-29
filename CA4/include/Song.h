#pragma once

#include <string>

// A single track with all of its metadata. Pure data object: it never
// touches std::cout / std::cin — all I/O is handled by the UI layer.
class Song {
public:
    Song(std::string title, std::string artist, std::string album,
         std::string genre, int year, int durationSec, std::string filePath);

    const std::string& title() const;
    const std::string& artist() const;
    const std::string& album() const;
    const std::string& genre() const;
    int year() const;
    int duration() const;                 // seconds
    const std::string& filePath() const;

    std::string durationString() const;   // "MM:SS"

private:
    std::string title_;
    std::string artist_;
    std::string album_;
    std::string genre_;
    int year_;
    int duration_;
    std::string filePath_;
};
