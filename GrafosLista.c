#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GrafosLista.h"

// variáveis globais locais
static int EH_PONDERADO;
static int EH_DIRIGIDO;
static int QUANTIDADE_VERTICES;
static GrafoL* vertices[100]; // vetor de ponteiros para os vértices

// ──────────────── AUXILIARES ────────────────

GrafoL* buscarVerticeLista(int id){
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(vertices[i] != NULL && vertices[i]->id == id){
            return vertices[i];
        }
    }
    return NULL;
}

int existeVerticeLista(int id){
    return buscarVerticeLista(id) != NULL;
}

int obterListaIDDosVerticesLista(int* lista_de_id){
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        lista_de_id[i] = vertices[i]->id;
    }
    return QUANTIDADE_VERTICES;
}

// DFS recursiva usada por descobrirCaminhoLista
static int dfsLista(int atual_id, int destino_id, int visitados[]){
    GrafoL* atual = buscarVerticeLista(atual_id);
    visitados[atual_id] = 1;

    printf("[%d] %s\n", atual->id, atual->nome);

    if(atual_id == destino_id){
        return 1;
    }

    GrafoL* adj = atual->proximo;
    while(adj != NULL){
        if(!visitados[adj->id]){
            if(dfsLista(adj->id, destino_id, visitados)){
                return 1;
            }
        }
        adj = adj->proximo;
    }
    return 0;
}

// ──────────────── PRINCIPAIS ────────────────

void inicializarGrafoLista(int eh_ponderado, int eh_dirigido){
    for(int i = 0; i < 100; i++){
        vertices[i] = NULL;
    }
    EH_PONDERADO = eh_ponderado;
    EH_DIRIGIDO  = eh_dirigido;
    QUANTIDADE_VERTICES = 0;
}

void adicionarVerticeLista(int id, char* texto){
    GrafoL* novo = (GrafoL*)malloc(sizeof(GrafoL));
    if(novo == NULL){
        printf("Erro na alocação de memória do novo vértice.\n");
        return;
    }
    novo->id      = id;
    novo->peso    = 0.0;
    novo->proximo = NULL;
    strncpy(novo->nome, texto, 99);
    novo->nome[99] = '\0';

    vertices[QUANTIDADE_VERTICES++] = novo;
    printf("Novo vértice (%d) adicionado ao grafo de lista.\n", id);
}

void adicionarArestaLista(int inicio, int fim, float peso){
    GrafoL* noinicio = buscarVerticeLista(inicio);
    GrafoL* nofim    = buscarVerticeLista(fim);

    if(noinicio == NULL){
        printf("Não é possível adicionar aresta: vértice inicial (%d) não existe.\n", inicio);
        return;
    }
    if(nofim == NULL){
        printf("Não é possível adicionar aresta: vértice final (%d) não existe.\n", fim);
        return;
    }

    // cria nó de adjacência: inicio → fim
    GrafoL* nova = (GrafoL*)malloc(sizeof(GrafoL));
    nova->id      = fim;
    nova->peso    = peso;
    nova->proximo = NULL;
    strncpy(nova->nome, nofim->nome, 99);
    nova->nome[99] = '\0';

    // insere no final da lista de adjacência de inicio
    if(noinicio->proximo == NULL){
        noinicio->proximo = nova;
    } else {
        GrafoL* atual = noinicio->proximo;
        while(atual->proximo != NULL) atual = atual->proximo;
        atual->proximo = nova;
    }

    if(!EH_DIRIGIDO){ // grafo não dirigido: adiciona também fim → inicio
        GrafoL* nova2 = (GrafoL*)malloc(sizeof(GrafoL));
        nova2->id      = inicio;
        nova2->peso    = peso;
        nova2->proximo = NULL;
        strncpy(nova2->nome, noinicio->nome, 99);
        nova2->nome[99] = '\0';

        if(nofim->proximo == NULL){
            nofim->proximo = nova2;
        } else {
            GrafoL* atual = nofim->proximo;
            while(atual->proximo != NULL) atual = atual->proximo;
            atual->proximo = nova2;
        }
    }

    if(EH_PONDERADO){
        printf("A aresta (%d, %d, %.2f) foi adicionada com sucesso.\n", inicio, fim, peso);
    } else {
        printf("A aresta (%d, %d) foi adicionada com sucesso.\n", inicio, fim);
    }
}

