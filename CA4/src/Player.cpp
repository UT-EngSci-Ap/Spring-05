// This is the single translation unit that compiles the miniaudio library.
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "Player.h"

#include <iostream>
#include <random>

#include "PlayHistory.h"
#include "Playlist.h"
#include "Song.h"

std::string toString(PlaybackMode mode) {
    switch (mode) {
        case PlaybackMode::NO_REPEAT:  return "NO_REPEAT";
        case PlaybackMode::REPEAT_ONE: return "REPEAT_ONE";
        case PlaybackMode::REPEAT_ALL: return "REPEAT_ALL";
        case PlaybackMode::SHUFFLE:    return "SHUFFLE";
    }
    return "NO_REPEAT";
}

std::string toString(PlayerState state) {
    switch (state) {
        case PlayerState::STOPPED: return "STOPPED";
        case PlayerState::PLAYING: return "PLAYING";
        case PlayerState::PAUSED:  return "PAUSED";
    }
    return "STOPPED";
}

bool parseMode(const std::string& text, PlaybackMode& out) {
    if (text == "NO_REPEAT")  { out = PlaybackMode::NO_REPEAT;  return true; }
    if (text == "REPEAT_ONE") { out = PlaybackMode::REPEAT_ONE; return true; }
    if (text == "REPEAT_ALL") { out = PlaybackMode::REPEAT_ALL; return true; }
    if (text == "SHUFFLE")    { out = PlaybackMode::SHUFFLE;    return true; }
    return false;
}

struct Player::Impl {
    ma_engine engine{};
    ma_sound sound{};
    bool engineReady = false;
    bool soundLoaded = false;
    std::mt19937 rng{std::random_device{}()};
};

Player::Player() : impl_(new Impl()) {
    if (ma_engine_init(nullptr, &impl_->engine) == MA_SUCCESS) {
        impl_->engineReady = true;
    } else {
        std::cerr << "[Player] Audio engine failed to start; "
                     "playback will be silent.\n";
    }
}

Player::~Player() {
    unloadSound();
    if (impl_->engineReady) {
        ma_engine_uninit(&impl_->engine);
    }
    delete impl_;
}

void Player::unloadSound() {
    if (impl_->soundLoaded) {
        ma_sound_uninit(&impl_->sound);
        impl_->soundLoaded = false;
    }
}

void Player::loadAndStart(Song* song) {
    if (song == nullptr) {
        return;
    }
    current_ = song;
    if (history_ != nullptr) {
        history_->record(song);   // count every song that starts playing
    }

    if (!impl_->engineReady) {
        // No audio device: still behave as the state machine says.
        state_ = PlayerState::PLAYING;
        return;
    }

    unloadSound();
    // The data files store paths relative to the Data/ directory (e.g.
    // "Musics/song.mp3"), but the program runs from the project root where the
    // real file lives under Data/. Prepend the prefix here unless it is
    // already present, so library.csv and the playlists stay unchanged.
    std::string path = song->filePath();
    if (path.rfind("Data/", 0) != 0) {
        path = "Data/" + path;
    }
    ma_result r = ma_sound_init_from_file(&impl_->engine,
                                          path.c_str(),
                                          0, nullptr, nullptr, &impl_->sound);
    if (r != MA_SUCCESS) {
        std::cerr << "[Player] Could not load file: " << path
                  << '\n';
        impl_->soundLoaded = false;
        state_ = PlayerState::STOPPED;
        return;
    }
    impl_->soundLoaded = true;
    ma_sound_start(&impl_->sound);
    state_ = PlayerState::PLAYING;
}

void Player::setHistory(PlayHistory* history) { history_ = history; }

void Player::setPlaylist(Playlist* playlist) {
    stop();
    playlist_ = playlist;
    currentIndex_ = (playlist_ != nullptr && current_ != nullptr)
                        ? playlist_->indexOf(current_)
                        : -1;
}

Playlist* Player::playlist() const { return playlist_; }

void Player::resyncIndex() {
    currentIndex_ = (playlist_ != nullptr && current_ != nullptr)
                        ? playlist_->indexOf(current_)
                        : -1;
}

void Player::setMode(PlaybackMode mode) { mode_ = mode; }
PlaybackMode Player::mode() const { return mode_; }

void Player::setCurrentSong(Song* song) {
    current_ = song;
    currentIndex_ = (playlist_ != nullptr && song != nullptr)
                        ? playlist_->indexOf(song)
                        : -1;
}

Song* Player::currentSong() const { return current_; }

bool Player::playIndex(int index) {
    if (playlist_ == nullptr || index < 0 ||
        index >= static_cast<int>(playlist_->size())) {
        return false;
    }
    currentIndex_ = index;
    loadAndStart(playlist_->at(static_cast<std::size_t>(index)));
    return true;
}

