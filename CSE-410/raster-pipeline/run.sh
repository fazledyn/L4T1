#!/usr/bin/bash

echo "Cleaning up output files"

rm ./test_case/1/my_*
rm ./test_case/2/my_*
rm ./test_case/3/my_*
rm ./test_case/4/my_*

echo "Cleanup completed"

g++ main.cpp -o program.out
./program.out
