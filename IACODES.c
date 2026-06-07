#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IACODES.h"

int IA_lerInteiro(const char *mensagem){
    // GERADO POR IA
    // Código genérico para fazer leitura de números int com segurança
    int num;
    while(1){
        printf("%s", mensagem);
        if(scanf("%d", &num) == 1){
           break; 
        }
        printf("Valor inválido.\n");
        while(getchar() != '\n'); // limpeza de buffer
    }
    while(getchar() != '\n'); // limpeza de buffer
    return num;
}

float IA_lerDecimal(const char *mensagem){
    // GERADO POR IA
    // Código genérico para fazer leitura de números float com segurança
    float num;
    while(1){
        printf("%s", mensagem);
        if(scanf("%f", &num) == 1){
           break; 
        }
        printf("Valor inválido.\n");
        while(getchar() != '\n'); // limpeza de buffer
    }
    while(getchar() != '\n'); // limpeza de buffer
    return num;
}

void IA_lerTexto(char *variavel, int tamanho, const char *mensagem){
    // GERADO POR IA
    // Função genérica para leitura de textos longos
    printf("%s", mensagem);
    fgets(variavel, tamanho, stdin);
    variavel[strcspn(variavel, "\n")] = '\0';
}

void IA_mostrarGrafoGrafica(GrafoG* raiz[], int quantidade_vertices, int eh_dirigido, int eh_ponderado){
    FILE *arquivo = fopen("grafo.dot", "w");

    if(arquivo == NULL){
        printf("Erro ao criar arquivo DOT.\n");
        return;
    }

    // dirigido ou não
    if(eh_dirigido){
        fprintf(arquivo, "digraph G {\n");
    }
    else{
        fprintf(arquivo, "graph G {\n");
    }

    // cria vértices
    for(int i = 0; i < quantidade_vertices; i++){

        if(raiz[i] == NULL){
            continue;
        }

        fprintf(
            arquivo,
            "    %d [label=\"%s\"];\n",
            raiz[i]->id,
            raiz[i]->nome
        );
    }

    // cria arestas
    for(int i = 0; i < quantidade_vertices; i++){

        if(raiz[i] == NULL){
            continue;
        }

        GrafoG* atual = raiz[i];

        for(int j = 0; j < atual->qntdVizinhos; j++){

            GrafoG* vizinho =
                atual->listaVizinhos[j];

            if(eh_dirigido){

                if(eh_ponderado){
                    fprintf(
                        arquivo,
                        "    %d -> %d [label=\"%.2f\"];\n",
                        atual->id,
                        vizinho->id,
                        atual->listaPesos[j]
                    );
                }
                else{
                    fprintf(
                        arquivo,
                        "    %d -> %d;\n",
                        atual->id,
                        vizinho->id
                    );
                }

            }
            else{

                // evita duplicar arestas
                if(atual->id < vizinho->id){

                    if(eh_ponderado){
                        fprintf(
                            arquivo,
                            "    %d -- %d [label=\"%.2f\"];\n",
                            atual->id,
                            vizinho->id,
                            atual->listaPesos[j]
                        );
                    }
                    else{
                        fprintf(
                            arquivo,
                            "    %d -- %d;\n",
                            atual->id,
                            vizinho->id
                        );
                    }
                }
            }
        }
    }

    fprintf(arquivo, "}\n");

    fclose(arquivo);

    // gera PNG
    if(system("dot -Tpng grafo.dot -o grafo.png") != 0){
        printf("Erro: Graphviz nao encontrado ou falhou ao gerar a imagem.\n");
        printf("Instale com: winget install graphviz.graphviz\n");
        printf("O arquivo grafo.dot foi gerado e pode ser visualizado em: https://dreampuf.github.io/GraphvizOnline\n");
        return;
    }

    // abre imagem
    system("start grafo.png");

    printf("Grafo renderizado com sucesso.\n");
}