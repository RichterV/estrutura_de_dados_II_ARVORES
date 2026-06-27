#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvRN.h"

/* ══════════════════════════════════════════════════════════════════════════════
   SENTINELA NIL
   ══════════════════════════════════════════════════════════════════════════════
   Um único nó sentinela PRETO compartilhado representa todas as folhas NIL.
   Isso evita verificações de NULL nas rotações e nos fixups, tornando o
   código mais próximo do pseudocódigo do CLRS (Introduction to Algorithms).
*/
static NoARN NIL_NODE = {0, "", PRETO, NULL, NULL, NULL};
NoARN *NIL_ARN = &NIL_NODE;

void inicializarARN(NoARN **raiz) {
    /* Faz a sentinela apontar para si mesma — simplifica rotações */
    NIL_NODE.esq = NIL_ARN;
    NIL_NODE.dir = NIL_ARN;
    NIL_NODE.pai = NIL_ARN;
    *raiz = NIL_ARN; /* árvore vazia: raiz aponta para a sentinela */
}

/* ══════════════════════════════════════════════════════════════════════════════
   ROTAÇÕES
   ══════════════════════════════════════════════════════════════════════════════
   As rotações são a operação primitiva que reequilibra a árvore.
   Preservam a propriedade de ordem da BST e mudam apenas os ponteiros.
*/

/*
 * rotacaoEsq: pivô em x, y = x->dir sobe.
 *
 *       x                y
 *      / \              / \
 *     A   y    -->     x   C
 *        / \          / \
 *       B   C        A   B
 */
static void rotacaoEsq(NoARN **raiz, NoARN *x) {
    NoARN *y = x->dir;
    x->dir = y->esq;                              /* B passa a ser filho dir de x */
    if (y->esq != NIL_ARN) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL_ARN)      *raiz = y;        /* x era a raiz: y vira nova raiz */
    else if (x == x->pai->esq) x->pai->esq = y;  /* x era filho esquerdo do pai    */
    else                        x->pai->dir = y;  /* x era filho direito  do pai    */
    y->esq = x;
    x->pai = y;
}

/*
 * rotacaoDir: simétrica a rotacaoEsq — pivô em x, y = x->esq sobe.
 *
 *       x                y
 *      / \              / \
 *     y   C    -->     A   x
 *    / \                  / \
 *   A   B                B   C
 */
static void rotacaoDir(NoARN **raiz, NoARN *x) {
    NoARN *y = x->esq;
    x->esq = y->dir;
    if (y->dir != NIL_ARN) y->dir->pai = x;
    y->pai = x->pai;
    if (x->pai == NIL_ARN)      *raiz = y;
    else if (x == x->pai->dir) x->pai->dir = y;
    else                        x->pai->esq = y;
    y->dir = x;
    x->pai = y;
}

/* ══════════════════════════════════════════════════════════════════════════════
   INSERÇÃO
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * fixupInserir: restaura as propriedades da ARN após inserir um nó VERMELHO.
 *
 * O único invariante violado após a inserção é a propriedade 4
 * (nó vermelho não pode ter pai vermelho). O loop sobe pela árvore
 * corrigindo violações em três casos:
 *
 *   Caso 1 — tio VERMELHO: recolorir pai, tio e avô; subir z ao avô.
 *   Caso 2 — tio PRETO, z forma "triângulo" com pai e avô:
 *             rotação transforma em linha reta (Caso 3).
 *   Caso 3 — tio PRETO, z forma "linha reta": recolorir + rotação simples.
 *
 * O lado esquerdo/direito do pai no avô determina qual versão dos casos usar.
 */
