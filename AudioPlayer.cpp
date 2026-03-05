/*
 * AudioPlayer.cpp
 * Author(s): [Your Name]
 * Description: Implementation of AudioPlayer.
 *              Manages PlayerState, enforces valid transitions, and wraps
 *              all DFRobotDFPlayerMini calls so no hardware code leaks
 *              into the main sketch.
 */

#include "AudioPlayer.h"

// ── Constructor ───────────────────────────────────────────────────────────────

AudioPlayer::AudioPlayer(SoftwareSerial* serial)
    : _serial(serial),
      _state(PlayerState::STOPPED),
      _currentTrack(0),
      _volume(15),
      _totalTracks(0)
{}

// ── Initialization ────────────────────────────────────────────────────────────

bool AudioPlayer::begin(int totalTracks) {
    _totalTracks = totalTracks;

    if (!_dfPlayer.begin(*_serial)) {
        Serial.println("[AudioPlayer] DFPlayer init failed.");
        Serial.println("  Check wiring: TX→D10, RX→D11 (1kΩ), GND shared.");
        _state = PlayerState::ERROR;
        printState();
        return false;
    }

    _dfPlayer.volume(_volume);
    Serial.println("[AudioPlayer] DFPlayer ready.");
    return true;
}

// ── Control Functions ─────────────────────────────────────────────────────────

void AudioPlayer::playTrack(int trackNumber) {
    // Validate track number
    if (trackNumber < 1 || trackNumber > _totalTracks) {
        Serial.print("[AudioPlayer] Invalid track: ");
        Serial.print(trackNumber);
        Serial.print("  (valid range: 1–");
        Serial.print(_totalTracks);
        Serial.println(")");
        _state = PlayerState::ERROR;
        printState();
        return;
    }

    _dfPlayer.play(trackNumber);
    _currentTrack = trackNumber;
    _state = PlayerState::PLAYING;
    printState();
}

void AudioPlayer::pause() {
    if (_state == PlayerState::STOPPED) {
        Serial.println("[AudioPlayer] Cannot pause – player is STOPPED.");
        return;
    }
    if (_state == PlayerState::ERROR) {
        Serial.println("[AudioPlayer] Cannot pause – player is in ERROR state.");
        return;
    }

    if (_state == PlayerState::PLAYING) {
        _dfPlayer.pause();
        _state = PlayerState::PAUSED;
        printState();
    } else if (_state == PlayerState::PAUSED) {
        // Resume playback
        _dfPlayer.start();
        _state = PlayerState::PLAYING;
        printState();
    }
}

void AudioPlayer::stop() {
    if (_state == PlayerState::STOPPED) {
        Serial.println("[AudioPlayer] Already stopped.");
        return;
    }
    _dfPlayer.stop();
    _state = PlayerState::STOPPED;
    printState();
}

void AudioPlayer::nextTrack() {
    if (_currentTrack == 0) {
        Serial.println("[AudioPlayer] No track selected – cannot go to next.");
        return;
    }

    int next = _currentTrack + 1;
    if (next > _totalTracks) {
        next = 1; // wrap around to first track
        Serial.println("[AudioPlayer] Wrapping to first track.");
    }

    playTrack(next);
}

void AudioPlayer::previousTrack() {
    if (_currentTrack == 0) {
        Serial.println("[AudioPlayer] No track selected – cannot go to previous.");
        return;
    }

    int prev = _currentTrack - 1;
    if (prev < 1) {
        prev = _totalTracks; // wrap around to last track
        Serial.println("[AudioPlayer] Wrapping to last track.");
    }

    playTrack(prev);
}

void AudioPlayer::setVolume(int level) {
    // Clamp to safe range 0–30
    if (level < 0)  level = 0;
    if (level > 30) level = 30;

    _volume = level;
    _dfPlayer.volume(_volume);

    Serial.print("[AudioPlayer] Volume set to ");
    Serial.println(_volume);
}

// ── Getters ───────────────────────────────────────────────────────────────────

int AudioPlayer::getCurrentTrack() const {
    return _currentTrack;
}

int AudioPlayer::getCurrentVolume() const {
    return _volume;
}

PlayerState AudioPlayer::getState() const {
    return _state;
}

// ── Status ────────────────────────────────────────────────────────────────────

void AudioPlayer::printStatus() const {
    Serial.println("──── Player Status ────");
    Serial.print("  State:   ");
    Serial.println(stateLabel());
    Serial.print("  Track:   ");
    if (_currentTrack > 0) Serial.println(_currentTrack);
    else                   Serial.println("(none)");
    Serial.print("  Volume:  ");
    Serial.println(_volume);
    Serial.println("───────────────────────");
}

// ── Private Helpers ───────────────────────────────────────────────────────────

void AudioPlayer::printState() const {
    Serial.print("[AudioPlayer] State → ");
    Serial.println(stateLabel());
}

const char* AudioPlayer::stateLabel() const {
    switch (_state) {
        case PlayerState::STOPPED: return "STOPPED";
        case PlayerState::PLAYING: return "PLAYING";
        case PlayerState::PAUSED:  return "PAUSED";
        case PlayerState::ERROR:   return "ERROR";
        default:                   return "UNKNOWN";
    }
}