void removerArestaLista(int inicio, int fim){
    GrafoL* noinicio = buscarVerticeLista(inicio);
    if(noinicio == NULL){
        printf("Não é possível remover aresta: vértice inicial (%d) não existe.\n", inicio);
        return;
    }

    // remove fim da lista de adjacência de inicio
    int achou = 0;
    GrafoL* anterior = NULL;
    GrafoL* atual    = noinicio->proximo;
    while(atual != NULL){
        if(atual->id == fim){
            if(anterior == NULL) noinicio->proximo = atual->proximo;
            else                 anterior->proximo  = atual->proximo;
            free(atual);
            achou = 1;
            break;
        }
        anterior = atual;
        atual    = atual->proximo;
    }

    if(!EH_DIRIGIDO){ // remove também inicio da lista de adjacência de fim
        GrafoL* nofim = buscarVerticeLista(fim);
        if(nofim != NULL){
            anterior = NULL;
            atual    = nofim->proximo;
            while(atual != NULL){
                if(atual->id == inicio){
                    if(anterior == NULL) nofim->proximo = atual->proximo;
                    else                 anterior->proximo = atual->proximo;
                    free(atual);
                    break;
                }
                anterior = atual;
                atual    = atual->proximo;
            }
        }
    }

    if(achou){
        printf("A aresta (%d, %d) foi removida com sucesso.\n", inicio, fim);
    } else {
        printf("Não foi possível encontrar a aresta (%d, %d) para remoção.\n", inicio, fim);
    }
}

void removerVerticeLista(int id){
    int posicao = -1;
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(vertices[i] != NULL && vertices[i]->id == id){
            posicao = i;
            break;
        }
    }

    if(posicao == -1){
        printf("Não foi possível encontrar o vértice (%d).\n", id);
        return;
    }

    // remove todas as arestas de outros vértices que apontam para id
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(vertices[i] == NULL || vertices[i]->id == id) continue;
        removerArestaLista(vertices[i]->id, id);
    }

    // libera a lista de adjacência do próprio vértice
    GrafoL* adj = vertices[posicao]->proximo;
    while(adj != NULL){
        GrafoL* prox = adj->proximo;
        free(adj);
        adj = prox;
    }

    free(vertices[posicao]);

    // desloca o vetor para preencher o buraco
    for(int i = posicao; i < QUANTIDADE_VERTICES - 1; i++){
        vertices[i] = vertices[i + 1];
    }
    vertices[QUANTIDADE_VERTICES - 1] = NULL;
    QUANTIDADE_VERTICES--;

    printf("O vértice (%d) foi removido com sucesso.\n", id);
}

void mostrarGrafoLista(){
    printf("\n=== Grafo (Lista de Adjacência) ===\n");
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(vertices[i] == NULL) continue;
        printf("[%d] %s ->", vertices[i]->id, vertices[i]->nome);
        GrafoL* adj = vertices[i]->proximo;
        while(adj != NULL){
            if(EH_PONDERADO){
                printf(" [%d](%.2f)", adj->id, adj->peso);
            } else {
                printf(" [%d]", adj->id);
            }
            adj = adj->proximo;
        }
        printf(" NULL\n");
    }
}

void descobrirCaminhoLista(int inicio, int fim){
    if(!existeVerticeLista(inicio)){
        printf("Não existe o vértice inicial (%d).\n", inicio);
        return;
    }
    if(!existeVerticeLista(fim)){
        printf("Não existe o vértice final (%d).\n", fim);
        return;
    }

    // vetor de visitados indexado por ID (suporta até 200 IDs distintos)
    int visitados[200];
    for(int i = 0; i < 200; i++) visitados[i] = 0;

    printf("\nBuscando caminho em profundidade...\n\n");
    if(dfsLista(inicio, fim, visitados)){
        printf("\nCaminho encontrado.\n");
    } else {
        printf("\nNão existe caminho.\n");
    }
}
