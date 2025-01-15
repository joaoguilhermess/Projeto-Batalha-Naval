@echo off
rem g++ main.cpp -o game.exe -ISDL2/include/SDL2 -lmingw32 -lSDL2main -lSDL2 -LSDL2/lib
g++ main.cpp console.cpp rules.cpp game.cpp credits.cpp -o game.exe
start game.exe