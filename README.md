# chippy
Just a simple Chip-8 emulator written in C++ and uses SDL2. Dumb name I know

## Status
* As of now, Chip-8 opcodes fully implemented
* Only exception is sound. No sound plays at the moment (will add soon)

## How to Build
### First download submodules if you don't have them!
```
git submodule init
git submodule update
```
### Now we build! (Tested on Ubuntu 20.04 LTS)
```
mkdir build
cd build
cmake ..
cmake --build
```
## Usage
```
./chippy <path to CHIP-8 rom file>
```

## Example program running (Sierpinski triangle)
![Alt text](images/example.png?raw=true "Example program")
