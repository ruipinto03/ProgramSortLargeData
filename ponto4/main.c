//
// Created by rui on 28/03/20.
//

#include "ponto4.h"
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
 //   cria_ficheiro_de_numeros(TAM);

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
    int inicio = 0, status, pid, posicao = -1, fim = tamanho, valorum=0,valordois =tamanho;

    //carregar de um ficheiro
    vetorInt = carregarFicheiro(vetorInt);

    int i,j;

    // Tamanho do buffer para escrever/ler do pipe
    char *socket_path = "/tmp/socket";

    int listenfd,connfd, uds;

    struct sockaddr_un channel_srv;

    struct sockaddr_un channel;

    char buf1[BUFSIZE];


    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {              // Creating the server socket

        perror("socket error");

        exit(-1);

    }

    unlink(socket_path);

    memset(&channel_srv, 0, sizeof(channel_srv));

    channel_srv.sun_family = AF_UNIX;

    strncpy(channel_srv.sun_path, socket_path, sizeof(channel_srv.sun_path)-1);

    memset(&channel, 0, sizeof(channel));

    channel.sun_family= AF_UNIX;

    strncpy(channel.sun_path, socket_path, sizeof(channel.sun_path)-1);

    if (bind(listenfd, (struct sockaddr*)&channel_srv, sizeof(channel_srv)) == -1) {      // Binding the server socket to its name

        perror("bind error");

        exit(-1);

    }

    if (listen(listenfd, NRFILHOS) == -1) {                                  // Configuring the listen queue

        perror("listen error");

        exit(-1);

    }

    printf("Vetor Original\n");

    printArray(vetorInt, TAM);

    printf("Final vetor\n\n");

    for (i = 0; i < NRFILHOS; i++) {

        valorum=inicio;

        valordois=fim;

        vetorIntAux = copiaArray(vetorInt, vetorIntAux, inicio, fim);

        inicio += tamanho; fim += tamanho; posicao++;

        if ((vetorPids[i] = fork()) < 0) {

            perror("Deu erro no fork");

            exit(EXIT_FAILURE);
        }

        if (vetorPids[i] == 0) {

            int iniAux, fimAux;

            vetorAux = realloc(vetorAux,sizeof(int)*TAM);

            merge_sort_td1(vetorIntAux, vetorAux, 0, tamanho-1);

            iniAux = valorum;

            fimAux = valordois-1;

            if ((uds = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {

                perror("socket error");

                exit(1);

            }

            if (connect(uds, (struct sockaddr*)&channel, sizeof(channel)) == -1) {

                perror("connect error");

                exit(1);

            }

            sprintf(buf1,"#%d*%d;%d*",getpid(),iniAux,fimAux);

            writen(uds, buf1, BUFSIZE);

            for (j=0;j<tamanho;j++){

                sprintf(buf1, "%d",vetorIntAux[j]);

                writen(uds, buf1, BUFSIZE);

            }

            sprintf(buf1, "|");

            writen(uds, buf1, BUFSIZE);

            close(uds);

            kill(getppid(), SIGUSR1);

            exit(0);

        }

    }

    int pidfilho, vetorini, vetorfim;

    char *token;

    ssize_t bytes  = 0;

    for (i=0;i<NRFILHOS;i++)
    {
        connfd = accept(listenfd, NULL, NULL);

        bytes = readn(connfd, buf1, BUFSIZE);

        token = strtok(buf1,"#");

        token = strtok(token,"*");

        pidfilho = atoi(token);

        token = strtok(NULL,";");

        vetorini = atoi(token);

        token = strtok(NULL,"*");

        vetorfim = atoi(token);

        while((bytes = readn(connfd, buf1, BUFSIZE))!=0){

            if (strcmp(buf1,"|")!=0){

                vetorInt[vetorini]=atoi(buf1);
                vetorini++;
            }
        }
        close(connfd);
    }

    for (j = 0; j < NRFILHOS; j++) {
        pid = waitpid(vetorPids[j], &status, 0);
        if (WIFEXITED(status)) {
          //  printf("PID %d Status: %d\n", pid, WEXITSTATUS(status));
        }
    }

    int Arrays = NRFILHOS, novosarrays, restoarray;

    int tamvectoraux=0;

    while (Arrays>1){

        novosarrays=Arrays/2;

        restoarray=Arrays%2;

        inicio = 0;

        fim = 0;

        posicao = -1;

        tamanho=tamanho*2;

        for (i = 0; i < novosarrays; i++) {

            if (restoarray==0 && i==novosarrays-1){

                fim = TAM;

                tamvectoraux = fim-inicio;

            }
            else{
                fim += tamanho;

                tamvectoraux = tamanho;
            }

            valorum=inicio;

            valordois=fim;

            vetorIntAux = realloc(vetorIntAux,sizeof(int)*tamvectoraux);

            vetorIntAux = copiaArray(vetorInt, vetorIntAux, inicio, fim);

            inicio = fim;

            posicao++;

            if ((vetorPids[i] = fork()) < 0) {

                perror("Deu erro no fork");

                exit(EXIT_FAILURE);

            }

            if (vetorPids[i] == 0) {

                int iniAux, fimAux;

                vetorAux = realloc(vetorAux,sizeof(int)*tamvectoraux);

                merge_arrays1(vetorIntAux, vetorAux, 0, (tamanho/2)-1, tamvectoraux-1);

                iniAux = valorum;

                fimAux = valordois-1;

                if ((uds = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {

                    perror("socket error");

                    exit(1);

                }

                if (connect(uds, (struct sockaddr*)&channel, sizeof(channel)) == -1) {

                    perror("connect error");

                    exit(1);

                }

                sprintf(buf1,"#%d*%d;%d*",getpid(),iniAux,fimAux);

                writen(uds, buf1, BUFSIZE);

                for (j=0;j<tamanho;j++){

                    sprintf(buf1, "%d",vetorIntAux[j]);

                    writen(uds, buf1, BUFSIZE);

                }

                sprintf(buf1, "|");

                writen(uds, buf1, BUFSIZE);

                close(uds);

                kill(getppid(), SIGUSR1);

                exit(0);
            }
        }

        for (i=0;i<novosarrays;i++){

            connfd = accept(listenfd, NULL, NULL);

            bytes = readn(connfd, buf1, BUFSIZE);

            token = strtok(buf1,"#");

            token = strtok(token,"*");

            pidfilho = atoi(token);

            token = strtok(NULL,";");

            vetorini = atoi(token);

            token = strtok(NULL,"*");

            vetorfim = atoi(token);

            while((bytes = readn(connfd, buf1, BUFSIZE))!=0){

                if (strcmp(buf1,"|")!=0){

                    vetorInt[vetorini]=atoi(buf1);

                    vetorini++;

                }

            }

            close(connfd);

        }

        for(i=0;i<novosarrays;i++){
            pid = waitpid(vetorPids[i], &status, 0);

            if (WIFEXITED(status)) {
          //      printf("PID %d Status: %d\n", pid, WEXITSTATUS(status));

            }

        }

        Arrays=novosarrays+restoarray;

    }

    printf("Vetor Ordenado\n");

    printArray(vetorInt,TAM);

    writeInts("/home/rui/Desktop/apresentacao/ponto4/dadosFinais.txt", vetorInt,TAM);

    exit(EXIT_SUCCESS);

}


