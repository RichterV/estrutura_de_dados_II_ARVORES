#ifndef GRAFOLISTA_H
#define GRAFOLISTA_H

typedef struct GrafoL{
    int id;                    // identificação única
    struct GrafoL* proximo;            // identifica o próximo nó da lista
    float peso;                // para grafos ponderados, é preciso armazenar o peso do grafo
    char nome[100];            // lista de dados a serem armazenados
} GrafoL;

//PRINCIPAIS
void inicializarGrafoLista(int, int);
void adicionarVerticeLista(int, char*);
void adicionarArestaLista(int, int, float);
void mostrarGrafoLista();
void descobrirCaminhoLista(int, int);
void removerArestaLista(int, int);
void removerVerticeLista(int);

//AUXILIARES
int existeVerticeLista(int);
int obterListaIDDosVerticesLista(int*);

#endif //GRAFOLISTA_H