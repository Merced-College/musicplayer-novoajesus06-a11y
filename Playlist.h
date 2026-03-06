/*
 * Playlist.h
 * Description: Manages a fixed-size array of AudioTrack objects.
 *              Provides add, list, linear search, and insertion sort.
 */

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Arduino.h>
#include "AudioTrack.h"

// Maximum number of tracks the playlist can hold
const int MAX_TRACKS = 20;

class Playlist {
private:
    AudioTrack _tracks[MAX_TRACKS];  // Fixed-size track array
    int        _count;               // Number of tracks currently stored

public:
    // ── Constructor ───────────────────────────────────────────────────────────
    Playlist();

    // ── Core Operations ───────────────────────────────────────────────────────
    bool addTrack(int trackNumber, String title);   // Add a track; returns false if full
    void listTracks()        const;                 // Print numbered playlist to Serial
    int  getCount()          const;                 // Return current track count

    // ── Access ────────────────────────────────────────────────────────────────
    AudioTrack getTrack(int index)             const; // Access by array index (0-based)
    AudioTrack getTrackByNumber(int trackNum)  const; // Access by 1-based track number
    bool       isValidTrack(int trackNum)      const; // True if trackNum is in range

    // ── Search ────────────────────────────────────────────────────────────────
    // Linear search: returns array index of first title containing keyword,
    // or -1 if not found. Case-insensitive partial match.
    int linearSearch(String keyword) const;

    // ── Sort ──────────────────────────────────────────────────────────────────
    // Insertion sort: re-orders _tracks alphabetically by title (A → Z).
    // Runs in-place; O(n²) worst case.
    void insertionSort();
};

#endif // PLAYLIST_H
