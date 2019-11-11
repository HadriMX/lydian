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
#define MAXRESERVED 8

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
    WHILE,
    IDENTIFIER,
    NUMBER,
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

/*
 * Controla si la información de cada token es impresa
 * al archivo de listado a medida que cada token sea
 * reconocido por el analizador léxico.
 */
extern int TraceScan;

#endif
