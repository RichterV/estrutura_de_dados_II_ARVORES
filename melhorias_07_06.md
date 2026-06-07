# Melhorias — 07/06/2026

## Atividade 1 — Pesos às arestas (GrafosLista)

### O que foi feito

Implementação completa de `GrafosLista.c`, com suporte a pesos nas arestas.
Antes desta data, todas as funções do arquivo retornavam apenas `"Função Não Implementada"`.

---

### Estrutura de dados utilizada

O grafo é representado por um vetor global de ponteiros `vertices[100]`, onde cada posição aponta para um nó `GrafoL` que representa um vértice. O campo `proximo` de cada vértice é o início de sua **lista encadeada de adjacência** — os nós dessa lista são também do tipo `GrafoL` e armazenam o `id` do vizinho e o `peso` da aresta.

```
vertices[0] → { id=0, nome="A" } → { id=1, peso=2.5 } → { id=2, peso=1.0 } → NULL
vertices[1] → { id=1, nome="B" } → { id=0, peso=2.5 } → NULL
vertices[2] → { id=2, nome="C" } → { id=0, peso=1.0 } → NULL
```

O struct `GrafoL` (definido em `GrafosLista.h`) já possuía o campo `peso`:

```c
typedef struct GrafoL {
    int id;
    struct GrafoL* proximo;  // próximo nó na lista encadeada
    float peso;              // peso da aresta (usado nos nós de adjacência)
    char nome[100];          // nome do vértice (usado nos nós-vértice)
} GrafoL;
```

---

### Funções implementadas

#### `inicializarGrafoLista(int eh_ponderado, int eh_dirigido)`
Zera o vetor `vertices[100]` e salva as flags que indicam se o grafo é ponderado e/ou dirigido. Essas flags controlam o comportamento das demais funções.

#### `adicionarVerticeLista(int id, char* texto)`
Aloca um novo `GrafoL` na memória heap, preenche `id` e `nome`, define `peso = 0.0` e `proximo = NULL`, e o insere em `vertices[QUANTIDADE_VERTICES]`.

#### `adicionarArestaLista(int inicio, int fim, float peso)`
- Localiza os nós-vértice de `inicio` e `fim` no vetor `vertices`.
- Aloca um nó de adjacência com `id = fim` e `peso = peso`, e o encadeia no final da lista de `inicio`.
- Se o grafo **não for dirigido**, faz o mesmo no sentido inverso (`fim → inicio`), também com o mesmo `peso`.
- Se o grafo **for ponderado**, exibe o peso na mensagem de confirmação.

#### `removerArestaLista(int inicio, int fim)`
- Percorre a lista de adjacência de `inicio` procurando um nó com `id == fim`.
- Ao encontrar, reconecta os ponteiros ao redor do nó e libera sua memória com `free`.
- Se não for dirigido, repete o processo na lista de `fim` procurando `inicio`.

#### `removerVerticeLista(int id)`
1. Remove, na lista de adjacência de cada outro vértice, qualquer nó que aponte para `id`.
2. Percorre e libera com `free` toda a lista de adjacência do próprio vértice.
3. Libera o nó-vértice.
4. Desloca o vetor `vertices` para preencher o espaço vago (shift).

#### `mostrarGrafoLista()`
Percorre `vertices` e, para cada vértice, imprime sua lista de adjacência no formato:

```
[id] nome -> [viz_id](peso) -> [viz_id](peso) -> NULL   (ponderado)
[id] nome -> [viz_id] -> [viz_id] -> NULL                (não ponderado)
```

O peso só aparece quando o grafo foi inicializado como ponderado.

#### `descobrirCaminhoLista(int inicio, int fim)`
Executa uma **busca em profundidade (DFS)** recursiva. Usa um vetor `visitados[200]` indexado por ID para evitar ciclos. Exibe cada vértice visitado durante o caminho e informa se o destino foi alcançado ou não.

#### `existeVerticeLista(int id)` / `obterListaIDDosVerticesLista(int* lista)`
Auxiliares usadas pelo `main.c` para verificar existência de vértices e listar IDs disponíveis no menu.

---

### Como o peso é armazenado

O peso fica salvo **no nó de adjacência**, não no vértice. Ou seja, cada aresta carrega seu próprio peso independentemente. Para grafos não ponderados, o campo `peso` existe na memória mas é ignorado na exibição e na inicialização recebe `0.0`.

### Como o menu usa os pesos

O controle de quando pedir o peso ao usuário já estava no `main.c` (função `adicionarAresta`):

```c
float peso = 0.0;
if(QUALPONDERACAO == EH_PONDERADO){
    peso = IA_lerDecimal("Diga qual o peso a ser adicionado na aresta:");
}
adicionarArestaLista(inicio, fim, peso);
```

O valor é sempre passado para `adicionarArestaLista` — se não for ponderado, chega como `0.0` e é armazenado mas não exibido.

---

### Outras alterações em `main.c`

#### Ativar modo ponderado (`main.c:28`)

A variável global `QUALPONDERACAO` foi alterada para ativar o modo ponderado por padrão:

```c
// antes
PONDERACAO QUALPONDERACAO = NAO_EH_PONDERADO;

// depois
PONDERACAO QUALPONDERACAO = EH_PONDERADO;
```

Sem essa mudança, o menu nunca pedia o peso ao adicionar uma aresta.

#### Limpeza de tela real (`main.c`, função `pausa`)

A função `pausa()` usava 20 quebras de linha para simular uma nova página. Substituído por `system("cls")` para limpar o terminal de fato no Windows:

```c
// antes
printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

// depois
system("cls");
```
