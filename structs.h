#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    char nome[100];
    char compositor[100];
    char trecho[101];
    int freq_palavra;
} Musica;

typedef struct {
    char palavra[100];
    Musica dados;
    int freq;
} PalavraInfo;

typedef struct {
    PalavraInfo **dados;
    int quantAloc;
    int quantElem;
} VetorBB;

typedef struct NoABB {
    PalavraInfo dados;
    struct NoABB* esquerda;
    struct NoABB* direita;
} NoABB;

typedef struct NoAVL {
    PalavraInfo dados;
    int altura;
    struct NoAVL* esquerda;
    struct NoAVL* direita;
} NoAVL;

#endif