#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvBin.h"

/* ══════════════════════════════════════════════════════════════════════════════
   VISUALIZAÇÃO GRÁFICA — gera arquivo .dot e abre imagem via Graphviz
   ══════════════════════════════════════════════════════════════════════════════ */

/* Escreve uma string no arquivo escapando " e \ para não quebrar o formato DOT. */
static void escreverTextoSeguro(FILE *f, const char *str) {
    for (; *str; str++) {
        if (*str == '"' || *str == '\\') fputc('\\', f);
        fputc(*str, f);
    }
}

/*
 * escreverNos: percorre a árvore em pré-ordem e escreve cada nó no arquivo .dot.
 * Formato: id [label="[id]\nnome"];
 * O label quebra em duas linhas para mostrar tanto a chave quanto o dado satélite.
 */
static void escreverNos(FILE *f, NoArvBin *no) {
    if (no == NULL) return;
    fprintf(f, "    %d [label=\"[%d]\\n", no->id, no->id);
    escreverTextoSeguro(f, no->nome);
    fprintf(f, "\"];\n");
    escreverNos(f, no->esq); /* escreve nós da subárvore esquerda */
    escreverNos(f, no->dir); /* escreve nós da subárvore direita  */
}

/*
 * escreverArestas: percorre a árvore e escreve as arestas pai → filho no .dot.
 * Só escreve a aresta se o filho existir (evita apontar para NULL).
 * Os atributos "L" e "R" rotulam as arestas indicando filho esquerdo/direito.
 */
static void escreverArestas(FILE *f, NoArvBin *no) {
    if (no == NULL) return;
    if (no->esq != NULL) {
        fprintf(f, "    %d -> %d [label=\"L\"];\n", no->id, no->esq->id); /* aresta para filho esquerdo */
    }
    if (no->dir != NULL) {
        fprintf(f, "    %d -> %d [label=\"R\"];\n", no->id, no->dir->id); /* aresta para filho direito  */
    }
    escreverArestas(f, no->esq);
    escreverArestas(f, no->dir);
}

/*
 * visualizarABBDot: ponto de entrada da visualização gráfica.
 *
 * Fluxo:
 *   1. Abre/cria "arvore.dot" para escrita.
 *   2. Escreve o cabeçalho do grafo dirigido (digraph).
 *   3. Chama escreverNos e escreverArestas para preencher o conteúdo.
 *   4. Fecha o arquivo e chama "dot" do Graphviz para gerar arvore.png.
 *   5. Abre a imagem com o visualizador padrão do Windows (start).
 *
 * Se o Graphviz não estiver instalado, informa o comando de instalação
 * e exibe o link para visualizar o .dot online.
 */
void visualizarABBDot(NoArvBin *raiz) {
    FILE *f = fopen("arvore.dot", "w");
    if (f == NULL) {
        printf("Erro ao criar arquivo arvore.dot.\n");
        return;
    }

    fprintf(f, "digraph ABB {\n");
    fprintf(f, "    node [shape=circle fontname=\"Helvetica\"];\n"); /* nós circulares, como árvore desenhada no papel */
    fprintf(f, "    graph [ordering=\"out\"];\n");                   /* garante filho esquerdo antes do direito no layout */
    fprintf(f, "    edge [fontsize=10];\n");

    if (raiz == NULL) {
        /* árvore vazia: exibe apenas um nó de aviso */
        fprintf(f, "    vazio [label=\"árvore vazia\" shape=plaintext];\n");
    } else {
        escreverNos(f, raiz);     /* escreve todos os nós */
        escreverArestas(f, raiz); /* escreve todas as arestas */
    }

    fprintf(f, "}\n");
    fclose(f);

    /* chama o Graphviz para converter .dot em .png */
    if (system("dot -Tpng arvore.dot -o arvore.png") != 0) {
        printf("Erro: Graphviz não encontrado ou falhou ao gerar a imagem.\n");
        printf("Instale com: winget install graphviz.graphviz\n");
        printf("O arquivo arvore.dot foi gerado e pode ser visualizado em:\n");
        printf("  https://dreampuf.github.io/GraphvizOnline\n");
        return;
    }

    system("start arvore.png"); /* abre a imagem no visualizador padrão do Windows */
    printf("Árvore renderizada com sucesso em arvore.png.\n");
}

