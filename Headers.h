typedef struct status {
    char parte[5];
} STATUS;

typedef struct ponto {
    int x;
    int y;
} PONTO, * pPONTO;

typedef struct inimigo {
    char morto;
    char tipo;
    PONTO posicao;
    char bola;
} INIMIGO, * pINIMIGO;

typedef struct fase {
    char num[3];
    char nome[20];
    char elementos[11][12];
    char pos_porta[3];
    PONTO pos_i_jogador;
    char texto_inic[200];
    char porta_estado;
    int num_coracoes;
    char num_especiais[2];
    int num_inimigos;
    INIMIGO inimigos[20];
} FASE, * pFASE;

typedef struct lolo {
    char vidas;
    PONTO posicao;
    char poder[3];
    char direcao;
} LOLO, * pLOLO;

typedef struct gravacao {
    char ident[3];
    char totalpts[3];
    char num_ult_fase[3];
    char vidas[3];
    char nomejogador[9];
} GRAVACAO, * pGRAVACAO;

typedef struct tiro {
    PONTO posicao;
    char mostrar;
    char direcao;
} TIRO, * pTIRO;