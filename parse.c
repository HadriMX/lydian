/*********************************************************/
/*  Analizador sintáctico                                */
/*********************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType currToken;

static TreeNode *stmt_sequence(void);
static TreeNode *statement(void);
static TreeNode *declare_stmt(void);
static TreeNode *if_stmt(void);
static TreeNode *while_stmt(void);
static TreeNode *assign_stmt(void);
static TreeNode *read_stmt(void);
static TreeNode *write_stmt(void);
static TreeNode *expr(void);
static TreeNode *complex_exp(void);
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

    while ((currToken != ENDFILE) && (currToken != RCB))
    {
        TreeNode *q;
        q = statement();

        if (q != NULL)
        {
            if (t == NULL)
            {
                t = p = q;
            }
            else // ahora p tampoco puede ser NULL
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
    case CHAR:
    case DECIMAL:
    case INT:
        t = declare_stmt();
        break;
    case IF:
        t = if_stmt();
        break;
    case WHILE:
        t = while_stmt();
        break;
    case IDENTIFIER:
        t = assign_stmt();
        break;
    case READ:
        t = read_stmt();
        break;
    case WRITE:
        t = write_stmt();
        break;

    default:
        syntaxError("Token inesperado -> ");
        printToken(currToken, tokenString);
        currToken = getToken();
        break;
    }

    return t;
}

TreeNode *declare_stmt(void)
{
    TreeNode *t = newStmtNode(DeclareK);

    if ((t != NULL) && ((currToken == CHAR) || (currToken == DECIMAL) || (currToken == INT)))
        t->attr.name = copyString(tokenString);

    match(currToken);
    if (t != NULL)
        t->child[0] = assign_stmt();

    return t;
}

TreeNode *if_stmt(void)
{
    TreeNode *t = newStmtNode(IfK);

    match(IF);
    match(LPAREN);
    if (t != NULL)
        t->child[0] = expr();

    match(RPAREN);
    match(LCB);
    if (t != NULL)
        t->child[1] = stmt_sequence();

    match(RCB);
    if (currToken == ELSE)
    {
        match(ELSE);
        match(LCB);
        if (t != NULL)
            t->child[2] = stmt_sequence();

        match(RCB);
    }

    return t;
}

TreeNode *while_stmt(void)
{
    TreeNode *t = newStmtNode(WhileK);

    match(WHILE);
    match(LPAREN);
    if (t != NULL)
        t->child[0] = expr();

    match(RPAREN);
    match(LCB);
    if (t != NULL)
        t->child[1] = stmt_sequence();

    match(RCB);

    return t;
}

TreeNode *assign_stmt(void)
{
    TreeNode *t = newStmtNode(AssignK);

    if ((t != NULL) && (currToken == IDENTIFIER))
        t->attr.name = copyString(tokenString);

    match(IDENTIFIER);
    match(ASSIGN);
    if (t != NULL)
        t->child[0] = expr();

    match(SEMICOLON);

    return t;
}

TreeNode *read_stmt(void)
{
    TreeNode *t = newStmtNode(ReadK);

    match(READ);
    if ((t != NULL) && (currToken == IDENTIFIER))
        t->attr.name = copyString(tokenString);

    match(IDENTIFIER);
    match(SEMICOLON);

    return t;
}

TreeNode *write_stmt(void)
{
    TreeNode *t = newStmtNode(WriteK);

    match(WRITE);
    if (t != NULL)
        t->child[0] = expr();

    match(SEMICOLON);

    return t;
}

TreeNode *expr(void)
{
    TreeNode *t = complex_exp();

    while ((currToken == AND) || (currToken == OR))
    {
        TreeNode *p = newExpNode(OpK);

        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = currToken;
            t = p;
        }

        match(currToken);
        if (t != NULL)
            t->child[1] = complex_exp();
    }

    return t;
}

TreeNode *complex_exp(void)
{
    TreeNode *t = simple_exp();

    if ((currToken == EQ) || (currToken == NEQ) || (currToken == LEQT) || (currToken == LT) || (currToken == GEQT) || (currToken == GT))
    {
        TreeNode *p = newExpNode(OpK);

        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = currToken;
            t = p;
        }

        match(currToken);
        if (t != NULL)
            t->child[1] = simple_exp();
    }

    return t;
}

TreeNode *simple_exp(void)
{
    TreeNode *t = term();

    while ((currToken == PLUS) || (currToken == MINUS))
    {
        TreeNode *p = newExpNode(OpK);

        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = currToken;
            t = p;
            match(currToken);
            t->child[1] = term();
        }
    }

    return t;
}

TreeNode *term(void)
{
    TreeNode *t = factor();

    while ((currToken == TIMES) || (currToken == OVER))
    {
        TreeNode *p = newExpNode(OpK);

        if (p != NULL)
        {
            p->child[0] = t;
            p->attr.op = currToken;
            t = p;
            match(currToken);
            p->child[1] = factor();
        }
    }

    return t;
}

TreeNode *factor(void)
{
    TreeNode *t = NULL;

    switch (currToken)
    {
    case NUMBER:
        t = newExpNode(ConstK);
        if ((t != NULL) && (currToken == NUMBER))
            t->attr.val = atoi(tokenString);

        match(NUMBER);
        break;

    case IDENTIFIER:
        t = newExpNode(IdK);
        if ((t != NULL) && (currToken == IDENTIFIER))
            t->attr.name = copyString(tokenString);

        match(IDENTIFIER);
        break;

    case STRLITERAL:
        t = newExpNode(StrLiteralK);
        if ((t != NULL) && (currToken == STRLITERAL))
            t->attr.name = copyString(tokenString);

        match(STRLITERAL);
        break;

    case LPAREN:
        match(LPAREN);
        t = expr();
        match(RPAREN);
        break;

    default:
        syntaxError("token inesperado -> ");
        printToken(currToken, tokenString);
        currToken = getToken();
        break;
    }

    return t;
}

// La función principal del analizador sintáctico
TreeNode *parse(void)
{
    TreeNode *t;

    currToken = getToken();
    t = stmt_sequence();
    if (currToken != ENDFILE)
        syntaxError("El código termina antes que el archivo.\n");

    return t;
}