/* ══════════════════════════════════════════════════════════════════════════════
   TRAVESSIAS — percorrem toda a árvore sem modificá-la
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * InOrder: esquerda → raiz → direita
 *
 * Por causa da propriedade da ABB (esq < raiz < dir), visitar nessa ordem
 * produz os ids em sequência crescente — é uma "ordenação grátis".
 *
 * Funciona de forma recursiva: cada chamada processa uma subárvore inteira
 * antes de retornar ao nível superior. O caso base implícito é atual == NULL
 * (chegou numa folha ou numa subárvore vazia), onde simplesmente não faz nada.
 */
void InOrder(struct NoArvBin *atual) {
    if (atual != NULL) {
        InOrder(atual->esq);                           /* visita toda a subárvore esquerda primeiro */
        printf("[%d] %s\n", atual->id, atual->nome);  /* então exibe o nó atual                    */
        InOrder(atual->dir);                           /* por último a subárvore direita             */
    }
}

/*
 * PreOrder: raiz → esquerda → direita
 *
 * A raiz é visitada ANTES dos filhos.
 * Propriedade importante: se inserirmos cada valor nessa ordem em uma ABB vazia,
 * obteremos exatamente a mesma estrutura de árvore original.
 * Por isso é útil para serializar/salvar a árvore.
 */
void PreOrder(struct NoArvBin *atual) {
    if (atual != NULL) {
        printf("[%d] %s\n", atual->id, atual->nome);  /* exibe o nó atual antes dos filhos */
        PreOrder(atual->esq);                         /* desce pela esquerda               */
        PreOrder(atual->dir);                         /* depois pela direita               */
    }
}

/*
 * PostOrder: esquerda → direita → raiz
 *
 * A raiz é visitada DEPOIS de ambos os filhos.
 * Ideal para liberar memória: sempre processamos os filhos antes do pai,
 * evitando ponteiros perdidos (dangling pointers) ao usar free().
 */
void PostOrder(struct NoArvBin *atual) {
    if (atual != NULL) {
        PostOrder(atual->esq);                        /* processa/libera a esquerda primeiro */
        PostOrder(atual->dir);                        /* depois a direita                    */
        printf("[%d] %s\n", atual->id, atual->nome); /* só então exibe (ou liberaria) a raiz */
    }
}

/* ══════════════════════════════════════════════════════════════════════════════
   CRIAÇÃO E INSERÇÃO
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * criarNo: aloca um novo nó e inicializa seus campos.
 * Os ponteiros esq e dir são NULL porque um nó recém-criado é sempre uma folha
 * (ainda não tem filhos).
 */
NoArvBin *criarNo(int id, const char *nome) {
    NoArvBin *novo = malloc(sizeof(NoArvBin)); /* reserva espaço na heap */
    novo->id  = id;
    strcpy(novo->nome, nome);
    novo->esq = NULL; /* sem filho esquerdo ainda */
    novo->dir = NULL; /* sem filho direito ainda  */
    return novo;
}

/*
 * inserir: localiza recursivamente a posição correta e insere o nó.
 *
 * Lógica de navegação (propriedade da ABB):
 *   id < raiz->id → vai para a esquerda (valores menores ficam à esquerda)
 *   id > raiz->id → vai para a direita  (valores maiores ficam à direita)
 *   id == raiz->id → duplicata; ignora a inserção sem modificar a árvore
 *
 * O padrão "retornar e reatribuir" (raiz->esq = inserir(...)) garante que
 * o ponteiro do pai aponte para o novo nó assim que ele é criado.
 */
