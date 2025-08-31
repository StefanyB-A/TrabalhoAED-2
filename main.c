#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "structs.h"
#include "buscaB.h"
#include "arvoreBB.h"
#include "arvoreAVL.h"
#include "processamento.h"

/*
Amanda Rossi Rabelo - Matrícula: 12411BCC034
Eduardo Martins Barbosa - Matrícula: 12411BCC022
Stéfany Batista Amparo - Matrícula: 12411BCC009
*/

void imprimirPalavraInfo(PalavraInfo info) {
    printf("\n=== Informações da Palavra ===\n");
    printf("Palavra: %s\n", info.palavra);
    printf("Frequência no repositório: %d\n", info.freq);
    printf("\n--- Música com maior frequência ---\n");
    printf("Nome: %s\n", info.dados.nome);
    printf("Compositor: %s\n", info.dados.compositor);
    printf("Trecho: %s\n", info.dados.trecho);
    printf("Frequência na música: %d\n", info.dados.freq_palavra);
    printf("==================================\n");
}


int main() {
    int op;
    VetorBB* vetor = (VetorBB*)malloc(sizeof(VetorBB));
    inicializarVetor(vetor, 100);
    NoABB* raiz_bst = NULL;
    NoAVL* raiz_avl = NULL;
    NoAVL* raiz_freq = NULL;
    char palavra_busca[100], nome_arquivo[100];
    int arquivo_carregado = 0;
    clock_t inicio, fim;


    do{
        printf("~~ BEM VINDO AO GERENCIADOR DE LETRAS DE MUSICAS ~~\n");
        printf("Menu:\n");
        printf("1. Carregar arquivo\n");
        printf("2. Buscar palavra\n");
        printf("3. Buscar palavra por frequência\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &op);

        switch(op) {
            case 1:
                printf("Digite o nome do arquivo: ");
                scanf("%s", nome_arquivo);
                
                printf("\n~~ CARREGANDO ARQUIVO ~~\n");
                if (lerArquivo(nome_arquivo, vetor, &raiz_bst, &raiz_avl, &raiz_freq)) {
                    printf("Arquivo carregado com sucesso!\n");
                    arquivo_carregado = 1;
                } else {
                    printf("Erro ao carregar o arquivo!\n");
                }
            break;
            case 2:
                if (!arquivo_carregado) {
                    printf("Erro: Nenhum arquivo foi carregado ainda!\n");
                    printf("Por favor, carregue um arquivo primeiro (opção 1).\n");
                    break;
                }
                
                printf("Digite a palavra que deseja buscar: ");
                scanf("%s", palavra_busca);
                
                for (int i = 0; palavra_busca[i]; i++) {
                    palavra_busca[i] = tolower(palavra_busca[i]);
                }
                
                inicio = clock();
                int resultado_vetor = buscaBinaria(vetor, palavra_busca, 0, vetor->quantElem - 1);
                fim = clock();
                double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                printf("Tempo de busca no vetor: %.6f segundos\n", tempo);

                inicio = clock();
                NoABB* resultado_bst = buscaABB(raiz_bst, palavra_busca);
                fim = clock();
                tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                printf("Tempo de busca na árvore BST: %.6f segundos\n", tempo);

                inicio = clock();
                NoAVL* resultado_avl = buscaAVL(raiz_avl, palavra_busca);
                fim = clock();
                tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                printf("Tempo de busca na árvore AVL: %.6f segundos\n", tempo);

                //imprimir
                imprimirPalavraInfo(resultado_avl->dados);
            break;
            case 3:
                if (!arquivo_carregado) {
                    printf("Erro: Nenhum arquivo foi carregado ainda!\n");
                    printf("Por favor, carregue um arquivo primeiro (opção 1).\n");
                    break;
                }

                printf("Digite a palavra que deseja buscar: ");
                scanf("%s", palavra_busca);

                for (int i = 0; palavra_busca[i]; i++) {
                    palavra_busca[i] = tolower(palavra_busca[i]);
                }
                
                inicio = clock();
                NoAVL* resultado_freq = buscaAVL(raiz_freq, palavra_busca);
                fim = clock();
                tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                printf("Tempo de busca na árvore AVL de frequência: %.6f segundos\n", tempo);

                //imprimir
                imprimirPalavraInfo(resultado_freq->dados);
            break;
            case 0:
                printf("Encerrando programa...\n");
            break;
            default:
                printf("Opção inválida!\n");
        }
    } while(op != 0);

    liberarVetor(vetor);
    liberaABB(raiz_bst);
    liberaAVL(raiz_avl);
    liberaAVL(raiz_freq);
    return 0;
}