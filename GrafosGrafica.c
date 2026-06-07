#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GrafosGrafica.h"
#include "IACODES.h"

//variável global local
static int EH_PONDERADO;
static int EH_DIRIGIDO;
static int QUANTIDADE_VERTICES;
static GrafoG* raiz[100];

int existeVerticeGrafica(int id){
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(raiz[i] != NULL && raiz[i]->id == id){
            return 1; //existe
        }
    }
    return 0; //não existe
}

GrafoG* buscarVerticeGrafica(int id){
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(raiz[i] != NULL && raiz[i]->id == id){
            return raiz[i];
        }
    }

    return NULL;
}

int obterListaIDDosVerticesGrafica(int* lista_de_id){
    //esta função apenas cria uma lista dos IDS válidos que tem no grafo
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        lista_de_id[i] = raiz[i]->id;
    }
    return QUANTIDADE_VERTICES; //envio por fim, a quantidade de vértices que temos
}

//Função auxiliar para realizar busca de caminho em grafo. 
//Este aqui, é a versão de BUSCA EM PROFUNDIDADE
int buscaEmProfundidade(GrafoG* atual, GrafoG* destino, int lista_visitados[]){

    lista_visitados[atual->id] = 1; // marca a posição do nó atual como visitado

    printf("[%d] %s\n", atual->id, atual->nome); // mostra pro usuário o caminho atual

    if(atual == destino){
        // Caso achar o caminho, encerrar a busca
        return 1;
    }

    for(int i = 0; i < atual->qntdVizinhos; i++){
        // Para cada vizinho do nó atual, varre as possibilidades
        GrafoG* vizinho = atual->listaVizinhos[i];

        if(lista_visitados[vizinho->id]){
            //Caso o id do vizinho já foi visitado, passa pro próximo vizinho
            continue;
        }

        // Chamo recursivamente a própria função, onde o atual vira vizinho
        // se eu encontrar o caminho, já encerro aqui
        if(buscaEmProfundidade(vizinho, destino, lista_visitados)){
            return 1;
        }
    }
    // Se chegou aqui, é pq não encontrou caminho, desta forma, desistir desse ramo da busca
    return 0;
}

void inicializarGrafoGrafica(int eh_ponderado, int eh_dirigido){
    //Aqui a gente inicializa o nó raiz deste grafo
    for(int i = 0; i<100; i++){
        raiz[i] = NULL; // não temos vizinhos existentes
    }
    
    //aqui inicializamos o controlador de grafo ponderado
    EH_PONDERADO = eh_ponderado;

    //aqui inicializamos o controlador de grafo dirigido
    EH_DIRIGIDO = eh_dirigido;

    //aqui inicializamos o controlador de número de vértices
    QUANTIDADE_VERTICES = 0;
}

void adicionarVerticeGrafica(int id, char* texto){
    // criando novo vertice na memória
    GrafoG *novo = (GrafoG*)malloc(sizeof(GrafoG));
    if(novo == NULL){
        printf("Erro na alocação de memória de novo vértice.\n");
        return;
    }
    novo->id = id;
    strncpy(novo->nome, texto, 99);
    novo->nome[99] = '\0';
    novo->qntdVizinhos = 0;
    for(int i = 0; i < 100; i++){
        novo->listaVizinhos[i] = NULL;
        novo->listaPesos[i] = 0.0;
    }

    // agora inserindo esse vértice à raiz
    raiz[QUANTIDADE_VERTICES++] = novo;

    printf("Novo vértice (%d) adicionado ao grafo de representação gráfica.\n", id);
}

