@echo off
set SDL_PATH=..\..\..
set INCLUDE_PATH=%SDL_PATH%\include
set LIB_PATH=%SDL_PATH%\lib\x64

gcc -I"%INCLUDE_PATH%" main.c audio.c renderer.c game_logic.c -L"%LIB_PATH%" -lSDL2main -lSDL2 -lSDL2_mixer -o running_man.exe

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Run with: running_man.exe
) else (
    echo Compilation failed!
)
pause