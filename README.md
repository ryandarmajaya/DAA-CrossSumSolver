# Cross Sum Solver

---

## Table of Contents

- [Running the Game](#running-the-game)
- [Building from Source](#building-from-source)

---

## Requirements

### Windows
- [MinGW-w64](https://sourceforge.net/projects/mingw-w64/) — C++ compiler for Windows
- Raylib (included in `compiler/lib/` and `compiler/include/`)

### Linux
- **Wine** — to run the pre-compiled `.exe` file
  ```bash
  sudo apt install wine
  ```
- **MinGW-w64 cross-compiler** — only needed if you want to build from source
  ```bash
  sudo apt update
  sudo apt install g++-mingw-w64-x86-64
  ```

---

## Running the Game

A pre-compiled `game.exe` is already included in the project root. No build step is required to run it.

### Windows

```bat
.\game.exe
```

### Linux

> Using wine to run windows executable file in linux

```bash
wine game.exe
```

---

## Building from Source

Follow these steps if you want to recompile the game yourself.

> **Note:** The required Raylib headers and libraries are already bundled under `compiler/include/` and `compiler/lib/`. You do not need to download Raylib separately unless you want a different version.
>
> If you do need a different Raylib release, download it from:  
> https://github.com/raysan5/raylib/releases

### Project Structure

```
.
├── build/
│   ├── include/        # Raylib headers
│   └── lib/            # Raylib static/dynamic libraries
│   └── main.cpp
│   └── solver.cpp
│   └── solver.h
│   └── utils.cpp
│   └── utils.h
└── game.exe
```

### Windows

**Prerequisites:** MinGW-w64 installed and added to your system `PATH`. Check 

```bat
g++ main.cpp solver.cpp utils.cpp -o game.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
```

### Linux (Cross-compile for Windows)

**Prerequisites:** `g++-mingw-w64-x86-64` installed (see [Requirements](#requirements)).

```bash
x86_64-w64-mingw32-g++ main.cpp solver.cpp utils.cpp -o game.exe -I./compiler/include -L./compiler/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
```
