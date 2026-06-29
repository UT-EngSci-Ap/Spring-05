#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

class Song;
class MusicLibrary;

// Counts how many times each song has been played, keyed by file path so the
// tally survives restarts. Powers the automatic "Most Played" playlist.
// Persisted as CSV lines "count,filePath" (count first so any commas in the
// path are harmless).
class PlayHistory {
public:
    explicit PlayHistory(std::string path);

    void load();
    void save() const;

    void record(const Song* song);     // increment this song's play count
    int count(const Song* song) const;

    // Songs with a play count > 0, ordered by count descending (ties broken by
    // title), limited to `limit` entries.
    std::vector<Song*> mostPlayed(const MusicLibrary& library,
                                  std::size_t limit) const;

private:
    std::string path_;
    std::map<std::string, int> counts_;
};
