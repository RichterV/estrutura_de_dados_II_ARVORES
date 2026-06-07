#ifndef GRAFOGRAFICA_H
#define GRAFOGRAFICA_H

typedef struct GrafoG{
    int id;                            // identificação única
    struct GrafoG* listaVizinhos[100]; // lista de adjacências com grau definico
    int qntdVizinhos;                  // controla a quantidade de vizinhos existentes
    float listaPesos[100];             // para grafos ponderados, é preciso armazenar o peso das arestas
    char nome[100];                    // lista de dados a serem armazenados
} GrafoG;

//PRINCIPAIS
void inicializarGrafoGrafica(int, int);
void adicionarVerticeGrafica(int, char*);
void adicionarArestaGrafica(int, int, float);
void removerArestaGrafica(int, int);
void removerVerticeGrafica(int);
void mostrarGrafoGrafica();
void descobrirCaminhoGrafica(int, int);

//AUXILIARES
int existeVerticeGrafica(int);
int obterListaIDDosVerticesGrafica(int*);

#endif //GRAFOGRAFICA_H