#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "buscaB.h"
#include "processamento.h"

int comparaPalavras(const void *a, const void *b) {
    PalavraInfo *palavraA = *(PalavraInfo **)a;
    PalavraInfo *palavraB = *(PalavraInfo **)b;
    return strcmp(palavraA->palavra, palavraB->palavra);
}

void inicializarVetor(VetorBB* vetor, int capacidade) {
    vetor->dados = (PalavraInfo**)malloc(sizeof(PalavraInfo*) * capacidade);
    if (vetor->dados == NULL) {
        printf("Erro ao alocar memória para o vetor.\n");
        return;
    }
    vetor->quantElem = 0;
    vetor->quantAloc = capacidade;
}
void redimensionarVetor(VetorBB* vetor) {
    vetor->quantAloc += 10;
    vetor->dados = (PalavraInfo**)realloc(vetor->dados, sizeof(PalavraInfo*) * vetor->quantAloc);
    if (vetor->dados == NULL) {
        printf("Erro ao realocar memória para os CEPs.\n");
        return;
    }
}

void inserirOrdenado (VetorBB* vetor, PalavraInfo info) {
    if (vetor->quantElem == vetor->quantAloc) {
        redimensionarVetor(vetor);
    }

    for (int i = 0; i < vetor->quantElem; i++) {
        if (strcmp(vetor->dados[i]->palavra, info.palavra) == 0) {
            if (info.dados.freq_palavra > vetor->dados[i]->dados.freq_palavra) {
                vetor->dados[i]->dados = info.dados;
            }
            vetor->dados[i]->freq += info.freq;
            return;
        }
    }

    vetor->dados[vetor->quantElem] = (PalavraInfo*)malloc(sizeof(PalavraInfo));
    if (vetor->dados[vetor->quantElem] == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    *(vetor->dados[vetor->quantElem]) = info;
    vetor->quantElem++;

    qsort(vetor->dados, vetor->quantElem, sizeof(PalavraInfo*), comparaPalavras);
}

int buscaBinaria (VetorBB* vetor, const char* chave_busca, int inicio, int fim) {
    if (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (strcmp(vetor->dados[meio]->palavra, chave_busca) == 0) {
            return meio;
        }

        if (strcmp(vetor->dados[meio]->palavra, chave_busca) > 0) {
            return buscaBinaria (vetor, chave_busca, inicio, meio - 1);
        }

        return buscaBinaria (vetor, chave_busca, meio + 1, fim);
    }

    return -1;
}

void liberarVetor(VetorBB* vetor) {
    for (int i = 0; i < vetor->quantElem; i++) {
        free(vetor->dados[i]);
    }
    free(vetor->dados);
    free(vetor);
}