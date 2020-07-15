//
// Created by rui on 13/04/20.
//

#ifndef PROJECTO_FUNCOESAUXILIARES_H
#define PROJECTO_FUNCOESAUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "ponto4.h"
int uniform(int val_min, int val_max);

char *newCharArray(int N);
int *newIntArray(int N);

void freeCharArray(char *v);

int uniformDistinctArray(int *a, int N, int val_min, int val_max);

void printArray(int A[], int size);


void merge_arrays1(int a[], int aux[], int lo, int mid, int hi);

void merge_sort_td1(int a[], int aux[], int lo, int hi);

void gravaFicheiro(int *nrFicheiro, int *vecNumeros, int lo, int hi,int inicio,int fim) ;
int * copiaArray(int * array,int *aux,int inicio,int fim);
void cria_ficheiro_de_numeros(int numero_dados);

int * carregarFicheiro(int * vetor);
ssize_t writen(int fd, const void *prt,size_t n);
ssize_t readn(int fd, void *prt,size_t n);
int * ficheiroDados(char * path,int * vetor);
void enviaMSG(int fd , char * msg);
char * convertArrayIntToString(int * vec,int tamanho);
void cria_ficheiro_de_numeros(int numero_dados);
int writeInts(char * filename, int * intvec, int n);
#endif //PROJECTO_FUNCOESAUXILIARES_H
