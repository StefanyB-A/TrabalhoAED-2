/*

fun��es avl:
-inserir
-buscara
-rotacao esquerda
-rotacao direita
-balancear

obs:
* tem que fazer uma avl por palavras e por frequencias
* Use strtok() para dividir palavras
* tolower() para min�sculas
* strlen() para verificar tamanho > 3
* Com excessao da insercao e da remocao, as demais funcoes das arvores
AVL e Binaria sao iguais

typedef struct NoAVL {
    PalavraInfo dados;
    int altura;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
} NoAVL; //struct pra arvore AVL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "arvoreAVL.h"
#include "processamento.h"

int alturaAVL(NoAVL* no) {
    if (no == NULL) {
        return -1;
    } else {
        return no->altura;
    }
}

int balanceiaAVL(NoAVL* no) {
    if (no == NULL) return 0;
    return alturaAVL(no->esquerda) - alturaAVL(no->direita);
}

int RetornaMaior(int x, int y) {
    return (x > y) ? x : y;
}

NoAVL* RotacaoLL(NoAVL* raiz) {
    NoAVL* no = raiz->esquerda;
    raiz->esquerda = no->direita;
    no->direita = raiz;

    raiz->altura = RetornaMaior(alturaAVL(raiz->esquerda), alturaAVL(raiz->direita)) + 1;
    no->altura = RetornaMaior(alturaAVL(no->esquerda), raiz->altura) + 1;

    return no;
}

NoAVL* RotacaoRR(NoAVL* raiz) {
    NoAVL* no = raiz->direita;
    raiz->direita = no->esquerda;
    no->esquerda = raiz;

    raiz->altura = RetornaMaior(alturaAVL(raiz->esquerda), alturaAVL(raiz->direita)) + 1;
    no->altura = RetornaMaior(alturaAVL(no->direita), raiz->altura) + 1;

    return no;
}

NoAVL* RotacaoLR(NoAVL* raiz) {
    raiz->esquerda = RotacaoRR(raiz->esquerda);
    return RotacaoLL(raiz);
}

NoAVL* RotacaoRL(NoAVL* raiz) {
    raiz->direita = RotacaoLL(raiz->direita);
    return RotacaoRR(raiz);
}

NoAVL* insereAVL(NoAVL* raiz, PalavraInfo Info) {
    if (raiz == NULL) {
        NoAVL* novo = (NoAVL*)malloc(sizeof(NoAVL));
        if (novo == NULL) {
            printf("Falha na alocacao da arvore AVL\n");
            return NULL;
        }

        novo->dados = Info;
        novo->altura = 0;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    int cmp = strcmp(Info.palavra, raiz->dados.palavra);

    if (cmp < 0) {
        raiz->esquerda = insereAVL(raiz->esquerda, Info);
    } else if (cmp > 0) {
        raiz->direita = insereAVL(raiz->direita, Info);
    } else {
        // Palavra já existe, atualiza frequência
        raiz->dados.freq += Info.freq;
        if (Info.dados.freq_palavra > raiz->dados.dados.freq_palavra) {
            raiz->dados.dados = Info.dados;
        }
        return raiz; 
    }

    raiz->altura = RetornaMaior(alturaAVL(raiz->esquerda), alturaAVL(raiz->direita)) + 1;

    int b = balanceiaAVL(raiz);

    // Casos de rotação
    if (b > 1 && strcmp(Info.palavra, raiz->esquerda->dados.palavra) < 0) {
        return RotacaoLL(raiz);
    }
    
    if (b < -1 && strcmp(Info.palavra, raiz->direita->dados.palavra) > 0) {
        return RotacaoRR(raiz);
    }
    
    if (b > 1 && strcmp(Info.palavra, raiz->esquerda->dados.palavra) > 0) {
        return RotacaoLR(raiz);
    }

    if (b < -1 && strcmp(Info.palavra, raiz->direita->dados.palavra) < 0) {
        return RotacaoRL(raiz);
    }

    return raiz;
}

NoAVL* insereAVLFreq(NoAVL* raiz, PalavraInfo Info) {
    int sucesso;
    if (raiz == NULL) {
        NoAVL* novo = (NoAVL*)malloc(sizeof(NoAVL));
        if (novo == NULL) {
            printf("Falha na alocacao da arvore AVL\n");
            sucesso = 0;
            return NULL;
        }

        novo->dados = Info;
        novo->altura = 0;
        novo->esquerda = NULL;
        novo->direita = NULL;
        sucesso = 1;
        return novo;
    }

    int cmp;
    if (Info.freq != raiz->dados.freq) {
        cmp = (raiz->dados.freq - Info.freq);
    } else {
        cmp = strcmp(Info.palavra, raiz->dados.palavra);
    }

    if (cmp < 0) {
        raiz->esquerda = insereAVLFreq(raiz->esquerda, Info);
    } else if (cmp > 0) {
        raiz->direita = insereAVLFreq(raiz->direita, Info);
    } else {
        raiz->dados.freq += Info.freq;
        if (Info.dados.freq_palavra > raiz->dados.dados.freq_palavra) {
            raiz->dados.dados = Info.dados;
        }
        sucesso = 0;
        return raiz;
    }

    if (!sucesso) return raiz;

    raiz->altura = RetornaMaior(alturaAVL(raiz->esquerda), alturaAVL(raiz->direita)) + 1;

    int b = balanceiaAVL(raiz);

    if (b > 1) {
        int cmp_esq;
        if (Info.freq != raiz->esquerda->dados.freq) {
            cmp_esq = (raiz->esquerda->dados.freq - Info.freq);
        } else {
            cmp_esq = strcmp(Info.palavra, raiz->esquerda->dados.palavra);
        }
        if (cmp_esq < 0) {
            return RotacaoLL(raiz);
        }
    }

    if (b < -1) {
        int cmp_dir;
        if (Info.freq != raiz->direita->dados.freq) {
            cmp_dir = (raiz->direita->dados.freq - Info.freq);
        } else {
            cmp_dir = strcmp(Info.palavra, raiz->direita->dados.palavra);
        }
        if (cmp_dir > 0) {
            return RotacaoRR(raiz);
        }
    }

    if (b > 1) {
        int cmp_esq;
        if (Info.freq != raiz->esquerda->dados.freq) {
            cmp_esq = (raiz->esquerda->dados.freq - Info.freq);
        } else {
            cmp_esq = strcmp(Info.palavra, raiz->esquerda->dados.palavra);
        }
        if (cmp_esq > 0) {
            return RotacaoLR(raiz);
        }
    }

    if (b < -1) {
        int cmp_dir;
        if (Info.freq != raiz->direita->dados.freq) {
            cmp_dir = (raiz->direita->dados.freq - Info.freq);
        } else {
            cmp_dir = strcmp(Info.palavra, raiz->direita->dados.palavra);
        }
        if (cmp_dir < 0) {
            return RotacaoRL(raiz);
        }
    }

    return raiz;
}

NoAVL* buscaAVL(NoAVL* raiz, const char* palavra) {
    if (raiz == NULL) {
        return NULL;
    }
    
    int cmp = strcmp(palavra, raiz->dados.palavra);
    
    if (cmp == 0) {
        return raiz;
    } else if (cmp < 0) {
        return buscaAVL(raiz->esquerda, palavra);
    } else {
        return buscaAVL(raiz->direita, palavra);
    }
}

void liberaAVL(NoAVL* raiz) {
    if (raiz != NULL) {
        liberaAVL(raiz->esquerda);
        liberaAVL(raiz->direita);
        free(raiz);
    }
}