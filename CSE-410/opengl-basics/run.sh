#!/usr/bin/bash
rm *.out
g++ $1 -o app.out -lglut -lGLU -lGL
./app.out
