#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "structs.h"
#include "processamento.h"
#include "arvoreAVL.h"
#include "arvoreBB.h"
#include "buscaB.h"

void limparPalavra(char* palavra_original, char* palavra_limpa) {
    int j = 0;
    int len = strlen(palavra_original);
    
    for (int i = 0; i < len; i++) {
        if (isalpha(palavra_original[i])) {
            palavra_limpa[j] = tolower(palavra_original[i]);
            j++;
        }
    }
    palavra_limpa[j] = '\0';
}

int palavraValida(char* palavra) {
    return strlen(palavra) > 3;
}

int extrairInfoMusica(FILE* arquivo, InfoMusica* info) {
    char linha[500];
    int info_extraida = 0;
    
    //Lê as primeiras linhas para extrair nome da música e compositor
    while (fgets(linha, sizeof(linha), arquivo) && info_extraida < 2) {
        linha[strcspn(linha, "\n")] = 0;
        
        if (strlen(linha) > 0) {
            if (info_extraida == 0) {
                strncpy(info->nome_musica, linha, 99);
                info->nome_musica[99] = '\0';
                info_extraida++;
            } else if (info_extraida == 1) {
                strncpy(info->compositor, linha, 99);
                info->compositor[99] = '\0';
                info_extraida++;
            }
        }
    }
    
    return info_extraida == 2;
}

char* lerLetraCompleta(FILE* arquivo, int* tamanho) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    rewind(arquivo);
    
    char* conteudo = malloc(tamanho_arquivo + 1);
    if (!conteudo) {
        return NULL;
    }
    
    size_t bytes_lidos = fread(conteudo, 1, tamanho_arquivo, arquivo);
    conteudo[bytes_lidos] = '\0';
    *tamanho = bytes_lidos;
    
    return conteudo;
}

void encontrarEstrofeComPalavra(char* letra_completa, char* palavra_procurada, char* estrofe_resultado) {
    char* copia_letra = malloc(strlen(letra_completa) + 1);
    strcpy(copia_letra, letra_completa);
    
    char* posicao_palavra = NULL;
    char* ptr = copia_letra;
    
    // Procurar a palavra na letra usando tokenização
    char* token = strtok(copia_letra, " \t\n\r.,!?;:()[]{}\"'-");
    while (token != NULL) {
        char palavra_limpa[100];
        limparPalavra(token, palavra_limpa);
        
        if (strcmp(palavra_limpa, palavra_procurada) == 0) {
            // Calcular posição aproximada na string original
            posicao_palavra = strstr(letra_completa, token);
            if (posicao_palavra == NULL) {
                // Se não encontrou exatamente, procura pela palavra limpa
                // Vamos procurar manualmente
                char* search_ptr = letra_completa;
                char temp_word[100];
                
                while (*search_ptr != '\0') {
                    // Pula caracteres não alfabéticos
                    while (*search_ptr != '\0' && !isalpha(*search_ptr)) {
                        search_ptr++;
                    }
                    
                    if (*search_ptr == '\0') break;
                    
                    // Extrai uma palavra
                    int word_len = 0;
                    char* word_start = search_ptr;
                    while (*search_ptr != '\0' && isalpha(*search_ptr) && word_len < 99) {
                        temp_word[word_len] = tolower(*search_ptr);
                        word_len++;
                        search_ptr++;
                    }
                    temp_word[word_len] = '\0';
                    
                    // Verifica se é a palavra procurada
                    if (strcmp(temp_word, palavra_procurada) == 0) {
                        posicao_palavra = word_start;
                        break;
                    }
                }
            }
            break;
        }
        token = strtok(NULL, " \t\n\r.,!?;:()[]{}\"'-");
    }
    
    if (posicao_palavra != NULL) {
        int posicao = posicao_palavra - letra_completa;
        
        int inicio_contexto = posicao;
        while (inicio_contexto > 0 && 
               letra_completa[inicio_contexto] != '\n' && 
               letra_completa[inicio_contexto] != '.' &&
               letra_completa[inicio_contexto] != '!' &&
               letra_completa[inicio_contexto] != '?') {
            inicio_contexto--;
        }
        

        if (inicio_contexto > 0) {
            inicio_contexto++;
            while (inicio_contexto < strlen(letra_completa) && 
                   (letra_completa[inicio_contexto] == ' ' || 
                    letra_completa[inicio_contexto] == '\n' ||
                    letra_completa[inicio_contexto] == '\t')) {
                inicio_contexto++;
            }
        }
        
        int chars_copiados = 0;
        for (int i = inicio_contexto; i < strlen(letra_completa) && chars_copiados < 100; i++) {
            char c = letra_completa[i];
            
            if (c == '\n' && i + 1 < strlen(letra_completa) && letra_completa[i + 1] == '\n') {
                break;
            }
            
            if (c == '\n') {
                c = ' ';
            }
            
            if (c == ' ' && chars_copiados > 0 && estrofe_resultado[chars_copiados - 1] == ' ') {
                continue;
            }
            
            estrofe_resultado[chars_copiados] = c;
            chars_copiados++;
        }
        
        estrofe_resultado[chars_copiados] = '\0';
        
        while (chars_copiados > 0 && estrofe_resultado[chars_copiados - 1] == ' ') {
            chars_copiados--;
            estrofe_resultado[chars_copiados] = '\0';
        }
        
    } else {
        int chars_copiados = 0;
        int i = 0;
        
        while (i < strlen(letra_completa) && 
               (letra_completa[i] == '\n' || letra_completa[i] == ' ' || letra_completa[i] == '\t')) {
            i++;
        }
        
        while (i < strlen(letra_completa) && chars_copiados < 100) {
            char c = letra_completa[i];
            
            if (c == '\n' && i + 1 < strlen(letra_completa) && letra_completa[i + 1] == '\n') {
                break;
            }
            
            if (c == '\n') c = ' ';
            
            if (c == ' ' && chars_copiados > 0 && estrofe_resultado[chars_copiados - 1] == ' ') {
                i++;
                continue;
            }
            
            estrofe_resultado[chars_copiados] = c;
            chars_copiados++;
            i++;
        }
        
        estrofe_resultado[chars_copiados] = '\0';
    }
    
    free(copia_letra);
}

