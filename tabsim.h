/*********************************************************/
/*   Interfaz de la tabla de símbolos                    */
/*********************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* Inserta el número de línea y la locación de memoria
 * en la tabla de símbolos
 * loc = locación de memoria se inserta solo
 * la primera vez, si no se ignora
 */
void st_insert(char *name, int lineNo, int loc);

/* Retorna la locación de memoria de una variable
 * o -1 si no se encuentra
 */
int st_lookup(char *name);

/* Imprime un listado con formato del contenido
 * de la tabla de símbolos
 */
void printSymTab(FILE *listing);

#endif