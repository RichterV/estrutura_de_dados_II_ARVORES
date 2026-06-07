# Aula 5 — Grafos (Estrutura de Dados II / IFFar)

Projeto de estudo fornecido pelo professor com código parcialmente implementado. O aluno deve completar as partes faltantes listadas em `todo.txt`.

---

## Organização do repositório

Todos os arquivos de código ficam na raiz do projeto — estrutura plana intencional. Para ~10 arquivos `.c`/`.h`, isso é o padrão em projetos C acadêmicos: criar subpastas (`src/`, `include/`) exigiria mudar todos os `#include` e adicionar flags `-I` na compilação, sem ganho real neste tamanho.

Arquivos gerados (`grafos.exe`, `grafo.dot`, `grafo.png`) são ignorados pelo git via `.gitignore`.

```
Aula 5/
├── main.c              ← ponto de entrada
├── IACODES.h / .c      ← utilitários de I/O + Graphviz
├── GrafosGrafica.h/.c  ← implementação com ponteiros (completa)
├── GrafosLista.h/.c    ← implementação com lista encadeada (pendente)
├── GrafosMatriz.h/.c   ← implementação com matriz (pendente)
├── .gitignore          ← exclui binários e arquivos gerados
├── DOCS/               ← documentação complementar
└── todo_1.txt          ← tarefas pendentes
```

## Estrutura de arquivos

| Arquivo | Papel |
|---|---|
| `main.c` | Loop principal com menu interativo; abstração que despacha para um dos três métodos |
| `IACODES.h / .c` | Funções auxiliares de entrada (`IA_lerInteiro`, `IA_lerDecimal`, `IA_lerTexto`) e geração de imagem via Graphviz |
| `GrafosGrafica.h / .c` | Implementação **completa** — listas de adjacência com ponteiros diretos |
| `GrafosLista.h / .c` | Implementação **pendente** — lista encadeada de adjacência |
| `GrafosMatriz.h / .c` | Implementação **pendente** — matriz de adjacência |
| `todo_1.txt` | Tarefas a completar |

---

## Arquitetura do programa

### Enumerações globais (`main.c`)

```c
GRAFOMETODO  // GRAFICA | LISTA | MATRIZ
DIRECAO      // EH_DIRIGIDO | NAO_EH_DIRIGIDO
PONDERACAO   // EH_PONDERADO | NAO_EH_PONDERADO
```

Cada operação do menu chama uma função intermediária em `main.c` (ex.: `adicionarVertice()`) que, por `switch(GRAFMET)`, redireciona para a implementação do método escolhido na inicialização.

### Fluxo do menu

1. Usuário escolhe o método de representação (opções 1/2/3).
2. Loop com 6 operações:
   - `[1]` Adicionar vértice
   - `[2]` Adicionar aresta
   - `[3]` Remover aresta
   - `[4]` Remover vértice
   - `[5]` Mostrar grafo
   - `[6]` Descobrir caminho entre dois vértices
3. `[0]` Sair.

---

## Implementações

### GrafosGrafica — COMPLETO

**Estrutura:**
```c
typedef struct GrafoG {
    int id;
    struct GrafoG* listaVizinhos[100];
    int qntdVizinhos;
    float listaPesos[100];
    char nome[100];
} GrafoG;
```

Usa um vetor global de ponteiros (`GrafoG* grafos[100]`). Arestas são referências diretas entre nós.

Funções implementadas: inicializar, adicionar/remover vértice, adicionar/remover aresta, mostrar, descobrir caminho (DFS recursivo), existeVertice, obterListaIDs.

O arquivo `IACODES.c` gera `grafo.dot` e chama `dot -Tpng` para visualização.

---

### GrafosLista — PENDENTE

**Estrutura:**
```c
typedef struct GrafoL {
    int id;
    struct GrafoL* proximo;  // próximo nó na lista encadeada
    float peso;
    char nome[100];
} GrafoL;
```

Todas as funções existem no `.c` mas retornam apenas `"Função Não Implementada"`.

---

### GrafosMatriz — PENDENTE

**Estrutura:**
```c
typedef struct GrafoM {
    int id;
    float peso;
    char nome[100];
} GrafoM;
```

Todas as funções existem no `.c` mas retornam apenas `"Função Não Implementada"`. A ideia é usar uma matriz 2D para representar as arestas entre vértices.

---

## Tarefas pendentes (`todo.txt`)

1. **Adicionar pesos às arestas** — a estrutura `GrafoL` já tem o campo `peso`; precisa ser usado na adição de arestas e no `mostrar`.
2. **Implementar funções 2 e 3** — completar todas as funções de `GrafosLista.c` e `GrafosMatriz.c`.
3. **No menu de inicialização**, descomentar/exibir as opções:
   ```
   [2] Grafo com lista de adjacência
   [3] Grafo com matriz de adjacência
   ```
4. **Instalar Graphviz** para que `system("dot -Tpng grafo.dot -o grafo.png")` funcione e a visualização gráfica seja gerada.

---

## Instalação do Graphviz (Windows)

Necessário para que `system("dot -Tpng grafo.dot -o grafo.png")` em `IACODES.c` funcione.

```powershell
winget install graphviz.graphviz
```

### Adicionar o Graphviz ao PATH

O instalador **não adiciona automaticamente** o `dot` ao PATH. Após instalar, execute no PowerShell:

```powershell
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\Program Files\Graphviz\bin", "User")
```

Feche e reabra o terminal, depois verifique:

```powershell
dot -V
```

Deve retornar algo como `dot - graphviz version 12.x.x`.

> **Alternativa gráfica:** Win+R → `sysdm.cpl` → Avançado → Variáveis de Ambiente → selecione `Path` em "Variáveis do usuário" → Editar → Novo → adicione `C:\Program Files\Graphviz\bin` → OK em tudo.

> **Atenção:** `IACODES.c` usa `xdg-open grafo.png` para abrir a imagem gerada, o que só funciona no Linux. No Windows troque essa linha por:
> ```c
> system("start grafo.png");
> ```

---

## Compilar tudo em um único .exe

O projeto tem vários arquivos `.c`; todos precisam ser passados juntos para o compilador. Com **GCC** (MinGW):

```powershell
gcc main.c IACODES.c GrafosGrafica.c GrafosLista.c GrafosMatriz.c -o grafos.exe
```

Para rodar depois:

```powershell
.\grafos.exe
```

Se quiser ativar avisos do compilador (recomendado durante o desenvolvimento):

```powershell
gcc main.c IACODES.c GrafosGrafica.c GrafosLista.c GrafosMatriz.c -o grafos.exe -Wall -Wextra
```

> Caso o GCC não esteja instalado, instale via: `winget install mingw.mingw`

---

## Notas técnicas relevantes

- O projeto compila em C padrão (sem C++).
- A DFS em `GrafosGrafica.c` usa um vetor de visitados (`visitados[]`) alocado com `malloc` e liberado após o uso.
- O campo `UNICID` em `main.c` é um contador global que garante IDs únicos para novos vértices.
- Vértices são removidos com deslocamento de array (shift), não com marcação lógica.