int lerArquivo(char* nome_arquivo, VetorBB* vetor, NoABB** raiz_bst, NoAVL** raiz_avl, NoAVL** raiz_freq) {
    
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return 0;
    }
    
    InfoMusica info_musica;
    
    if (!extrairInfoMusica(arquivo, &info_musica)) {
        printf("Erro ao extrair informações da música do arquivo: %s\n", nome_arquivo);
        fclose(arquivo);
        return 0;
    }
    
    fseek(arquivo, 0, SEEK_SET);
    info_musica.letra_completa = lerLetraCompleta(arquivo, &info_musica.tamanho_letra);
    fclose(arquivo);
    
    if (!info_musica.letra_completa) {
        printf("Erro ao ler a letra da música: %s\n", nome_arquivo);
        return 0;
    }
    
    
    ContadorPalavra contadores[1000]; //Assumindo máximo de 1000 palavras únicas por música
    int total_palavras_unicas = 0;
    
    char* token;
    char* copia_letra = malloc(strlen(info_musica.letra_completa) + 1);
    strcpy(copia_letra, info_musica.letra_completa);
    
    token = strtok(copia_letra, " \t\n\r.,!?;:()[]{}\"'-");
    
    while (token != NULL) {
        char palavra_limpa[100];
        limparPalavra(token, palavra_limpa);

        if (palavraValida(palavra_limpa)) {
            //Procura se a palavra já foi contada nesta música
            int encontrada = 0;
            for (int i = 0; i < total_palavras_unicas; i++) {
                if (strcmp(contadores[i].palavra, palavra_limpa) == 0) {
                    contadores[i].frequencia++;
                    encontrada = 1;
                    break;
                }
            }
            
            //Se não foi encontrada, adiciona nova entrada
            if (!encontrada && total_palavras_unicas < 1000) {
                strcpy(contadores[total_palavras_unicas].palavra, palavra_limpa);
                contadores[total_palavras_unicas].frequencia = 1;
                if (!encontrada && total_palavras_unicas < 1000) {
                    strcpy(contadores[total_palavras_unicas].palavra, palavra_limpa);
                    contadores[total_palavras_unicas].frequencia = 1;
                    encontrarEstrofeComPalavra(info_musica.letra_completa, palavra_limpa, contadores[total_palavras_unicas].trecho);

                    total_palavras_unicas++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n\r.,!?;:()[]{}\"'-");
    }
    
    //Insere cada palavra única nas estruturas de dados
    for (int i = 0; i < total_palavras_unicas; i++) {
        PalavraInfo entrada;
        strcpy(entrada.palavra, contadores[i].palavra);
        strcpy(entrada.dados.nome, info_musica.nome_musica);
        strcpy(entrada.dados.compositor, info_musica.compositor);
        strcpy(entrada.dados.trecho, contadores[i].trecho);
        entrada.dados.freq_palavra = contadores[i].frequencia;
        entrada.freq = contadores[i].frequencia; //Será atualizado pelas funções de inserção
        
        //Chama as funções de inserção
        clock_t inicio = clock();
        inserirOrdenado(vetor, entrada);
        clock_t fim = clock();
        double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo de inserção no vetor: %.6f segundos\n", tempo);

        inicio = clock();
        *raiz_bst = insereABB(*raiz_bst, entrada);
        fim = clock();
        tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo de inserção na árvore BST: %.6f segundos\n", tempo);

        inicio = clock();
        *raiz_avl = insereAVL(*raiz_avl, entrada);
        fim = clock();
        tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo de inserção na árvore AVL: %.6f segundos\n", tempo);

        inicio = clock();
        *raiz_freq = insereAVLFreq(*raiz_freq, entrada);
        fim = clock();
        tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo de inserção na árvore AVL de frequência: %.6f segundos\n", tempo);
    }
    
    
    free(info_musica.letra_completa);
    free(copia_letra);
    
    printf("Arquivo processado com sucesso: %s\n", nome_arquivo);
    printf("Total de palavras únicas encontradas: %d\n", total_palavras_unicas);
    
    return 1;
}