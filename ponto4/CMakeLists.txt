cmake_minimum_required(VERSION 3.15)
project(ponto4 C)

set(CMAKE_C_STANDARD 99)
file(
        GLOB SOURCES
        main.c
        *.c
        *.h
)
add_executable(ponto4 main.c funcoesAuxiliares.c funcoesAuxiliares.h)
