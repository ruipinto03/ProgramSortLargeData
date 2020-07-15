//
// Created by rui on 13/04/20.
//

#include "funcoesAuxiliares.h"

/**
 * Recebe um vetor e o seu tamanho e imprime no ecra
 * @param A vetor
 * @param size o seu tamanho
 */
void printArray(int A[], int size) {
    int i, count = 0;
    for (i = 0; i < size; i++) {
        if (count == 10) {
            printf("\n");
            count = 0;
        }
        printf("%d ", A[i]);
        count++;
    }
    printf("\n\n");
}

/**
 * Preenche um array recebe um valor minimo e maximo
 * @param val_min valor minimo
 * @param val_max valor maximo
 * @return
 */
int uniform(int val_min, int val_max) {
    return val_min + rand() % (val_max - val_min + 1);
}

/**
 * Aloca memoria para um array de char
 * @param N recebe o tamanho
 * @return retorna o espaco de memoria de um array
 */
char *newCharArray(int N) {
    return (char *) malloc(sizeof(char) * N);
}/**
 * Aloca memoria para um array de int
 * @param N recebe o tamanho
 * @return retorna o espaco de memoria de um array
 */
int *newIntArray(int N) {
    return (int *) malloc(sizeof(int) * N);
}

/**
 * Liberta o espaço de memoria reservado para um vetor de char's
 * @param v vetor
 */
void freeCharArray(char *v) {
    free(v);
}

/**
 * Preenche um array com valores destintos
 * @param a e o array
 * @param N tamanho
 * @param val_min valor minimo
 * @param val_max valor maximo
 * @return
 */
int uniformDistinctArray(int *a, int N, int val_min, int val_max) {
    int i, value, range = val_max - val_min + 1;
    char *b = newCharArray(range);
    for (i = 0; i < range; i++)
        b[i] = 0;
    for (i = 0; i < N; i++) {
        do {
            value = uniform(val_min, val_max);

        } while (b[value - val_min] != 0);
        b[value - val_min] = 1;
        a[i] = value;
    }
    freeCharArray(b);
    return 0;
}

/**
 * Funcao de merge sort
 * @param a
 * @param aux
 * @param lo
 * @param hi
 */
void merge_sort_td1(int a[], int aux[], int lo, int hi) {
    int mid = lo + (hi - lo) / 2;
    if (hi <= lo) return;
    merge_sort_td1(a, aux, lo, mid);
    merge_sort_td1(a, aux, mid + 1, hi);
    merge_arrays1(a, aux, lo, mid, hi);
}

/**
 * Faz merge entre dois arrays
 * @param a recebe o original
 * @param aux array auxiliar com o mesmo tamanho
 * @param lo valor minimo
 * @param mid valor medio
 * @param hi valor final
 */
void merge_arrays1(int a[], int aux[], int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k;

    for (k = lo; k <= hi; k++) // faz a copia da origem para o auxiliar
        aux[k] = a[k];
// Aqui os arrays ja estao ordenados é só uma funcao para juntar por ondem na oridem
    for (k = lo; k <= hi; k++) { // merge
        if (i > mid) a[k] = aux[j++];// verifica se o inicio ja ultrapassou o valor meio+1
        else if (j > hi) a[k] = aux[i++];// verifica se o meio+1 ja ultrapassou o valor do tamanho
        else if (aux[j] < aux[i]) a[k] = aux[j++];// verifica se o conteudo aux[meio+1] é maior que aux[inicio]
        else a[k] = aux[i++];// verifica se o conteudo aux[meio+1] é menor que aux[inicio]
    }
}

/**
 * Funcao para gravar o ficheiro de texto
 * @param nrFicheiro
 * @param vecNumeros
 * @param lo
 * @param hi
 * @param inicio
 * @param fim
 */
