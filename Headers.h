

typedef struct gravacao {
    char identificador;
    char totalpts;
    char ultimafase;
    char vidas;
    char nomejogador[9];
} GRAVACAO;

typedef struct fase {
    char estrutura[11][11];
    int pos_porta;
} FASE;

typedef struct ponto {
    int x;
    int y;
} PONTO;

typedef struct status {
    char parte[5];
} STATUS;

typedef struct lolo {
    char vidas;
    PONTO ponto;
    char poder[3];
} LOLO;

typedef struct inimigo {
    char vidas;
    PONTO ponto;
} INIMIGO;