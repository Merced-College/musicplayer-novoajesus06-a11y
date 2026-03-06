/*
 * AudioTrack.cpp
 * Description: Implementation of the AudioTrack class.
 *              Stores a track number and title; provides getters, setters, and print.
 */

#include "AudioTrack.h"

// ── Constructors ──────────────────────────────────────────────────────────────

// Default constructor: unset track
AudioTrack::AudioTrack() : _trackNumber(0), _title("") {}

// Parameterized constructor
AudioTrack::AudioTrack(int trackNumber, String title)
    : _trackNumber(trackNumber), _title(title) {}

// ── Getters ───────────────────────────────────────────────────────────────────

int AudioTrack::getTrackNumber() const {
    return _trackNumber;
}

String AudioTrack::getTitle() const {
    return _title;
}

// ── Setters ───────────────────────────────────────────────────────────────────

void AudioTrack::setTrackNumber(int trackNumber) {
    _trackNumber = trackNumber;
}

void AudioTrack::setTitle(String title) {
    _title = title;
}

// ── Utility ───────────────────────────────────────────────────────────────────

// Print formatted track info to Serial Monitor
void AudioTrack::print() const {
    Serial.print(_trackNumber);
    Serial.print(". ");
    Serial.println(_title);
}