static void fixupInserir(NoARN **raiz, NoARN *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) { /* pai é filho ESQUERDO do avô */
            NoARN *tio = z->pai->pai->dir;
            if (tio->cor == VERMELHO) {
                /* Caso 1: recolorir e subir */
                z->pai->cor        = PRETO;
                tio->cor           = PRETO;
                z->pai->pai->cor   = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    /* Caso 2: triângulo → rotação esquerda converte em Caso 3 */
                    z = z->pai;
                    rotacaoEsq(raiz, z);
                }
                /* Caso 3: linha reta → recolorir + rotação direita */
                z->pai->cor      = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDir(raiz, z->pai->pai);
            }
        } else { /* simétrico: pai é filho DIREITO do avô */
            NoARN *tio = z->pai->pai->esq;
            if (tio->cor == VERMELHO) {
                z->pai->cor        = PRETO;
                tio->cor           = PRETO;
                z->pai->pai->cor   = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDir(raiz, z);
                }
                z->pai->cor      = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsq(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO; /* Propriedade 2: raiz sempre PRETA */
}

void inserirARN(NoARN **raiz, int id, const char *nome) {
    /* Aloca e inicializa novo nó vermelho */
    NoARN *z = malloc(sizeof(NoARN));
    z->id  = id;
    strcpy(z->nome, nome);
    z->cor = VERMELHO;
    z->esq = NIL_ARN;
    z->dir = NIL_ARN;
    z->pai = NIL_ARN;

    /* Localiza a posição de inserção como em uma BST comum */
    NoARN *y = NIL_ARN;
    NoARN *x = *raiz;
    while (x != NIL_ARN) {
        y = x;
        if      (z->id < x->id) x = x->esq;
        else if (z->id > x->id) x = x->dir;
        else { free(z); return; } /* id duplicado: ignora */
    }

    z->pai = y;
    if      (y == NIL_ARN)    *raiz = z;   /* árvore estava vazia */
    else if (z->id < y->id)   y->esq = z;
    else                       y->dir = z;

    fixupInserir(raiz, z);
}

/* ══════════════════════════════════════════════════════════════════════════════
   REMOÇÃO
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * transplant: substitui a subárvore com raiz u pela subárvore com raiz v.
 * Atualiza o pai de u para apontar para v; atualiza v->pai.
 * Não atualiza v->esq nem v->dir — o chamador é responsável.
 */
static void transplant(NoARN **raiz, NoARN *u, NoARN *v) {
    if      (u->pai == NIL_ARN)  *raiz = v;
    else if (u == u->pai->esq)  u->pai->esq = v;
    else                         u->pai->dir = v;
    v->pai = u->pai; /* sempre atualiza, mesmo quando v é NIL_ARN */
}

static NoARN *minimoARN(NoARN *x) {
    while (x->esq != NIL_ARN) x = x->esq;
    return x;
}

/*
 * fixupRemover: restaura as propriedades da ARN após a remoção de um nó PRETO.
 *
 * Quando o nó removido era PRETO, um caminho perdeu um nó PRETO, violando
 * a propriedade 5. O fixup sobe pela árvore com quatro casos simétricos:
 *
 *   Caso 1 — irmão VERMELHO: rotação + recolorir → cai em Caso 2, 3 ou 4.
 *   Caso 2 — irmão PRETO, filhos do irmão PRETOS: recolorir irmão; sobe x.
 *   Caso 3 — irmão PRETO, filho próximo VERMELHO: rotação + recolorir → Caso 4.
 *   Caso 4 — irmão PRETO, filho distante VERMELHO: rotação + recolorir; termina.
 */
static void fixupRemover(NoARN **raiz, NoARN *x) {
    while (x != *raiz && x->cor == PRETO) {
        if (x == x->pai->esq) {
            NoARN *w = x->pai->dir; /* irmão de x */
            if (w->cor == VERMELHO) {
                /* Caso 1 */
                w->cor       = PRETO;
                x->pai->cor  = VERMELHO;
                rotacaoEsq(raiz, x->pai);
                w = x->pai->dir;
            }
            if (w->esq->cor == PRETO && w->dir->cor == PRETO) {
                /* Caso 2 */
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->dir->cor == PRETO) {
                    /* Caso 3 */
                    w->esq->cor = PRETO;
                    w->cor      = VERMELHO;
                    rotacaoDir(raiz, w);
                    w = x->pai->dir;
                }
                /* Caso 4 */
                w->cor       = x->pai->cor;
                x->pai->cor  = PRETO;
                w->dir->cor  = PRETO;
                rotacaoEsq(raiz, x->pai);
                x = *raiz; /* termina o loop */
            }
        } else { /* simétrico */
            NoARN *w = x->pai->esq;
            if (w->cor == VERMELHO) {
                w->cor       = PRETO;
                x->pai->cor  = VERMELHO;
                rotacaoDir(raiz, x->pai);
                w = x->pai->esq;
            }
            if (w->dir->cor == PRETO && w->esq->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esq->cor == PRETO) {
                    w->dir->cor = PRETO;
                    w->cor      = VERMELHO;
                    rotacaoEsq(raiz, w);
                    w = x->pai->esq;
                }
                w->cor       = x->pai->cor;
                x->pai->cor  = PRETO;
                w->esq->cor  = PRETO;
                rotacaoDir(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    x->cor = PRETO;
}

void removerARN(NoARN **raiz, int id) {
    /* Busca o nó com o id dado */
    NoARN *z = *raiz;
    while (z != NIL_ARN) {
        if      (id < z->id) z = z->esq;
        else if (id > z->id) z = z->dir;
        else break;
    }
    if (z == NIL_ARN) {
        printf("Nó [%d] não encontrado na ARN.\n", id);
        return;
    }

    NoARN *y = z;              /* nó que será de fato desvinculado */
    NoARN *x;                  /* filho que sobe para o lugar de y */
    int cor_original = y->cor; /* cor de y antes de ser movido     */

    if (z->esq == NIL_ARN) {
        /* Caso: sem filho esquerdo (zero filhos ou só filho direito) */
        x = z->dir;
        transplant(raiz, z, z->dir);
    } else if (z->dir == NIL_ARN) {
        /* Caso: só filho esquerdo */
        x = z->esq;
        transplant(raiz, z, z->esq);
    } else {
        /* Caso: dois filhos — usa o sucessor em ordem (mínimo da subárvore dir) */
        y = minimoARN(z->dir);
        cor_original = y->cor;
        x = y->dir;
        if (y->pai == z) {
            /* Caso especial: y é filho direto de z — x->pai já aponta para y */
            x->pai = y;
        } else {
            transplant(raiz, y, y->dir);
            y->dir      = z->dir;
            y->dir->pai = y;
        }
        transplant(raiz, z, y);
        y->esq      = z->esq;
        y->esq->pai = y;
        y->cor      = z->cor;
    }
    free(z);

    /* Só precisa de fixup se a cor removida era PRETA */
    if (cor_original == PRETO) {
        fixupRemover(raiz, x);
    }
}

/* ══════════════════════════════════════════════════════════════════════════════
   VISUALIZAÇÃO GRÁFICA — Graphviz DOT com cores e rótulos de posição
   ══════════════════════════════════════════════════════════════════════════════ */

/* Escreve uma string no arquivo escapando " e \ para não quebrar o formato DOT. */
static void escreverTextoSeguro(FILE *f, const char *str) {
    for (; *str; str++) {
        if (*str == '"' || *str == '\\') fputc('\\', f);
        fputc(*str, f);
    }
}

/*
 * escreverNosARN: percorre em pré-ordem e emite cada nó com:
 *   - fillcolor=red  (VERMELHO) ou fillcolor=black (PRETO)
 *   - fontcolor=white para contraste
 *   - label mostrando id e nome (com escaping de aspas e barras)
 */
static void escreverNosARN(FILE *f, NoARN *no) {
    if (no == NIL_ARN) return;
    const char *cor_fill = (no->cor == VERMELHO) ? "red" : "black";
    fprintf(f, "    %d [label=\"[%d]\\n", no->id, no->id);
    escreverTextoSeguro(f, no->nome);
    fprintf(f, "\" style=filled fillcolor=%s fontcolor=white];\n", cor_fill);
    escreverNosARN(f, no->esq);
    escreverNosARN(f, no->dir);
}

/*
 * escreverArestasARN: emite arestas rotuladas com "E" (esquerda) e "D" (direita)
 * para que o diagrama mostre claramente a posição de cada filho.
 */
static void escreverArestasARN(FILE *f, NoARN *no) {
    if (no == NIL_ARN) return;
    if (no->esq != NIL_ARN)
        fprintf(f, "    %d -> %d [label=\"E\"];\n", no->id, no->esq->id);
    if (no->dir != NIL_ARN)
        fprintf(f, "    %d -> %d [label=\"D\"];\n", no->id, no->dir->id);
    escreverArestasARN(f, no->esq);
    escreverArestasARN(f, no->dir);
}

void visualizarARNDot(NoARN *raiz) {
    FILE *f = fopen("arvore_rn.dot", "w");
    if (f == NULL) {
        printf("Erro ao criar arquivo arvore_rn.dot.\n");
        return;
    }

    fprintf(f, "digraph ARN {\n");
    fprintf(f, "    node [shape=circle fontname=\"Helvetica\"];\n");
    fprintf(f, "    graph [ordering=\"out\"];\n");
    fprintf(f, "    edge [fontsize=10];\n");

    if (raiz == NIL_ARN) {
        fprintf(f, "    vazio [label=\"árvore vazia\" shape=plaintext];\n");
    } else {
        escreverNosARN(f, raiz);
        escreverArestasARN(f, raiz);
    }

    fprintf(f, "}\n");
    fclose(f);

    if (system("dot -Tpng arvore_rn.dot -o arvore_rn.png") != 0) {
        printf("Erro: Graphviz não encontrado ou falhou ao gerar a imagem.\n");
        printf("Instale com: winget install graphviz.graphviz\n");
        printf("O arquivo arvore_rn.dot foi gerado e pode ser visualizado em:\n");
        printf("  https://dreampuf.github.io/GraphvizOnline\n");
        return;
    }

    system("start arvore_rn.png");
    printf("Árvore Rubro-Negra renderizada com sucesso em arvore_rn.png.\n");
}
