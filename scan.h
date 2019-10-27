/*********************************************************/
/*  Interfaz del analizador léxico                       */
/*********************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

// Tamaño máximo del lexema de un token
#define MAXTOKENLEN 50

// Almacena el lexema de cada token
extern char tokenString[MAXTOKENLEN+1];

// Devuelve el token siguiente en el archivo fuete
TokenType getToken(void);

#endif