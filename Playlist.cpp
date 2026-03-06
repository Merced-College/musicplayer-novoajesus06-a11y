/*
 * Playlist.cpp
 * Description: Implementation of the Playlist class.
 *              Stores AudioTrack objects and supports list, linear search,
 *              and insertion sort (alphabetical by title).
 */

#include "Playlist.h"

// ── Constructor ───────────────────────────────────────────────────────────────

Playlist::Playlist() : _count(0) {}

// ── Core Operations ───────────────────────────────────────────────────────────

// Add a new track. Returns false (and prints a warning) if the array is full.
bool Playlist::addTrack(int trackNumber, String title) {
    if (_count >= MAX_TRACKS) {
        Serial.println("Playlist full – cannot add more tracks.");
        return false;
    }
    _tracks[_count] = AudioTrack(trackNumber, title);
    _count++;
    return true;
}

// Print every track as "N. Title" to the Serial Monitor.
void Playlist::listTracks() const {
    if (_count == 0) {
        Serial.println("Playlist is empty.");
        return;
    }
    Serial.println("──── Playlist ────");
    for (int i = 0; i < _count; i++) {
        _tracks[i].print();
    }
    Serial.println("──────────────────");
}

// Return number of tracks currently stored.
int Playlist::getCount() const {
    return _count;
}

// ── Access ────────────────────────────────────────────────────────────────────

// Return track at 0-based array index.
// Returns a blank AudioTrack if index is out of range.
AudioTrack Playlist::getTrack(int index) const {
    if (index < 0 || index >= _count) {
        return AudioTrack(); // empty / invalid track
    }
    return _tracks[index];
}

// Return track whose _trackNumber equals trackNum (1-based).
// Returns a blank AudioTrack if not found.
AudioTrack Playlist::getTrackByNumber(int trackNum) const {
    for (int i = 0; i < _count; i++) {
        if (_tracks[i].getTrackNumber() == trackNum) {
            return _tracks[i];
        }
    }
    return AudioTrack();
}

// True if at least one track with this 1-based track number exists.
bool Playlist::isValidTrack(int trackNum) const {
    for (int i = 0; i < _count; i++) {
        if (_tracks[i].getTrackNumber() == trackNum) {
            return true;
        }
    }
    return false;
}

// ── Search ────────────────────────────────────────────────────────────────────

// Linear search (O(n)) – case-insensitive partial title match.
// Returns the array index of the first match, or -1 if not found.
int Playlist::linearSearch(String keyword) const {
    // Convert keyword to lowercase for case-insensitive comparison
    String lowerKeyword = keyword;
    lowerKeyword.toLowerCase();

    for (int i = 0; i < _count; i++) {
        String lowerTitle = _tracks[i].getTitle();
        lowerTitle.toLowerCase();

        if (lowerTitle.indexOf(lowerKeyword) != -1) {
            return i; // found
        }
    }
    return -1; // not found
}

// ── Sort ──────────────────────────────────────────────────────────────────────

// Insertion sort (O(n²) worst case) – sorts _tracks alphabetically by title.
// Chosen over selection sort because it is stable and performs well on
// nearly-sorted data (O(n) best case).
void Playlist::insertionSort() {
    for (int i = 1; i < _count; i++) {
        AudioTrack key = _tracks[i];
        int j = i - 1;

        // Shift tracks that are alphabetically greater than key one position right
        while (j >= 0 && _tracks[j].getTitle() > key.getTitle()) {
            _tracks[j + 1] = _tracks[j];
            j--;
        }
        _tracks[j + 1] = key;
    }
}
