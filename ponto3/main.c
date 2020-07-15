//
// Created by rui on 28/03/20.
//

#include "ponto3.h"
#include "funcoesAuxiliares.h"

/**
 * Trata o sinal que recebe
 * @param signal_number
 */
void handler(int signal_number) {
    switch (signal_number) {
        case SIGUSR1:
            //  printf("Foi organizado o vetor\n");
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

    // cria_ficheiro_de_numeros(TAM);

    //declaracao de estrutura sigaction
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    // declaracao de pids;

    pid_t vetorPids[NRFILHOS];

    //alocação de memoria
    int *vetorInt = (int *) malloc(sizeof(int) * TAM);

    int tamanho = TAM / NRFILHOS;

    int *vetorIntAux = (int *) malloc(sizeof(int) * tamanho);

    int *vetorAux = (int *) malloc(sizeof(int) * tamanho);

    //declaracao de variaveis de auxilio

    int inicio = 0, status, pid, posicao = -1, fim = tamanho, valorum = 0, valordois = tamanho;

    //carregar de um ficheiro

    vetorInt = carregarFicheiro(vetorInt);

    int i, j;

    /**Criacao de um pipe*/
    int fd[2];

    if (pipe(fd) < 0) {
        perror("Pipe -> Cano esta estragado!");
        exit(EXIT_FAILURE);
    }

    // Tamanho do buffer para escrever/ler do pipe
    char buf1[BUFSIZE], buf1aux[BUFSIZE];

    //   uniformDistinctArray(vetorInt, TAM, 0, TAM);

    printf("Vetor Desorganizado\n");

    printArray(vetorInt, TAM);

    printf("Final do Vetor \n\n");


    /**
     * Organizar apartir do filho enviar para o pipe
     */
    for (i = 0; i < NRFILHOS; i++) {

        valorum = inicio;

        valordois = fim;

        vetorIntAux = copiaArray(vetorInt, vetorIntAux, inicio, fim);

        //printArray(vetorIntAux, tamanho - 1);

        inicio += tamanho;
        fim += tamanho;
        posicao++;

        if ((vetorPids[i] = fork()) < 0) {

            perror("Deu erro no fork");


            exit(EXIT_FAILURE);

        }

        if (vetorPids[i] == 0) {

            close(fd[0]);

            int iniAux, fimAux;

            char bufAux[BUFSIZE], bufIni[BUFSIZE];

            vetorAux = realloc(vetorAux, sizeof(int) * TAM);

            merge_sort_td1(vetorIntAux, vetorAux, 0, tamanho - 1);

            iniAux = valorum; //

            fimAux = valordois - 1; //

            sprintf(buf1aux, "*");

            sprintf(bufIni, "#%d*%d;%d", getpid(), iniAux, fimAux);

            sprintf(bufAux, "%s*", bufIni);

            for (j = 0; j < tamanho; j++) {

                sprintf(bufAux, "%s%d%s", bufAux, vetorIntAux[j], ",");

                if (strlen(bufAux) > BUFSIZE) { // Esta funcao é para verificar se caso o tamanho do buf seja superior ao buf do pipe tem que se dividir

                    sprintf(bufIni, "#%d*%d;%d", getpid(), iniAux, j - 1);

                    sprintf(buf1, "%s%s|", bufIni, buf1aux);

                    writen(fd[1], buf1, BUFSIZE);

                    iniAux = j;

                    sprintf(bufIni, "#%d*%d;%d", getpid(), iniAux, fimAux);

                    sprintf(bufAux, "%s*%d%s", bufIni, vetorIntAux[j], ",");

                    sprintf(buf1aux, "*");

                }

                sprintf(buf1aux, "%s%d%s", buf1aux, vetorIntAux[j], ",");
            }

            sprintf(buf1, "%s%s|", bufIni, buf1aux);

            writen(fd[1], buf1, BUFSIZE);

            close(fd[1]); // fecha se o descritor de escrita do pipe apos ele ter enviado tudo

            kill(getpid(), SIGUSR1);

            exit(0);

        }

    }

    close(fd[1]);

    int pidfilho, vetorini, vetorfim; // manipular o protocolo

    char *token;

    ssize_t bytes = 0;

    while ((bytes = readn(fd[0], buf1, BUFSIZE)) != 0) {

        token = strtok(buf1, "#");

        token = strtok(token, "*");

        pidfilho = atoi(token);

        token = strtok(NULL, ";");

        vetorini = atoi(token);

        token = strtok(NULL, "*");

        vetorfim = atoi(token);

        token = strtok(NULL, ",");

        while (strcmp(token, "|") != 0) {

            vetorInt[vetorini] = atoi(token);

            vetorini++;

            token = strtok(NULL, ",");

        }

    }

    /**
     * Esperar que o filho termine
     */
    for (j = 0; j < NRFILHOS; j++) {

        pid = waitpid(vetorPids[j], &status, 0);

        if (WIFEXITED(status)) {

            //   printf("PID %d Status: %d\n", pid, WEXITSTATUS(status));

        }

    }

    close(fd[0]);


    int Arrays = NRFILHOS, novosarrays, restoarray;

    int tamvectoraux = 0;   for (j = 0; j < NRFILHOS; j++) {

        pid = waitpid(vetorPids[j], &status, 0);

        if (WIFEXITED(status)) {

            //   printf("PID %d Status: %d\n", pid, WEXITSTATUS(status));

        }

    }

    while (Arrays > 1) {

        if (pipe(fd) < 0) {

            perror("Pipe -> Cano esta estragado!");

            exit(EXIT_FAILURE);

        }

        novosarrays = Arrays / 2; // para saber quantos filhos sao

        restoarray = Arrays % 2; // e para saber se é par ou impar

        inicio = 0;

        fim = 0;

        posicao = -1;

        tamanho = tamanho * 2; // vai receber o dobro do antigo tamanho porque vai receber dois arrays

        for (i = 0; i < novosarrays; i++) { // ciclo para o numero de filhos

            if (restoarray == 0 && i == novosarrays - 1) { // para ver sobras. 100

                fim = TAM;

                tamvectoraux = fim - inicio;

            } else { // caso seja par

                fim += tamanho;

                tamvectoraux = tamanho;

            }

            valorum = inicio;

            valordois = fim;

            vetorIntAux = realloc(vetorIntAux, sizeof(int) * tamvectoraux);

            vetorIntAux = copiaArray(vetorInt, vetorIntAux, inicio, fim);

            inicio = fim;

            posicao++;

            if ((vetorPids[i] = fork()) < 0) {

                perror("Deu erro no fork");

                exit(EXIT_FAILURE);

            }

            if (vetorPids[i] == 0) {

                close(fd[0]);

                int iniAux, fimAux;

                char bufAux[BUFSIZE], bufIni[BUFSIZE];

                vetorAux = realloc(vetorAux, sizeof(int) * tamvectoraux);

                merge_arrays1(vetorIntAux, vetorAux, 0, (tamanho / 2) - 1, tamvectoraux - 1);

                iniAux = valorum;

                fimAux = valordois - 1;

                sprintf(buf1aux, "*");

                sprintf(bufIni, "#%d*%d;%d", getpid(), iniAux, fimAux);

                sprintf(bufAux, "%s*", bufIni);

                for (j = 0; j < tamvectoraux; j++) {

                    sprintf(bufAux, "%s%d%s", bufAux, vetorIntAux[j], ",");

                    if (strlen(bufAux) > BUFSIZE) {

                        sprintf(bufIni, "#%d*%d;%d", getpid(), iniAux, j - 1);

                        sprintf(buf1, "%s%s|", bufIni, buf1aux);

                        writen(fd[1], buf1, BUFSIZE);

                        iniAux = j;

                        sprintf(bufIni, "#%d*%d;%d", getpid(), iniAux, fimAux);

                        sprintf(bufAux, "%s*%d%s", bufIni, vetorIntAux[j], ",");

                        sprintf(buf1aux, "*");

                    }

                    sprintf(buf1aux, "%s%d%s", buf1aux, vetorIntAux[j], ",");

                }

                sprintf(buf1, "%s%s|", bufIni, buf1aux);

                writen(fd[1], buf1, BUFSIZE);

                close(fd[1]);

                kill(getpid(), SIGUSR1);

                exit(0);

            }

        }

        close(fd[1]);

        while ((bytes = readn(fd[0], buf1, BUFSIZE)) != 0) {

            token = strtok(buf1, "#");

            token = strtok(token, "*");

            pidfilho = atoi(token);

            token = strtok(NULL, ";");

            vetorini = atoi(token);

            token = strtok(NULL, "*");

            vetorfim = atoi(token);

            token = strtok(NULL, ",");

            while (strcmp(token, "|") != 0) {

                vetorInt[vetorini] = atoi(token);

                vetorini++;

                token = strtok(NULL, ",");

            }

        }

        for (i = 0; i < novosarrays; i++) {

            pid = waitpid(vetorPids[i], &status, 0);

            if (WIFEXITED(status)) {

                //  printf("PID %d Status: %d\n", pid, WEXITSTATUS(status));

            }

        }

        close(fd[0]);

        Arrays = novosarrays + restoarray;

    }

    printf("Vetor Ordenado\n");

    printArray(vetorInt, TAM);

    printf("Final do Vetor \n\n");

    writeInts("ArrayOrdenado.txt", vetorInt, TAM);

    exit(EXIT_SUCCESS);

}
