@echo off
setlocal
for /f "delims=" %%i in ('git rev-parse --show-toplevel') do set REPO_ROOT=%%i
set "REPO_ROOT=%REPO_ROOT:/=\%"
@echo on

@echo "===================================================="
@echo "Generating build files using Emscripten + CMake..."
@echo "===================================================="
@echo.
emcmake cmake -S "%REPO_ROOT%\client" -B "%REPO_ROOT%\client\builds\web"

@echo.
@echo "===================================================="
@echo "Compiling via CMake..."
@echo "===================================================="
@echo.
cmake --build "%REPO_ROOT%\client\builds\web" --config debug --target GameClient --parallel