NoArvBin *inserir(NoArvBin *raiz, int id, const char *nome) {
    if (raiz == NULL) {
        return criarNo(id, nome); /* chegou na posição vazia: cria o nó aqui */
    }
    if (id < raiz->id) {
        raiz->esq = inserir(raiz->esq, id, nome); /* desce à esquerda */
    } else if (id > raiz->id) {
        raiz->dir = inserir(raiz->dir, id, nome); /* desce à direita  */
    }
    /* se id == raiz->id: sem duplicatas — não faz nada */
    return raiz; /* retorna a raiz inalterada (só as folhas mudam) */
}

/* ══════════════════════════════════════════════════════════════════════════════
   CONSULTAS DE EXTREMO
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * minimo: retorna o nó com o menor id a partir de 'no'.
 *
 * Na ABB, o menor valor está sempre no nó mais à esquerda possível.
 * Basta descer pela esquerda até não haver mais filho esquerdo.
 * Complexidade: O(h), onde h é a altura da subárvore.
 */
NoArvBin *minimo(NoArvBin *no) {
    while (no->esq != NULL) {
        no = no->esq; /* continua descendo para a esquerda */
    }
    return no; /* chegou ao nó mais à esquerda: é o mínimo */
}

/*
 * maximo: retorna o nó com o maior id a partir de 'no'.
 *
 * Simétrico ao minimo: o maior valor está sempre no nó mais à direita.
 * Desce pela direita até não haver mais filho direito.
 */
NoArvBin *maximo(NoArvBin *no) {
    while (no->dir != NULL) {
        no = no->dir; /* continua descendo para a direita */
    }
    return no; /* chegou ao nó mais à direita: é o máximo */
}

/* ══════════════════════════════════════════════════════════════════════════════
   SUCESSOR E ANTECESSOR EM ORDEM
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * sucessor: próximo valor maior que o nó de id dado na ordem crescente.
 *
 * Dois casos possíveis ao encontrar o nó:
 *
 *   CASO 1 — o nó tem filho direito:
 *     O sucessor é o MÍNIMO da subárvore direita.
 *     (o menor dos valores que são maiores que ele)
 *
 *   CASO 2 — o nó NÃO tem filho direito:
 *     O sucessor é o ancestral mais próximo pelo qual subimos à esquerda.
 *     Rastreamos esse ancestral em 'candidato' durante a descida:
 *     sempre que vamos para a esquerda, o nó atual pode ser o sucessor.
 *
 * Retorna NULL se o nó for o maior da árvore (não há sucessor).
 */
NoArvBin *sucessor(NoArvBin *raiz, int id) {
    NoArvBin *atual     = raiz;
    NoArvBin *candidato = NULL; /* guarda o último ancestral pelo qual descemos à esquerda */

    while (atual != NULL) {
        if (id < atual->id) {
            candidato = atual;  /* atual é maior que id: pode ser o sucessor */
            atual = atual->esq; /* continua buscando à esquerda (pode haver algo menor e ainda maior que id) */
        } else if (id > atual->id) {
            atual = atual->dir; /* id está à direita: vai para lá */
        } else {
            /* encontrou o nó com o id procurado */
            if (atual->dir != NULL) {
                return minimo(atual->dir); /* caso 1: sucessor é o mínimo da subárvore direita */
            }
            return candidato; /* caso 2: sem filho direito, usa o ancestral guardado */
        }
    }
    return NULL; /* id não existe na árvore */
}

/*
 * antecessor: próximo valor menor que o nó de id dado na ordem crescente.
 *
 * Simétrico ao sucessor (troca esquerda/direita e min/max):
 *
 *   CASO 1 — o nó tem filho esquerdo:
 *     O antecessor é o MÁXIMO da subárvore esquerda.
 *     (o maior dos valores que são menores que ele)
 *
 *   CASO 2 — o nó NÃO tem filho esquerdo:
 *     O antecessor é o ancestral mais próximo pelo qual subimos à direita.
 *     'candidato' rastreia esse ancestral: sempre que vamos para a direita,
 *     o nó atual pode ser o antecessor.
 *
 * Retorna NULL se o nó for o menor da árvore (não há antecessor).
 */
