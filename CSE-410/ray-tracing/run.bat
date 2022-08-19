@echo off

del main.exe
g++ 1705066_main.cpp -o main.exe -lopengl32 -lglu32 -lfreeglut
main.exe