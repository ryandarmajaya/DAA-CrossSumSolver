# Cross Sum Solver 

---

# Requirement

- Raylib (to compile the library)

## Install

Download Raylib releases here:  
https://github.com/raysan5/raylib/releases

Pilih release yang sesuai dengan OS kamu (Windows/Linux).


## Linux

### MingGW

```bash
sudo apt update
sudo apt install g++-mingw-w64-x86-64
```

## Compile

```
x86_64-w64-mingw32-g++ main.cpp solver.cpp utils.cpp -o game.exe -I./include -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
```






## Windows

### MingGW
```
https://sourceforge.net/projects/mingw-w64/
```

### Compile
```
g++ main.cpp solver.cpp utils.cpp -o game.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
```

### How to run
- Buka folder tempat game.exe
- Jalankan dengan command ini, di linux:
```
wine game.exe
```

- di Windows:
```
.\game.exe
```