bool Player::playSong(Song* song) {
    if (song == nullptr) {
        return false;
    }
    currentIndex_ = (playlist_ != nullptr) ? playlist_->indexOf(song) : -1;
    loadAndStart(song);
    return true;
}

void Player::play() {
    if (current_ != nullptr) {
        loadAndStart(current_);
    } else if (playlist_ != nullptr && !playlist_->empty()) {
        playIndex(0);
    }
}

void Player::pause() {
    if (state_ != PlayerState::PLAYING) {
        return;   // nothing playing: silently ignored
    }
    if (impl_->soundLoaded) {
        ma_sound_stop(&impl_->sound);
    }
    state_ = PlayerState::PAUSED;
}

void Player::resume() {
    if (state_ != PlayerState::PAUSED) {
        return;
    }
    if (impl_->soundLoaded) {
        ma_sound_start(&impl_->sound);
    }
    state_ = PlayerState::PLAYING;
}

void Player::stop() {
    if (impl_->soundLoaded) {
        ma_sound_stop(&impl_->sound);
        ma_sound_seek_to_pcm_frame(&impl_->sound, 0);
    }
    state_ = PlayerState::STOPPED;
}

int Player::pickShuffleIndex() const {
    int n = static_cast<int>(playlist_->size());
    if (n <= 1) {
        return 0;
    }
    std::uniform_int_distribution<int> dist(0, n - 1);
    int idx = currentIndex_;
    while (idx == currentIndex_) {     // avoid immediate repeat
        idx = dist(impl_->rng);
    }
    return idx;
}

void Player::next() {
    if (playlist_ == nullptr || playlist_->empty()) {
        return;
    }
    int n = static_cast<int>(playlist_->size());

    switch (mode_) {
        case PlaybackMode::REPEAT_ONE:
            loadAndStart(playlist_->at(
                static_cast<std::size_t>(currentIndex_ < 0 ? 0 : currentIndex_)));
            return;
        case PlaybackMode::SHUFFLE:
            currentIndex_ = pickShuffleIndex();
            break;
        case PlaybackMode::REPEAT_ALL:
            currentIndex_ = (currentIndex_ + 1) % n;
            break;
        case PlaybackMode::NO_REPEAT:
            if (currentIndex_ + 1 < n) {
                ++currentIndex_;
            } else {
                stop();             // stop at the end of the playlist
                return;
            }
            break;
    }
    loadAndStart(playlist_->at(static_cast<std::size_t>(currentIndex_)));
}

void Player::previous() {
    if (playlist_ == nullptr || playlist_->empty()) {
        return;
    }
    int n = static_cast<int>(playlist_->size());
    if (currentIndex_ <= 0) {
        currentIndex_ = (mode_ == PlaybackMode::REPEAT_ALL) ? n - 1 : 0;
    } else {
        --currentIndex_;
    }
    loadAndStart(playlist_->at(static_cast<std::size_t>(currentIndex_)));
}

void Player::seekBy(int seconds) {
    if (!impl_->soundLoaded) {
        return;
    }
    ma_uint64 cursor = 0, length = 0;
    ma_sound_get_cursor_in_pcm_frames(&impl_->sound, &cursor);
    ma_sound_get_length_in_pcm_frames(&impl_->sound, &length);

    ma_uint32 rate = ma_engine_get_sample_rate(&impl_->engine);
    ma_int64 newFrame = static_cast<ma_int64>(cursor) +
                        static_cast<ma_int64>(seconds) * rate;

    if (newFrame < 0) {
        newFrame = 0;
    }
    if (static_cast<ma_uint64>(newFrame) >= length) {
        next();
        return;
    }
    ma_sound_seek_to_pcm_frame(&impl_->sound,
                               static_cast<ma_uint64>(newFrame));
}

void Player::tick() {
    if (state_ != PlayerState::PLAYING) {
        return;
    }
    if (impl_->soundLoaded && ma_sound_at_end(&impl_->sound)) {
        next();
    }
}

PlayerState Player::state() const { return state_; }

float Player::cursorSeconds() const {
    if (!impl_->soundLoaded) {
        return 0.0f;
    }
    ma_uint64 frames = 0;
    ma_sound_get_cursor_in_pcm_frames(&impl_->sound, &frames);
    ma_uint32 rate = ma_engine_get_sample_rate(&impl_->engine);
    if (rate == 0) {
        return 0.0f;
    }
    return static_cast<float>(frames) / static_cast<float>(rate);
}

int Player::totalSeconds() const {
    return current_ != nullptr ? current_->duration() : 0;
}
