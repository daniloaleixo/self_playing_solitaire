/* ********************************
    EP1 - Estrutura de Dados
    Prof. Andre Fujita

    Danilo Aleixo Gomes de Souza
    n USP: 7972370

    Julio Cesar Seki
    n USP: 7991145

***********************************/

#include "StringOps.h"

FILE *leEntrada (char *nomeArquivo)
{
    FILE *entrada;
    /* depuraçao * printf("entrei leEntrada string: %s\n", nomeArquivo);*/
    entrada = fopen(nomeArquivo, "r");
    /* depuraçao * printf("entrada %p\n", entrada);*/
    if(entrada == NULL)
    {
        fprintf(stderr, "Nao consegui ler o arquivo!");
        exit(-1);
    }

    return entrada;
}


/*      Algoritmo baseado no codigo do professor Paulo Feofiloff */

char *readLine(FILE *entrada)
{
    char *line, *nLine;
    int n, ch, size;

    n = 0;
    size = 6;
    line = malloc(size * sizeof(char) + 1);
    while((ch = getc(entrada)) != '\n' && ch != EOF)
    {
        line[n++] = ch;

    }
    if((n == 0) && (ch == EOF))
    {
        free(line);
        return NULL;
    }
    line[n] = '\0';
    nLine = (char *) malloc(n * sizeof(char) + 1);
    strcpy(nLine, line);
    free(line);
    return nLine;
}
