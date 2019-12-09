/*********************************************************/
/*   Analizador semántico                                */
/*********************************************************/

#include "globals.h"
#include "tabsim.h"
#include "semantica.h"

// Contador para las variables
static int location = 0;

/* Recorre el árbol de sintaxis de forma recursiva:
 * aplica preProc en preorden y postProc en postorden
 * al árbol apuntado por t.
 */
static void traverse(TreeNode *t,
                     void (*preProc)(TreeNode *),
                     void (*postProc)(TreeNode *))
{
    if (t != NULL)
    {
        preProc(t);

        for (int i = 0; i < MAXCHILDREN; i++)
        {
            traverse(t->child[i], preProc, postProc);
        }

        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

/* No hace nada; es utilizado para generar recorridos
 * solo en preorden o solo en postorden
 * para el metodo traverse.
 */
static void emptyProc(TreeNode *t)
{
    return;
}

static void insertNode(TreeNode *t)
{
    switch (t->nodeKind)
    {
    case StmtK:
        switch (t->kind.stmt)
        {
        case AssignK:
        case ReadK:
            /* Buscar si ya existe en la tabla e insertar,
               o si ya existe ignorar la ubicación y
               solo usar el número de línea */
            if (st_lookup(t->attr.name) == -1)
                st_insert(t->attr.name, t->lineNo, location++);
            else
                st_insert(t->attr.name, t->lineNo, 0);
            break;

        default:
            break;
        }
        break;

    case ExpK:
        if (t->kind.exp == IdK)
        {
            /* Buscar si ya existe en la tabla e insertar,
               o si ya existe ignorar la ubicación y
               solo usar el número de línea */
            if (st_lookup(t->attr.name) == -1)
                st_insert(t->attr.name, t->lineNo, location++);
            else
                st_insert(t->attr.name, t->lineNo, 0);
        }
        break;

    default:
        break;
    }
}

void buildSymtab(TreeNode *syntaxTree)
{
    traverse(syntaxTree, insertNode, emptyProc);

    if (TraceAnalyze)
    {
        fprintf(listing, "\n<< Tabla de símbolos >>\n\n");
        printSymTab(listing);
    }
}

static void typeError(TreeNode *t, char *message)
{
    fprintf(listing, "Error de tipo en la línea %d: %s\n",
            t->lineNo, message);
}

static void checkNode(TreeNode *t)
{
    switch (t->nodeKind)
    {
    case ExpK:
        switch (t->kind.exp)
        {
        case OpK:
            if ((t->attr.op == AND) || (t->attr.op == OR))
            {
                if ((t->child[0]->type != Boolean) ||
                    (t->child[1]->type != Boolean))
                {
                    typeError(t, "La operación lógica requiere dos booleanos.");
                }
            }
            else
            {
                if ((t->child[0]->type != Decimal) ||
                    (t->child[1]->type != Decimal))
                {
                    typeError(t, "Op aplicada a un valor no numérico.");
                }
            }

            if ((t->attr.op == EQ) || (t->attr.op == NEQ)
                || (t->attr.op == LEQT) || (t->attr.op == LT)
                || (t->attr.op == GEQT) || (t->attr.op == GT)
                || (t->attr.op == AND) || (t->attr.op == OR))
                t->type = Boolean;
            else
                t->type = Decimal;
            break;
        case ConstK:
        case IdK:
            t->type = Decimal;
            break;
        case StrLiteralK:
            t->type = StrLiteral;
            break;
        default:
            break;
        }
        break;
    case StmtK:
        switch (t->kind.stmt)
        {
        case IfK:
            if (t->child[0]->type != Boolean)
                typeError(t->child[0], "La condición del if no es un booleano.");
            break;
        case AssignK:
            if (t->child[0]->type != Decimal)
                typeError(t->child[0], "Solo se admiten asignaciones de tipo numérico.");
            break;
        case WriteK:
            if ((t->child[0]->type != Decimal) && (t->child[0]->type != StrLiteral))
                typeError(t->child[0], "Write no soporta el tipo especificado.");
            break;
        case WhileK:
            if (t->child[0]->type != Boolean)
                typeError(t->child[1], "La condición del while no es un booleano.");
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/* Realiza una comprobación de tipos
 * recorriendo en postorden el árbol de sintaxis
 */
void typeCheck(TreeNode *syntaxTree)
{
    traverse(syntaxTree, emptyProc, checkNode);
}
