#ifndef ARVBIN_H
#define ARVBIN_H

typedef struct NoArvBin {
    int id;                 // chave
    char nome[100];         // dado satélite
    struct NoArvBin *esq;   // filho esquerdo
    struct NoArvBin *dir;   // filho direito
} NoArvBin;

//só mostra coisas do nó
void InOrder( struct NoArvBin *atual);
void PreOrder(struct NoArvBin *atual);
void PostOrder(struct NoArvBin *atual);
//modifica o nó
NoArvBin *criarNo(int id, const char *nome);
NoArvBin *inserir(NoArvBin *raiz, int id, const char *nome);
NoArvBin *minimo(NoArvBin *no);
NoArvBin *remover(NoArvBin *raiz, int id);

#endif //ARVBIN_H