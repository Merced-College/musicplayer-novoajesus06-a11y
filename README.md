# Modular MP3 Controller System

## Team Members & Roles

| Name | Role |
|------|------|
| [Your Name] | Architecture Lead + Algorithms & Data Lead |
| [Partner Name] | Hardware / Integration Lead |

---

## Authorship Map

| File | Author(s) | Responsibility |
|------|-----------|----------------|
| `AudioTrack.h / .cpp` | [Your Name] | Track data model — number + title, getters/setters |
| `Playlist.h / .cpp` | [Your Name] | Track array, linear search, insertion sort |
| `AudioPlayer.h / .cpp` | [Your Name] | DFPlayer wrapper, state machine, control functions |
| `MusicPlayer.ino` | Both | Serial menu, `setup()`, `loop()`, command dispatch |

---

## Class Responsibilities

- **`AudioTrack`** — Lightweight data object. Holds one track number (1-based) and a title string.
- **`Playlist`** — Owns a fixed-size `AudioTrack` array. Provides `addTrack`, `listTracks`, `linearSearch`, and `insertionSort`.
- **`AudioPlayer`** — Encapsulates all DFPlayer Mini hardware calls. Maintains `PlayerState` (STOPPED / PLAYING / PAUSED / ERROR) and enforces valid transitions.
- **`MusicPlayer.ino`** — Controller. Reads Serial input, calls class functions only (no direct DFPlayer calls).

---

## Wiring Notes (Arduino Uno → DFPlayer Mini)

| DFPlayer Pin | Arduino Pin | Notes |
|---|---|---|
| VCC | 5V | 3.2–5V supported |
| GND | GND | Shared ground required |
| TX | D10 | DFPlayer TX → Arduino SoftwareSerial RX |
| RX | D11 | Through **1 kΩ resistor** to protect DFPlayer |
| SPK1 | Speaker + | 4–8 Ω speaker |
| SPK2 | Speaker − | Direct connection |

SoftwareSerial is initialized at 9600 baud in `setup()`.

---

## SD Card Setup

Format the microSD card as **FAT32** and name files:

```
0001.mp3   ← Chinese Dream
0002.mp3   ← Crack The Whip
0003.mp3   ← Dexter Meme
0004.mp3   ← Epic
0005.mp3   ← Skeleton With Shield
0006.mp3   ← Taco Bell Bong SFX
0007.mp3   ← Toy Phone Ringtone
0008.mp3   ← Vine Boom
```

---

## How to Run

1. Wire the DFPlayer Mini per the table above.
2. Copy MP3 files to microSD card (FAT32, numeric names).
3. Open `MusicPlayer.ino` in Arduino IDE.
4. Install **DFRobotDFPlayerMini** library via Library Manager.
5. Upload to Arduino Uno.
6. Open **Serial Monitor** at **9600 baud**.
7. Use the commands below.

---

## Serial Command Reference

| Command | Action |
|---|---|
| `p <num>` | Play track by number (e.g. `p 3`) |
| `n` | Next track |
| `b` | Previous track |
| `pause` | Toggle pause / resume |
| `s` or `stop` | Stop playback |
| `v <0-30>` | Set volume (e.g. `v 20`) |
| `v+` | Volume up |
| `v-` | Volume down |
| `list` | Show full playlist |
| `find <word>` | Search by title keyword (e.g. `find boom`) |
| `sort` | Sort playlist alphabetically and display |
| `status` | Show current state, track, and volume |
| `help` | Show command menu |

---

## Algorithms

- **Search:** Linear search — O(n). Iterates each track and checks if the title contains the keyword (case-insensitive).
- **Sort:** Insertion sort — O(n²) worst case, O(n) best case (already sorted). Sorts `AudioTrack` array in-place by title string using Arduino's `String` `>` operator.
