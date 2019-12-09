/*********************************************************/
/*   Interfaz del analizador semántico                   */
/*********************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_

/* Construye la tabla de símbolos recorriendo
 * en preorden el árbol de sintaxis
 */
void buildSymtab(TreeNode *);

/* Comprueba los tipos recorriendo
 * en postorden el árbol de sintaxis
 */
void typeCheck(TreeNode *);

#endif