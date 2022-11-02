@echo off

gcc translator.c -o translator
translator.exe %1
gcc brainfuck.c -o %2

del translator.exe
del brainfuck.c