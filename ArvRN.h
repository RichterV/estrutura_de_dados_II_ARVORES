#ifndef ARVRN_H
#define ARVRN_H

/*
 * Árvore Rubro-Negra (ARN)
 *
 * Propriedades invariantes:
 *   1. Todo nó é VERMELHO ou PRETO.
 *   2. A raiz é PRETO.
 *   3. Todo nó folha (NIL_ARN) é PRETO.
 *   4. Se um nó é VERMELHO, ambos os filhos são PRETOS.
 *   5. Para todo nó, todos os caminhos simples até folhas descendentes
 *      possuem o mesmo número de nós PRETOS (altura-negra).
 *
 * Usa um nó sentinela NIL_ARN (preto) no lugar de NULL para
 * simplificar as rotações e os fixups de inserção/remoção.
 */

#define VERMELHO 0
#define PRETO    1

typedef struct NoARN {
    int id;
    char nome[100];
    int cor;               /* VERMELHO ou PRETO               */
    struct NoARN *esq;     /* filho esquerdo — ids menores     */
    struct NoARN *dir;     /* filho direito  — ids maiores     */
    struct NoARN *pai;     /* ponteiro para o pai              */
} NoARN;

/* Sentinela global: representa todas as folhas NIL (sempre PRETO). */
extern NoARN *NIL_ARN;

/* Inicializa a sentinela e define *raiz = NIL_ARN.
   Deve ser chamada antes de qualquer outra operação na ARN. */
void inicializarARN(NoARN **raiz);

/* Insere um novo nó com id/nome e restaura as propriedades da ARN.
   Ignora silenciosamente inserções de ids duplicados. */
void inserirARN(NoARN **raiz, int id, const char *nome);

/* Remove o nó com o id dado e restaura as propriedades da ARN.
   Informa ao usuário se o id não existir na árvore. */
void removerARN(NoARN **raiz, int id);

/* Gera arvore_rn.dot com nós coloridos (vermelho/preto) e arestas
   rotuladas com E (esquerda) e D (direita). Abre arvore_rn.png
   no visualizador padrão do Windows via Graphviz. */
void visualizarARNDot(NoARN *raiz);

#endif /* ARVRN_H */
