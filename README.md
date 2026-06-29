# Estrutura de Dados II — IFFar

Repositório de estudos da disciplina **Estrutura de Dados II** do curso de **Ciências da Computação** do [Instituto Federal Farroupilha (IFFar)](https://www.iffarroupilha.edu.br/).

O repositório concentra o estudo de dois temas principais: **grafos** e **árvores** (binárias de busca e rubro-negras).

---

## Estrutura de pastas

```
Execicios aulas/
├── Aula 1/                  ← Exercícios iniciais: ordenação e algoritmos básicos
│   ├── bubbleSort.c
│   ├── selectionSort.c
│   ├── insertionSort.c
│   ├── mergeSort.c
│   ├── quickSort.c
│   └── ...
│
├── Aula 2/                  ← Continuação de exercícios de ordenação
├── Aula 3/                  ← Continuação de exercícios
├── Aula 4/                  ← Continuação de exercícios
│
├── Aula 5/                  ← Projeto principal: Grafos e Árvores
│   ├── main.c               ← Ponto de entrada; menu interativo
│   ├── IACODES.h / .c       ← Utilitários de I/O e geração de imagem via Graphviz
│   ├── GrafosGrafica.h / .c ← Grafos com ponteiros diretos (completo)
│   ├── GrafosLista.h / .c   ← Grafos com lista encadeada
│   ├── GrafosMatriz.h / .c  ← Grafos com matriz de adjacência
│   ├── ArvBin.h / .c        ← Árvore binária de busca (ABB)
│   ├── ArvRN.h / .c         ← Árvore rubro-negra
│   ├── Aulas/               ← PDFs e anotações das aulas
│   ├── images/              ← Imagens geradas (grafos e árvores)
│   ├── todo/                ← Tarefas e progresso por aula
│   └── melhorias_documentadas/
│
└── Atividades/              ← Atividades avaliativas entregues
    └── Atividade 01/        ← MergeSort (vinicius_richter_mergeSort.zip)
```

---

## Temas estudados

### Grafos (`Aula 5/`)

Três formas de representação implementadas, selecionáveis no menu do programa:

| Implementação | Arquivo | Descrição |
|---|---|---|
| Ponteiros diretos | `GrafosGrafica.c` | Listas de adjacência com referências diretas entre nós |
| Lista encadeada | `GrafosLista.c` | Lista encadeada de adjacência |
| Matriz de adjacência | `GrafosMatriz.c` | Matriz 2D para representar arestas |

Operações disponíveis: adicionar/remover vértice, adicionar/remover aresta, visualizar grafo, descobrir caminho entre dois vértices (DFS).

### Árvores (`Aula 5/`)

| Estrutura | Arquivo | Descrição |
|---|---|---|
| Árvore Binária de Busca (ABB) | `ArvBin.c` | Inserção, remoção, mínimo, máximo, sucessor, antecessor, travessias in-order / pre-order / post-order |
| Árvore Rubro-Negra | `ArvRN.c` | Inserção com rotações e recoloração; remoção |

---

## Como compilar

É necessário ter o **GCC** instalado. Se não tiver, instale via:

```powershell
winget install mingw.mingw
```

Na pasta `Aula 5/`, compile todos os arquivos `.c` juntos:

```powershell
gcc main.c IACODES.c GrafosGrafica.c GrafosLista.c GrafosMatriz.c ArvBin.c ArvRN.c -o programa.exe
```

Para ativar avisos do compilador (recomendado):

```powershell
gcc main.c IACODES.c GrafosGrafica.c GrafosLista.c GrafosMatriz.c ArvBin.c ArvRN.c -o programa.exe -Wall -Wextra
```

---

## Como executar

```powershell
.\programa.exe
```

O programa exibe um menu interativo. Na inicialização, escolha o método de representação do grafo. Em seguida, use as opções numeradas para operar sobre a estrutura.

### Visualização gráfica (opcional)

O programa pode gerar imagens dos grafos e árvores via **Graphviz**. Para instalar:

```powershell
winget install graphviz.graphviz
```

Após instalar, adicione o Graphviz ao PATH:

```powershell
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\Program Files\Graphviz\bin", "User")
```

Feche e reabra o terminal, depois verifique:

```powershell
dot -V
```

As imagens geradas (`grafo.png`, `arvore.png`) ficam salvas na pasta `images/`.

---

## Dependências

- GCC (MinGW) — compilação
- Graphviz — visualização gráfica (opcional)
