cmake_minimum_required(VERSION 3.30)
project(sudoku C)

set(CMAKE_C_STANDARD 11)

add_executable(sudoku main.c
        engine.h
        solver.h)