void adicionarArestaGrafica(int inicio, int fim, float peso){
    //primeiramente, vamos nos garantir de que o nó inicio existe
    GrafoG *noinicio, *nofim;
    if(existeVerticeGrafica(inicio)){
        noinicio = buscarVerticeGrafica(inicio);
    }
    else{
        printf("Não é possível encontrar aresta, pois não existe o vértice inicial\n");
        return;
    }

    if(existeVerticeGrafica(fim)){
        nofim = buscarVerticeGrafica(fim);
    }
    else{
        printf("Não é possível encontrar aresta, pois não existe o vértice final\n");
        return;
    }

    //colocando a aresta de cá pra lá
    noinicio->listaVizinhos[noinicio->qntdVizinhos] = nofim;
    noinicio->listaPesos[noinicio->qntdVizinhos] = peso;
    noinicio->qntdVizinhos++;
    if(EH_DIRIGIDO == 0){ //ou seja, não é dirigido, então precisa colocar a aresta de lá pra cá
        nofim->listaVizinhos[nofim->qntdVizinhos] = noinicio;
        nofim->listaPesos[nofim->qntdVizinhos] = peso;
        nofim->qntdVizinhos++;
    }

    if(EH_PONDERADO){
        printf("A aresta (%d, %d, %f) foi adicionada com sucesso.\n", inicio, fim, peso);
    }
    else{
        printf("A aresta (%d, %d) foi adicionada com sucesso.\n", inicio, fim);
    }
}

void mostrarGrafoGrafica(){
    IA_mostrarGrafoGrafica(raiz, QUANTIDADE_VERTICES, EH_DIRIGIDO, EH_PONDERADO);
}

void descobrirCaminhoGrafica(int inicio, int fim){
    //primeira coisa é ver se os vértices existem
    GrafoG *noinicio, *nofim;
    if(existeVerticeGrafica(inicio)){
        noinicio = buscarVerticeGrafica(inicio);
    }
    else{
        printf("Não é possível encontrar caminho, pois não existe o vértice inicial\n");
        return;
    }

    if(existeVerticeGrafica(fim)){
        nofim = buscarVerticeGrafica(fim);
    }
    else{
        printf("Não é possível encontrar caminho, pois não existe o vértice final\n");
        return;
    }


    int eh_pra_usar_solucao_em_profundidade = 1;
    if(eh_pra_usar_solucao_em_profundidade){
        //SOLUÇÃO EM PROFUNDIDADE
        //pra profundidade, a primeira coisa a se fazer é criar a lista de nós que foram visitados na busca
        //Veja, se eu tenho um vetor chamado raiz que tem todos os vértices inseridos,
        //então vou utilizar esse vetor como apoio. Crio um vetor de posições e penso nele como um booleano
        int lista_visitados[QUANTIDADE_VERTICES];
        for(int i = 0; i < QUANTIDADE_VERTICES; i++){
            lista_visitados[i] = 0; //Para cada posição, o indice do vértice ainda não foi visitado
        }

        printf("\nBuscando caminho em profundidade...\n\n");

        if(buscaEmProfundidade(noinicio, nofim, lista_visitados)){
            printf("\nCaminho encontrado.\n");
        }
        else{
            printf("\nNão existe caminho.\n");
        }
    }
    else{
        //SOLUÇÃO EM PROFUNDIDADE

        //ALUNO, IMPLEMENTE
    }

}

