// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas adicionais
#include "raylib.h"

// Nossas bibliotecas
#include "Headers.h"

// Declaração de constantes
#define ARQ_GRAVACAO "fase.bin"

void escreve_fase(char * nome_arquivo, pFASE fase) {
    FILE * arquivo;
    if (!(arquivo = fopen(nome_arquivo, "a+b"))) {
        printf("Erro ao abrir o arquivo de fases");
    }
    else {
        fwrite(fase, sizeof(FASE), 1, arquivo);
        fclose(arquivo);
    }
}

void imprime_fase(char * nome_arquivo) {
    FILE * arquivo;
    char intermediario;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de fases");
    }
    else {
        while(!feof(arquivo)) {
            fread(&intermediario, sizeof(char), 1, arquivo);
            printf("%c", intermediario);
        }
        fclose(arquivo);
    }
}

int main(void)
{    
    //char elementos[132] = "LLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\0";
    //char texto_inicial[200] = "Ao longo da primeira fase, mate os inimigos e passe pela porta";
    
    FASE nivel1 = {"01", "Fase inicial", "LLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\nLLLLLLLLLLL\0", "01", {3, 5}, "Ao longo da primeira fase, mate os inimigos e passe pela porta\0"};
    
    escreve_fase(ARQ_GRAVACAO, &nivel1);
    imprime_fase(ARQ_GRAVACAO);
    return 0;
}