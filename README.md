# Getting started

1. Clone the repository
2. Create a build folder at the same level than template. You can name it as you wish (For the next command, consider a folder named `build`)
3. `cd build`
4. `cmake ../MasterCraft`
5. `make`

For the moment the CMake file is configured to compile only sources that are contained in a folder prefixed by `TP`.

To execute MasterCraft :
`./TP_MasterCraft/TP_MasterCraft_main <typeOfWindow>`

typeOfWindow :
- w for Windowed mode
- f for Fullscreen mode

# Extra libraries used

- GLFW (Tested with OSX with the version 3.3 and Ubuntu with the 3.2.1)


# Trivia

- Shaders and assets are copied by the cmake command, you need to `cmake` everytime you modify a shader or an asset.
- glimac has been modified with our classes to perform this MasterCraft.

# Known issues

- The trees are not clipping when distant. 
- Trees are not biome based. You can have very green trees in desert.
- The map loading is made in the main thread, so the game freezes for few seconds when loading new chunks.
- The atlas has 3 free slots of textures. It is not done due to a lack of imagination about biomes.

# In-game commands

- The key "E" is used to cycle between day and night.
- The key "F" is used to enable flight and cancel it.
- The key "ESC" is used to quit the game.


# LaraCraft

<img src="https://i.imgur.com/fNwcfH3.png" height="400">
