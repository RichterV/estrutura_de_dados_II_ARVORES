#ifndef ARVBIN_H
#define ARVBIN_H

/*
 * Árvore Binária de Busca (ABB)
 *
 * Propriedade fundamental: para todo nó N,
 *   - subárvore esquerda contém apenas ids < N->id
 *   - subárvore direita  contém apenas ids > N->id
 *
 * Essa propriedade garante busca, inserção e remoção em O(log n) no caso médio.
 */

typedef struct NoArvBin {
    int id;               /* chave de comparação: define a posição do nó na ABB */
    char nome[100];       /* dado satélite: informação associada ao id           */
    struct NoArvBin *esq; /* filho esquerdo — contém ids menores que este nó    */
    struct NoArvBin *dir; /* filho direito  — contém ids maiores que este nó    */
} NoArvBin;

/* ── Travessias (apenas leitura) ─────────────────────────────────────────── */

/* Exibe nós em ordem crescente de id (esq → raiz → dir).
   Resultado: sequência ordenada, útil para listar todos os valores. */
void InOrder(struct NoArvBin *atual);

/* Exibe raiz antes dos filhos (raiz → esq → dir).
   Resultado: ordem de inserção que reconstrói exatamente a mesma árvore nó a nó. */
void PreOrder(struct NoArvBin *atual);

/* Exibe filhos antes da raiz (esq → dir → raiz).
   Resultado: ordem ideal para liberar memória (filhos liberados antes do pai). */
void PostOrder(struct NoArvBin *atual);

/* ── Operações de modificação ─────────────────────────────────────────────── */

/* Aloca um novo nó com id e nome; filhos inicializados como NULL (folha). */
NoArvBin *criarNo(int id, const char *nome);

/* Insere id/nome na posição correta da ABB de forma recursiva.
   Retorna sempre a raiz da subárvore (padrão "retornar e reatribuir"). */
NoArvBin *inserir(NoArvBin *raiz, int id, const char *nome);

/* Remove o nó com id da ABB de forma recursiva; mantém a propriedade da ABB.
   Retorna a nova raiz da subárvore após a remoção. */
NoArvBin *remover(NoArvBin *raiz, int id);

/* ── Consultas de valor extremo ───────────────────────────────────────────── */

/* Retorna o nó com o menor id da subárvore com raiz em 'no'
   (percorre sempre para a esquerda até o fim).
   Pré-condição: 'no' não pode ser NULL. */
NoArvBin *minimo(NoArvBin *no);

/* Retorna o nó com o maior id da subárvore com raiz em 'no'
   (percorre sempre para a direita até o fim).
   Pré-condição: 'no' não pode ser NULL. */
NoArvBin *maximo(NoArvBin *no);

/* ── Consultas de vizinhança em ordem ────────────────────────────────────── */

/* Retorna o sucessor em ordem do nó com o id dado:
   - se o nó tem filho direito: é o mínimo da subárvore direita.
   - caso contrário: é o ancestral mais próximo pelo qual subimos à esquerda.
   Retorna NULL se o nó for o maior da árvore (sem sucessor). */
NoArvBin *sucessor(NoArvBin *raiz, int id);

/* Retorna o antecessor em ordem do nó com o id dado:
   - se o nó tem filho esquerdo: é o máximo da subárvore esquerda.
   - caso contrário: é o ancestral mais próximo pelo qual subimos à direita.
   Retorna NULL se o nó for o menor da árvore (sem antecessor). */
NoArvBin *antecessor(NoArvBin *raiz, int id);

/* ── Visualização gráfica ────────────────────────────────────────────────── */

/* Gera arvore.dot no formato Graphviz e abre arvore.png com a imagem da ABB.
   Requer o Graphviz instalado (winget install graphviz.graphviz). */
void visualizarABBDot(NoArvBin *raiz);

#endif /* ARVBIN_H */
