//
// Created by rui on 28/03/20.
//

#include "ponto12.h"
#include "funcoesAuxiliares.h"

/**
 * Trata o sinal que recebe
 * @param signal_number
 */
void handler(int signal_number) {
    switch (signal_number) {
        case SIGUSR1:
            printf("Foi organizado o vetor\n");
            //  exit(0);
        default:
            //  printf("Received a %d signal...\n", signal_number);
            break;
    }
}

/**
 * Funcao main onde esta escrito a o programa
 * @param argc
 * @param argv
 */
void main(int argc, char **argv) {
    // cria_ficheiro_de_numeros(TAM); // funcao para criar um ficheiro de inteiros

    //declaracao de estrutura sigaction
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    // declaracao de pids;
    pid_t vetorPids[NRFILHOS];

    //alocação de memoria
    int *vetorInt = (int *) malloc(sizeof(int) * TAM); // vetor original

    int tamanho = TAM / NRFILHOS;

    int *vetorIntAux = (int *) malloc(sizeof(int) * tamanho); // vetor final que vai ser imprimido depois de ser organizado

    int *vetorAux = (int *) malloc(sizeof(int) * tamanho); // vetor auxiliar para usar no merge

    //declaracao de variaveis de auxilio
    int inicio = 0, status, pid, posicao = -1, fim = tamanho, valorum = 0, valordois = tamanho;

    //carregar de um ficheiro
    vetorInt = carregarFicheiro(vetorInt);


    printf("Vetor Desorganizado\n");

    printArray(vetorInt, TAM);

    printf("Final do Vetor \n\n");


    for (int i = 0; i < NRFILHOS; i++) {

        valorum = inicio, valordois = fim;

        vetorIntAux = copiaArray(vetorInt, vetorIntAux, inicio, fim);

        inicio += tamanho, fim += tamanho, posicao++;

        if ((vetorPids[i] = fork()) < 0) {

            perror("Deu erro no fork");

            exit(EXIT_FAILURE);

        }

        if (vetorPids[i] == 0) { // funcao caso seja o filho

            merge_sort_td1(vetorIntAux, vetorAux, 0, tamanho); // organiza o vetor copia

            gravaFicheiro(&posicao, vetorIntAux, 0, tamanho - 1, valorum, valordois); // grava o ficheiro txt com o vetor organizado

            kill(getpid(), SIGUSR1); // manda sinal para terminar o filho

            exit(0);

        }

    }

    /*Espera que os filhos terminem*/
    for (int j = 0; j < NRFILHOS; j++) {

        pid = waitpid(vetorPids[j], &status, 0);

        if (WIFEXITED(status)) {

            printf("PID %d Status: %d\n", pid, WEXITSTATUS(status));

        }

    }

}