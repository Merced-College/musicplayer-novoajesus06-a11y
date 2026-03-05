/*
 * MusicPlayer.ino
 * Author(s): Adan Bahena-Guardado, Jesus Novoa Vasquez
 * Description: Main controller for the Modular MP3 Player System.
 *              Handles Serial command menu and coordinates AudioPlayer + Playlist.
 *
 * Wiring (Arduino Uno → DFPlayer Mini):
 *   VCC  → 5V
 *   GND  → GND
 *   TX   → D10 (SoftwareSerial RX)
 *   RX   → D11 (SoftwareSerial TX) through 1kΩ resistor
 *   SPK1 → Speaker +
 *   SPK2 → Speaker -
 *
 * SD Card file mapping (FAT32, numeric names):
 *   0001.mp3 → Chinese Dream
 *   0002.mp3 → Crack The Whip
 *   0003.mp3 → Dexter Meme
 *   0004.mp3 → Epic
 *   0005.mp3 → Skeleton With Shield
 *   0006.mp3 → Taco Bell Bong SFX
 *   0007.mp3 → Toy Phone Ringtone
 *   0008.mp3 → Vine Boom
 */

#include <SoftwareSerial.h>
#include "AudioTrack.h"
#include "Playlist.h"
#include "AudioPlayer.h"

// SoftwareSerial pins for DFPlayer (RX=10, TX=11)
SoftwareSerial dfSerial(10, 11);

// Global objects
AudioPlayer player(&dfSerial);
Playlist playlist;

// Number of tracks on the SD card (must match your actual files)
const int TOTAL_TRACKS = 8;

// ─────────────────────────────────────────
// Forward declarations
// ─────────────────────────────────────────
void loadPlaylist();
void printMenu();
void handleCommand(String cmd);

// ─────────────────────────────────────────
// setup()
// ─────────────────────────────────────────
void setup() {
    Serial.begin(9600);
    dfSerial.begin(9600);
    delay(1000); // Allow DFPlayer to boot

    Serial.println("=================================");
    Serial.println("  Modular MP3 Controller System  ");
    Serial.println("=================================");

    // Load songs into the playlist
    loadPlaylist();

    // Initialize the DFPlayer hardware
    if (!player.begin(TOTAL_TRACKS)) {
        Serial.println("Hardware init failed. Running in software-only mode.");
    }

    // Show command menu
    printMenu();
}

// ─────────────────────────────────────────
// loop()
// ─────────────────────────────────────────
void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.length() > 0) {
            Serial.print("> ");
            Serial.println(input);
            handleCommand(input);
        }
    }
}

// ─────────────────────────────────────────
// loadPlaylist() - populate the song array
// Array index 0 → track 1 (0001.mp3), etc.
// Titles match the actual MP3 files on the SD card.
// ─────────────────────────────────────────
void loadPlaylist() {
    // Song titles — index matches SD card file number (index 0 → 0001.mp3)
    String songs[] = {
        "Chinese Dream",         // 0001.mp3
        "Crack The Whip",        // 0002.mp3
        "Dexter Meme",           // 0003.mp3
        "Epic",                  // 0004.mp3
        "Skeleton With Shield",  // 0005.mp3
        "Taco Bell Bong SFX",    // 0006.mp3
        "Toy Phone Ringtone",    // 0007.mp3
        "Vine Boom"              // 0008.mp3
    };

    int numSongs = sizeof(songs) / sizeof(songs[0]);

    for (int i = 0; i < numSongs; i++) {
        // Track numbers are 1-based; array index is 0-based
        playlist.addTrack(i + 1, songs[i]);
    }

    Serial.print("Playlist loaded: ");
    Serial.print(playlist.getCount());
    Serial.println(" tracks.");
}

