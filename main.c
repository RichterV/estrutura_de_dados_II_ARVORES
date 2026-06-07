/*
Autoria: Professor Alex Borges
Ano 2026
Código desenvolvido para ensino da disciplina de Estrutura de Dados 2
Bacharelado em Ciência da Computação
Instituto Federal Farroupilha
*/

#include <stdio.h>
#include <string.h> //<> faz buscar no dir de instalação do mingw
#ifdef _WIN32
#include <windows.h>
#endif
#include "GrafosGrafica.h" //Aspas faz buscar na raiz do diretório de trabalho
#include "GrafosLista.h"
#include "GrafosMatriz.h"
#include "IACODES.h"

// VARIAVEIS GLOBAIS
typedef enum {INVALIDO, GRAFICA, LISTA, MATRIZ} GRAFOMETODO; // armazena o método de se trabalhar com grafos
typedef enum {EH_DIRIGIDO, NAO_EH_DIRIGIDO} DIRECAO; // informa se o grafo é ou não é direcionado
typedef enum {EH_PONDERADO, NAO_EH_PONDERADO} PONDERACAO; // informa se o grafo é ou não é ponderado

// VALORES INICIAIS DO PROGRAMA
int UNICID = 0;
GRAFOMETODO GRAFMET = INVALIDO;
DIRECAO QUALDIRECAO = NAO_EH_DIRIGIDO;
PONDERACAO QUALPONDERACAO = NAO_EH_PONDERADO;

// SÉRIE DE FUNÇÕES PRÁTICAS
int 
MENU(){ /* MOSTRA O MENU PARA O USUÁRIO */
    int opcao;
    while(1){ // repita pra sempre
        if(GRAFMET == INVALIDO){
            printf("Por favor, escolha um método de se trabalhar com grafos:\n");
            printf("[1] Grafos com representação gráfica\n");
            printf("[2] Grafo com lista de adjacência\n");
            printf("[3] Grafo com matriz de adjacência\n");
            printf("Opte apenas por [1], [2] ou [3]: ");
            scanf("%d", &opcao);
            while(getchar() != '\n'); // limpar buffer após scanf
            if(opcao < 1 || opcao > 3){
                printf("Valor inválido digitado\n");
                continue;
            }
        }
        else{
            printf("Por favor, escolha uma opção:\n");

            printf("[1] Adicionar Vértice\n");
            printf("[2] Adicionar Aresta\n");
            printf("[3] Remover Aresta\n");
            printf("[4] Remover Vértice\n");
            printf("[5] Ver Grafo\n");
            printf("[6] Descobrir Caminho\n");
            printf("[0] SAIR\n");
            printf("Opte apenas por [1], [2], [3], [4], [5], [6] ou [0]: ");
            scanf("%d", &opcao);
            while(getchar() != '\n'); // limpar buffer após scanf
            if(opcao < 0 || opcao > 6){
                printf("Valor inválido digitado\n");
                continue;
            }
        }
        //se nenhum continue foi executado, pode sair do laço
        break;
    }

    return opcao;
}

void pausa() { /* FAZ O CÓDIGO DAR UMA PAUSA PARA MOSTRAR MENSAGEM BONITA AO UDUÁRIO */
    //Essa função serve apenas para que o menu não seja executado direto
    printf("Aperte a tecla ENTER para continuar...");
    getchar(); // aguarda o Enter do usuário (buffer já foi limpo após cada scanf)
    system("cls"); // limpa o terminal no Windows
}

int lerIDVertices(){
    //função que mostra os IDs dos vértices e retorna o ID digitado, ou não há vertices e retorna -1
    int lista_ids[100];
    int qntd_id = 0;
    if(GRAFMET == GRAFICA){
        qntd_id = obterListaIDDosVerticesGrafica(lista_ids);
    }
    else if(GRAFMET == LISTA){
        qntd_id = obterListaIDDosVerticesLista(lista_ids);
    }
    else if(GRAFMET == MATRIZ){
        qntd_id = obterListaIDDosVerticesMatriz(lista_ids);
    }

    if(qntd_id == 0){
        printf("Não há vértices no grafo para serem removidos.\n");
        return -1;
    }
    else{
        int id_vertice;
        int digitou_certo = 0;
        while(digitou_certo == 0){
            printf("Há os seguintes vértices no grafo: ");
            for(int i = 0; i < qntd_id; i++){
                printf("[%d], ", lista_ids[i]);
            }
            id_vertice = IA_lerInteiro("agora, escolha um vértice: ");
            for(int i = 0; i < qntd_id; i++){
                if(lista_ids[i] == id_vertice){
                    digitou_certo = 1;
                }
            }

            if(digitou_certo == 0){
                printf("O id digitado não existe na lista apresentada.\n");
            }
        }
        return id_vertice;
    }
}

int existeVertice(int id){
    int existe = 0;
    if(GRAFMET == GRAFICA){
        existe = existeVerticeGrafica(id);
    }
    else if(GRAFMET == LISTA){
        existe = existeVerticeLista(id);
    }
    else if(GRAFMET == MATRIZ){
        existe = existeVerticeMatriz(id);
    }
    return existe;
}

void inicializarGrafo(){
    int pond = QUALPONDERACAO == NAO_EH_PONDERADO ? 0 : 1;
    int dire = QUALDIRECAO == NAO_EH_DIRIGIDO ? 0 : 1;
    if(GRAFMET == GRAFICA){
        inicializarGrafoGrafica(pond, dire);
    }
    else if(GRAFMET == LISTA){
        inicializarGrafoLista(pond, dire);
    }
    else if(GRAFMET == MATRIZ){
        inicializarGrafoMatriz(pond, dire);
    }
}

