#include "Song.h"

#include <iomanip>
#include <sstream>
#include <utility>

Song::Song(std::string title, std::string artist, std::string album,
           std::string genre, int year, int durationSec, std::string filePath)
    : title_(std::move(title)),
      artist_(std::move(artist)),
      album_(std::move(album)),
      genre_(std::move(genre)),
      year_(year),
      duration_(durationSec),
      filePath_(std::move(filePath)) {}

const std::string& Song::title() const { return title_; }
const std::string& Song::artist() const { return artist_; }
const std::string& Song::album() const { return album_; }
const std::string& Song::genre() const { return genre_; }
int Song::year() const { return year_; }
int Song::duration() const { return duration_; }
const std::string& Song::filePath() const { return filePath_; }

std::string Song::durationString() const {
    int mins = duration_ / 60;
    int secs = duration_ % 60;
    std::ostringstream out;
    out << std::setw(2) << std::setfill('0') << mins << ':'
        << std::setw(2) << std::setfill('0') << secs;
    return out.str();
}