void gravaFicheiro(int *nrFicheiro, int *vecNumeros, int lo, int hi, int inicio, int fim) {
    int count = *nrFicheiro;
    int count1 = count + 1;
    *nrFicheiro = count1;
    char vec1[] = "/home/rui/Desktop/apresentacao/ponto2/ficheirosDivididos/";
    char vec2[] = ".txt";
    char path[100];
    sprintf(path, "%s%d%s", vec1, *nrFicheiro, vec2);
    int count3 = 10;
    FILE *f;
    if ((f = fopen(path, "w")) == NULL) {
        printf("Erro abrir o ficheiro\n");
        return;
    }

    int i;
    for (i = 0; i < hi - lo + 1; i++) {
        fprintf(f, "%d", vecNumeros[i]);

        if (i < hi - lo) {
            fprintf(f, "%c", ';');
        }
        if (count3-1 == i) {
            fprintf(f, "%c", '\n');
            count3 += 10;
        }
    }
    //  fprintf(f, "%c", '|');
    fclose(f);
}

/**
 * Faz copia de um array
 * @param array
 * @param aux
 * @param inicio
 * @param fim
 * @return
 */
int *copiaArray(int *array, int *aux, int inicio, int fim) {
    int j = 0, i;
    for (i = inicio; i < fim; i++) {
        *(aux + j) = *(array + i);
        j++;
    }
    return aux;
}

int *carregarFicheiro(int *vetor) {
    ssize_t n;
    int fd, x = 0, aux;
    char buf[1];
    int count = 0;
    //if ((fd = open("/home/rui/Desktop/apresentacao/ponto3/dados.txt", O_CREAT | O_RDWR, 0644)) < 0) {
    if ((fd = open("/home/rui/Desktop/apresentacao/ponto3/dados1.txt", O_CREAT | O_RDWR, 0644)) < 0) {
        perror("Erro na abertura do ficheiro");
        exit(EXIT_FAILURE);
    }
    while ((n = read(fd, buf, sizeof(buf))) != 0) {
        if (strcmp(buf, ";") == 0) {
            *(vetor + x) = count;
            count = 0;
            x++;
        } else {
            aux = atoi(buf);
            count = (count * 10) + aux;
        }
    }
    return vetor;
}

ssize_t /* Read "n" bytes from a descriptor */
readn(int fd, void *ptr, size_t n) {
    size_t nleft;
    ssize_t nread;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return (-1); /* error, return -1 */
            else
                break; /* error, return amount read so far */
        } else if (nread == 0) {
            break; /* EOF */
        }
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft); /* return >= 0 */
}

ssize_t /* Write "n" bytes to a descriptor */
writen(int fd, const void *ptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return (-1); /* error, return -1 */
            else
                break; /* error, return amount written so far */
        } else if (nwritten == 0) {
            break;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n - nleft); /* return >= 0 */
}

int *ficheiroDados(char *path, int *vetor) {
    ssize_t n;
    int fd, x = 0, aux;
    char buf[BUFSIZE];
    char *token;
    if ((fd = open(path, O_CREAT | O_RDWR, 0644)) < 0) {
        perror("Erro na abertura do ficheiro");
        exit(EXIT_FAILURE);
    }
    while ((n = read(fd, buf, 1024)) != 0) {
        token = strtok(buf, ";");
        while (token != NULL) {
            aux = atoi(token);
            //   printf("%d\n",aux);
            *(vetor + x) = aux;
            token = strtok(NULL, ";");
            x++;
        }
    }
    return vetor;
}

void enviaMSG(int fd, char *msg) {
    if ((writen(fd, msg, sizeof(msg))) < 0) {
        perror("Erro de escrita no pipe");
        exit(EXIT_FAILURE);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}

char *convertArrayIntToString(int *vec, int tamanho) {
    char *aux = newCharArray(tamanho);
    int i;
    for (i = 0; i < tamanho; i++) {
        sprintf((aux + i), "%d", vec[i]);
        printf("%c", *aux + i);
    }
    return aux;
}

int writeInts(char *filename, int *intvec, int n) {
    FILE *fp;
    int i = 0, count = 10;
    char *quebra = "\n";
    fp = fopen(filename, "w");
    if (fp != NULL) {
        while (i < n) {
            fprintf(fp, "%d ", intvec[i++]);
            if (i == count) {
                fprintf(fp, "%s", quebra);
                count += 10;
            }
        }
        fclose(fp);
    } else return -1; // erro na leitura do ficheiro
    return 0;
}