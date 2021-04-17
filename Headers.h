typedef struct status {
    char parte[5];
} STATUS;

typedef struct ponto {
    int x;
    int y;
} PONTO, * pPONTO;

typedef struct fase {
    char num[3];
    char nome[20];
    char elementos[11][12];
    char pos_porta[3];
    PONTO pos_i_jogador;
    char texto_inic[200];
    char porta_estado;
    int num_coracoes;
} FASE, * pFASE;

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

void escreve_gravacao(char * nome_arquivo, pGRAVACAO gravacao);
int nome_unico(char * nome_arquivo, char * nome);

int IsAnyKeyPressed();
char * string_to_lower (char * string, char * nova_string);