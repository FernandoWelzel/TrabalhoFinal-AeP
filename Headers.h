typedef struct status {
    char parte[5];
} STATUS;

typedef struct ponto {
    int x;
    int y;
} PONTO;

typedef struct fase {
    int num;
    char nome;
    char elementos[11][11];
    int pos_porta;
    PONTO pos_i_jog;
    char texto_inic[200];
} FASE;

typedef struct lolo {
    char vidas;
    PONTO ponto;
    char poder[3];
} LOLO;

typedef struct inimigo {
    char vidas;
    PONTO ponto;
} INIMIGO;

typedef struct gravacao {
    char ident[3];
    char totalpts[3];
    char num_ult_fase[3];
    char vidas[3];
    char nomejogador[9];
} GRAVACAO, * pGRAVACAO;

bool IsAnyKeyPressed();