// ─────────────────────────────────────────
// printMenu() - display available commands
// ─────────────────────────────────────────
void printMenu() {
    Serial.println();
    Serial.println("======= COMMANDS =======");
    Serial.println(" p <num>      - Play track by number");
    Serial.println(" n            - Next track");
    Serial.println(" b            - Previous track");
    Serial.println(" pause        - Pause / Resume");
    Serial.println(" s            - Stop");
    Serial.println(" v <0-30>     - Set volume");
    Serial.println(" v+           - Volume up");
    Serial.println(" v-           - Volume down");
    Serial.println(" list         - Show playlist");
    Serial.println(" find <word>  - Search by title keyword");
    Serial.println(" sort         - Sort playlist alphabetically");
    Serial.println(" status       - Show player status");
    Serial.println(" help         - Show this menu");
    Serial.println("========================");
    Serial.println();
}

// ─────────────────────────────────────────
// handleCommand() - parse and execute input
// ─────────────────────────────────────────
void handleCommand(String cmd) {
    cmd.trim();

    // --- Play track: "p <number>" ---
    if (cmd.startsWith("p ")) {
        String numStr = cmd.substring(2);
        numStr.trim();
        int trackNum = numStr.toInt();

        if (trackNum == 0) {
            Serial.println("Invalid track number. Usage: p <number>");
            return;
        }

        // Look up track title from playlist
        if (playlist.isValidTrack(trackNum)) {
            AudioTrack t = playlist.getTrackByNumber(trackNum);
            Serial.print("Playing: ");
            Serial.println(t.getTitle());
        }

        player.playTrack(trackNum);
    }

    // --- Next track ---
    else if (cmd == "n") {
        player.nextTrack();
        int cur = player.getCurrentTrack();
        if (cur > 0 && playlist.isValidTrack(cur)) {
            Serial.print("Track: ");
            Serial.println(playlist.getTrackByNumber(cur).getTitle());
        }
    }

    // --- Previous track ---
    else if (cmd == "b") {
        player.previousTrack();
        int cur = player.getCurrentTrack();
        if (cur > 0 && playlist.isValidTrack(cur)) {
            Serial.print("Track: ");
            Serial.println(playlist.getTrackByNumber(cur).getTitle());
        }
    }

    // --- Pause / Resume ---
    else if (cmd == "pause") {
        player.pause();
    }

    // --- Stop ---
    else if (cmd == "s" || cmd == "stop") {
        player.stop();
    }

    // --- Set volume: "v <0-30>" ---
    else if (cmd.startsWith("v ")) {
        String numStr = cmd.substring(2);
        numStr.trim();
        int vol = numStr.toInt();
        player.setVolume(vol);
    }

    // --- Volume up ---
    else if (cmd == "v+") {
        player.setVolume(player.getCurrentVolume() + 1);
    }

    // --- Volume down ---
    else if (cmd == "v-") {
        player.setVolume(player.getCurrentVolume() - 1);
    }

    // --- List tracks ---
    else if (cmd == "list") {
        playlist.listTracks();
    }

    // --- Find track by keyword ---
    else if (cmd.startsWith("find ")) {
        String keyword = cmd.substring(5);
        keyword.trim();

        int idx = playlist.linearSearch(keyword);

        if (idx == -1) {
            Serial.println("Song not found.");
        } else {
            AudioTrack found = playlist.getTrack(idx);
            Serial.print("Song found: ");
            Serial.println(found.getTitle());
            Serial.print("Track number: ");
            Serial.println(found.getTrackNumber());
        }
    }

    // --- Sort playlist alphabetically ---
    else if (cmd == "sort") {
        playlist.insertionSort();
        Serial.println("Sorted Playlist (alphabetical):");
        playlist.listTracks();
    }

    // --- Player status ---
    else if (cmd == "status") {
        player.printStatus();
    }

    // --- Help menu ---
    else if (cmd == "help") {
        printMenu();
    }

    // --- Unknown command ---
    else {
        Serial.print("Unknown command: \"");
        Serial.print(cmd);
        Serial.println("\"  (type 'help' for commands)");
    }
}
