// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#define MAX_INPUT_CHARS 8

// Nossas bibliotecas
#include "Headers.h"

// Bibliotecas adicionais
#include "raylib.h"

// Declaração de constantes
#define MAX_INPUT_CHARS 8
#define ARQ_GRAVACAO "gravacao.bin"

void escreve_gravacao(char * nome_arquivo, pGRAVACAO gravacao) {
    FILE * arquivo;
    if (!(arquivo = fopen(nome_arquivo, "wb"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        fwrite(gravacao, sizeof(GRAVACAO), 1, arquivo);
        fclose(arquivo);
    }
}

void imprime_gravacao(char * nome_arquivo) {
    FILE * arquivo;
    char intermediario;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de gravações");
    }
    else {
        while(!feof(arquivo)) {
            fread(&intermediario, sizeof(char), 1, arquivo);
            printf("%c/n", intermediario);
        }
        fclose(arquivo);
    }
}

int main(void)
{    
    imprime_gravacao(ARQ_GRAVACAO);
    return 0;
}