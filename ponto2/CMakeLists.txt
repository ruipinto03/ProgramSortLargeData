cmake_minimum_required(VERSION 3.15)
project(ponto2 C)

set(CMAKE_C_STANDARD 99)
file(
        GLOB SOURCES
        main.c
        *.c
        *.h
)



add_executable(ponto2 main.c funcoesAuxiliares.h funcoesAuxiliares.c ponto12.h )