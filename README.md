# FDS_SGF
  A "Simpe as Hell" framework for game dev in C++.

vendors:
- [glm](https://github.com/g-truc/glm)
- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)
- [spdlog](https://github.com/gabime/spdlog)
- [nlohmann_json](https://github.com/nlohmann/json)

What did this repo do?
- A "Simpe as Hell" framework for game dev in C++.
- A prepared project for Visual Studio Code.
- Prepared libs and dlls for distribution.

How to start?
- Clone this repo.
- Open the project in Visual Studio Code.
- Config CMake.
- Build and run.
- (I use the MSVC compiler, you can use any compiler you like but you need to change the compiler settings in CMake.)

How to use?
- Mostly you are interested in the `src` folder.
- `src/core/` contains the framework stuff.
- `src/game/` contains the actual game contents.
- Well, the singleton Game class wants a Scene with our lovely three-piece(handleEvents, update, render). Modify your own Scene hired from the interface Scene class and plug it to the Game instance.

- in `src/main.cpp` you can find the entry point of the game.
- in `src/core/Game.h(.cpp)` you can find the singleton game class and the main game loop.
- DO notice the comments in the source code for more information.

- A demo was provided in `src/game/` where you can learn about how to use the framework.

- in `res/` you can find the resources used in the game, however, you are not supposed to use them for your own purposes.

- You may need to modify the paths in the code.(Well maybe not, but it's better to check it out.)

// 2025 Fordans
