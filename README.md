# nGragasstein 3D (cub3D)
Ray-casting project inspired by the world-famous 90’s game - **[Wolfenstein 3D](https://en.wikipedia.org/wiki/Wolfenstein_3D)**.

Written in **C** with **[MiniLibX](https://github.com/42Paris/minilibx-linux)** graphics/event library *(which is API to X11/X-Window)* and **[cute_sound](https://github.com/RandyGaul/cute_headers/blob/master/cute_sound.h)** audio library.

## How to play:
**Compatibility:**
- **Mac OS** 
- **Linux** *(tested on WSL2 Ubuntu)* 
- **Windows** *(tested on Cygwin)*

**To build: `make bonus`**

*Prereqs: X11 lib + XRender extension, SDL2 core lib*

**To run: `./cub3D game_level.cub`**

*Prereqs: Linux/Windows needs X11 server. For sound on WSL2 - PulseAudio server*

### Bundled game levels:
- **e1m1.cub** (original Wolfenstein 3D first level)
- **horror.cub** (by @mharriso)
- **neon.cub** (by @ckendall)

## "Let's Play" demo video:
https://youtu.be/ujFoM7Q15zM

[![Watch the video](https://img.youtube.com/vi/ujFoM7Q15zM/maxresdefault.jpg)](https://youtu.be/ujFoM7Q15zM)
