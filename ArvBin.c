#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvBin.h"

// ──────────────── PRINCIPAIS ────────────────

void InOrder( struct NoArvBin *atual){ //função recursiva demoniaca somente os de alma impura conseguem compreender
    if(atual != NULL){                //...mas funciona
        InOrder(atual->esq);
        printf("%d", atual->id);
        InOrder(atual->dir);
    }
}

void PreOrder(struct NoArvBin *atual){//Permite mostrar o valor de modo que,                                
    if(atual != NULL){//se os valores de output forem inseridos na árvore binária, retorna a sua estrutura original
        printf("%d ", atual->id);
        PreOrder(atual->esq);
        PreOrder(atual->dir);
    }
}

void PostOrder(struct NoArvBin *atual){//Retorna o mesmo que PreOrder só que inverso
    if(atual != NULL){
        PostOrder(atual->esq);
        PostOrder(atual->dir);
        printf("%d ", atual->id);
    }
}

//Criar um nó novo
NoArvBin *criarNo(int id, const char *nome){
    NoArvBin *novo = malloc(sizeof(NoArvBin));

    novo->id = id;
    strcpy(novo->nome, nome);

    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

//Inserir um valor na árvore
NoArvBin *inserir(NoArvBin *raiz, int id, const char *nome){

    if(raiz == NULL){
        return criarNo(id, nome);
    }

    if(id < raiz->id){
        raiz->esq = inserir(raiz->esq, id, nome);
    }
    else if(id > raiz->id){
        raiz->dir = inserir(raiz->dir, id, nome);
    }

    return raiz;
}

// Encontra o menor elemento da subárvore
// (sempre o nó mais à esquerda)
NoArvBin *minimo(NoArvBin *no){
    while(no->esq != NULL){
        no = no->esq; // vai descendo sempre para a esquerda
    }
    return no; // retorna o nó mais à esquerda (menor valor)
}

// Função que remove um nó da Árvore Binária de Busca (ABB)
NoArvBin *remover(NoArvBin *raiz, int id){

    // Caso base: árvore vazia ou chegou em um ramo inexistente
    if(raiz == NULL)
        return NULL;

    // Se o valor a ser removido é menor que o nó atual,
    // continua a busca na subárvore esquerda
    if(id < raiz->id){
        raiz->esq = remover(raiz->esq, id);
    }

    // Se o valor a ser removido é maior que o nó atual,
    // continua a busca na subárvore direita
    else if(id > raiz->id){
        raiz->dir = remover(raiz->dir, id);
    }

    // Se chegou aqui, encontrou o nó a ser removido
    else{

        // CASO 1: nó não tem filho esquerdo
        // (pode ter nenhum ou só filho direito)
        if(raiz->esq == NULL){
            NoArvBin *temp = raiz->dir; // salva o filho direito
            free(raiz);                // libera o nó atual
            return temp;               // retorna o filho para subir na árvore
        }

        // CASO 2: nó não tem filho direito
        // (só existe filho esquerdo)
        else if(raiz->dir == NULL){
            NoArvBin *temp = raiz->esq; // salva o filho esquerdo
            free(raiz);                // libera o nó atual
            return temp;               // retorna o filho para subir na árvore
        }

        // CASO 3: nó tem dois filhos
        // precisa encontrar um substituto válido

        // pega o menor valor da subárvore direita (sucessor em ordem)
        NoArvBin *temp = minimo(raiz->dir);

        // substitui os dados do nó atual pelo sucessor
        raiz->id = temp->id;
        strcpy(raiz->nome, temp->nome);

        // remove o sucessor original da subárvore direita
        raiz->dir = remover(raiz->dir, temp->id);
    }

    // retorna a raiz atualizada da subárvore
    return raiz;
}