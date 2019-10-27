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
        case VOID:
        case WHILE:
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
                "NUMBER, val = %s\n", tokenString);
            break;
        case IDENTIFIER:
            fprintf(listing,
                "IDENTIFIER, name = %s\n", tokenString);
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
