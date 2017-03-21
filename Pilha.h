
#include <stdio.h>
#include <stdlib.h>

struct carta {
    char naipe;
    char cor;
    int valor;
    int visivel;
    struct carta *prox;
};

typedef struct carta Carta;


typedef struct monte {
    Carta *topo;
    Carta *base;
} Monte;


void empilha(Carta *carta, Monte *monte);
Carta *desempilha(Monte *monte);
void imprimePilha(Monte *monte);

