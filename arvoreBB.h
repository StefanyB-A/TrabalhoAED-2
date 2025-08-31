#ifndef ARVOREBB_H
#define ARVOREBB_H

#include "structs.h"

NoABB* criarNoABB(PalavraInfo info);
NoABB* insereABB(NoABB* raiz, PalavraInfo info);
NoABB* buscaABB(NoABB* raiz, const char* palavra);
void percorrerInorderABB(NoABB* raiz);
void liberaABB(NoABB* raiz);

#endif