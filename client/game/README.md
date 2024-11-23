# Game Client

Written in C++ and built on top of SDL, CMake and Emscripten to build a web and a desktop version of the game.

## Setup

Make sure all submodules has been cloned properly.

To set up SDL_image correctly, go to `/externals/SDL_image/external` and run the `.\Get-GitModules.ps1` or the `download.sh` script depending on your operating system.

## Building the Game

Follow the steps listed below depending on what target you wish to build. All build targets also copy over the resources to the same directory as the output executable.

### Web Build

To build to web on Windows, first set up the terminal by running the `client\tools\emsdk\emcmdprompt.bat` batch script.

After this has succeeded, run the `client\tools\scripts\build-web-win.bat` script.

To run the development web build, you can run the following command `emrun index.html` in `client/builds/web`.

### Windows Build

To build the game for Windows, run the `client\tools\scripts\build-win.bat` batch script.
