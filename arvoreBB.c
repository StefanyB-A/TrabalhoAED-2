#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreBB.h"
#include "structs.h"
#include "processamento.h"

NoABB* criarNoABB(PalavraInfo info) {
    NoABB* novo = (NoABB*)malloc(sizeof(NoABB));
    if (novo == NULL) {
        printf("Falha na alocação de memória\n");
        return NULL;
    }
    
    novo->dados = info;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

NoABB* insereABB(NoABB* raiz, PalavraInfo info) {
    if (raiz == NULL) {
        return criarNoABB(info);
    }
    
    int cmp = strcmp(info.palavra, raiz->dados.palavra);
    
    if (cmp < 0) {
        raiz->esquerda = insereABB(raiz->esquerda, info);
    } else if (cmp > 0) {
        raiz->direita = insereABB(raiz->direita, info);
    } else {
        // Palavra já existe - atualiza dados
        if (info.dados.freq_palavra > raiz->dados.dados.freq_palavra) {
            raiz->dados.dados = info.dados;
        }
        raiz->dados.freq += info.freq;
    }
    
    return raiz; 
}


NoABB* buscaABB(NoABB* raiz, const char* palavra) {
    if (raiz == NULL) return NULL;
    
    int cmp = strcmp(palavra, raiz->dados.palavra);
    
    if (cmp == 0) {
        return raiz;
    } else if (cmp < 0) {
        return buscaABB(raiz->esquerda, palavra);
    } else {
        return buscaABB(raiz->direita, palavra);
    }
}

void percorrerInorderABB(NoABB* raiz) {
    if (raiz != NULL) {
        percorrerInorderABB(raiz->esquerda);
        
        printf("Palavra: %s, Frequência: %d, Música: %s\n",
               raiz->dados.palavra,
               raiz->dados.freq,
               raiz->dados.dados.nome);
        
        percorrerInorderABB(raiz->direita);
    }
}

void liberaABB(NoABB* raiz) {
    if (raiz != NULL) {
        liberaABB(raiz->esquerda);
        liberaABB(raiz->direita);
        free(raiz);
    }
}