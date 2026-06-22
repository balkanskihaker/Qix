# Qix

A faithful clone of the classic 1981 Taito arcade game **Qix**, built in **C++17** with **OpenGL 1.x / GLUT** as a university project for the Computer Graphics course at the Faculty of Engineering Sciences, University of Kragujevac.

---

## Gameplay

You control **Stix**, a marker that moves along the borders of a rectangular field. Move into the open area to draw lines and claim territory — but watch out:

- **Sparx** patrols the edges and hunts you down while you're on the border
- **Qix** roams the unclaimed space and attacks your line while you're drawing
- If the **fuse** burns down before you close your line, you lose a life

Claim enough of the field (75%, or 85% on the final level) to advance.

---

## Features

| Feature | Description |
|---|---|
| Full game loop | Menu → Game → Scoreboard with persistent high scores |
| 10 progressive levels | Each level introduces a new mechanic or difficulty increase |
| Fuse mechanic | Lines burn from the start if you take too long (levels 2+) |
| Reactive Sparx AI | Sparx hunts you on the border; goes random when you start drawing |
| Buffered input | Multiple keys tracked simultaneously for smooth control |
| CRT visual filter | Scanlines + vignette applied as a multiply-blend texture overlay |
| Color correction | Base colors pre-compensated for the CRT filter's color cast |
| High score table | Top 10 results persisted to `data/scores.txt` |

---

## Controls

| Key | Action |
|---|---|
| Arrow keys | Move Stix |
| `S` | Switch to slow draw mode (2× points, longer exposure) |
| `F` | Switch to fast draw mode |
| `Esc` | Pause / resume |
| `M` | Return to menu (while paused) |
| `N` | Next level (after level complete) |
| `R` | Restart |

---

## Project Structure

```
Qix/
├── src/
│   ├── main.cpp                  # Entry point, GLUT init, game loop
│   ├── core/
│   │   ├── renderer.h/.cpp       # Drawing primitives, CRT effect
│   │   ├── grid.h/.cpp           # Game field state, flood fill algorithm
│   │   ├── gamestate.h/.cpp      # Lives, score, invincibility timer
│   │   ├── levelmanager.h/.cpp   # Level configs and spawn logic
│   │   ├── scene_manager.h       # Scene enum and interface
│   │   └── RgbImage.h/.cpp       # BMP texture loader
│   ├── entities/
│   │   ├── stix.h/.cpp           # Player character, trail, fuse logic
│   │   ├── sparx.h/.cpp          # Edge-following enemy with reactive AI
│   │   └── qix.h/.cpp            # Free-roaming enemy with hunting mode
│   ├── scenes/
│   │   ├── menu.h/.cpp           # Main menu
│   │   ├── game.h/.cpp           # Game scene, entity coordination
│   │   └── scoreboard.h/.cpp     # High score display and name input
│   └── utils/
│       └── config.h              # All constants and CRT-corrected color palette
├── assets/
│   ├── crt_overlay.bmp           # Pre-generated CRT texture (800×600)
│   └── create_crt_overlay.py     # Python script to regenerate the texture
└── data/
    └── scores.txt                # Persistent high score storage
```

---

## Build

### Requirements

- macOS (Apple Silicon or Intel)
- Xcode Command Line Tools (`xcode-select --install`)
- Visual Studio Code (optional, `.vscode/tasks.json` included)

The project uses Apple's built-in **GLUT.framework** — no additional dependencies needed.

### Compile manually

```bash
clang++ -std=c++17 -Wall -g \
  src/main.cpp \
  src/core/*.cpp \
  src/entities/*.cpp \
  src/scenes/*.cpp \
  -framework OpenGL -framework GLUT \
  -Wno-deprecated \
  -o build/qix
```

### Run

```bash
./build/qix
```

> Make sure to run from the project root directory so that `assets/` and `data/` paths resolve correctly.

### Regenerate CRT overlay (optional)

The BMP texture is already included. To regenerate it:

```bash
cd assets
python3 create_crt_overlay.py
```

---

## CRT Color Correction

The CRT filter is a 800×600 BMP texture applied as a **multiply blend** (`GL_DST_COLOR, GL_ZERO`) over the entire rendered scene. Since the filter has a cold blue-dominant tint (R=0.566, G=0.712, B=0.811), all in-game colors are pre-compensated using the formula:

```
BASE_COLOR = TARGET_COLOR / CRT_COLOR  (per channel, clamped to 1.0)
```

This ensures the final on-screen colors match the intended palette exactly.

---

## Level Progression

| Level | Change |
|---|---|
| 1 | Standard rules, claim 75% |
| 2 | Fuse activates after 3s of drawing |
| 3 | Qix speed increases to 2.0 |
| 4 | New Sparx spawns every 60s; +1 bonus life |
| 5 | Fast draw speed reduced |
| 6 | Fuse activates after 1s |
| 7 | New Sparx spawns every 30s |
| 8 | Border movement slowed; +1 bonus life |
| 9 | Qix speed increases to 2.5 |
| 10 | Claim 85% to win; victory screen |

---
