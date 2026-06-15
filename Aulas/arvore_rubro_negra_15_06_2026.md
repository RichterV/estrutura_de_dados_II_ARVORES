# Árvore Rubro-Negra (Red-Black Tree)

## Conceito

A Árvore Rubro-Negra é uma árvore binária de busca balanceada que utiliza uma informação extra em cada nó: sua **cor** (vermelho ou preto).

Esse balanceamento garante operações eficientes de:

- Inserção
- Remoção
- Busca

### Complexidade

| Operação | Complexidade |
|-----------|-------------|
| Busca | O(log n) |
| Inserção | O(log n) |
| Remoção | O(log n) |

A altura máxima da árvore é limitada por:

h ≤ 2·log₂(n + 1)

onde:

- n = quantidade de nós
- h = altura da árvore

---

## Representação das Cores

| Valor | Cor |
|---------|---------|
| 0 | Preto |
| 1 | Vermelho |

---

# Propriedades da Árvore Rubro-Negra

Toda árvore rubro-negra deve obedecer às seguintes regras.

## 1. A raiz é sempre preta

```text
Raiz = Preto
```

---

## 2. Toda folha é preta

As folhas são os ponteiros NULL.

```text
NULL = Preto
```

Exemplo:

```text
    10(P)
   /    \
 NULL  NULL
```

---

## 3. Um nó vermelho nunca possui filho vermelho

Não podem existir dois nós vermelhos consecutivos.

Correto:

```text
      10(P)
      /
    5(V)
    /
  3(P)
```

Incorreto:

```text
      10(P)
      /
    5(V)
    /
  3(V)
```

---

## 4. Todo caminho da raiz até uma folha possui a mesma altura preta

A quantidade de nós pretos em qualquer caminho da raiz até um NULL deve ser igual.

Essa quantidade é chamada de:

### Altura Preta (Black Height)

```text
hPreta
```

Conta apenas os nós pretos.

Exemplo:

```text
        10(P)
       /     \
    5(V)    20(V)
    /          \
 2(P)         25(P)
```

Caminho:

```text
10 -> 5 -> 2 -> NULL
```

Nós pretos:

```text
10, 2, NULL
```

Logo:

```text
hPreta = 3
```

Todos os demais caminhos devem possuir o mesmo valor.

---

# Inserção

## Regra Inicial

Todo novo nó é inserido como:

```text
Vermelho
```

Isso evita aumentar imediatamente a altura preta da árvore.

Após inserir, pode ser necessário corrigir as propriedades da árvore.

---

# Casos de Correção

Considere:

```text
A = Avô
P = Pai
N = Novo Nó
T = Tio
```

---

## Caso 1 — Pai Preto

Se o pai do novo nó for preto:

```text
P = Preto
```

Nenhuma propriedade foi violada.

```text
Fim da inserção
```

---

## Caso 2 — Pai Vermelho e Tio Vermelho

Situação:

```text
       A(P)
      /    \
   P(V)   T(V)
   /
 N(V)
```

### Correção

1. Pai vira preto.
2. Tio vira preto.
3. Avô vira vermelho.

Resultado:

```text
       A(V)
      /    \
   P(P)   T(P)
   /
 N(V)
```

Depois disso:

```text
Aplicar correção novamente em A
```

---

## Caso 3 — Pai Vermelho e Tio Preto

Situação:

```text
T = Preto (ou NULL)
```

### Passos

1. Alinhar A → P → N.
2. Trocar cores entre pai e avô.
3. Rotacionar o avô.

---

### Caso LL (Left-Left)

Antes:

```text
       A(P)
      /
   P(V)
   /
 N(V)
```

Troca de cores:

```text
P = Preto
A = Vermelho
```

Rotação à direita em A:

```text
      P(P)
     /   \
  N(V)  A(V)
```

---

### Caso RR (Right-Right)

Antes:

```text
A(P)
   \
   P(V)
      \
      N(V)
```

Troca de cores:

```text
P = Preto
A = Vermelho
```

Rotação à esquerda em A:

```text
      P(P)
     /   \
   A(V) N(V)
```

---

### Caso LR (Left-Right)

Antes:

```text
      A
     /
    P
      \
       N
```

Passos:

1. Rotação à esquerda em P.
2. Transformar em caso LL.
3. Rotação à direita em A.

---

### Caso RL (Right-Left)

Antes:

```text
      A
        \
         P
        /
       N
```

Passos:

1. Rotação à direita em P.
2. Transformar em caso RR.
3. Rotação à esquerda em A.

---

# Rotações

## Rotação à Direita

Antes:

```text
      A
     /
    B
   /
  C
```

Depois:

```text
      B
     / \
    C   A
```

---

## Rotação à Esquerda

Antes:

```text
      A
        \
         B
           \
            C
```

Depois:

```text
      B
     / \
    A   C
```

---

# Garantia Final

Após qualquer inserção:

```text
Raiz = Preto
```

Se a raiz ficou vermelha durante as correções:

```c
raiz->cor = PRETO;
```

---

# Pseudocódigo da Inserção

```text
InserirRN(N)

Inserir N como Vermelho

Enquanto pai(N) for Vermelho:

    Se pai e tio forem Vermelhos:
        pai <- Preto
        tio <- Preto
        avô <- Vermelho
        N <- avô

    Senão:
        alinhar A -> P -> N

        pai <- Preto
        avô <- Vermelho

        rotacionar(avô)

Raiz <- Preto
```

---

# Resumo dos Casos

| Situação | Ação |
|-----------|--------|
| Pai preto | Nada |
| Pai vermelho e tio vermelho | Recolorir e subir para o avô |
| Pai vermelho e tio preto | Rotacionar e recolorir |
| Raiz vermelha ao final | Pintar de preto |

---

# Vantagens

- Mantém a árvore balanceada.
- Busca eficiente.
- Inserção eficiente.
- Remoção eficiente.
- Menos rotações que uma AVL em muitas situações.

## Aplicações

- STL (`map`, `set`) em C++
- Estruturas internas de sistemas operacionais
- Bancos de dados
- Índices de memória
- Sistemas de arquivos