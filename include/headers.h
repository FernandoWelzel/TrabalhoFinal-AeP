// --->> Declaração de estruturas <<--- //

typedef struct ponto {
    int x;
    int y;
} PONTO, * pPONTO;

typedef struct tiro {
    PONTO posicao;
    char mostrar;
    char direcao;
} TIRO, * pTIRO;

typedef struct inimigo {
    char morto;
    char tipo;
    PONTO posicao;
    char bola;
    TIRO tiro;
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

// --->> Protótipos de Função <<--- //

void copiar_gravacao (pGRAVACAO gravacao1, pGRAVACAO gravacao2);
void carregar_gravacao(pGRAVACAO gravacao_atual, pFASE fase_atual, GRAVACAO gravacao);
void escreve_gravacao(char * nome_arquivo, pGRAVACAO gravacao);
void atualiza_pos_lolo(pLOLO lolo, pFASE fase);

int nome_unico(char * nome_arquivo, char * nome);
int numero_gravacoes(char * nome_arquivo);
int pos_por_nomejogador(char * nome_arquivo, char * nomejogador);
int apagar_gravacao(char * nome_arquivo, char *ident);
int substitui_gravacao(char * nome_arquivo, pGRAVACAO nova_gravacao);
int bloco_eh_imovel(pFASE fase, char orientacao, int y, int x);
int eh_inimigo_imovel (pFASE fase, char orientacao, int y, int x, char trigger);
int testar_inimigos_moveis(pFASE fase, char orientacao, int y, int x);
int testar_pontos_imoveis(pFASE fase, char orientacao, PONTO ponto1, PONTO ponto2, char trigger);
int testar_inimigos_moveis_pontos(pFASE fase, char orientacao, PONTO ponto1, PONTO ponto2);
int teste_tiro_bateu(pFASE fase, char direcao, int y, int x);

char atualiza_pos_chiclete(pTIRO Ptiro, pLOLO Plolo);
char * string_to_lower (char * string, char * nova_string);
char atualiza_pos_tiro(pTIRO Ptiro, pFASE fase);

GRAVACAO le_gravacao_por_pos(char * nome_arquivo, int pos);
FASE le_fase_por_pos(char * nome_arquivo, int pos);
PONTO pos_tiro_bateu(pTIRO Ptiro, pFASE fase);