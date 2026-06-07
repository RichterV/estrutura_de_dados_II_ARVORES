#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GrafosMatriz.h"

// variáveis globais locais
static int EH_PONDERADO;
static int EH_DIRIGIDO;
static int QUANTIDADE_VERTICES;
static GrafoM vertices[100];
static float matriz[100][100]; // -1.0 = sem aresta

// ──────────────── AUXILIARES ────────────────

static int posicaoDoVertice(int id){
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(vertices[i].id == id) return i;
    }
    return -1;
}

int existeVerticeMatriz(int id){
    return posicaoDoVertice(id) != -1;
}

int obterListaIDDosVerticesMatriz(int* lista_de_id){
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        lista_de_id[i] = vertices[i].id;
    }
    return QUANTIDADE_VERTICES;
}

// DFS recursiva usada por descobrirCaminhoMatriz
static int dfsMatriz(int pos_atual, int pos_destino, int visitados[]){
    visitados[pos_atual] = 1;
    printf("[%d] %s\n", vertices[pos_atual].id, vertices[pos_atual].nome);

    if(pos_atual == pos_destino) return 1;

    for(int j = 0; j < QUANTIDADE_VERTICES; j++){
        if(matriz[pos_atual][j] >= 0.0 && !visitados[j]){
            if(dfsMatriz(j, pos_destino, visitados)) return 1;
        }
    }
    return 0;
}

// ──────────────── PRINCIPAIS ────────────────

void inicializarGrafoMatriz(int eh_ponderado, int eh_dirigido){
    for(int i = 0; i < 100; i++)
        for(int j = 0; j < 100; j++)
            matriz[i][j] = -1.0; // sem aresta

    EH_PONDERADO = eh_ponderado;
    EH_DIRIGIDO  = eh_dirigido;
    QUANTIDADE_VERTICES = 0;
}

void adicionarVerticeMatriz(int id, char* texto){
    vertices[QUANTIDADE_VERTICES].id   = id;
    vertices[QUANTIDADE_VERTICES].peso = 0.0;
    strncpy(vertices[QUANTIDADE_VERTICES].nome, texto, 99);
    vertices[QUANTIDADE_VERTICES].nome[99] = '\0';
    QUANTIDADE_VERTICES++;
    printf("Novo vértice (%d) adicionado ao grafo de matriz.\n", id);
}

void adicionarArestaMatriz(int inicio, int fim, float peso){
    int pi = posicaoDoVertice(inicio);
    int pf = posicaoDoVertice(fim);

    if(pi == -1){
        printf("Não é possível adicionar aresta: vértice inicial (%d) não existe.\n", inicio);
        return;
    }
    if(pf == -1){
        printf("Não é possível adicionar aresta: vértice final (%d) não existe.\n", fim);
        return;
    }

    // para não-ponderado armazena 1.0 para indicar "há aresta"
    float valor = EH_PONDERADO ? peso : 1.0;

    matriz[pi][pf] = valor;
    if(!EH_DIRIGIDO) matriz[pf][pi] = valor;

    if(EH_PONDERADO){
        printf("A aresta (%d, %d, %.2f) foi adicionada com sucesso.\n", inicio, fim, peso);
    } else {
        printf("A aresta (%d, %d) foi adicionada com sucesso.\n", inicio, fim);
    }
}

void removerArestaMatriz(int inicio, int fim){
    int pi = posicaoDoVertice(inicio);
    int pf = posicaoDoVertice(fim);

    if(pi == -1){
        printf("Não é possível remover aresta: vértice inicial (%d) não existe.\n", inicio);
        return;
    }
    if(pf == -1){
        printf("Não é possível remover aresta: vértice final (%d) não existe.\n", fim);
        return;
    }

    if(matriz[pi][pf] < 0.0){
        printf("Não foi possível encontrar a aresta (%d, %d) para remoção.\n", inicio, fim);
        return;
    }

    matriz[pi][pf] = -1.0;
    if(!EH_DIRIGIDO) matriz[pf][pi] = -1.0;

    printf("A aresta (%d, %d) foi removida com sucesso.\n", inicio, fim);
}

void removerVerticeMatriz(int id){
    int pos = posicaoDoVertice(id);
    if(pos == -1){
        printf("Não foi possível encontrar o vértice (%d).\n", id);
        return;
    }

    // remove linha `pos`: desloca linhas para cima
    for(int i = pos; i < QUANTIDADE_VERTICES - 1; i++)
        for(int j = 0; j < QUANTIDADE_VERTICES; j++)
            matriz[i][j] = matriz[i + 1][j];

    // remove coluna `pos`: desloca colunas para a esquerda
    for(int i = 0; i < QUANTIDADE_VERTICES - 1; i++)
        for(int j = pos; j < QUANTIDADE_VERTICES - 1; j++)
            matriz[i][j] = matriz[i][j + 1];

    // remove o vértice do vetor
    for(int i = pos; i < QUANTIDADE_VERTICES - 1; i++)
        vertices[i] = vertices[i + 1];

    QUANTIDADE_VERTICES--;

    printf("O vértice (%d) foi removido com sucesso.\n", id);
}

void mostrarGrafoMatriz(){
    printf("\n=== Grafo (Matriz de Adjacência) ===\n");

    // cabeçalho com nomes
    printf("      ");
    for(int j = 0; j < QUANTIDADE_VERTICES; j++)
        printf(" %6s", vertices[j].nome);
    printf("\n");

    // linhas da matriz
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        printf("%6s", vertices[i].nome);
        for(int j = 0; j < QUANTIDADE_VERTICES; j++){
            if(matriz[i][j] < 0.0){
                // sem aresta
                if(EH_PONDERADO) printf("    0.0 ");
                else             printf("      0 ");
            } else {
                if(EH_PONDERADO) printf(" %6.2f ", matriz[i][j]);
                else             printf("      1 ");
            }
        }
        printf("\n");
    }
}

void descobrirCaminhoMatriz(int inicio, int fim){
    int pi = posicaoDoVertice(inicio);
    int pf = posicaoDoVertice(fim);

    if(pi == -1){
        printf("Não existe o vértice inicial (%d).\n", inicio);
        return;
    }
    if(pf == -1){
        printf("Não existe o vértice final (%d).\n", fim);
        return;
    }

    int visitados[100] = {0};

    printf("\nBuscando caminho em profundidade...\n\n");
    if(dfsMatriz(pi, pf, visitados)){
        printf("\nCaminho encontrado.\n");
    } else {
        printf("\nNão existe caminho.\n");
    }
}
