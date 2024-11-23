@echo off
for /f "delims=" %%i in ('git rev-parse --show-toplevel') do set REPO_ROOT=%%i
set "REPO_ROOT=%REPO_ROOT:/=\%"
@echo on

@echo "===================================================="
@echo "Generating build files with CMake..."
@echo "===================================================="
@echo.
cmake -S "%REPO_ROOT%\client" -B "%REPO_ROOT%\client\builds\win"

@echo "===================================================="
@echo "Compiling via CMake..."
@echo "===================================================="
@echo.
cmake --build "%REPO_ROOT%\client\builds\win" --config debug --target GameClient --parallel