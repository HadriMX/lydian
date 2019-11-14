/*********************************************************/
/*  Funciones de utilidad para el compilador.            */
/*********************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

// Imprime un token y su lexema al archivo de listado
void printToken(TokenType token, const char* tokenString);

/* Crea un nuevo nodo de sentencia para la
 * construcción del árbol sintáctico
 */
TreeNode *newStmtNode(StmtKind kind);

/* Crea un nuevo nodo de expresión para la
 * construcción del árbol sintáctico
 */
TreeNode *newExpNode(ExpKind kind);

/* Asigna y crea una nueva copia de una cadena
 * existente
 */
char *copyString(char *s);

/* Imprime un árbol sintáctico para el archivo de
 * listado en el que los subárboles se indican
 * mediante sangrías
 */
void printTree(TreeNode *tree);

#endif
