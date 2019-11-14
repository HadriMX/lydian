/*********************************************************/
/*  Aquí se incluyen los tipos globales y variables      */
/*  para el compilador.                                  */
/*  Se debe incluir antes de cualquier otro "include"    */
/*********************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// Número de palabras reservadas
#define MAXRESERVED 10

typedef enum
{
    ENDFILE,
    ERROR,
    CHAR,
    CONST,
    DECIMAL,
    ELSE,
    IF,
    INT,
    NOT,
    READ,
    WHILE,
    WRITE,
    IDENTIFIER,
    NUMBER,
    STRLITERAL,
    PLUS,
    MINUS,
    TIMES,
    OVER,
    ASSIGN,
    AND,
    OR,
    EQ,
    NEQ,
    LT,
    LEQT,
    GT,
    GEQT,
    LPAREN,
    RPAREN,
    LCB,
    RCB,
    SEMICOLON
} TokenType;

// Archivo de texto de código fuente
extern FILE *source;

// Archivo de texto de salida de listado
extern FILE *listing;

// Archivo de texto para el código generado
extern FILE *code;

// Número de línea fuente para el listado
extern int lineNo;

/*********************************************************/
/***** ÁRBOL SINTÁCTICO PARA EL ANÁLISIS SINTÁCTICO ******/
/*********************************************************/

typedef enum
{
    StmtK,
    ExpK
} NodeKind;

typedef enum
{
    IfK,
    WhileK,
    AssignK,
    ReadK,
    WriteK
} StmtKind;

typedef enum
{
    OpK,
    ConstK,
    IdK
} ExpKind;

typedef enum
{
    Void,
    Decimal,
    Boolean
} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineNo;
    NodeKind nodeKind;
    union {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    union {
        TokenType op;
        int val;
        char *name;
    } attr;
    ExpType type;
} TreeNode;

/*********************************************************/
/***********************  BANDERAS  **********************/
/*********************************************************/

/*
 * Controla si la información de cada token es impresa
 * al archivo de listado a medida que cada token sea
 * reconocido por el analizador léxico.
 */
extern int TraceScan;

/*
 * Controla si el árbol sintáctico es impreso en el
 * archivo del listado en forma linealizada
 * (utilizando sangrías para los hijos)
 */
extern int TraceParse;

#endif