NoArvBin *antecessor(NoArvBin *raiz, int id) {
    NoArvBin *atual     = raiz;
    NoArvBin *candidato = NULL; /* guarda o último ancestral pelo qual descemos à direita */

    while (atual != NULL) {
        if (id > atual->id) {
            candidato = atual;  /* atual é menor que id: pode ser o antecessor */
            atual = atual->dir; /* continua buscando à direita */
        } else if (id < atual->id) {
            atual = atual->esq; /* id está à esquerda: vai para lá */
        } else {
            /* encontrou o nó com o id procurado */
            if (atual->esq != NULL) {
                return maximo(atual->esq); /* caso 1: antecessor é o máximo da subárvore esquerda */
            }
            return candidato; /* caso 2: sem filho esquerdo, usa o ancestral guardado */
        }
    }
    return NULL; /* id não existe na árvore */
}

/* ══════════════════════════════════════════════════════════════════════════════
   REMOÇÃO
   ══════════════════════════════════════════════════════════════════════════════ */

/*
 * remover: remove o nó com o id da ABB mantendo a propriedade de busca.
 *
 * Três casos ao encontrar o nó a remover:
 *
 *   CASO 1 — nó sem filho esquerdo (zero filhos ou só filho direito):
 *     Substitui o nó pelo seu filho direito (ou NULL se for folha).
 *     O filho direito "sobe" para ocupar o lugar do nó removido.
 *
 *   CASO 2 — nó sem filho direito (só filho esquerdo):
 *     Substitui o nó pelo seu filho esquerdo.
 *
 *   CASO 3 — nó com dois filhos:
 *     Não podemos simplesmente remover sem quebrar a estrutura.
 *     Estratégia: substituímos os DADOS do nó pelo seu SUCESSOR em ordem
 *     (mínimo da subárvore direita) e depois removemos o sucessor original.
 *     O sucessor sempre cai no caso 1 ou 2 (tem no máximo um filho direito).
 *
 * Usa o padrão "retornar e reatribuir" para o pai atualizar o ponteiro filho.
 */
NoArvBin *remover(NoArvBin *raiz, int id) {
    if (raiz == NULL)
        return NULL; /* caso base: chegou em ramo vazio, id não existe */

    if (id < raiz->id) {
        raiz->esq = remover(raiz->esq, id); /* busca na subárvore esquerda */
    } else if (id > raiz->id) {
        raiz->dir = remover(raiz->dir, id); /* busca na subárvore direita  */
    } else {
        /* encontrou o nó a remover */

        /* CASO 1: sem filho esquerdo */
        if (raiz->esq == NULL) {
            NoArvBin *temp = raiz->dir; /* salva o filho direito (pode ser NULL) */
            free(raiz);                 /* libera o nó atual                      */
            return temp;               /* filho direito sobe para ocupar o lugar  */
        }

        /* CASO 2: sem filho direito */
        if (raiz->dir == NULL) {
            NoArvBin *temp = raiz->esq; /* salva o filho esquerdo */
            free(raiz);                 /* libera o nó atual       */
            return temp;               /* filho esquerdo sobe      */
        }

        /* CASO 3: dois filhos — usa o sucessor em ordem como substituto */
        NoArvBin *temp = minimo(raiz->dir); /* sucessor: menor da subárvore direita    */
        raiz->id = temp->id;               /* copia id e nome do sucessor para cá     */
        strcpy(raiz->nome, temp->nome);
        raiz->dir = remover(raiz->dir, temp->id); /* remove o sucessor original (caso 1 ou 2) */
    }

    return raiz; /* retorna a raiz da subárvore (possivelmente a mesma) */
}
