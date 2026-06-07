#ifndef IACODES_H //se ainda não foi definido IACODES_H
#define IACODES_H // defina IACODES_H (impede conflito de tentar carregar duas libs ao mesmo tempo)

#include "GrafosGrafica.h"

int IA_lerInteiro(const char *);
float IA_lerDecimal(const char *);
void IA_lerTexto(char*, int, const char *);

void IA_mostrarGrafoGrafica(); // função que gera o grafo em versão visual

#endif //IACODES_H - FIM DA DEFINIÇÃO DE IACODES_H