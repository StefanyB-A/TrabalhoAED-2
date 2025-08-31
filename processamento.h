#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include "structs.h"

typedef struct {
    char nome_musica[100];
    char compositor[100];
    char* letra_completa;
    int tamanho_letra;
} InfoMusica;

typedef struct {
    char palavra[100];
    int frequencia;
    char trecho[101];
} ContadorPalavra;

int lerArquivo(char* nome_arquivo, VetorBB* vetor, NoABB** raiz_abb, NoAVL** raiz_avl, NoAVL** raiz_freq);

// Funções auxiliares
void limparPalavra(char* palavra_original, char* palavra_limpa);
int palavraValida(char* palavra);
void encontrarEstrofeComPalavra(char* letra_completa, char* palavra_procurada, char* estrofe_resultado);
char* lerLetraCompleta(FILE* arquivo, int* tamanho);
int extrairInfoMusica(FILE* arquivo, InfoMusica* info);
#endif