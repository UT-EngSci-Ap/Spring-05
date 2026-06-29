#pragma once

#include <string>

class Song;
class Playlist;
class PlayHistory;

enum class PlayerState { STOPPED, PLAYING, PAUSED };
enum class PlaybackMode { NO_REPEAT, REPEAT_ONE, REPEAT_ALL, SHUFFLE };

std::string toString(PlaybackMode mode);
std::string toString(PlayerState state);
bool parseMode(const std::string& text, PlaybackMode& out);

// Playback state machine wrapping the miniaudio engine.
// All miniaudio types live in the .cpp (pImpl) so MINIAUDIO_IMPLEMENTATION
// is defined in exactly one translation unit.
class Player {
public:
    Player();
    ~Player();

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    // Optional play-count tracker (bonus). When set, every song that starts
    // playing is recorded. Player does not own it.
    void setHistory(PlayHistory* history);

    void setPlaylist(Playlist* playlist);   // active playlist; stops playback
    Playlist* playlist() const;

    // Recompute the current index from the current song. Call after the active
    // playlist's contents change in place (e.g. the auto-generated playlists are
    // rebuilt) so next/previous keep working without restarting playback.
    void resyncIndex();

    void setMode(PlaybackMode mode);
    PlaybackMode mode() const;

    void setCurrentSong(Song* song);        // for "last played" display only
    Song* currentSong() const;

    bool playIndex(int index);              // play song at index of active list
    bool playSong(Song* song);              // play a specific song
    void play();                            // (re)start current song
    void pause();
    void resume();
    void stop();
    void next();
    void previous();
    void seekBy(int seconds);               // +/- seconds (bonus seek support)

    void tick();                            // call each UI refresh: auto-advance

    PlayerState state() const;
    float cursorSeconds() const;            // elapsed playback time
    int totalSeconds() const;               // total length from metadata

private:
    struct Impl;                            // hides miniaudio types
    Impl* impl_;

    void loadAndStart(Song* song);
    void unloadSound();
    int pickShuffleIndex() const;

    Playlist* playlist_ = nullptr;
    PlayHistory* history_ = nullptr;        // non-owning; may be null
    Song* current_ = nullptr;
    int currentIndex_ = -1;
    PlayerState state_ = PlayerState::STOPPED;
    PlaybackMode mode_ = PlaybackMode::NO_REPEAT;
};
