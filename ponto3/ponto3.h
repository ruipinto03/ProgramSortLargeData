//
// Created by rui on 28/03/20.
//

#ifndef PROJECTO_PONTO3_H
#define PROJECTO_PONTO3_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#define NRFILHOS 10
#define TAM 1000
#define BUFSIZE 4096
char * retorna_protocolo_string(int pid,int inicio,int fim,char * vec);
#endif //PROJECTO_PONTO3_H
