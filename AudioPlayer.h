/*
 * AudioPlayer.h
 * Author(s): [Your Name]
 * Description: Wraps DFRobotDFPlayerMini calls and manages player state.
 *              All hardware interaction is encapsulated here; no DFPlayer
 *              calls should appear outside this class.
 */

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ── Player State Enum ─────────────────────────────────────────────────────────
// Tracks whether the player is stopped, playing, paused, or in an error state.
enum class PlayerState {
    STOPPED,
    PLAYING,
    PAUSED,
    ERROR
};

class AudioPlayer {
private:
    DFRobotDFPlayerMini _dfPlayer;      // DFPlayer Mini driver object
    SoftwareSerial*     _serial;        // Pointer to the SoftwareSerial bus

    PlayerState _state;                 // Current player state
    int         _currentTrack;          // 1-based; 0 = none selected
    int         _volume;                // Current volume (0–30)
    int         _totalTracks;           // Total tracks on SD card

    // ── Private Helpers ───────────────────────────────────────────────────────
    void        printState() const;     // Print state label to Serial
    const char* stateLabel() const;     // Return C-string label for _state

public:
    // ── Constructor ───────────────────────────────────────────────────────────
    explicit AudioPlayer(SoftwareSerial* serial);

    // ── Initialization ────────────────────────────────────────────────────────
    // Call once in setup(). Returns false if DFPlayer does not respond.
    bool begin(int totalTracks);

    // ── Control Functions (required by spec) ─────────────────────────────────
    void playTrack(int trackNumber);
    void pause();
    void stop();
    void nextTrack();
    void previousTrack();
    void setVolume(int level);

    // ── Getters ───────────────────────────────────────────────────────────────
    int         getCurrentTrack()  const;
    int         getCurrentVolume() const;
    PlayerState getState()         const;

    // ── Status ────────────────────────────────────────────────────────────────
    void printStatus() const;  // Print full status summary to Serial
};

#endif // AUDIO_PLAYER_H
