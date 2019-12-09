#include "globals.h"
#include "util.h"
#include "lexico.h"
#include "sintaxis.h"
#include "semantica.h"

int lineNo = 0;
FILE *source;
FILE *listing;
FILE *code;

int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;

int main(int argc, char* argv[])
{
    TreeNode *syntaxTree;
    // nombre de archivo de código fuente
    char pgm[31];
    
    if (argc != 2)
    {
        argv[1] = "prueba.lyd";
        // printf("Uso: %s <nombre-archivo>\n", argv[0]);
        // exit(1);
    }

    strcpy(pgm, argv[1]);

    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".lyd");

    source = fopen(pgm, "r");

    if (source == NULL)
    {
        printf("El archivo %s no se encuentra.\n", pgm);
        exit(1);
    }

    listing = stdout;   // enviar listado a la pantalla

    fprintf(listing, "\nCOMPILANDO LYDIAN: %s\n", pgm);

    syntaxTree = parse();

    if (TraceParse)
    {
        fprintf(listing, "\n<< Árbol de sintaxis >>\n");
        printTree(syntaxTree);
    }
    
    buildSymtab(syntaxTree);

    if (TraceAnalyze)
        fprintf(listing, "\nComprobando tipos...\n");

    typeCheck(syntaxTree);

    if (TraceAnalyze)
        fprintf(listing, "\nComprobación de tipos finalizada\n");

    fclose(source);
    return 0;
}