void adicionarVertice(){
    char texto[255];
    IA_lerTexto(texto, 255, "Diga a informação que deseja armazenar no vértice: ");
    int id = UNICID++; //id = UNICID depois UNICID++;

    if(GRAFMET == GRAFICA){
        adicionarVerticeGrafica(id, texto);
    }
    else if(GRAFMET == LISTA){
        adicionarVerticeLista(id, texto);
    }
    else if(GRAFMET == MATRIZ){
        adicionarVerticeMatriz(id, texto);
    }
}

void adicionarAresta(){
    printf("Vamos adicionar uma aresta:\n");
    int inicio = lerIDVertices();
    int fim = lerIDVertices();

    if(inicio == -1 || fim == -1){
        printf("Não há vértices disponíveis a serem adicionados arestas.\n");
    }

    if(inicio == fim){
        printf("Não é permitido adicionar uma aresta laço.\n");
        return;
    }

    float peso = 0.0;

    if(QUALPONDERACAO == EH_PONDERADO){
        peso = IA_lerDecimal("Diga qual o peso a ser adicionado na aresta:");
    }

    if(GRAFMET == GRAFICA){
        adicionarArestaGrafica(inicio, fim, peso);
    }
    else if(GRAFMET == LISTA){
        adicionarArestaLista(inicio, fim, peso);
    }
    else if(GRAFMET == MATRIZ){
        adicionarArestaMatriz(inicio, fim, peso);
    }
}

void removerAresta(){
    printf("Vamos remover uma aresta:\n");
    int inicio = lerIDVertices();
    int fim = lerIDVertices();

    if(inicio == -1 || fim == -1){
        printf("Não há vértices disponíveis a serem removidas arestas.\n");
    }

    if(inicio == fim){
        printf("Não existe aresta laço.\n");
        return;
    }
    
    if(GRAFMET == GRAFICA){
        removerArestaGrafica(inicio, fim);
    }
    else if(GRAFMET == LISTA){
        removerArestaLista(inicio, fim);
    }
    else if(GRAFMET == MATRIZ){
        removerArestaMatriz(inicio, fim);
    }
}

void removerVertice(){
    printf("Vamos remover um vértice:\n");
    int id = lerIDVertices();

    if(id == -1){
        printf("Não há vértices disponíveis a serem removidas.\n");
    }
    
    if(GRAFMET == GRAFICA){
        removerVerticeGrafica(id);
    }
    else if(GRAFMET == LISTA){
        removerVerticeLista(id);
    }
    else if(GRAFMET == MATRIZ){
        removerVerticeMatriz(id);
    }
}

void mostrarGrafo(){
    if(GRAFMET == GRAFICA){
        mostrarGrafoGrafica();
    }
    else if(GRAFMET == LISTA){
        mostrarGrafoLista();
    }
    else if(GRAFMET == MATRIZ){
        mostrarGrafoMatriz();
    }
}

void descobrirCaminho(){
    printf("Vamos descobrir o caminho:");
    int inicio = lerIDVertices();
    int fim = lerIDVertices();

    if(inicio == -1 || fim == -1){
        printf("Não há vértices disponíveis para traçar um caminho.\n");
    }
    
    if(GRAFMET == GRAFICA){
        descobrirCaminhoGrafica(inicio, fim);
    }
    else if(GRAFMET == LISTA){
        descobrirCaminhoLista(inicio, fim);
    }
    else if(GRAFMET == MATRIZ){
        descobrirCaminhoMatriz(inicio, fim);
    }
}

int main(){
    //variáveis que auxiliam o fluxo de interação
    int opcaomenu;
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    //Código Principal
    printf("Olá aluno de ED2, seja bem vindo ao programa.\n");
    do{ //repete pra sempre
        opcaomenu = MENU();

        if(GRAFMET == INVALIDO){ // primeira execução será sempre essa daqui, definindo tipo de grafo
            if(opcaomenu == 1){
                GRAFMET = GRAFICA;
                printf("Aplicando sistema de grafos com representação gráfica...\n");
            }
            else if(opcaomenu == 2){
                GRAFMET = LISTA;
                printf("Aplicando sistema de grafos com lista de adjacência...\n");
            }
            else if(opcaomenu == 3){
                GRAFMET = MATRIZ;
                printf("Aplicando sistema de grafos com matriz de adjacência...\n");
            }

            // pergunta se o grafo é ponderado
            int resp;
            do {
                printf("O grafo é ponderado (possui pesos nas arestas)?\n");
                printf("[1] Sim\n[2] Não\nEscolha: ");
                scanf("%d", &resp);
                while(getchar() != '\n'); // limpar buffer após scanf
            } while(resp != 1 && resp != 2);
            QUALPONDERACAO = (resp == 1) ? EH_PONDERADO : NAO_EH_PONDERADO;

            // pergunta se o grafo é dirigido
            do {
                printf("O grafo é dirigido (as arestas têm sentido)?\n");
                printf("[1] Sim\n[2] Não\nEscolha: ");
                scanf("%d", &resp);
                while(getchar() != '\n'); // limpar buffer após scanf
            } while(resp != 1 && resp != 2);
            QUALDIRECAO = (resp == 1) ? EH_DIRIGIDO : NAO_EH_DIRIGIDO;

            inicializarGrafo();
        }
        else{
            if     (opcaomenu == 1) adicionarVertice();
            else if(opcaomenu == 2) adicionarAresta();
            else if(opcaomenu == 3) removerAresta();
            else if(opcaomenu == 4) removerVertice();
            else if(opcaomenu == 5) mostrarGrafo();
            else if(opcaomenu == 6) descobrirCaminho();
        }

        if(opcaomenu != 0) pausa();
    }while(opcaomenu != 0);
    //fim código principal
    printf("Agradecemos a preferência\n");

    return 0;
}