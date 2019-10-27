#include "globals.h"
#include "scan.c"

int lineNo = 0;
FILE *source;
FILE *listing;
FILE *code;

int TraceScan = TRUE;

int main(int argc, char* argv[])
{
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

    while (getToken() != ENDFILE);

    return 0;
}