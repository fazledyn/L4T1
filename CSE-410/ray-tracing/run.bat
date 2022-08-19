@echo off

cls
del main.exe
del *.bmp
g++ 1705066_main.cpp -o main.exe -lopengl32 -lglu32 -lfreeglut
main.exe
