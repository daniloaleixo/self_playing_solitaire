/* ********************************
    EP1 - Estrutura de Dados
    Prof. Andre Fujita

    Danilo Aleixo Gomes de Souza
    n USP: 7972370

    Julio Cesar Seki
    n USP: 7991145

***********************************/

#include "Pilha.h"
#include "StringOps.h"
#define ANSI_COLOR_REVERSE "\x1b[7m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define NAIPE_ESPADA "\xe2\x99\xa0"
#define NAIPE_PAUS "\xe2\x99\xa3"
#define NAIPE_COPAS "\xe2\x99\xa5"
#define NAIPE_OUROS "\xe2\x99\xa6"

void montaBaralho(Carta *baralho, FILE *entrada);
void imprimeTela(Monte monteCompras, Monte colunas[], Monte pilhasBase[]);
int trocaPilha(Monte *monte1, Monte *monte2, Monte monteCompras, Monte pilhasBase[], Monte colunas[]);
int verificaRei(Monte *colunas, Monte monteCompras, Monte pilhasBase[]);
void virarCartasParaCima(Monte colunas[], Monte monteCompras, Monte pilhasBase[]);
int buscaCarta(Monte colunas[], Monte pilhasBase[], Monte monteCompras);
int verificaUmaCartaDoMonte(Monte *monteCompras, Monte colunas[], Monte pilhasBase[]);
void insereMonteComprasFinal(Carta *carta, Monte *monteCompras);
int verificaFim();
void verificaMonteCompras(Monte *monteCompras);
int maiorValorPilhaBase[4];
int *pontuacao;
int *numCartasMonteCompras;
Carta *primeiraCarta;
int *flag;

int main(int argc, char *argv[])
    {
    FILE *entrada;
    char *nomeEntrada = argv[1];
    Carta baralho[52];
    int i, j, k;
    int n = 0;
    Monte monteCompras, colunas[7], pilhasBase[4];
    pontuacao = 0;
    flag = 0;
    numCartasMonteCompras = (int *)96;

    /* Definimos a pilhaBase como
    pilhaBase[0] -> Paus
    pilhaBase[1] -> Copas
    pilhaBase[2] -> Espada
    pilhaBase[3] -> Ouro */

    monteCompras.base = NULL;
    monteCompras.topo = NULL;
    for(i = 0; i < 7; i++)
        {
        colunas[i].base = NULL;
        colunas[i].topo = NULL;
        }
    for(i = 0; i < 4; i++)
        {
        pilhasBase[i].base = NULL;
        pilhasBase[i].topo = NULL;
        }

    entrada = leEntrada(nomeEntrada);

    montaBaralho(baralho, entrada);

    for(j = 0; j < 7; j++)
        {
        for(i = 0; i <= j; i++)
            {
            if(i == j)
                {
                baralho[n].visivel = 1;
                colunas[j].base = &baralho[n];
                }
            else
                {
                baralho[n].visivel = 0;
                }
            empilha(&baralho[n++], &colunas[j]);
            }
        }

    while(n < 52)
        {
        baralho[n].visivel = 1;
        empilha(&baralho[n++], &monteCompras);
        }

    printf(ANSI_COLOR_YELLOW  "\n\n================================================================================================================================================" ANSI_COLOR_RESET "\n");
    printf("INICIO DO JOGO: \n");
    printf("PONTUACAO ATUAL: %d\n", (int)pontuacao/4);
    printf(ANSI_COLOR_YELLOW  "================================================================================================================================================" ANSI_COLOR_RESET "\n\n");


    primeiraCarta = monteCompras.topo;
    imprimeTela(monteCompras, colunas, pilhasBase);
    printf("\nPRESSIONE ENTER PARA INICIAR A SOLUCAO\n\n\n\n");
    getchar();

    for(k=0; k<4; k++) /*inicializando o vetor*/
        {
        maiorValorPilhaBase[k] = 0;
        }

    /* Estrategias de resoluçao */
    while(verificaFim() != 1)
        {
        int houveInteracao = 1;
        while(houveInteracao == 1)
            {
            houveInteracao = buscaCarta(colunas, pilhasBase, monteCompras);
            virarCartasParaCima(colunas, monteCompras, pilhasBase);
            getchar();

            houveInteracao = verificaRei(colunas, monteCompras, pilhasBase);
            if(houveInteracao == 1)
                {
                virarCartasParaCima(colunas, monteCompras, pilhasBase);
                getchar();
                }

            houveInteracao = buscaCarta(colunas, pilhasBase, monteCompras);
            if(verificaFim() == 1)
                break;
                if(pontuacao < (int*)-400)
                break;
            }

        /*verificaMonteCompras(&monteCompras);*/
        if(monteCompras.topo != NULL)
            {
            houveInteracao = verificaUmaCartaDoMonte(&monteCompras, colunas, pilhasBase);
            }
        getchar();
        }
    printf("TODAS AS PILHAS BASES PREENCHIDAS\n");
    printf("FIM DE JOGO!!! OBRIGADO POR JOGAR\n");
    printf(ANSI_COLOR_YELLOW  "================================================================================================================================================" ANSI_COLOR_RESET "\n");
    printf("PONTUACAO FINAL: %d\n", (int)pontuacao/4);
    printf(ANSI_COLOR_YELLOW  "================================================================================================================================================" ANSI_COLOR_RESET "\n\n\n");
    return 0;
    }

