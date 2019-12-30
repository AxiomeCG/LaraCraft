# LaraCraft
Minecraft clone in OpenGL (C++)

<img src="https://i.imgur.com/fNwcfH3.png" height="400">

# Getting started

1. Clone the repository
2. Create a build folder at the same level than template. You can name it as you wish (For the next command, consider a folder named `build`)
3. `cd build`
4. `cmake ../template`
5. `make`

Now you have a bunch of `TP_...` folder containing the different compiled examples/projects.
For the moment the CMake file is configured to compile only sources that are contained in a folder prefixed by `TP`.

# Trivia

- Shaders are copied by the cmake command, you need to `cmake` everytime you modify a shader.
- `glupem` is the lib based on glimac that is used to conduct the project. 

