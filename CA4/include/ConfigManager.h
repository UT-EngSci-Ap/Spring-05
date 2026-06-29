#pragma once

#include <map>
#include <string>

// Loads and saves settings.cfg using a simple key=value format, e.g.
//   active_playlist=rock_hits
//   playback_mode=SHUFFLE
//   last_song=Data/Musics/song.mp3
class ConfigManager {
public:
    explicit ConfigManager(std::string path);

    void load();
    void save() const;

    std::string get(const std::string& key,
                    const std::string& fallback = "") const;
    void set(const std::string& key, const std::string& value);

private:
    std::string path_;
    std::map<std::string, std::string> values_;
};
