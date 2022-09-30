# nGragasstein 3D (cub3D)
Ray-casting project inspired by the world-famous 90’s game - **[Wolfenstein 3D](https://en.wikipedia.org/wiki/Wolfenstein_3D)**.

Written in **C** with **[MiniLibX](https://github.com/42Paris/minilibx-linux)** graphics/event library *(which is API to X11/X-Window)* and **[cute_sound](https://github.com/RandyGaul/cute_headers/blob/master/cute_sound.h)** audio library.

## How to play:
**Compatibility:**
- Mac OS 
- Linux *(tested on WSL2 Ubuntu)* 
- Windows *(tested on Cygwin)*
- Windows 10 WSL2
- Windows 11 WSLg

**Known bugs:**
- Windows Cygwin: no sound
- Windows 10 WSL2: sound lags behind by 1 sec
- Windwos 11 WSLg: crazy mouse. To disable mouse comment line `initialize_bonus.c`-`initialize_values()`:\
`// game->key.mouse = true`

**To build: `make bonus`**

Compiler: **Clang**\
Required libs: **libxext-dev, libxrender-dev, libsdl2-dev**

**To run: `./cub3D game_level.cub`**

Windows 10 / WSL2 needs X11 server (for sound on WSL2 - PulseAudio server)

### Bundled game levels:
- **e1m1.cub** (original Wolfenstein 3D first level)
- **horror.cub** (by @mharriso)
- **neon.cub** (by @ckendall)

## "Let's Play" demo video:
https://youtu.be/ujFoM7Q15zM

[![Watch the video](https://img.youtube.com/vi/ujFoM7Q15zM/maxresdefault.jpg)](https://youtu.be/ujFoM7Q15zM)
