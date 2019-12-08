#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

int lineNo = 0;
FILE *source;
FILE *listing;
FILE *code;

int TraceScan = FALSE;
int TraceParse = TRUE;

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
        fprintf(listing, "\nÁrbol de sintaxis:\n");
        printTree(syntaxTree);
    }

    fclose(source);
    return 0;
}