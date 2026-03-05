/*
 * AudioTrack.h
 * Author(s): [Your Name]
 * Description: Represents a single audio track with a track number and title.
 *              Used by the Playlist class to store and retrieve song metadata.
 */

#ifndef AUDIO_TRACK_H
#define AUDIO_TRACK_H

#include <Arduino.h>

class AudioTrack {
private:
    int    _trackNumber;  // 1-based track number (matches SD card filename)
    String _title;        // Human-readable song title

public:
    // ── Constructors ──────────────────────────────────────────────────────────
    AudioTrack();
    AudioTrack(int trackNumber, String title);

    // ── Getters ───────────────────────────────────────────────────────────────
    int    getTrackNumber() const;
    String getTitle()       const;

    // ── Setters ───────────────────────────────────────────────────────────────
    void setTrackNumber(int trackNumber);
    void setTitle(String title);

    // ── Utility ───────────────────────────────────────────────────────────────
    void print() const;  // Print "TrackNum. Title" to Serial
};

#endif // AUDIO_TRACK_H