void removerArestaGrafica(int inicio, int fim){
    //primeira coisa é ver se os vértices existem
    GrafoG *noinicio, *nofim;
    if(existeVerticeGrafica(inicio)){
        noinicio = buscarVerticeGrafica(inicio);
    }
    else{
        printf("Não é possível encontrar o vértice inicial\n");
        return;
    }

    if(existeVerticeGrafica(fim)){
        nofim = buscarVerticeGrafica(fim);
    }
    else{
        printf("Não é possível encontrar o vértice final\n");
        return;
    }

    //A ideia é fazer o processo inverso do adicionarVertice
    //mas primeiro, temos que achar a posição do vértice final na lista de vizinhos
    int achou = 0; // serve pra mostrar a mensagem correta ao usuário, depois
    for(int i = 0; i < noinicio->qntdVizinhos; i++){
        if(noinicio->listaVizinhos[i] == nofim){
            //assim que eu achar a posição do vertice desejado, vou desfazer a aresta
            //Quando eu achar o vizinho, vou pegando todos na posição seguinte e puxando pra trás

            achou = 1; // antes que eu esqueça, já vou dizer que achei
            
            //agora, da posição atual até a posição final -1, vou copiando da frente pra trás
            for(int j = i; j < noinicio->qntdVizinhos - 1; j++){
                noinicio->listaVizinhos[j] = noinicio->listaVizinhos[j + 1];
                noinicio->listaPesos[j] = noinicio->listaPesos[j + 1];
            }

            //quando for a ultima posição de lista de vizinhos, agora sim removo
            noinicio->listaVizinhos[noinicio->qntdVizinhos - 1] = NULL;
            noinicio->listaPesos[noinicio->qntdVizinhos - 1] = 0.0;

            //ultimo vizinho removido, agora sim reduzo a sua quantidade
            noinicio->qntdVizinhos--; 
            break; // fez o que tinha que fazer, pode encerrar o laço
        }
    }

    if(EH_DIRIGIDO == 0){ //ou seja, não é dirigido, então preciso fazer o mesmo processo de lá pra cá
        for(int i = 0; i < nofim->qntdVizinhos; i++){
            if(nofim->listaVizinhos[i] == noinicio){
                //assim que eu achar a posição do vertice desejado, vou desfazer a aresta
                //Quando eu achar o vizinho, vou pegando todos na posição seguinte e puxando pra trás

                //agora, da posição atual até a posição final -1, vou copiando da frente pra trás
                for(int j = i; j < nofim->qntdVizinhos - 1; j++){
                    nofim->listaVizinhos[j] = nofim->listaVizinhos[j + 1];
                    nofim->listaPesos[j] = nofim->listaPesos[j + 1];
                }

                //quando for a ultima posição de lista de vizinhos, agora sim removo
                nofim->listaVizinhos[nofim->qntdVizinhos - 1] = NULL;
                nofim->listaPesos[nofim->qntdVizinhos - 1] = 0.0;

                //ultimo vizinho removido, agora sim reduzo a sua quantidade
                nofim->qntdVizinhos--;
                break; // fez o que tinha que fazer, pode encerrar o laço
            }
        }
    }

    if(achou){
        printf("A aresta (%d, %d) foi removida com sucesso.\n", inicio, fim);
    }
    else{
        printf("Não foi possível encontrar a aresta (%d, %d) para remoção.\n", inicio, fim);
    }
}

void removerVerticeGrafica(int id){
    // primeiro precisamos verificar se o vértice a ser removido existe
    GrafoG *remover;
    int posicao;
    int achou = 0;

    //poderia usar o remover = buscarVerticeGrafica(id), todavia, preciso do índice na raiz depois
    for(int i = 0; i < QUANTIDADE_VERTICES; i++){
        if(raiz[i] != NULL && raiz[i]->id == id){
            //achei o vértice que estou procurando
            remover = raiz[i];
            posicao = i;
            achou = 1;
            break;
        }
    }

    if(achou == 0){
        // Deu ruim, por alguma razão, tentei remover quem não existe
        printf("Não foi possível encontrar o vértice (%d).\n", id);
        return;
    }

    // Se chegamos aqui, então precisamos fazer três coisas
    // 1) remover TODAS as arestas que têm o vértice que queremos apgar como ponta de fim
    // 2) remover todas as arestas que o vértice que queremos apagar tem para outros vértices
    // 3) remover o vertice em si

    for(int i = 0; i < QUANTIDADE_VERTICES; i++){

        if(raiz[i] == NULL || raiz[i] == remover){
            //Se o vértice não existir ou se for o próprio vértice a ser apagado, IGNORAR
            continue;
        }

        //Senão, pode apagar a aresta
        removerArestaGrafica(raiz[i]->id, remover->id);
    }

    // agora removemos todas as arestas do tal vértice
    for(int i = 0; i < remover->qntdVizinhos; i++){
        remover->listaVizinhos[i] = NULL;
        remover->listaPesos[i] = 0.0;
    }
    remover->qntdVizinhos = 0;

    // lá em adicionarVerticeGrafica a gente usa malloc
    // PORTANTO, aqui precisamos usar o free pra fazer a liberação correta da memória
    free(remover);

    // agora puxa todos os vértices do nosso vetor de controle pra trás
    for(int i = posicao; i < QUANTIDADE_VERTICES - 1; i++){
        raiz[i] = raiz[i + 1];
    }

    // limpa última posição do nosso vetor de controle
    raiz[QUANTIDADE_VERTICES - 1] = NULL;

    // reduz quantidade de vértices no nosso sistema
    QUANTIDADE_VERTICES--;

    printf("O vértice (%d) foi removido com sucesso.\n", id);
}