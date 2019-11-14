/*********************************************************/
/*  Analizador sintáctico                                */
/*********************************************************/

#include "globals.h"
#include "util.c"
#include "scan.c"
#include "parse.h"

static TokenType currToken;

static TreeNode *stmt_sequence(void);
static TreeNode *statement(void);
static TreeNode *if_stmt(void);
static TreeNode *while_stmt(void);
static TreeNode *assign_stmt(void);
static TreeNode *read_stmt(void);
static TreeNode *write_stmt(void);
static TreeNode *exp_stmt(void);
static TreeNode *simple_exp(void);
static TreeNode *term(void);
static TreeNode *factor(void);

static void syntaxError(char *message)
{
    fprintf(listing, "\n>>> ");
    fprintf(listing,
        "Error de sintáxis en la línea %d: %s",
        lineNo, message);
    // TODO: Error = TRUE;
}

static void match(TokenType expected)
{
    if (currToken == expected)
    {
        currToken = getToken();
    }
    else
    {
        syntaxError("Token inesperado -> ");
        printToken(currToken, tokenString);
        fprintf(listing, "       ");
    }
}

TreeNode *stmt_sequence(void)
{
    TreeNode *t = statement();
    TreeNode *p = t;

    while ((currToken != ENDFILE) && (currToken != RCB)
        && (currToken != ELSE))
    {
        TreeNode *q;
        match(SEMICOLON);
        q = statement();

        if (q != NULL)
        {
            if (t == NULL)
            {
                t = p = q;
            }
            else
            {
                p->sibling = q;
                p = q;
            }
        }
    }
    
    return t;
}

TreeNode *statement(void)
{
    TreeNode *t = NULL;

    switch (currToken)
    {
        case IF:
            t = if_stmt();
            break;

        // TODO: los demás case

        default:
            syntaxError("Token inesperado -> ");
            printToken(currToken, tokenString);
            currToken = getToken();
            break;
    }

    return t;
}
