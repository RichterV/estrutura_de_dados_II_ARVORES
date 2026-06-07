#ifndef GRAFOMATRIZ_H
#define GRAFOMATRIZ_H

typedef struct GrafoM{
    int id;                    // identificação única
    float peso;                // para grafos ponderados, é preciso armazenar o peso do grafo
    char nome[100];            // lista de dados a serem armazenados
} GrafoM;

//PRINCIPAIS
void inicializarGrafoMatriz(int, int);
void adicionarVerticeMatriz(int, char*);
void adicionarArestaMatriz(int, int, float);
void mostrarGrafoMatriz();
void descobrirCaminhoMatriz(int, int);
void removerArestaMatriz(int, int);
void removerVerticeMatriz(int);

//AUXILIARES
int existeVerticeMatriz(int);
int obterListaIDDosVerticesMatriz(int*);

#endif //GRAFOMATRIZ_H