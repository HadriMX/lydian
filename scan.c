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
    INNUML,
    INNUMDOT,
    INNUMR,
    INID,
    INNOTEQUALS,
    INLESSTHAN,
    INGREATERTHAN,
    INSTRINGLITERAL,
    DONE
} StateType;

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
            // TODO: imprimir línea de código

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
                                {"read", READ},
                                {"while", WHILE},
                                {"write", WRITE}};

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
                    currState = INNUML;
                else if (isalpha(c))
                    currState = INID;
                else if (c == '<')
                {
                    currState = INLESSTHAN;
                    currToken = LT;
                }
                else if (c == '>')
                {
                    currState = INGREATERTHAN;
                    currToken = GT;
                }
                else if (c == '!')
                    currState = INNOTEQUALS;
                else if ((c == ' ') || (c == '\t') || (c == '\n'))
                    save = FALSE;
                else if (c == '"')
                {
                    save = FALSE;
                    currState = INSTRINGLITERAL;
                }
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

                        case '(':
                            currToken = LPAREN;
                            break;

                        case ')':
                            currToken = RPAREN;
                            break;

                        case '{':
                            currToken = LCB;
                            break;

                        case '}':
                            currToken = RCB;
                            break;
                            
                        case '*':
                            currToken = TIMES;
                            break;

                        case '/':
                            currToken = OVER;
                            break;

                        case '+':
                            currToken = PLUS;
                            break;

                        case '-':
                            currToken = MINUS;
                            break;

                        case '=':
                            currToken = EQ;
                            break;

                        case '&':
                            currToken = AND;
                            break;

                        case '|':
                            currToken = OR;
                            break;

                        case ':':
                            currToken = ASSIGN;
                            break;

                        case ';':
                            currToken = SEMICOLON;
                            break;

                        default:
                            currToken = ERROR;
                            break;
                    }
                }
                break;

            case INCOMMENT:
                save = FALSE;
                if (c == '#')
                {
                    currState = START;
                }
                else if (c == EOF)
                {
                    currState = DONE;
                    currToken = ERROR;
                }
                break;

            case INLESSTHAN:
                currState = DONE;

                if (c == '=')
                    currToken = LEQT;
                else
                    ungetNextChar();
                break;

            case INGREATERTHAN:
                currState = DONE;

                if (c == '=')
                    currToken = GEQT;
                else
                    ungetNextChar();
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

            case INNUML:
                if (c == '.')
                {
                    currState = INNUMDOT;
                }
                else if (!isdigit(c))
                {
                    ungetNextChar();
                    save = FALSE;
                    currState = DONE;
                    currToken = NUMBER;
                }
                break;

            case INNUMDOT:
                if (isdigit(c))
                {
                    currState = INNUMR;
                }
                else
                {
                    save = FALSE;
                    currState = DONE;
                    currToken = ERROR;
                }
                break;

            case INNUMR:
                if (c == '.')
                {
                    currState = DONE;
                    currToken = ERROR;
                }
                else if (!isdigit(c))
                {
                    ungetNextChar();
                    save = FALSE;
                    currState = DONE;
                    currToken = NUMBER;
                }
                break;

            case INID:
                if ((!isalpha(c)) && (!isdigit(c)))
                {
                    ungetNextChar();
                    save = FALSE;
                    currState = DONE;
                    currToken = IDENTIFIER;
                }
                break;

            case INSTRINGLITERAL:
                if (c == '"')
                {
                    save = FALSE;
                    currState = DONE;
                    currToken = STRLITERAL;
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
        fprintf(listing, "\t%2d ", lineNo);
        printToken(currToken, tokenString);
    }

    return currToken;
}
