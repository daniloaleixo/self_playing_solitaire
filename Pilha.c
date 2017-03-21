/* ********************************
    EP1 - Estrutura de Dados
    Prof. Andre Fujita

    Danilo Aleixo Gomes de Souza
    n USP: 7972370

    Julio Cesar Seki
    n USP: 7991145

***********************************/

#include "Pilha.h"
#define ANSI_COLOR_REVERSE     "\x1b[7m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void empilha(Carta *carta, Monte *monte)
{
    if(monte->topo != NULL)
    {
        carta->prox = monte->topo;
    }
    else     /* quando nao temos nenhuma carta no monte */
    {
        monte->base = carta;
    }
    monte->topo = carta;

}

Carta *desempilha(Monte *monte)
{
    Carta *x;
    x = NULL;

    if(monte->topo != NULL) /*existe pelo menos uma carta na pilha*/
    {
        x = monte->topo;
        monte->topo = monte->topo->prox;
    }
    if(monte->topo == NULL) /*topo ERA a ultima carta da pilha*/
    {
        monte->base = NULL;
    }
    else /*top nao ERA a ultima carta da pilha*/
    {
        if(monte->topo->visivel == 1)
        {
            monte->base = monte->topo;
        }
        else
            monte->base = NULL;
    }
    x->prox = NULL;
    return x;
}

void imprimePilha(Monte *monte)
{
    Carta *x;
    x = monte->topo;
    while(x != NULL)
    {
        if(x->visivel)
        {
            if(x->valor <10  && x->cor == 'V')
                printf(ANSI_COLOR_RED    " %d %c  "    ANSI_COLOR_RESET, x->valor, x->naipe );
            if(x->valor < 10 && x->cor == 'P')
                printf(" %d %c  ", x->valor, x->naipe );
            if(x->valor >= 10 && x->cor == 'V') /*valor maior que 10*/
                printf(ANSI_COLOR_RED    "%d %c  "    ANSI_COLOR_RESET, x->valor, x->naipe );
            if(x->valor >= 10 && x->cor == 'P')
                printf("%d %c  ", x->valor, x->naipe );
        }
        else /*carta não visivel*/
        {
            if(x->valor <10  && x->cor == 'V')
                printf(ANSI_COLOR_RED  ANSI_COLOR_REVERSE  " %d %c "    ANSI_COLOR_RESET  " ", x->valor, x->naipe );
            if(x->valor < 10 && x->cor == 'P')
                printf(ANSI_COLOR_REVERSE  " %d %c " ANSI_COLOR_RESET " ", x->valor, x->naipe );
            if(x->valor >= 10 && x->cor == 'V') /*valor maior que 10*/
                printf(ANSI_COLOR_RED  ANSI_COLOR_REVERSE  "%d %c "    ANSI_COLOR_RESET " ", x->valor, x->naipe );
            if(x->valor >= 10 && x->cor == 'P')
                printf(ANSI_COLOR_REVERSE  "%d %c " ANSI_COLOR_RESET " ", x->valor, x->naipe );
        }
        x = x->prox;
    }
}