int verificaFim()
    {
    if(maiorValorPilhaBase[0] == 13 && maiorValorPilhaBase[1] == 13)
        {
        if(maiorValorPilhaBase[2] == 13 && maiorValorPilhaBase[3] ==13)
            {
            return 1;
            }
        else
            return 0;
        }
    else
        return 0;
    }
void verificaMonteCompras(Monte *monteCompras)
    {
    Carta *aux;
    int cont = 0;
    aux = monteCompras->topo;
    printf("Primeira carta do monte de compras: %d %c\n", aux->valor, aux->naipe);
    for(aux = monteCompras->topo; aux->prox != NULL; aux = aux->prox)
        {
        cont++;
        }
    printf("Numero de cartas no monte de compras: %d\n", cont);
    }

int verificaUmaCartaDoMonte(Monte *monteCompras, Monte colunas[], Monte pilhasBase[])
    {
    int i;
    Carta *card = desempilha(monteCompras);
    printf("Carta retirada do monte de compras: %d %c\n", card->valor, card->naipe);
    if(card->valor == 13)
        {
        for(i = 0; i < 7; i++)
            {
            if(colunas[i].topo == NULL)
                {
                empilha(card, &colunas[i]);
                printf("Carta %d %c foi movida para a coluna %d\n", card->valor, card->naipe, i);
                imprimeTela(*monteCompras, colunas, pilhasBase);
                pontuacao = pontuacao + (int)5;
                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                return 1;
                }
            }
        }

    switch(card->naipe)
        {
        case 'P':
            if(pilhasBase[0].topo != NULL)
                {
                if(pilhasBase[0].topo->valor == card->valor - 1)
                    {
                    printf("Carta: %d %c foi movida para a pilha base %s\n", card->valor, card->naipe, NAIPE_PAUS);
                    empilha(card, &pilhasBase[0]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            else
                {
                if(card->valor == 1)/*caso a carta seja um AS ele entra direto no monte base*/
                    {
                    printf("Carta: %d %c foi movida para a pilha base %s\n", card->valor, card->naipe, NAIPE_PAUS);
                    empilha(card, &pilhasBase[0]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            break;
        case 'C':
            if(pilhasBase[1].topo != NULL)
                {
                if(pilhasBase[1].topo->valor == card->valor - 1)
                    {
                    printf("Carta: %d %c foi movida para a pilha base " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", card->valor, card->naipe, NAIPE_COPAS);
                    empilha(card, &pilhasBase[1]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            else
                {
                if(card->valor == 1)/*caso a carta seja um AS ele entra direto no monte base*/
                    {
                    printf("Carta: %d %c foi movida para a pilha base " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", card->valor, card->naipe, NAIPE_COPAS);
                    empilha(card, &pilhasBase[1]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            break;
        case 'E':
            if(pilhasBase[2].topo != NULL)
                {
                if(pilhasBase[2].topo->valor == card->valor - 1)
                    {
                    printf("Carta: %d %c foi movida para a pilha base %s\n", card->valor, card->naipe, NAIPE_ESPADA);
                    empilha(card, &pilhasBase[2]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            else
                {
                if(card->valor == 1)/*caso a carta seja um AS ele entra direto no monte base*/
                    {
                    printf("Carta: %d %c foi movida para a pilha base %s\n", card->valor, card->naipe, NAIPE_ESPADA);
                    empilha(card, &pilhasBase[2]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            break;
        case 'O':
            if(pilhasBase[3].topo != NULL)
                {
                if(pilhasBase[3].topo->valor == card->valor - 1)
                    {
                    printf("Carta: %d %c foi movida para a pilha base " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", card->valor, card->naipe, NAIPE_OUROS);
                    empilha(card, &pilhasBase[3]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            else
                {
                if(card->valor == 1)/*caso a carta seja um AS ele entra direto no monte base*/
                    {
                    printf("Carta: %d %c foi movida para a pilha base " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", card->valor, card->naipe, NAIPE_OUROS);
                    empilha(card, &pilhasBase[3]);
                    imprimeTela(*monteCompras, colunas, pilhasBase);
                    pontuacao = pontuacao + (int)10;
                    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                    getchar();
                    return 1;
                    }
                }
            break;
        }

    for(i = 0; i < 7; i++)
        {
        if(colunas[i].topo != NULL)
            {
            if(card->cor != colunas[i].topo->cor && card->valor == colunas[i].topo->valor - 1)
                {
                printf("Carta %d %c foi movida para o monte%d\n", card->valor, card->naipe, i);
                empilha(card, &colunas[i]);
                imprimeTela(*monteCompras, colunas, pilhasBase);
                if(monteCompras->topo != NULL)
                    {
                    if(card == primeiraCarta)
                        {
                        primeiraCarta = monteCompras->topo;
                        printf("Primeira carta do monte de compras: %d %c\n", primeiraCarta->valor, primeiraCarta->naipe);
                        if(flag != (int*)0 && monteCompras ->topo != NULL)
                            {
                            pontuacao = pontuacao - (int)100;
                            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
                            }
                        }
                    }
                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                return 1;
                }
            }
        }
    insereMonteComprasFinal(card, monteCompras);
    printf("Carta %d %c retornada ao monte de compras\n", card->valor, card->cor);
    imprimeTela(*monteCompras, colunas, pilhasBase);
    printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
    if(primeiraCarta != NULL)
        {
        if(card == primeiraCarta && flag != (int*)0)
            {
            pontuacao = pontuacao - (int)100;
            printf("Monte de compras voltou ao inicio, pontuacao atual: %d\n", (int)pontuacao/4);
            }
        }
        flag = flag + (int)1;
    return 0;
    }

/* funcao verifica se tem algum rei nas colunas que tem cartas viradas em baixo dele
 se tiver e alguma coluna estiver vazia, movemos o rei para a coluna vazia, possibilitando as
 cartas de baixo sejam viradas */
int verificaRei(Monte *colunas, Monte monteCompras, Monte pilhasBase[])
    {
    int i, j;
    Carta *topo;
    for(i = 0; i < 7; i++)
        {
        if(colunas[i].base != NULL)
            {
            Carta *aux;
            if(colunas[i].base->valor == 13 && colunas[i].base->prox != NULL) /*base da coluna i eh um rei mas não eh a ultima carta*/
                {
                for(j = 0; j < 7; j++)
                    {
                    if(colunas[j].topo == NULL)
                        {
                        if(colunas[i].base->prox != NULL)
                            {
                            aux = colunas[i].base->prox;
                            topo = colunas[i].topo;
                            colunas[j].base = colunas[i].base;
                            colunas[j].topo = topo;
                            colunas[i].topo = colunas[i].base->prox; /*erro*/
                            colunas[j].base->prox = NULL;
                            colunas[i].base = aux;
                            printf("O rei da colunas %d foi movido para a coluna %d\n", i, j);
                            imprimeTela(monteCompras, colunas, pilhasBase);
                            pontuacao = pontuacao + (int)5;
                            printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                            getchar();
                            return 1;
                            }
                        }
                    }
                }
            }
        }
    return 0;
    }

/* funçao insere a carta no final do monte de compras */
void insereMonteComprasFinal(Carta *carta, Monte *monteCompras)
    {
    Carta *aux;
    if(monteCompras->topo != NULL)
        {
        for(aux = monteCompras->topo; aux->prox != NULL; aux = aux->prox)
            {}
        aux->prox = carta;
        }
    else empilha(carta, monteCompras);

    }

/* verifica as cartas que estao viradas para baixo e ve se ela
pode ser virada para cima */
void virarCartasParaCima(Monte colunas[], Monte monteCompras, Monte pilhasBase[])
    {
    int i;
    for(i = 0; i < 7; i++)
        {
        if(colunas[i].topo != NULL)
            {
            if(colunas[i].topo->visivel == 0)
                {
                colunas[i].topo->visivel = 1;
                colunas[i].base = colunas[i].topo;
                }
            }
        }
    imprimeTela(monteCompras, colunas, pilhasBase);
    printf("\nMontes atualizados\n");
    printf("Pontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
    }

int trocaPilha(Monte *monte1, Monte *monte2, Monte monteCompras, Monte pilhasBase[], Monte colunas[])
    {
    Carta *aux;
    if(monte1->base != NULL && monte2->base!= NULL)
        {
        if(monte1->base->visivel == 1 && monte2->base->visivel == 1)
            {
            if(monte2->base->cor != monte1->topo->cor && monte2->base->valor == monte1->topo->valor - 1 && monte2->base->prox == NULL)
                {
                printf("Carta: %d %c foi movida sobre a carta %d %c\n", monte2->base->valor, monte2->base->naipe, monte1->topo->valor, monte1->topo->naipe);
                monte2->base->prox = monte1->topo;
                monte1->topo = monte2->topo;
                monte2->topo = NULL;
                monte2->base = NULL;
                imprimeTela(monteCompras, colunas, pilhasBase);
                pontuacao = pontuacao + (int)5;
                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                getchar();
                return 1;
                }
            if(monte2->base->cor != monte1->topo->cor && monte2->base->valor == monte1->topo->valor - 1 && monte2->base->prox != NULL) /*existem cartas ocultas abaixo da base do monte2*/
                {
                printf("Carta: %d %c foi movida sobre a carta %d %c\n", monte2->base->valor, monte2->base->naipe, monte1->topo->valor, monte1->topo->naipe);
                aux = monte2->base->prox;
                monte2->base->prox = monte1->topo;
                monte1->topo = monte2->topo;
                monte2->base = aux;
                monte2->topo = aux;
                monte2->topo->visivel = 1;
                imprimeTela(monteCompras, colunas, pilhasBase);
                pontuacao = pontuacao + (int)5;
                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                getchar();
                return 1;
                }
            else
                return 0;
            }

        }
    return 0;
    }

int buscaCarta(Monte colunas[], Monte pilhasBase[], Monte monteCompras)
    {
    Carta *aux;
    int i, j, temp = 0, flag;

    /* Definimos as cartas que estão no topo das pilhas bases
     assim podemos saber quais cartas poderao subir para as pilhas
     */
    for(i = 0; i < 4; i++)
        {
        if(pilhasBase[i].topo != NULL)
            {
            maiorValorPilhaBase[i] = pilhasBase[i].topo->valor;
            }
        else
            {
            maiorValorPilhaBase[i] = 0;
            }
        }

    for(j=0; j<7; j++)
        {
        flag = 1;
        while(flag == 1)
            {
            if(colunas[j].topo != NULL)
                {
                if(colunas[j].topo->visivel == 1)
                    {
                    switch(colunas[j].topo->naipe)
                        {

                        case 'P':
                            if(colunas[j].topo->valor == maiorValorPilhaBase[0] + 1)
                                {
                                printf("Carta: %d %c foi movida para a pilha base %s\n", colunas[j].topo->valor, colunas[j].topo->naipe, NAIPE_PAUS);
                                aux = desempilha(&colunas[j]);
                                empilha(aux, &pilhasBase[0]);
                                maiorValorPilhaBase[0] = aux->valor;
                                imprimeTela(monteCompras, colunas, pilhasBase);
                                pontuacao = pontuacao + (int)10;
                                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                                getchar();
                                flag = 1;
                                }
                            else
                                {
                                flag = 0;
                                }
                            break;
                        case 'C':
                            if(colunas[j].topo->valor == maiorValorPilhaBase[1] + 1)
                                {
                                printf("Carta: %d %c foi movida para a pilha base " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", colunas[j].topo->valor, colunas[j].topo->naipe, NAIPE_COPAS);
                                aux = desempilha(&colunas[j]);
                                empilha(aux, &pilhasBase[1]);
                                maiorValorPilhaBase[1] = aux->valor;
                                imprimeTela(monteCompras, colunas, pilhasBase);
                                pontuacao = pontuacao + (int)10;
                                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                                getchar();
                                flag = 1;
                                }
                            else
                                {
                                flag = 0;
                                }
                            break;
                        case 'E':
                            if(colunas[j].topo->valor == maiorValorPilhaBase[2] + 1)
                                {
                                printf("Carta: %d %c foi movida para a pilha base %s\n", colunas[j].topo->valor, colunas[j].topo->naipe, NAIPE_ESPADA);
                                aux = desempilha(&colunas[j]);
                                empilha(aux, &pilhasBase[2]);
                                maiorValorPilhaBase[2] = aux->valor;
                                imprimeTela(monteCompras, colunas, pilhasBase);
                                pontuacao = pontuacao + (int)10;
                                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                                getchar();
                                flag = 1;
                                }
                            else
                                {
                                flag = 0;
                                }
                            break;
                        case 'O':
                            if(colunas[j].topo->valor ==  maiorValorPilhaBase[3] + 1)
                                {
                                printf("Carta: %d %c foi movida para a pilha base " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", colunas[j].topo->valor, colunas[j].topo->naipe, NAIPE_OUROS);
                                aux = desempilha(&colunas[j]);
                                empilha(aux, &pilhasBase[3]);
                                maiorValorPilhaBase[3] = aux->valor;
                                imprimeTela(monteCompras, colunas, pilhasBase);
                                pontuacao = pontuacao + (int)10;
                                printf("\n\nPontuacao atual: %d\n\n\n\n", (int)pontuacao/4);
                                getchar();
                                flag = 1;
                                }
                            else
                                {
                                flag = 0;
                                }
                            break;
                        }/* fim do switch */
                    }
                else flag = 0;
                }
            else flag = 0;

            }/* fim do while */

        for(i=0; i<7; i++)
            {
            if(i != j)
                {
                temp = trocaPilha(&colunas[i], &colunas[j], monteCompras, pilhasBase, colunas);
                }
            }
        }

    if(temp == 1 || flag == 1)
        {
        return 1;
        }
    else return 0;

    }

void montaBaralho(Carta *baralho, FILE *entrada)
    {
    char *linha;
    int n = 0;

    while((linha = readLine(entrada)) != NULL)
        {
        baralho[n].prox = NULL;
        switch(linha[0])
            {
            case 'A':
                baralho[n].valor = 1;
                break;
            case '2':
                baralho[n].valor = 2;
                break;
            case '3':
                baralho[n].valor = 3;
                break;
            case '4':
                baralho[n].valor = 4;
                break;
            case '5':
                baralho[n].valor = 5;
                break;
            case '6':
                baralho[n].valor = 6;
                break;
            case '7':
                baralho[n].valor = 7;
                break;
            case '8':
                baralho[n].valor = 8;
                break;
            case '9':
                baralho[n].valor = 9;
                break;
            case '1':
                baralho[n].valor = 10;

                break;
            case 'J':
                baralho[n].valor = 11;
                break;
            case 'Q':
                baralho[n].valor = 12;
                break;
            case 'K':
                baralho[n].valor = 13;
                break;
            }/* fim switch */

        switch(linha[2])

            {
            case 'P':
                baralho[n].naipe = 'P';
                baralho[n].cor = 'P';
                break;
            case 'O':
                baralho[n].naipe = 'O';
                baralho[n].cor = 'V';
                break;
            case 'C':
                baralho[n].naipe = 'C';
                baralho[n].cor = 'V';
                break;
            case 'E':
                baralho[n].naipe = 'E';
                baralho[n].cor = 'P';
                break;
            case ' ':
                if(linha[3] != '\n')
                    {
                    switch(linha[3])
                        {
                        case 'P':
                            baralho[n].naipe = 'P';
                            baralho[n].cor = 'P';
                            break;
                        case 'O':
                            baralho[n].naipe = 'O';
                            baralho[n].cor = 'V';
                            break;
                        case 'C':
                            baralho[n].naipe = 'C';
                            baralho[n].cor = 'V';
                            break;
                        case 'E':
                            baralho[n].naipe = 'E';
                            baralho[n].cor = 'P';
                            break;

                        }
                    }
            } /* fim do switch */
        n++;
        }
    }


void imprimeTela(Monte monteCompras, Monte colunas[], Monte pilhasBase[])
    {
    int i;
    printf("___________________________\n");
    for(i = 0; i < 7; i++)
        {
        printf("COLUNA %d: ", i);
        imprimePilha(&colunas[i]);
        printf("\n");
        }

    for(i = 0; i < 4; i++)
        {
        switch(i)
            {
            case 0:
                printf("PILHAS BASE %s: ", NAIPE_PAUS);
                imprimePilha(&pilhasBase[i]);
                printf("\n");
                break;
            case 1:
                printf("PILHAS BASE " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET " : ", NAIPE_COPAS);
                imprimePilha(&pilhasBase[i]);
                printf("\n");
                break;
            case 2:
                printf("PILHAS BASE %s : ", NAIPE_ESPADA);
                imprimePilha(&pilhasBase[i]);
                printf("\n");
                break;
            case 3:
                printf("PILHAS BASE " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET " : ", NAIPE_OUROS);
                imprimePilha(&pilhasBase[i]);
                printf("\n");
                break;
            }
        }
    printf("MONTE COMPRAS: ");
    imprimePilha(&monteCompras);
    printf("\n");
    }

