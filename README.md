# Tetris game

## Using SDL2 and C++ to make a tetris game :)

## To start, run the executable

## Controls

 - `q`, to quit
 - `p`, to pause/unpause (no indicator though)
 - Arrow Up, to rotate
 - Arrow Down, to soft drop
 - Arrow Right, to soft move right
 - Arrow Left, to soft move left
 - Spacebar, to hard drop

## Build

- Install SDL2
- Install GLEW
- `mkdir build`
- `cd build`
- `cmake -G <whatever CMake generator you want> ../`
- Build using the build system that you generated

## Considerations

- This was only tested and ran on Linux and not on any other platforms such as Windows or MacOS
- There is no scoring system.
- There is no sound.
- No starting menu.
- No pause screen.
- No adjustable settings.
- No textures.

## TODO -- use glslang to go from glsl to spirv and then use spirv-cross to decompile spirv to wgsl and learn webgpu
