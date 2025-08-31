/*

funções avl:
-inserir
-buscar
-rotacao esquerda
-rotacao direita
-balancear

obs:
* tem que fazer uma avl por palavras e por frequencias
* Use strtok() para dividir palavras
* tolower() para minúsculas
* strlen() para verificar tamanho > 3

*/
#ifndef ARVOREAVL_H
#define ARVOREAVL_H
#include "structs.h"

typedef struct NoAVL* AVL;

int alturaAVL(NoAVL* no);
int balanceiaAVL(NoAVL* no);
int RetornaMaior(int x, int y);
NoAVL* RotacaoLL(NoAVL* raiz);
NoAVL* RotacaoRR(NoAVL* raiz);
NoAVL* RotacaoLR(NoAVL* raiz);
NoAVL* RotacaoRL(NoAVL* raiz);
NoAVL* insereAVL(NoAVL* raiz, PalavraInfo Info);
NoAVL* insereAVLFreq(NoAVL* raiz, PalavraInfo Info);
NoAVL* buscaAVL(NoAVL* raiz, const char* palavra);
void liberaAVL(NoAVL* raiz);

#endif
