/*********************************************************/
/*   Tabla de símbolos                                   */
/*   Esta implementación utiliza una tabla hash          */
/*   encadenada                                          */
/*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabsim.h"

// Tamaño de la tabla de símbolos
#define SIZE 211

/* Se utiliza como multiplicador para la función hash.
 * Es una potencia de 2.
 */
#define SHIFT 4

static int hash(char *key)
{
    int temp = 0;
    int i = 0;
    while (key[i] != '\0')
    {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        i++;
    }
    
    return temp;
}

/* La lista de números de línea del código fuente
 * en el que se hace referencia a una variable.
 */
typedef struct LineListRec
{
    int lineNo;
    struct LineListRec *next;
} *LineList;

/* Registro de listas para cada variable,
 * incluyendo nombre, ubicación de memoria asignada,
 * y la lista de números de línea en el que
 * aparece en el código fuente.
 */
typedef struct BucketListRec
{
    char *name;
    LineList lines;
    int memLoc;
    struct BucketListRec *next;
} *BucketList;

// La tabla de símbolos
static BucketList hashTable[SIZE];

void st_insert(char *name, int lineNo, int loc)
{
    int h = hash(name);
    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0))
    {
        l = l->next;
    }

    if (l == NULL)  // todavía no existe en la tabla
    {
        l = (BucketList)malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList)malloc(sizeof(struct LineListRec));
        l->lines->lineNo = lineNo;
        l->memLoc = loc;
        l->lines->next = NULL;
        l->next = hashTable[h];
        hashTable[h] = l;
    }
    else    // ya existe en la tabla, solo añadir número de línea
    {
        LineList t = l->lines;
        while (t->next != NULL)
        {
            t = t->next;
        }

        t->next = (LineList)malloc(sizeof(struct LineListRec));
        t->next->lineNo = lineNo;
        t->next->next = NULL;
    }
}

int st_lookup(char *name)
{
    int h = hash(name);
    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0))
    {
        l = l->next;
    }

    if (l == NULL)
        return -1;
    else
        return l->memLoc;
}

void printSymTab(FILE *listing)
{
    int i;
    fprintf(listing, "Nombre variable   Ubicación   Números línea\n");
    fprintf(listing, "---------------   ---------   -------------\n");
    for (i = 0; i < SIZE; ++i)
    {
        if (hashTable[i] != NULL)
        {
            BucketList l = hashTable[i];
            while (l != NULL)
            {
                LineList t = l->lines;
                fprintf(listing, "%-17s ", l->name);
                fprintf(listing, "%-9d  ", l->memLoc);
                while (t != NULL)
                {
                    fprintf(listing, "%4d ", t->lineNo);
                    t = t->next;
                }
                fprintf(listing, "\n");
                l = l->next;
            }
        }
    }
}
