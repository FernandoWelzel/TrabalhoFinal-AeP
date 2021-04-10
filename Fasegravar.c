// Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas adicionais
#include "raylib.h"

// Nossas bibliotecas
#include "Headers.h"

// Declaração de constantes
#define ARQ_FASE "fase.bin"

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

FASE le_fase_por_pos(char * nome_arquivo, int pos) {
    FILE * arquivo;
    FASE fase_lida;
    if (!(arquivo = fopen(nome_arquivo, "rb"))) {
        printf("Erro ao abrir o arquivo de fases");
    }
    else {
        fseek(arquivo, sizeof(FASE)*pos, SEEK_SET);
        fread(&fase_lida, sizeof(FASE), 1, arquivo);
    }
    return fase_lida;
}

int main(void)
{    
    int i;
    char elementos[11][12] = {"LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL",
                              "LLLLLLLLLLL"};
                              
    char texto_inicial[200] = "Ao longo da primeira fase, mate os inimigos e passe pela porta.";
    PONTO pos_jogador = {3, 5};
    
    FASE nivel1;
    strcpy(nivel1.num, "01");
    strcpy(nivel1.nome, "Fase inicial");
    
    for (i = 0; i < 11; i++) {
        strcpy(nivel1.elementos[i], elementos[i]);
    }
    
    strcpy(nivel1.pos_porta, "01");
    nivel1.pos_i_jogador = pos_jogador;
    strcpy(nivel1.texto_inic, texto_inicial);
    
    //escreve_fase(ARQ_FASE, &nivel1);
    
    FASE fase0 = le_fase_por_pos(ARQ_FASE, 2);
    printf("%s\n", fase0.texto_inic);
    
    
    
    return 0;
}