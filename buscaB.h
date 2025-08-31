#ifndef BUSCAB_H
#define BUSCAB_H
#include "structs.h"

void inicializarVetor(VetorBB* vetor, int capacidade);
void redimensionarVetor(VetorBB* vetor);
void inserirOrdenado(VetorBB* vetor, PalavraInfo info);
int buscaBinaria(VetorBB* vetor, const char* chave_busca, int inicio, int fim);
void liberarVetor (VetorBB* vetor);
#endif