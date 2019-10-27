/*********************************************************/
/*  Analizador léxico (tokenizer)                        */
/*********************************************************/

#include "globals.h"
#include "util.c"
#include "scan.h"

// Estados del analizador léxico DFA
typedef enum
{
    START,
    INCOMMENT,
    INNUM,
    INID,
    INNOTEQUALS,
    INLESSTHAN,
    INGREATERTHAN,
    DONE
} StateType;

// Lexema de identificador o palabra reservada
char tokenString[MAXTOKENLEN + 1];

/* Longitud del búffer de entrada para las líneas
 * del código fuente
 */
#define BUFLEN 256

// Mantiene la línea actual
static char lineBuf[BUFLEN];

// Posición actual en lineBuf
static int linePos = 0;

// Tamaño actual de la cadena del búffer
static int bufSize = 0;

/* Obtiene el siguiente carácter distinto del blanco
 * de lineBuf leyendo en una nueva línea si lineBuf
 * está agotado
 */
static char getNextChar(void)
{
    if (!(linePos < bufSize))
    {
        lineNo++;

        if (fgets(lineBuf, BUFLEN - 1, source))
        {
            // imprimir línea de código

            bufSize = strlen(lineBuf);
            linePos = 0;
            return lineBuf[linePos++];
        }
        else
        {
            return EOF;
        }
    }
    else
    {
        return lineBuf[linePos++];
    }
}

/* Reajusta un carácter en lineBuf
 */
static void ungetNextChar(void)
{
    linePos--;
}

/* Tabla de búsqueda de palabras reservadas
 */
static struct
{
    char *str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {{"char", CHAR},
                                {"const", CONST},
                                {"decimal", DECIMAL},
                                {"else", ELSE},
                                {"if", IF},
                                {"int", INT},
                                {"not", NOT},
                                {"void", VOID},
                                {"while", WHILE}};

/* Busca linealmente un identificador para ver si es una
 * palabra reservada
 */
static TokenType reservedLookup(char *s)
{
    for (int i = 0; i < MAXRESERVED; i++)
    {
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;
    }

    return IDENTIFIER;
}

TokenType getToken(void)
{
    // índice para almacenamiento en tokenString
    int tokenStringIndex = 0;

    // indica si guardar en tokenString
    int save;

    TokenType currToken;
    StateType currState = START;

    while (currState != DONE)
    {
        char c = getNextChar();
        save = TRUE;

        switch (currState)
        {
            case START:
                if (isdigit(c))
                    currState = INNUM;
                else if (isalpha(c))
                    currState = INID;
                else if (c == '!')
                    currState = INNOTEQUALS;
                else if ((c == ' ') || (c == '\t') || (c == '\n'))
                    save = FALSE;
                else if (c == '#')
                {
                    save = FALSE;
                    currState = INCOMMENT;
                }
                else
                {
                    currState = DONE;

                    switch (c)
                    {
                        case EOF:
                            save = FALSE;
                            currToken = ENDFILE;
                            break;

                        case ':':
                            currToken = ASSIGN;
                            break;

                        case '=':
                            currToken = EQ;
                            break;

                        case '+':
                            currToken = PLUS;
                            break;

                        case '-':
                            currToken = MINUS;
                            break;

                        case '*':
                            currToken = TIMES;
                            break;

                        case '/':
                            currToken = OVER;
                            break;

                        case '&':
                            currToken = AND;
                            break;

                        case '|':
                            currToken = OR;
                            break;

                        case ';':
                            currToken = SEMICOLON;
                            break;

                        // case del resto...
                    }
                }
                break;

            case INCOMMENT:
                save = FALSE;
                if (c == '#')
                    currState = START;
                break;

            case INNOTEQUALS:
                currState = DONE;

                if (c == '=')
                {
                    currToken = NEQ;
                }
                else
                {
                    ungetNextChar();
                    save = FALSE;
                    currToken = ERROR;
                }
                break;

            case INNUM:
                if (!isdigit(c))
                {
                    ungetNextChar();
                    save = FALSE;
                    currState = DONE;
                    currToken = NUMBER;
                }
                break;

            case INID:
                if (!isalpha(c))
                {
                    ungetNextChar();
                    save = FALSE;
                    currState = DONE;
                    currToken = IDENTIFIER;
                }
                break;

            case DONE:
            default:
                fprintf(listing,
                    "Bug al analizar: estado = %d\n", currState);

                currState = DONE;
                currToken = ERROR;
                break;
        }

        if ((save) && (tokenStringIndex <= MAXTOKENLEN))
        {
            tokenString[tokenStringIndex++] = c;
        }

        if (currState == DONE)
        {
            tokenString[tokenStringIndex] = '\0';

            if (currToken == IDENTIFIER)
            {
                currToken = reservedLookup(tokenString);
            }
        }
    }

    if (TraceScan)
    {
        fprintf(listing, "\t%d: ", lineNo);
        printToken(currToken, tokenString);
    }

    return currToken;
}
