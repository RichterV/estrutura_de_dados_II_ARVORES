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

#### Limpeza de tela real (`main.c`, função `pausa`)

A função `pausa()` usava 20 quebras de linha para simular uma nova página. Substituído por `system("cls")` para limpar o terminal de fato no Windows:

```c
// antes
printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

// depois
system("cls");
```

---

## Atividade 2 — Opção [2] Grafo com lista de adjacência totalmente funcional

### O que foi feito

A opção `[2] Grafo com lista de adjacência` já aparecia no menu desde o início, mas quatro problemas impediam o funcionamento completo:

1. As configurações de ponderado/dirigido ficavam fixas no código-fonte — o usuário não podia escolher pelo menu.
2. O `scanf` no `MENU()` deixava `\n` no buffer, fazendo `pausa()` limpar a tela sem esperar o Enter do usuário.
3. `IA_lerTexto` tinha um `while(getchar() != '\n')` extra que bloqueava aguardando um Enter desnecessário.
4. O `mostrarGrafoLista` exibia apenas o índice do vizinho, sem o nome.

---

### Alteração 1 — Sub-menu de configuração (`main.c`)

Adicionado logo após o usuário escolher o método, antes de chamar `inicializarGrafo()`:

```c
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
```

Fluxo resultante:
```
Escolha um método: [1] [2] [3]  →  [2] selecionado
O grafo é ponderado? [1] Sim  [2] Não
O grafo é dirigido?  [1] Sim  [2] Não
→ Menu principal
```

---

### Alteração 2 — Correção do buffer de entrada (`main.c` e `IACODES.c`)

**Problema:** `scanf("%d")` deixa o `\n` no buffer. O `pausa()` consumia esse `\n` com `while(getchar() != '\n')` e limpava a tela imediatamente, sem esperar o Enter — o grafo aparecia e sumia na hora.

**Solução:** separar as responsabilidades:

Em `MENU()`, limpar o buffer logo após cada `scanf`:
```c
scanf("%d", &opcao);
while(getchar() != '\n'); // limpar buffer após scanf
```

Em `pausa()`, apenas aguardar o Enter (buffer já está limpo):
```c
// antes
while (getchar() != '\n'); // consumia o \n do scanf e encerrava imediatamente

// depois
getchar(); // aguarda o Enter do usuário
```

Em `IACODES.c`, removido o `while(getchar() != '\n')` do início de `IA_lerTexto`, que foi adicionado como contorno para o buffer sujo e passou a causar duplo Enter após a correção acima:
```c
// antes
while(getchar() != '\n'); // limpeza desnecessária após a correção no MENU
printf("%s", mensagem);

// depois
printf("%s", mensagem); // buffer já limpo, leitura direta
```

---

### Alteração 3 — Exibição do nome do vizinho em `mostrarGrafoLista` (`GrafosLista.c`)

Os nós de adjacência já armazenavam o `nome` do vizinho (copiado em `adicionarArestaLista`). O formato de exibição foi atualizado para incluí-lo:

```c
// antes
printf(" [%d](%.2f)", adj->id, adj->peso);

// depois
printf(" %s[%d](%.2f)", adj->nome, adj->id, adj->peso);
```

Saída resultante:
```
=== Grafo (Lista de Adjacência) ===
[0] A -> B[1](2.00) C[2](5.00) NULL
[1] B -> C[2](6.00) NULL
[2] C -> NULL
```

---

### Comportamento final da opção [2]

| Configuração | Efeito |
|---|---|
| Ponderado = Sim | Menu pede o peso ao adicionar aresta; peso exibido no `mostrarGrafoLista` |
| Ponderado = Não | Peso ignorado; arestas exibidas sem valor |
| Dirigido = Sim | Aresta adicionada apenas no sentido `inicio → fim` |
| Dirigido = Não | Aresta adicionada nos dois sentidos (`inicio ↔ fim`) |

---

## Atividade 3 — Opção [3] Grafo com matriz de adjacência

### O que foi feito

Implementação completa de `GrafosMatriz.c`. Antes desta data, todas as funções retornavam apenas `"Função Não Implementada"`.

---

### Estrutura de dados utilizada

O grafo é representado por dois elementos globais:

- `GrafoM vertices[100]` — vetor de vértices, cada um com `id` e `nome`
- `float matriz[100][100]` — matriz de adjacência onde a linha `i` e coluna `j` correspondem aos vértices nas posições `i` e `j` do vetor

Sentinela: `matriz[i][j] = -1.0` significa **sem aresta**. Qualquer valor `≥ 0.0` indica aresta existente.

```
Vértices: [0]=A  [1]=B  [2]=C

         A     B     C
    A [  -1   2.0   5.0 ]   → A→B(2.0), A→C(5.0)
    B [  -1   -1   6.0  ]   → B→C(6.0)
    C [  -1   -1   -1   ]   → sem arestas
```

Para grafos **não ponderados**, armazena `1.0` em vez do peso (para distinguir "aresta existe" de "sem aresta").

---

### Funções implementadas

#### `inicializarGrafoMatriz(int eh_ponderado, int eh_dirigido)`
Preenche toda a `matriz[100][100]` com `-1.0` e salva as flags de ponderado e dirigido.

#### `adicionarVerticeMatriz(int id, char* texto)`
Insere o vértice diretamente em `vertices[QUANTIDADE_VERTICES]`. A matriz não precisa de alteração — a nova posição já está inicializada com `-1.0`.

#### `adicionarArestaMatriz(int inicio, int fim, float peso)`
- Localiza as posições `pi` e `pf` dos vértices no vetor.
- Armazena `peso` em `matriz[pi][pf]` (ponderado) ou `1.0` (não ponderado).
- Se não for dirigido, espelha o valor em `matriz[pf][pi]`.

#### `removerArestaMatriz(int inicio, int fim)`
Define `matriz[pi][pf] = -1.0`. Se não for dirigido, também `matriz[pf][pi] = -1.0`.

#### `removerVerticeMatriz(int id)`
1. Remove a **linha** do vértice: desloca todas as linhas seguintes uma posição para cima.
2. Remove a **coluna** do vértice: desloca todas as colunas seguintes uma posição para a esquerda.
3. Remove o vértice do vetor `vertices[]` com shift.
4. Decrementa `QUANTIDADE_VERTICES`.

#### `mostrarGrafoMatriz()`
Exibe a matriz com os nomes dos vértices como cabeçalho de linha e coluna:

```
=== Grafo (Matriz de Adjacência) ===
            A      B      C
    A     0.0    2.0    5.0
    B     0.0    0.0    6.0
    C     0.0    0.0    0.0
```

Para grafos não ponderados, exibe `1` (aresta) ou `0` (sem aresta).

#### `descobrirCaminhoMatriz(int inicio, int fim)`
DFS recursiva usando posições no vetor (não IDs). Para cada posição `j`, verifica se `matriz[pos_atual][j] >= 0.0` e se ainda não foi visitado.

---

### Comportamento final da opção [3]

| Configuração | Efeito |
|---|---|
| Ponderado = Sim | Armazena o peso real na célula; exibido na matriz |
| Ponderado = Não | Armazena `1.0` para indicar presença de aresta; exibido como `1` |
| Dirigido = Sim | Apenas `matriz[i][j]` é preenchida |
| Dirigido = Não | `matriz[i][j]` e `matriz[j][i]` são preenchidas com o mesmo valor |
