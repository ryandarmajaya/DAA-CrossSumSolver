Requirement :

- Raylib (to compile library)


Install [here](https://github.com/raysan5/raylib/releases)


- Find suitable releases based on 


Linux :

- MingGW

```
sudo apt update
sudo apt install g++-mingw-w64-x86-64
```

Windows :

- MingGW

```
g++ ../main.cpp ../solver.cpp ../utils.cpp -o testing.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
```

