/*********************************************************/
/*  Implementación de utilidades para el compilador.     */
/*********************************************************/

#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString)
{
    switch (token)
    {
        case CHAR:
        case CONST:
        case DECIMAL:
        case ELSE:
        case IF:
        case INT:
        case NOT:
        case READ:
        case WHILE:
        case WRITE:
            fprintf(listing,
                "KEYWORD: %s\n", tokenString);
            break;
        case PLUS:
            fprintf(listing, "+\n"); break;
        case MINUS:
            fprintf(listing, "-\n"); break;
        case TIMES:
            fprintf(listing, "*\n"); break;
        case OVER:
            fprintf(listing, "/\n"); break;
        case ASSIGN:
            fprintf(listing, ":\n"); break;
        case AND:
            fprintf(listing, "&\n"); break;
        case OR:
            fprintf(listing, "|\n"); break;
        case EQ:
            fprintf(listing, "=\n"); break;
        case NEQ:
            fprintf(listing, "!=\n"); break;
        case LT:
            fprintf(listing, "<\n"); break;
        case LEQT:
            fprintf(listing, "<=\n"); break;
        case GT:
            fprintf(listing, ">\n"); break;
        case GEQT:
            fprintf(listing, ">=\n"); break;
        case LPAREN:
            fprintf(listing, "(\n"); break;
        case RPAREN:
            fprintf(listing, ")\n"); break;
        case LCB:
            fprintf(listing, "{\n"); break;
        case RCB:
            fprintf(listing, "}\n"); break;
        case SEMICOLON:
            fprintf(listing, ";\n"); break;
        case ENDFILE:
            fprintf(listing, "EOF\n"); break;
        case NUMBER:
            fprintf(listing,
                "NUMBER: %s\n", tokenString);
            break;
        case IDENTIFIER:
            fprintf(listing,
                "IDENTIFIER: %s\n", tokenString);
            break;
        case STRLITERAL:
            fprintf(listing,
                "STRING LITERAL: '%s'\n", tokenString);
            break;
        case ERROR:
            fprintf(listing,
                "ERROR: %s\n", tokenString);
            break;
        default:
            fprintf(listing,
                "Token desconocido: %d\n", token);
    }
}

TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));

    if (t == NULL)
    {
        fprintf(listing,
                "Se acabó la memoria en la línea %d\n", lineNo);
    }
    else
    {
        for (int i = 0; i < MAXCHILDREN; i++)
        {
            t->child[i] = NULL;
        }

        t->sibling = NULL;
        t->nodeKind = StmtK;
        t->kind.stmt = kind;
        t->lineNo = lineNo;
    }

    return t;
}

TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));

    if (t == NULL)
    {
        fprintf(listing,
                "Se acabó la memoria en la línea %d\n", lineNo);
    }
    else
    {
        for (int i = 0; i < MAXCHILDREN; i++)
        {
            t->child[i] = NULL;
        }

        t->sibling = NULL;
        t->nodeKind = ExpK;
        t->kind.exp = kind;
        t->lineNo = lineNo;
        t->type = Void;
    }

    return t;
}

char *copyString(char *s)
{
    int n;
    char *t;

    if (s == NULL)
        return NULL;

    n = strlen(s) + 1;
    t = malloc(n);

    if (t == NULL)
    {
        fprintf(listing,
                "Se acabó la memoria en la línea %d\n", lineNo);
    }
    else
    {
        strcpy(t, s);
    }

    return t;
}

// Almacena el número actual de espacios para sangría
static int indentNo = 0;

// Aumenta la sangría
#define INDENT indentNo += 2

// Decrementa la sangría
#define UNINDENT indentNo -= 2

// Realiza las sangrías mediante impresión de espacios
static void printSpaces(void)
{
    for (int i = 0; i < indentNo; i++)
    {
        fprintf(listing, " ");
    }
}

void printTree(TreeNode *tree)
{
    INDENT;
    while (tree != NULL)
    {
        printSpaces();
        if (tree->nodeKind == StmtK)
        {
            switch (tree->kind.stmt)
            {
            case IfK:
                fprintf(listing, "If\n");
                break;

            case WhileK:
                fprintf(listing, "While\n");
                break;

            case AssignK:
                fprintf(listing,
                        "Asignar a: %s\n", tree->attr.name);
                break;

            case ReadK:
                fprintf(listing,
                        "Leer: %s\n", tree->attr.name);
                break;

            case WriteK:
                fprintf(listing, "Escribir\n");
                break;

            default:
                fprintf(listing,
                        "Tipo de nodo Stmt desconocido\n");
                break;
            }
        }
        else if (tree->nodeKind == ExpK)
        {
            switch (tree->kind.exp)
            {
            case OpK:
                fprintf(listing, "Op: ");
                printToken(tree->attr.op, "\0");
                break;

            case ConstK:
                fprintf(listing,
                        "Const: %d\n", tree->attr.val);
                break;

            case IdK:
                fprintf(listing,
                        "Id: %s\n", tree->attr.name);
                break;

            default:
                fprintf(listing,
                        "Tipo de nodo ExpNode desconocido\n");
                break;
            }
        }
        else
        {
            fprintf(listing, "Tipo de nodo desconocido\n");
        }

        for (int i = 0; i < MAXCHILDREN; i++)
        {
            printTree(tree->child[i]);
        }

        tree = tree->sibling;
    }

    UNINDENT;
}
