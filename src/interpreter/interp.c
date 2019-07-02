/*
 * File:
 *     interp.c
 *
 * Package:
 *     Gua
 *
 * Description:
 *     This file implements an expression interpreter.
 *     Currently it supports the following features:
 *
 *     operators:
 *         !, ~, **, *, /, %, +, -, <<, >>, <, <=, >, >=, ==, !=, &, ^, |,
 *         &|, &&, ||, =, $, @
 *
 *     types:
 *         integer, real, complex, string, array, matrix, handle, nspace
 *
 *     constants:
 *         GUA_VERSION, GUA_INTEGER, GUA_REAL, GUA_COMPLEX, GUA_STRING,
 *         GUA_ARRAY, GUA_MATRIX, GUA_HANDLE, GUA_NAMESPACE, TRUE, FALSE, NULL, i, argc, argv, env
 *
 *     functions:
 *         array, arrayToString, complex, dim, error, eval, exists, expr, ident, inv, keys, length, matrix,
 *         matrix2D, matrixToString, toString, type and user defined functions
 *
 *     variables:
 *         automatic(integer, real, complex, string, array, matrix, handle, namespace)
 *
 *     comments:
 *         Any line beginning with the hash character '#' and ending with
 *         new line '\n' is a comment.
 *
 *     cammand separator:
 *         Commands can be separated one by other in a same line, using
 *         the character ';'.
 *
 *     statements:
 *         if, elseif, else, while, do, for, foreach, function, try, catch,
 *         test.
 *
 * Copyright:
 *     Copyright (c) 2017 Roberto Luiz Souza Monteiro, Hernane B. B. Pereira, Marcelo A. Moret.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * RCS: @(#) $Id: interp.c,v 3.1 2017/05/27 09:42:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "interp.h"

#define EXPRESSION_SIZE  4096
#define BUFFER_SIZE      4096

/* This table is for error report. */
static Gua_TokenData Gua_TokenTable[] = {
    {"TOKEN_TYPE_INTEGER", ""},
    {"TOKEN_TYPE_REAL", ""},
    {"TOKEN_TYPE_STRING", ""},
    {"TOKEN_TYPE_PARENTHESIS", ""},
    {"TOKEN_TYPE_BRACKET", ""},
    {"TOKEN_TYPE_BRACE", ""},
    {"TOKEN_TYPE_NOT", "!"},
    {"TOKEN_TYPE_BIT_NOT", "~"},
    {"TOKEN_TYPE_POWER", "**"},
    {"TOKEN_TYPE_MULT", "*"},
    {"TOKEN_TYPE_DIV", "/"},
    {"TOKEN_TYPE_MOD", "%"},
    {"TOKEN_TYPE_PLUS", "+"},
    {"TOKEN_TYPE_MINUS", "-"},
    {"TOKEN_TYPE_LEFT_SHIFT", "<<"},
    {"TOKEN_TYPE_RIGHT_SHIFT", ">>"},
    {"TOKEN_TYPE_LESS", "<"},
    {"TOKEN_TYPE_LE", "<="},
    {"TOKEN_TYPE_GREATER", ">"},
    {"TOKEN_TYPE_GE", ">="},
    {"TOKEN_TYPE_EQ", "=="},
    {"TOKEN_TYPE_NE", "!="},
    {"TOKEN_TYPE_BIT_AND", "&"},
    {"TOKEN_TYPE_BIT_XOR", "^"},
    {"TOKEN_TYPE_BIT_OR", "|"},
    {"TOKEN_TYPE_AND", "&&"},
    {"TOKEN_TYPE_OR", "||"},
    {"TOKEN_TYPE_AND_OR", "&|"},
    {"TOKEN_TYPE_FUNCTION", ""},
    {"TOKEN_TYPE_VARIABLE", ""},
    {"TOKEN_TYPE_MACRO", "$"},
    {"TOKEN_TYPE_INDIRECTION", "@"},
    {"TOKEN_TYPE_ARG_SEPARATOR", ","},
    {"TOKEN_TYPE_SEPARATOR", ";"},
    {"TOKEN_TYPE_COMMENT", "#"},
    {"TOKEN_TYPE_ASSIGN", "="},
    {"TOKEN_TYPE_IF", "if"},
    {"TOKEN_TYPE_ELSEIF", "elseif"},
    {"TOKEN_TYPE_ELSE", "else"},
    {"TOKEN_TYPE_WHILE", "while"},
    {"TOKEN_TYPE_DO", "do"},
    {"TOKEN_TYPE_FOR", "for"},
    {"TOKEN_TYPE_FOREACH", "foreach"},
    {"TOKEN_TYPE_DEFINE_FUNCTION", "function"},
    {"TOKEN_TYPE_TRY", "try"},
    {"TOKEN_TYPE_CATCH", "catch"},
    {"TOKEN_TYPE_TEST", "test"},
    {"TOKEN_TYPE_END", ""},
    {"TOKEN_TYPE_UNKNOWN", ""}
};

/* Error table. */
static char *Gua_StatusTable[] = {
    GUA_OK_MSG,
    GUA_CONTINUE_MSG,
    GUA_BREAK_MSG,
    GUA_RETURN_MSG,
    GUA_EXIT_MSG,
    GUA_ERROR_MSG,
    GUA_ERROR_UNEXPECTED_TOKEN_MSG,
    GUA_ERROR_OUT_OF_RANGE_MSG,
    GUA_ERROR_UNDERFLOW_MSG,
    GUA_ERROR_OVERFLOW_MSG,
    GUA_ERROR_UNTERMINATED_STRING_MSG,
    GUA_ERROR_UNCLOSED_EXPRESSION_MSG,
    GUA_ERROR_ILLEGAL_OPERAND_MSG,
    GUA_ERROR_DIVISION_BY_ZERO_MSG,
    GUA_ERROR_ILLEGAL_ASSIGNMENT_MSG,
    GUA_ERROR_FUNCTION_MSG
};

/* This table contains variables that can not be set by the Gua_Init function. */
static Gua_Variable Gua_ConstantTable[] = {
    {LEXEME_TRUE, {OBJECT_TYPE_INTEGER, 1, 0.0, 0.0, NULL, NULL, NULL, NULL, NULL, NULL, 0, true}, NULL, NULL},
    {LEXEME_FALSE, {OBJECT_TYPE_INTEGER, 0, 0.0, 0.0, NULL, NULL, NULL, NULL, NULL, NULL, 0, true}, NULL, NULL},
    {LEXEME_NULL, {OBJECT_TYPE_UNKNOWN, 0, 0.0, 0.0, NULL, NULL, NULL, NULL, NULL, NULL, 0, true}, NULL, NULL},
    {LEXEME_COMPLEX, {OBJECT_TYPE_COMPLEX, 0, 0.0, 1.0, NULL, NULL, NULL, NULL, NULL, NULL, 0, true}, NULL, NULL},
    {"", {OBJECT_TYPE_UNKNOWN, 0, 0.0, 0.0, NULL, NULL, NULL, NULL, NULL, NULL, 0, true}, NULL, NULL}
};

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanNumber(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid number and try to guess it type.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanNumber(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);

    p = start;
    
    errno = 0;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (!Gua_IsNumber(*p)) {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->type = TOKEN_TYPE_INTEGER;
    token->status = GUA_OK;
    
    while (isxdigit(*p) || (*p == '.') || (*p == '+') || (*p == '-') || (*p == 'x')) {
        if ((*p == '.') || (*p == 'e') || (*p == 'E')) {
            token->type = TOKEN_TYPE_REAL;
        }
        if (((*p == '-') || (*p == '+')) && !((*(p - 1) == 'e') || (*(p - 1) == 'E'))) {
            break;
        }
        p++;
    }
    
    token->start = start;
    token->length = (Gua_Length)(p - start);
    
    if (token->type == TOKEN_TYPE_INTEGER) {
        token->integer = strtoul(start, NULL, 0);
        if (errno == ERANGE) {
            token->status = GUA_ERROR_OUT_OF_RANGE;
        }
    } else {
        token->real = strtod(start, NULL);
        if (errno == ERANGE) {
            if (token->real == 0.0) {
                token->status = GUA_ERROR_UNDERFLOW;
            } else {
                token->status = GUA_ERROR_OVERFLOW;
            }
        }
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanOperator(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid operator and try to guess it type.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanOperator(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);

    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (!Gua_IsOperator(*p)) {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->status = GUA_OK;
    
    switch (*p) {
        case OPERATOR_NOT:
            p++;
            if (*p == OPERATOR_EQ) {
                token->type = TOKEN_TYPE_NE;
                p++;
            } else {
                token->type = TOKEN_TYPE_NOT;
            }
            break;
        case OPERATOR_BIT_NOT:
            token->type = TOKEN_TYPE_BIT_NOT;
            p++;
            break;
        case OPERATOR_MULT:
            p++;
            if (*p == OPERATOR_MULT) {
                token->type = TOKEN_TYPE_POWER;
                p++;
            } else {
                token->type = TOKEN_TYPE_MULT;
            }
            break;
        case OPERATOR_DIV:
            token->type = TOKEN_TYPE_DIV;
            p++;
            break;
        case OPERATOR_MOD:
            token->type = TOKEN_TYPE_MOD;
            p++;
            break;
        case OPERATOR_PLUS:
            token->type = TOKEN_TYPE_PLUS;
            p++;
            break;
        case OPERATOR_MINUS:
            token->type = TOKEN_TYPE_MINUS;
            p++;
            break;
        case OPERATOR_LESS:
            p++;
            if (*p == OPERATOR_LESS) {
                token->type = TOKEN_TYPE_LEFT_SHIFT;
                p++;
            } else if (*p == OPERATOR_EQ) {
                token->type = TOKEN_TYPE_LE;
                p++;
            } else {
                token->type = TOKEN_TYPE_LESS;
            }
            break;
        case OPERATOR_GREATER:
            p++;
            if (*p == OPERATOR_GREATER) {
                token->type = TOKEN_TYPE_RIGHT_SHIFT;
                p++;
            } else if (*p == OPERATOR_EQ) {
                token->type = TOKEN_TYPE_GE;
                p++;
            } else {
                token->type = TOKEN_TYPE_GREATER;
            }
            break;
        case OPERATOR_EQ:
            p++;
            if (*p == OPERATOR_EQ) {
                token->type = TOKEN_TYPE_EQ;
                p++;
            } else {
                token->type = TOKEN_TYPE_ASSIGN;
            }
            break;
        case OPERATOR_BIT_AND:
            p++;
            if (*p == OPERATOR_BIT_AND) {
                token->type = TOKEN_TYPE_AND;
                p++;
            } else if (*p == OPERATOR_BIT_OR) {
                token->type = TOKEN_TYPE_AND_OR;
                p++;
            } else {
                token->type = TOKEN_TYPE_BIT_AND;
            }
            break;
        case OPERATOR_BIT_XOR:
            token->type = TOKEN_TYPE_BIT_XOR;
            p++;
            break;
        case OPERATOR_BIT_OR:
            p++;
            if (*p == OPERATOR_BIT_OR) {
                token->type = TOKEN_TYPE_OR;
                p++;
            } else {
                token->type = TOKEN_TYPE_BIT_OR;
            }
            break;
        case OPERATOR_MACRO:
            token->type = TOKEN_TYPE_MACRO;
            p++;
            break;
        case OPERATOR_INDIRECTION:
            token->type = TOKEN_TYPE_INDIRECTION;
            p++;
            break;
        default:
            token->status = GUA_ERROR;
            p++;
            break;
    }
    
    token->start = start;
    token->length = (Gua_Length)(p - start);
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanIdentifier(Gua_Namespace *nspace, Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid identifier and try to guess it type.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a structure containing the start and end pointers to the token
 *                found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanIdentifier(Gua_Namespace *nspace, Gua_String start, Gua_Token *token)
{
    Gua_String p;
    Gua_String name;
    Gua_Object object;
    Gua_Function function;
    Gua_Integer i;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);

    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (!Gua_IsIdentifier(*p)) {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    while (isalnum(*p) || (*p == '.') || (*p == '_')) {
        p++;
    }
    
    token->start = start;
    token->length = (Gua_Length)(p - start);
    
    name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
    memset(name, '\0', sizeof(char) * (token->length + 1));
    strncpy(name, token->start, token->length);
    
    if (strcmp(name, LEXEME_IF) == 0) {
        token->type = TOKEN_TYPE_IF;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_ELSEIF) == 0) {
        token->type = TOKEN_TYPE_ELSEIF;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_ELSE) == 0) {
        token->type = TOKEN_TYPE_ELSE;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_WHILE) == 0) {
        token->type = TOKEN_TYPE_WHILE;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_DO) == 0) {
        token->type = TOKEN_TYPE_DO;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_FOR) == 0) {
        token->type = TOKEN_TYPE_FOR;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_FOREACH) == 0) {
        token->type = TOKEN_TYPE_FOREACH;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_DEFINE_FUNCTION) == 0) {
        token->type = TOKEN_TYPE_DEFINE_FUNCTION;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_TRY) == 0) {
        token->type = TOKEN_TYPE_TRY;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_CATCH) == 0) {
        token->type = TOKEN_TYPE_CATCH;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    if (strcmp(name, LEXEME_TEST) == 0) {
        token->type = TOKEN_TYPE_TEST;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    
    if (Gua_SearchVariable(nspace, name, &object, SCOPE_STACK) != OBJECT_TYPE_UNKNOWN) {
        token->type = TOKEN_TYPE_VARIABLE;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    
    i = 0;
    
    while (strlen(Gua_ConstantTable[i].name) > 0) {
        if (strcmp(Gua_ConstantTable[i].name, name) == 0) {
            token->type = TOKEN_TYPE_VARIABLE;
            token->status = GUA_OK;
            Gua_Free(name);
            return p;
        }
        i++;
    }
    
    if (Gua_SearchFunction(nspace, name, &function) == GUA_OK) {
        token->type = TOKEN_TYPE_FUNCTION;
        token->status = GUA_OK;
        Gua_Free(name);
        return p;
    }
    
    Gua_Free(name);
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanSingleQuotes(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid string and try to get it.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanSingleQuotes(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);

    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == SINGLE_QUOTE) {
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->type = TOKEN_TYPE_SCRIPT;
    token->status = GUA_OK;
    
    while ((*p != SINGLE_QUOTE) && (*p != EXPRESSION_END)) {
        if (*p == '\\') {
            if (*(p + 1) == '\'') {
                p++;
            }
        }
        p++;
    }
    
    if (*p == EXPRESSION_END) {
        token->status = GUA_ERROR_UNTERMINATED_STRING;
        token->start = start;
        token->length = (Gua_Length)(p - start) + 1;
    } else {
        token->start = start + 1;
        token->length = (Gua_Length)(p - start) - 1;
        p++;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanDoubleQuotes(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid string and try to get it.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanDoubleQuotes(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);

    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == DOUBLE_QUOTE) {
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->type = TOKEN_TYPE_STRING;
    token->status = GUA_OK;
    
    while ((*p != DOUBLE_QUOTE) && (*p != EXPRESSION_END)) {
        if (*p == '\\') {
            if (*(p + 1) == '"') {
                p++;
            }
        }
        p++;
    }
    
    if (*p == EXPRESSION_END) {
        token->status = GUA_ERROR_UNTERMINATED_STRING;
        token->start = start;
        token->length = (Gua_Length)(p - start) + 1;
    } else {
        token->start = start + 1;
        token->length = (Gua_Length)(p - start) - 1;
        p++;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanString(Gua_String target, Gua_String source, Gua_Integer n)
 *
 * Description:
 *     Do backslash substitutions on the string.
 *
 * Arguments:
 *     source,    a pointer to the original string;
 *     target,    a pointer to the parsed string.
 *
 * Results:
 *     The function returns a pointer to the parsed string.
 */
Gua_String Gua_ScanString(Gua_String target, Gua_String source, Gua_Integer n)
{
    while (n > 0) {
        if (*source == '\\') {
            source++;
            n--;
            switch (*source) {
                case '\'':
                    *target = '\'';
                    break;
                case '"':
                    *target = '"';
                    break;
                case 'a':
                    *target = '\a';
                    break;
                case 'b':
                    *target = '\b';
                    break;
                case 'f':
                    *target = '\f';
                    break;
                case 'n':
                    *target = '\n';
                    break;
                case 'r':
                    *target = '\r';
                    break;
                case 't':
                    *target = '\t';
                    break;
                case 'v':
                    *target = '\v';
                    break;
                default:
                    *target = *source;
                    break;
            }
        } else {
            *target = *source;
        }
        source++;
        target++;
        n--;
    }
    
    return target;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanArgSeparator(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid argument separator and try to get the next expression.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanArgSeparator(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);

    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if ((*p == COMMA) || (*p == END_OF_LINE)) {
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->type = TOKEN_TYPE_ARG_SEPARATOR;
    token->start = p;
    token->length = (Gua_Length)(p - start);
    token->status = GUA_OK;
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanSeparator(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid expression separator and try to get the next expression.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanSeparator(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);
    
    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if ((*p == SEMI_COLON) || (*p == END_OF_LINE)) {
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->type = TOKEN_TYPE_SEPARATOR;
    token->start = p;
    token->length = (Gua_Length)(p - start);
    token->status = GUA_OK;
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanComment(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid comment and try to get it.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanComment(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);
    
    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == HASH) {
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->type = TOKEN_TYPE_COMMENT;
    token->status = GUA_OK;
    
    while ((*p != END_OF_LINE) && (*p != EXPRESSION_END)) {
        p++;
    }
    
    token->start = start;
    token->length = (Gua_Length)(p - start) + 1;
    
    if (*p == END_OF_LINE) {
        p++;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanParenthesis(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a valid parenthesised expression and try to get it.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanParenthesis(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    Gua_Short opened;
    Gua_Short closed;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);
    
    p = start;
    
    opened = 0;
    closed = 0;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == PARENTHESIS_OPEN) {
        opened++;
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->status = GUA_OK;
    
    while ((*p != EXPRESSION_END) && (opened > closed)) {
        if (*p == SINGLE_QUOTE) {
            p = Gua_ScanSingleQuotes(p, token);
        }
        if (*p == DOUBLE_QUOTE) {
            p = Gua_ScanDoubleQuotes(p, token);
        }
        if (*p == PARENTHESIS_OPEN) {
            opened++;
        }
        if (*p == PARENTHESIS_CLOSE) {
            closed++;
        }
        p++;
    }
    
    if (token->status != GUA_OK) {
        return p;
    }
    
    token->type = TOKEN_TYPE_PARENTHESIS;
    
    if (*p == EXPRESSION_END) {
        if (opened > closed) {
            token->start = start;
            token->length = (Gua_Length)(p - start) + 1;
            token->status = GUA_ERROR_UNCLOSED_EXPRESSION;
        } else {
            token->start = start + 1;
            token->length = (Gua_Length)(p - start) - 2;
        }
    } else {
        token->start = start + 1;
        token->length = (Gua_Length)(p - start) - 2;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanBracket(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a close bracket and try to get it.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanBracket(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    Gua_Short opened;
    Gua_Short closed;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);
    
    p = start;
    
    opened = 0;
    closed = 0;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == BRACKET_OPEN) {
        opened++;
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->status = GUA_OK;
    
    while ((*p != EXPRESSION_END) && (opened > closed)) {
        if (*p == SINGLE_QUOTE) {
            p = Gua_ScanSingleQuotes(p, token);
        }
        if (*p == DOUBLE_QUOTE) {
            p = Gua_ScanDoubleQuotes(p, token);
        }
        if (*p == BRACKET_OPEN) {
            opened++;
        }
        if (*p == BRACKET_CLOSE) {
            closed++;
        }
        p++;
    }
    
    if (token->status != GUA_OK) {
        return p;
    }
    
    token->type = TOKEN_TYPE_BRACKET;
    
    if (*p == EXPRESSION_END) {
        if (opened > closed) {
            token->start = start;
            token->length = (Gua_Length)(p - start) + 1;
            token->status = GUA_ERROR_UNCLOSED_EXPRESSION;
        } else {
            token->start = start + 1;
            token->length = (Gua_Length)(p - start) - 2;
        }
    } else {
        token->start = start + 1;
        token->length = (Gua_Length)(p - start) - 2;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ScanBrace(Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Check if the token is a close brace and try to get it.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse;
 *     token,    a structure containing the start and end pointers to the token
 *               found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_ScanBrace(Gua_String start, Gua_Token *token)
{
    Gua_String p;
    Gua_Short opened;
    Gua_Short closed;
    
    /* The default token object is: TOKEN_TYPE_UNKNOWN; GUA_ERROR_UNEXPECTED_TOKEN. */
    Gua_ClearPToken(token);
    
    p = start;
    
    opened = 0;
    closed = 0;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == BRACE_OPEN) {
        opened++;
        p++;
    } else {
        token->start = p;
        token->length = 1;
        p++;
        return p;
    }
    
    token->status = GUA_OK;
    
    while ((*p != EXPRESSION_END) && (opened > closed)) {
        if (*p == SINGLE_QUOTE) {
            p = Gua_ScanSingleQuotes(p, token);
        }
        if (*p == DOUBLE_QUOTE) {
            p = Gua_ScanDoubleQuotes(p, token);
        }
        if (*p == BRACE_OPEN) {
            opened++;
        }
        if (*p == BRACE_CLOSE) {
            closed++;
        }
        p++;
    }
    
    if (token->status != GUA_OK) {
        return p;
    }
    
    token->type = TOKEN_TYPE_BRACE;
    
    if (*p == EXPRESSION_END) {
        if (opened > closed) {
            token->start = start;
            token->length = (Gua_Length)(p - start) + 1;
            token->status = GUA_ERROR_UNCLOSED_EXPRESSION;
        } else {
            token->start = start + 1;
            token->length = (Gua_Length)(p - start) - 2;
        }
    } else {
        token->start = start + 1;
        token->length = (Gua_Length)(p - start) - 2;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_NextToken(Gua_Namespace *nspace, Gua_String start, Gua_Token *token)
 *
 * Description:
 *     Get the next token from expr.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a structure containing the start and end pointers to the token
 *                found in the expression, and also the token type.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 */
Gua_String Gua_NextToken(Gua_Namespace *nspace, Gua_String start, Gua_Token *token)
{
    Gua_String p;
    
    token->status = GUA_OK;
    
    p = start;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    if (*p == EXPRESSION_END) {
        token->start = p;
        token->length = 1;
        token->type = TOKEN_TYPE_END;
        return p;
    }
    
    if (Gua_IsSeparator(*p)) {
        p = Gua_ScanSeparator(p, token);
    }else if (Gua_IsNumber(*p)) {
        p = Gua_ScanNumber(p, token);
    } else if (Gua_IsOperator(*p)) {
        p = Gua_ScanOperator(p, token);
    } else if (Gua_IsIdentifier(*p)) {
        p = Gua_ScanIdentifier(nspace, p, token);
    } else if (*p == PARENTHESIS_OPEN) {
        p = Gua_ScanParenthesis(p, token);
    } else if (*p == PARENTHESIS_CLOSE) {
        token->start = p;
        token->length = 1;
        token->type = TOKEN_TYPE_PARENTHESIS;
        token->status = GUA_ERROR_UNCLOSED_EXPRESSION;
        p++;
    } else if (*p == BRACKET_OPEN) {
        p = Gua_ScanBracket(p, token);
    } else if (*p == BRACKET_CLOSE) {
        token->start = p;
        token->length = 1;
        token->type = TOKEN_TYPE_BRACKET;
        token->status = GUA_ERROR_UNCLOSED_EXPRESSION;
        p++;
    } else if (*p == BRACE_OPEN) {
        p = Gua_ScanBrace(p, token);
    } else if (*p == BRACE_CLOSE) {
        token->start = p;
        token->length = 1;
        token->type = TOKEN_TYPE_BRACE;
        token->status = GUA_ERROR_UNCLOSED_EXPRESSION;
        p++;
    } else if (*p == SINGLE_QUOTE) {
        p = Gua_ScanSingleQuotes(p, token);
    } else if (*p == DOUBLE_QUOTE) {
        p = Gua_ScanDoubleQuotes(p, token);
    } else if (*p == COMMA) {
        p = Gua_ScanArgSeparator(p, token);
    } else if (*p == HASH) {
        p = Gua_ScanComment(p, token);
    } else {
        token->start = p;
        token->length = 1;
        token->type = TOKEN_TYPE_UNKNOWN;
        token->status = GUA_ERROR_UNEXPECTED_TOKEN;
        p++;
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_ClearNamespace(Gua_Namespace *nspace)
 *
 * Description:
 *     Clear the namespace hash slots.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace.
 *
 * Results:
 *     The function clear the namespace hash slots.
 */
void Gua_ClearNamespace(Gua_Namespace *nspace)
{
    Gua_Short i;
    
    for (i = 0; i < NAMESPACE_HASH_SIZE; i++) {
        nspace->variable[i] = NULL;
        nspace->function[i] = NULL;
    }
    
    nspace->previous = NULL;
    nspace->next = NULL;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeNamespace(Gua_Namespace *nspace)
 *
 * Description:
 *     Free the variable and function namespace.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace.
 *
 * Results:
 *     The function frees the variable and function namespace.
 */
void Gua_FreeNamespace(Gua_Namespace *nspace)
{
    Gua_Short i;
    
    if (nspace->next) {
        Gua_FreeNamespace((Gua_Namespace *)nspace->next);
    }
    
    for (i = 0; i < NAMESPACE_HASH_SIZE; i++) {
        if (nspace->variable[i]) {
            Gua_FreeVariableList(nspace->variable[i]);
        }
        if (nspace->function[i]) {
            Gua_FreeFunctionList(nspace->function[i]);
        }
    }
    
    Gua_Free(nspace);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short Gua_NamespaceSlot(Gua_PChar *name)
 *
 * Description:
 *     Get a namespace hash slots number.
 *
 * Arguments:
 *     name,    a name to calculate the appropriate hash slot number.
 *
 * Results:
 *     The function returns the appropriate hash slot number.
 */
Gua_Short Gua_NamespaceSlot(Gua_String name)
{
    Gua_Integer i;
    Gua_Integer sum;
    Gua_Short hash;
    
    sum = 0;
    
    for (i = 0; i < strlen(name); i++) {
        sum = sum + name[i];
    }
    
    hash = sum % NAMESPACE_HASH_SIZE;
    
    return hash;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeVariableList(Gua_Variable *variable)
 *
 * Description:
 *     Free a variable list.
 *
 * Arguments:
 *     variable,    a pointer to a variable.
 *
 * Results:
 *     The function frees the variable list.
 */
void Gua_FreeVariableList(Gua_Variable *variable)
{
    if (variable->next) {
        Gua_FreeVariableList((Gua_Variable *)variable->next);
    }
    
    Gua_FreeVariable(variable);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeVariable(Gua_Variable *variable)
 *
 * Description:
 *     Free a variable.
 *
 * Arguments:
 *     variable,    a pointer to a variable.
 *
 * Results:
 *     The function frees the variable.
 */
void Gua_FreeVariable(Gua_Variable *variable)
{
    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_STRING) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_FILE) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_HANDLE)) {
        Gua_FreeObject(&variable->object);
    }
    Gua_Free(variable->name);
    Gua_Free(variable);
    variable = NULL;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_UnsetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Short scope)
 *
 * Description:
 *     Delete a variable from the nspace.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     name,      a pointer to the variable name;
 *     scope,     the variable scope.
 *
 * Results:
 *     The function deletes the variable from the variable list.
 */
Gua_Status Gua_UnsetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Short scope)
{
    Gua_Namespace *p;
    Gua_Variable *variable;
    Gua_Variable *previous;
    Gua_Variable *next;
    
    variable = nspace->variable[Gua_NamespaceSlot(name)];
    
    if ((scope == SCOPE_LOCAL) || (scope == SCOPE_STACK)) {
        if (variable) {
            while (true) {
                if (strcmp(variable->name, name) == 0) {
                    previous = (Gua_Variable *)variable->previous;
                    next = (Gua_Variable *)variable->next;
                    if (previous) {
                        previous->next = (struct Gua_Variable *)next;
                    }
                    if (next) {
                        next->previous = (struct Gua_Variable *)previous;
                    }
                    if (!previous) {
                        if (!next) {
                            nspace->variable[Gua_NamespaceSlot(name)] = NULL;
                        } else {
                            nspace->variable[Gua_NamespaceSlot(name)] = next;
                        }
                    }
                    Gua_FreeVariable(variable);
                    return GUA_OK;
                }
                if (variable->next) {
                    variable = (Gua_Variable *)variable->next;
                } else {
                    break;
                }
            }
        }
    }
    
    if (scope == SCOPE_STACK) {
        p = nspace;
        if (p->previous) {
            p = (Gua_Namespace *)p->previous;
            return Gua_UnsetVariable(p, name, SCOPE_STACK);
        }
    }
    
    if (scope == SCOPE_GLOBAL) {
        p = nspace;
        if (p->previous) {
            while (p->previous) {
                p = (Gua_Namespace *)p->previous;
            }
            return Gua_UnsetVariable(p, name, SCOPE_LOCAL);
        } else {
            return Gua_UnsetVariable(p, name, SCOPE_LOCAL);
        }
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_SearchVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope)
 *
 * Description:
 *     Search a variable and returns it object.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     name,      a pointer to the variable name;
 *     object,    a structure containing the return object of the variable;
 *     scope,     the variable scope.
 *
 * Results:
 *     The function returns the variable object.
 */
Gua_Status Gua_SearchVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope)
{
    Gua_Namespace *p;
    Gua_Variable *variable;
    
    Gua_ClearPObject(object);
    
    variable = nspace->variable[Gua_NamespaceSlot(name)];

    if ((scope == SCOPE_LOCAL) || (scope == SCOPE_STACK)) {
        if (variable) {
            while (true) {
                if (strcmp(variable->name, name) == 0) {
                    Gua_LinkToPObject(object, variable->object);
                    Gua_SetStoredPObject(object);
                    return Gua_PObjectType(object);
                }
                if (variable->next) {
                    variable = (Gua_Variable *)variable->next;
                } else {
                    break;
                }
            }
        }
    }
    
    if (scope == SCOPE_STACK) {
        p = nspace;
        if (p->previous) {
            p = (Gua_Namespace *)p->previous;
            Gua_SearchVariable(p, name, object, SCOPE_STACK);
        }
    }
    
    if (scope == SCOPE_GLOBAL) {
        p = nspace;
        if (p->previous) {
            while (p->previous) {
                p = (Gua_Namespace *)p->previous;
            }
            Gua_SearchVariable(p, name, object, SCOPE_LOCAL);
        } else {
            Gua_SearchVariable(p, name, object, SCOPE_LOCAL);
        }
    }
    
    return Gua_PObjectType(object);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_GetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope)
 *
 * Description:
 *     Get the variable object.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     name,      a pointer to the variable name;
 *     object,    a structure containing the return object of the variable;
 *     scope,     the variable scope.
 *
 * Results:
 *     The function returns the variable object.
 */
Gua_Status Gua_GetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope)
{
    Gua_Integer i;
    
    Gua_ClearPObject(object);
    
    if (Gua_SearchVariable(nspace, name, object, scope) != OBJECT_TYPE_UNKNOWN) {
        return Gua_PObjectType(object);
    }
    
    i = 0;
    
    while (strlen(Gua_ConstantTable[i].name) > 0) {
        if (strcmp(Gua_ConstantTable[i].name, name) == 0) {
            Gua_LinkToPObject(object, Gua_ConstantTable[i].object);
            Gua_SetStoredPObject(object);
            break;
        }
        i++;
    }
    
    return Gua_PObjectType(object);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Gua_SetVariable(Gua_Namespace *nspace, char *name, Gua_Object *object, int scope)
 *
 * Description:
 *     Search a variable and set its object.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     name,      a pointer to the variable name;
 *     object,    a structure containing the object to the variable;
 *     scope,     the variable scope.
 *
 * Results:
 *     The function set the variable object.
 */
int Gua_SetVariable(Gua_Namespace *nspace, char *name, Gua_Object *object, int scope)
{
    Gua_Namespace *p;
    Gua_Variable *variable;
    Gua_Variable *newVariable;
    
    variable = nspace->variable[Gua_NamespaceSlot(name)];
    
    if ((scope == SCOPE_LOCAL) || (scope == SCOPE_STACK)) {
        /* If the variable list already exists, let's work with it. */
        if (variable) {
            while (true) {
                if (strcmp(variable->name, name) == 0) {
                    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_STRING) && (Gua_ObjectToString(variable->object) == Gua_PObjectToString(object))) {
                        return GUA_OK;
                    }
                    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_ARRAY) && (Gua_ObjectToArray(variable->object) == Gua_PObjectToArray(object))) {
                        return GUA_OK;
                    }
                    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_MATRIX) && (Gua_ObjectToMatrix(variable->object) == Gua_PObjectToMatrix(object))) {
                        return GUA_OK;
                    }
                    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_FILE) && (Gua_ObjectToFile(variable->object) == Gua_PObjectToFile(object))) {
                        return GUA_OK;
                    }
                    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_HANDLE) && (Gua_ObjectToHandle(variable->object) == Gua_PObjectToHandle(object))) {
                        return GUA_OK;
                    }
                    
                    /* Free the old variable object. */
                    if ((Gua_ObjectType(variable->object) == OBJECT_TYPE_STRING) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_FILE) || (Gua_ObjectType(variable->object) == OBJECT_TYPE_HANDLE)) {
                        Gua_FreeObject(&(variable->object));
                    }
                    
                    /* Set the new variable object. */
                    if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_ByteArrayToObject(variable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                        } else {
                            Gua_LinkByteArrayToObject(variable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyArray(&(variable->object), (Gua_Object *)object, true);
                        } else {
                            Gua_SetStoredArray(object, true);
                            Gua_ArrayToObject(variable->object, Gua_PObjectToArray(object), Gua_PObjectLength(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyMatrix(&(variable->object), (Gua_Object *)object, true);
                        } else {
                            Gua_SetStoredMatrix(object, true);
                            Gua_MatrixToObject(variable->object, Gua_PObjectToMatrix(object), Gua_PObjectLength(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyFile(&(variable->object), (Gua_Object *)object, true);
                        } else {
                            Gua_FileToObject(variable->object, Gua_PObjectToFile(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyHandle(&(variable->object), (Gua_Object *)object, true);
                        } else {
                            Gua_HandleToObject(variable->object, Gua_PObjectToHandle(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else {
                        Gua_LinkFromPObject(variable->object, object);
                        Gua_SetStoredPObject(object);
                    }
                    Gua_SetStoredObject(variable->object);
                    
                    return GUA_OK;
                }
                if (variable->next) {
                    variable = (Gua_Variable *)variable->next;
                } else {
                    break;
                }
            }
            
            if (scope == SCOPE_LOCAL) {
                Gua_NewVariable(newVariable, name);
                
                Gua_ClearObject(newVariable->object);
                
                /* Set the new variable object. */
                if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_ByteArrayToObject(newVariable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                    } else {
                        Gua_LinkByteArrayToObject(newVariable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyArray(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_SetStoredArray(object, true);
                        Gua_ArrayToObject(newVariable->object, Gua_PObjectToArray(object), Gua_PObjectLength(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyMatrix(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_SetStoredMatrix(object, true);
                        Gua_MatrixToObject(newVariable->object, Gua_PObjectToMatrix(object), Gua_PObjectLength(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyFile(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_FileToObject(newVariable->object, Gua_PObjectToFile(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyHandle(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_HandleToObject(newVariable->object, Gua_PObjectToHandle(object));
                        Gua_SetStoredPObject(object);
                    }
                } else {
                    Gua_LinkFromPObject(newVariable->object, object);
                    Gua_SetStoredPObject(object);
                }
                Gua_SetStoredObject(newVariable->object);
                
                /* Now, let's insert it into the nspace variable list. */
                newVariable->previous = (struct Gua_Variable *)variable;
                newVariable->next = NULL;
                
                variable->next = (struct Gua_Variable *)newVariable;
                return GUA_OK;
            }
        } else {
            if (scope == SCOPE_LOCAL) {
                Gua_NewVariable(newVariable, name);
                
                Gua_ClearObject(newVariable->object);
                
                /* Set the new variable object. */
                if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_ByteArrayToObject(newVariable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                    } else {
                        Gua_LinkByteArrayToObject(newVariable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyArray(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_SetStoredArray(object, true);
                        Gua_ArrayToObject(newVariable->object, Gua_PObjectToArray(object), Gua_PObjectLength(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyMatrix(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_SetStoredMatrix(object, true);
                        Gua_MatrixToObject(newVariable->object, Gua_PObjectToMatrix(object), Gua_PObjectLength(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyFile(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_FileToObject(newVariable->object, Gua_PObjectToFile(object));
                        Gua_SetStoredPObject(object);
                    }
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
                    if (Gua_IsPObjectStored(object)) {
                        Gua_CopyHandle(&(newVariable->object), (Gua_Object *)object, true);
                    } else {
                        Gua_HandleToObject(newVariable->object, Gua_PObjectToHandle(object));
                        Gua_SetStoredPObject(object);
                    }
                } else {
                    Gua_LinkFromPObject(newVariable->object, object);
                    Gua_SetStoredPObject(object);
                }
                Gua_SetStoredObject(newVariable->object);
                
                /* Now, let's insert it into the nspace variable list. */
                newVariable->previous = NULL;
                newVariable->next = NULL;
                
                nspace->variable[Gua_NamespaceSlot(name)] = newVariable;
                return GUA_OK;
            }
        }
    }
    
    if (scope == SCOPE_STACK) {
        p = nspace;
        if (p->previous) {
            p = (Gua_Namespace *)p->previous;
            return Gua_SetVariable(p, name, object, SCOPE_STACK);
        } else {
            return Gua_SetVariable(p, name, object, SCOPE_LOCAL);
        }
    }
    
    if (scope == SCOPE_GLOBAL) {
        p = nspace;
        if (p->previous) {
            while (p->previous) {
                p = (Gua_Namespace *)p->previous;
            }
            return Gua_SetVariable(p, name, object, SCOPE_LOCAL);
        } else {
            return Gua_SetVariable(p, name, object, SCOPE_LOCAL);
        }
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_UpdateObject(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope)
 *
 * Description:
 *     Search a variable and updates its object and length.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     name,      a pointer to the variable name;
 *     object,    a structure containing the object to the variable;
 *     scope,     the variable scope.
 *
 * Results:
 *     The function updates the variable object and length.
 */
Gua_Status Gua_UpdateObject(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope)
{
    Gua_Namespace *p;
    Gua_Variable *variable;
    
    variable = nspace->variable[Gua_NamespaceSlot(name)];
    
    if ((scope == SCOPE_LOCAL) || (scope == SCOPE_STACK)) {
        if (variable) {
            while (true) {
                if (strcmp(variable->name, name) == 0) {
                    if (Gua_ObjectType(variable->object) == OBJECT_TYPE_STRING) {
                        Gua_LinkByteArrayToObject(variable->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                        return GUA_OK;
                    }
                    if (Gua_ObjectType(variable->object) == OBJECT_TYPE_ARRAY) {
                        Gua_ArrayToObject(variable->object, Gua_PObjectToArray(object), Gua_PObjectLength(object));
                        return GUA_OK;
                    }
                    if (Gua_ObjectType(variable->object) == OBJECT_TYPE_MATRIX) {
                        Gua_MatrixToObject(variable->object, Gua_PObjectToMatrix(object), Gua_PObjectLength(object));
                        return GUA_OK;
                    }
                    if (Gua_ObjectType(variable->object) == OBJECT_TYPE_FILE) {
                        Gua_FileToObject(variable->object, Gua_PObjectToFile(object));
                        return GUA_OK;
                    }
                    if (Gua_ObjectType(variable->object) == OBJECT_TYPE_HANDLE) {
                        Gua_HandleToObject(variable->object, Gua_PObjectToHandle(object));
                        return GUA_OK;
                    }
                }
                if (variable->next) {
                    variable = (Gua_Variable *)variable->next;
                } else {
                    break;
                }
            }
        }
    }
    
    if (scope == SCOPE_STACK) {
        p = nspace;
        if (p->previous) {
            p = (Gua_Namespace *)p->previous;
            return Gua_UpdateObject(p, name, object, SCOPE_STACK);
        } else {
            return Gua_UpdateObject(p, name, object, SCOPE_LOCAL);
        }
    }
    
    if (scope == SCOPE_GLOBAL) {
        p = nspace;
        if (p->previous) {
            while (p->previous) {
                p = (Gua_Namespace *)p->previous;
            }
            return Gua_UpdateObject(p, name, object, SCOPE_LOCAL);
        } else {
            return Gua_UpdateObject(p, name, object, SCOPE_LOCAL);
        }
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeFunctionList(Gua_Function *function)
 *
 * Description:
 *     Free a function list.
 *
 * Arguments:
 *     function,    a pointer to a function.
 *
 * Results:
 *     The function frees the function list.
 */
void Gua_FreeFunctionList(Gua_Function *function)
{
    if (function->next) {
        Gua_FreeFunctionList((Gua_Function *)function->next);
    }
    Gua_FreeFunction(function);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeFunction(Gua_Function *function)
 *
 * Description:
 *     Free a function.
 *
 * Arguments:
 *     variable,    a pointer to a function.
 *
 * Results:
 *     The function frees the function.
 */
void Gua_FreeFunction(Gua_Function *function)
{
    Gua_Argument *args;
    Gua_Short i;
    
    Gua_Free(function->name);
    
    if (function->type == FUNCTION_TYPE_SCRIPT) {
        if ((int)function->argc > 0) {
            args = (Gua_Argument *)function->argv;
            for (i = 0; i < (int)function->argc; i++) {
                Gua_Free(Gua_ArgName(args[i]));
                Gua_FreeObject(&(Gua_ArgObject(args[i])));
            }
            Gua_Free(function->argv);
        }
        Gua_Free(function->script);
    }
    
    Gua_Free(function);
    function = NULL;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_UnsetFunction(Gua_Namespace *nspace, Gua_String name)
 *
 * Description:
 *     Delete a function from the nspace.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     name,      a pointer to the function name.
 *
 * Results:
 *     The function deletes the function from the function list.
 */
Gua_Status Gua_UnsetFunction(Gua_Namespace *nspace, Gua_String name)
{
    Gua_Namespace *ns;
    Gua_Function *function;
    Gua_Function *previous;
    Gua_Function *next;
    
    ns = (Gua_Namespace *)nspace;
    
    function = ns->function[Gua_NamespaceSlot(name)];
    
    if (function) {
        while (true) {
            if (strcmp(function->name, name) == 0) {
                previous = (Gua_Function *)function->previous;
                next = (Gua_Function *)function->next;
                
                if (previous) {
                    previous->next = (struct Gua_Function *)next;
                }
                if (next) {
                    next->previous = (struct Gua_Function *)previous;
                }
                if (!previous) {
                    if (!next) {
                        ns->function[Gua_NamespaceSlot(name)] = NULL;
                    } else {
                        ns->function[Gua_NamespaceSlot(name)] = next;
                    }
                }
                
                Gua_FreeFunction(function);
                
                return GUA_OK;
            }
            if (function->next) {
                function = (Gua_Function *)function->next;
            } else {
                break;
            }
        }
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_SearchFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function)
 *
 * Description:
 *     Search a function and returns it.
 *
 * Arguments:
 *     nspace,      a pointer to a structure containing the variable and function namespace;
 *     name,        a pointer to the function name;
 *     function,    a structure containing a pointer to the function.
 *
 * Results:
 *     The function returns a pointer to the function.
 */
Gua_Status Gua_SearchFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function)
{
    Gua_Namespace *ns;
    Gua_Function *p;
    
    ns = (Gua_Namespace *)nspace;
    
    p = ns->function[Gua_NamespaceSlot(name)];
    
    if (p) {
        while (true) {
            if (strcmp(p->name, name) == 0) {
                Gua_LinkPFunctions(function, p);
                return GUA_OK;
            }
            if (p->next) {
                p = (Gua_Function *)p->next;
            } else {
                break;
            }
        }
    }
    
    ns = nspace;
    if (ns->previous) {
        ns = (Gua_Namespace *)ns->previous;
        return Gua_SearchFunction(ns, name, function);
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_GetFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function)
 *
 * Description:
 *     Get the function pointer.
 *
 * Arguments:
 *     nspace,      a pointer to a structure containing the variable and function namespace;
 *     name,        a pointer to the function name;
 *     function,    a structure containing a pointer to the function.
 *
 * Results:
 *     The function returns a pointer to the function.
 */
Gua_Status Gua_GetFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function)
{
    if (Gua_SearchFunction(nspace, name, function) == GUA_OK) {
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_SetFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function)
 *
 * Description:
 *     Search a function and set its pointer.
 *
 * Arguments:
 *     nspace,      a pointer to a structure containing the variable and function namespace;
 *     name,        a pointer to the function name;
 *     function,    a structure containing a pointer to the function.
 *
 * Results:
 *     The function set the pointer to the function.
 */
Gua_Status Gua_SetFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function)
{
    Gua_Namespace *ns;
    Gua_Function *p;
    Gua_Function *newFunction;
    Gua_Argument *args;
    Gua_Short i;
    
    ns = (Gua_Namespace *)nspace;
    
    p = ns->function[Gua_NamespaceSlot(name)];
    
    if (p) {
        while (true) {
            if (strcmp(p->name, name) == 0) {
                if (p->type == FUNCTION_TYPE_SCRIPT) {
                    if ((int)p->argc > 0) {
                        args = (Gua_Argument *)p->argv;
                        for (i = 0; i < (int)p->argc; i++) {
                            Gua_Free(Gua_ArgName(args[i]));
                            Gua_FreeObject(&(Gua_ArgObject(args[i])));
                        }
                        Gua_Free(p->argv);
                    }
                    Gua_Free(p->script);
                }
                
                Gua_SetPFunctionFromPFunction(p, function);
                
                return GUA_OK;
            }
            if (p->next) {
                p = (Gua_Function *)p->next;
            } else {
                break;
            }
        }
        
        Gua_NewFunction(newFunction, name);
        Gua_SetPFunctionFromPFunction(newFunction, function);
        
        newFunction->previous = (struct Gua_Function *)p;
        newFunction->next = NULL;
        
        p->next = (struct Gua_Function *)newFunction;
        
        return GUA_OK;
    } else {
        Gua_NewFunction(newFunction, name);
        Gua_SetPFunctionFromPFunction(newFunction, function);
        
        newFunction->previous = NULL;
        newFunction->next = NULL;
        
        ns->function[Gua_NamespaceSlot(name)] = (Gua_Function *)newFunction;
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeArray(Gua_Element *array)
 *
 * Description:
 *     Free an associative array.
 *
 * Arguments:
 *     array,    a pointer to an associative array.
 *
 * Results:
 *     The function frees the given associative array.
 */
void Gua_FreeArray(Gua_Element *array)
{
    if (array) {
        if (array->next) {
            Gua_FreeArray((Gua_Element *)array->next);
        }
        
        Gua_FreeObject(&array->key);
        Gua_FreeObject(&array->object);
        
        Gua_Free(array);
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeMatrix(Gua_Matrix *matrix)
 *
 * Description:
 *     Free a matrix.
 *
 * Arguments:
 *     matrix,    a pointer to a matrix.
 *
 * Results:
 *     The function frees the given matrix.
 */
void Gua_FreeMatrix(Gua_Matrix *matrix)
{
    Gua_Object *o;
    Gua_Integer length;
    Gua_Integer i;
    
    if (matrix) {
        length = 1;
        
        for (i = 0; i < matrix->dimc; i++) {
            length = length * matrix->dimv[i];
        }
        
        o = (Gua_Object *)matrix->object;
        
        for (i = 0; i < length; i++) {
            Gua_FreeObject(&o[i]);
        }
        
        Gua_Free(matrix->object);
        Gua_Free(matrix->dimv);
        Gua_Free(matrix);
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeHandle(Gua_Handle *handle)
 *
 * Description:
 *     Free a handle.
 *
 * Arguments:
 *     handle,    a pointer to a handle.
 *
 * Results:
 *     The function frees the given handle.
 */
void Gua_FreeHandle(Gua_Handle *handle)
{
    if (handle) {
        if (Gua_GetHandleType(handle) != NULL) {
            Gua_Free(Gua_GetHandleType(handle));
        }
        
        Gua_Free(handle);
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeObject(Gua_Object *object)
 *
 * Description:
 *     Free a string or an associative array object.
 *
 * Arguments:
 *     object,    a pointer to a object.
 *
 * Results:
 *     The function frees the given object.
 */
void Gua_FreeObject(Gua_Object *object)
{
    if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
        Gua_Free(Gua_PObjectToString(object));
    } else if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
        Gua_FreeArray((Gua_Element *)Gua_PObjectToArray(object));
    } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
        Gua_FreeMatrix((Gua_Matrix *)Gua_PObjectToMatrix(object));
    } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
        Gua_Free(Gua_PObjectToFile(object));
    } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
        Gua_FreeHandle((Gua_Handle *)Gua_PObjectToHandle(object));
    }
    
    Gua_ClearPObject(object);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_UnsetArrayElement(Gua_Object *array, Gua_Object *key)
 *
 * Description:
 *     Unset an associative array element object.
 *
 * Arguments:
 *     array,    a pointer to an associative array;
 *     key,      a pointer to a key to search for.
 *
 * Results:
 *     The function unsets the associative array element object.
 */
Gua_Status Gua_UnsetArrayElement(Gua_Object *array, Gua_Object *key)
{
    Gua_Element *element;
    Gua_Element *previous;
    Gua_Element *next;
    
    element = (Gua_Element *)Gua_PObjectToArray(array);
    
    if (element) {
        while (true) {
            if ((Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) && (Gua_PObjectType(key) == OBJECT_TYPE_STRING)) {
                if (strcmp(Gua_ObjectToString(element->key), Gua_PObjectToString(key)) == 0) {
                    previous = (Gua_Element *)element->previous;
                    next = (Gua_Element *)element->next;
                    if (previous) {
                        previous->next = (struct Gua_Element *)next;
                    }
                    if (next) {
                        next->previous = (struct Gua_Element *)previous;
                    }
                    if (!previous) {
                        if (!next) {
                            array->array = NULL;
                        } else {
                            array->array = (struct Gua_Element *)next;
                        }
                    }
                    element->previous = NULL;
                    element->next = NULL;
                    
                    Gua_FreeArray(element);
                    
                    /* Update the array length entry. */
                    
                    Gua_SetPObjectLength(array, Gua_PObjectLength(array) - 1);
                    
                    return GUA_OK;
                }
            } else if ((Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(key) == OBJECT_TYPE_INTEGER)) {
                if (Gua_ObjectToInteger(element->key) == Gua_PObjectToInteger(key)) {
                    previous = (Gua_Element *)element->previous;
                    next = (Gua_Element *)element->next;
                    if (previous) {
                        previous->next = (struct Gua_Element *)next;
                    }
                    if (next) {
                        next->previous = (struct Gua_Element *)previous;
                    }
                    if (!previous) {
                        if (!next) {
                            array->array = NULL;
                        } else {
                            array->array = (struct Gua_Element *)next;
                        }
                    }
                    element->previous = NULL;
                    element->next = NULL;
                    
                    Gua_FreeArray(element);
                    
                    /* Update the array length entry. */
                    Gua_SetPObjectLength(array, Gua_PObjectLength(array) - 1);
                    
                    return GUA_OK;
                }
            }
            if (element->next) {
                element = (Gua_Element *)element->next;
            } else {
                break;
            }
        }
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_GetArrayElement(Gua_Object *array, Gua_Object *key, Gua_Object *object)
 *
 * Description:
 *     Get an associative array element object.
 *
 * Arguments:
 *     array,     a pointer to an associative array;
 *     key,       a pointer to a key to search for;
 *     object,    a structure containing the return object of the associative array element.
 *
 * Results:
 *     The function returns the associative array element object.
 */
Gua_Status Gua_GetArrayElement(Gua_Object *array, Gua_Object *key, Gua_Object *object)
{
    Gua_Element *element;
    
    Gua_ClearPObject(object);
    
    element = (Gua_Element *)array->array;
    
    if (element) {
        while (true) {
            if ((Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) && (Gua_PObjectType(key) == OBJECT_TYPE_STRING)) {
                if (strcmp(Gua_ObjectToString(element->key), Gua_PObjectToString(key)) == 0) {
                    if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
                        Gua_ByteArrayToPObject(object, Gua_ObjectToString(element->object), Gua_ObjectLength(element->object));
                        Gua_SetStoredObject(element->object);
                    } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
                        Gua_CopyFile(object, &(element->object), false);
                        Gua_SetStoredObject(element->object);
                    } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
                        Gua_CopyHandle(object, &(element->object), false);
                        Gua_SetStoredObject(element->object);
                    } else {
                        Gua_LinkToPObject(object, element->object);
                    }
                    
                    return Gua_PObjectType(object);
                }
            } else if ((Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(key) == OBJECT_TYPE_INTEGER)) {
                if (Gua_ObjectToInteger(element->key) == Gua_PObjectToInteger(key)) {
                    if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
                        Gua_ByteArrayToPObject(object, Gua_ObjectToString(element->object), Gua_ObjectLength(element->object));
                        Gua_SetStoredObject(element->object);
                    } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
                        Gua_CopyFile(object, &(element->object), false);
                        Gua_SetStoredObject(element->object);
                    } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
                        Gua_CopyHandle(object, &(element->object), false);
                        Gua_SetStoredObject(element->object);
                    } else {
                        Gua_LinkToPObject(object, element->object);
                    }
                    
                    return Gua_PObjectType(object);
                }
            }
            if (element->next) {
                element = (Gua_Element *)element->next;
            } else {
                break;
            }
        }
    }
    
    return object->type;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_SetArrayElement(Gua_Object *array, Gua_Object *key, Gua_Object *object, Gua_Stored stored)
 *
 * Description:
 *     Set an associative array element object.
 *
 * Arguments:
 *     array,     a pointer to an associative array;
 *     key,       a pointer to a key to search for;
 *     object,    a structure containing the return object of the associative array element;
 *     stored,    if TRUE the object of the associative array element is stored in a variable.
 *
 * Results:
 *     The function sets an associative array element object.
 */
Gua_Status Gua_SetArrayElement(Gua_Object *array, Gua_Object *key, Gua_Object *object, Gua_Stored stored)
{
    Gua_Element *element;
    Gua_Element *newElement;
    
    if ((Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) || (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) || (Gua_PObjectType(object) == OBJECT_TYPE_UNKNOWN)) {
        return GUA_ERROR_ILLEGAL_ASSIGNMENT;
    }
    
    element = (Gua_Element *)Gua_PObjectToArray(array);
    
    if (element) {
        while (true) {
            if ((Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) && (Gua_PObjectType(key) == OBJECT_TYPE_STRING)) {
                if (strcmp(Gua_ObjectToString(element->key), Gua_PObjectToString(key)) == 0) {
                    if ((Gua_ObjectToString(element->object) == Gua_PObjectToString(object)) && (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING)) {
                        return GUA_OK;
                    }
                    
                    if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
                        Gua_Free(Gua_ObjectToString(element->object));
                    } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
                        Gua_Free(Gua_ObjectToFile(element->object));
                    } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
                        Gua_FreeHandle((Gua_Handle *)Gua_ObjectToHandle(element->object));
                    }
                    
                    if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_ByteArrayToObject(element->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                        } else {
                            Gua_LinkByteArrayToObject(element->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyFile(&(element->object), object, stored);
                        } else {
                            Gua_LinkFromPObject(element->object, object);
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyHandle(&(element->object), object, stored);
                        } else {
                            Gua_LinkFromPObject(element->object, object);
                            Gua_SetStoredPObject(object);
                        }
                    } else {
                        Gua_LinkFromPObject(element->object, object);
                    }
                    Gua_SetObjectStoredState(element->object, stored);
                    
                    return GUA_OK;
                }
            } else if ((Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(key) == OBJECT_TYPE_INTEGER)) {
                if (Gua_ObjectToInteger(element->key) == Gua_PObjectToInteger(key)) {
                    if ((Gua_ObjectToString(element->object) == Gua_PObjectToString(object)) && (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING)) {
                        return GUA_OK;
                    }
                    
                    if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
                        Gua_Free(Gua_ObjectToString(element->object));
                    }
                    if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
                        Gua_Free(Gua_ObjectToFile(element->object));
                    }
                    if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
                        Gua_FreeHandle((Gua_Handle *)Gua_ObjectToHandle(element->object));
                    }
                    
                    if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_ByteArrayToObject(element->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                        } else {
                            Gua_LinkByteArrayToObject(element->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyFile(&(element->object), object, stored);
                        } else {
                            Gua_LinkFromPObject(element->object, object);
                            Gua_SetStoredPObject(object);
                        }
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
                        if (Gua_IsPObjectStored(object)) {
                            Gua_CopyHandle(&(element->object), object, stored);
                        } else {
                            Gua_LinkFromPObject(element->object, object);
                            Gua_SetStoredPObject(object);
                        }
                    } else {
                        Gua_LinkFromPObject(element->object, object);
                    }
                    Gua_SetObjectStoredState(element->object, stored);
                    
                    return GUA_OK;
                }
            }
            if (element->next) {
                element = (Gua_Element *)element->next;
            } else {
                break;
            }
        }
        
        /* Create a new element. */
        newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
        
        Gua_ClearObject(newElement->key);
        Gua_ClearObject(newElement->object);
        
        /* The element key. */
        if (Gua_PObjectType(key) == OBJECT_TYPE_STRING) {
            if (Gua_IsPObjectStored(key)) {
                Gua_ByteArrayToObject(newElement->key, Gua_PObjectToString(key), Gua_PObjectLength(key));
            } else {
                Gua_LinkByteArrayToObject(newElement->key, Gua_PObjectToString(key), Gua_PObjectLength(key));
                Gua_SetStoredPObject(key);
            }
        } else {
            Gua_LinkFromPObject(newElement->key, key);
        }
        Gua_SetObjectStoredState(newElement->key, stored);
        /* The element object. */
        if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
            if (Gua_IsPObjectStored(object)) {
                Gua_ByteArrayToObject(newElement->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
            } else {
                Gua_LinkByteArrayToObject(newElement->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                Gua_SetStoredPObject(object);
            }
        } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
            if (Gua_IsPObjectStored(object)) {
                Gua_CopyFile(&(newElement->object), object, stored);
            } else {
                Gua_LinkFromPObject(newElement->object, object);
                Gua_SetStoredPObject(object);
            }
        } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
            if (Gua_IsPObjectStored(object)) {
                Gua_CopyHandle(&(newElement->object), object, stored);
            } else {
                Gua_LinkFromPObject(newElement->object, object);
                Gua_SetStoredPObject(object);
            }
        } else {
            Gua_LinkFromPObject(newElement->object, object);
        }
        Gua_SetObjectStoredState(newElement->object, stored);
        
        /* Set the array chain. */
        newElement->previous = (struct Gua_Element *)element;
        newElement->next = NULL;
        element->next = (struct Gua_Element *)newElement;
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(array, Gua_PObjectLength(array) + 1);
        
        return GUA_OK;
    } else {
        /* Create a new element. */
        newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
        
        Gua_ClearObject(newElement->key);
        Gua_ClearObject(newElement->object);
        
        /* The element key. */
        if (Gua_PObjectType(key) == OBJECT_TYPE_STRING) {
            if (Gua_IsPObjectStored(key)) {
                Gua_ByteArrayToObject(newElement->key, Gua_PObjectToString(key), Gua_PObjectLength(key));
            } else {
                Gua_LinkByteArrayToObject(newElement->key, Gua_PObjectToString(key), Gua_PObjectLength(key));
                Gua_SetStoredPObject(key);
            }
        } else {
            Gua_LinkFromPObject(newElement->key, key);
        }
        Gua_SetObjectStoredState(newElement->key, stored);
        /* The element object. */
        if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
            if (Gua_IsPObjectStored(object)) {
                Gua_ByteArrayToObject(newElement->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
            } else {
                Gua_LinkByteArrayToObject(newElement->object, Gua_PObjectToString(object), Gua_PObjectLength(object));
                Gua_SetStoredPObject(object);
            }
        } else if (Gua_PObjectType(object) == OBJECT_TYPE_FILE) {
            if (Gua_IsPObjectStored(object)) {
                Gua_CopyFile(&(newElement->object), object, stored);
            } else {
                Gua_LinkFromPObject(newElement->object, object);
                Gua_SetStoredPObject(object);
            }
        } else if (Gua_PObjectType(object) == OBJECT_TYPE_HANDLE) {
            if (Gua_IsPObjectStored(object)) {
                Gua_CopyHandle(&(newElement->object), object, stored);
            } else {
                Gua_LinkFromPObject(newElement->object, object);
                Gua_SetStoredPObject(object);
            }
        } else {
            Gua_LinkFromPObject(newElement->object, object);
        }
        Gua_SetObjectStoredState(newElement->object, stored);
        
        /* Set the array chain. */
        newElement->previous = NULL;
        newElement->next = NULL;
        
        Gua_ArrayToPObject(array, (struct Gua_Element *)newElement, 1);
        Gua_SetPObjectStoredState(array, stored);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_SetStoredArray(Gua_Object *array, Gua_Stored stored)
 *
 * Description:
 *     Set the stored status of an associative array object.
 *
 * Arguments:
 *     array,     a pointer to an associative array;
 *     stored,    if TRUE the object of the associative array is stored in a variable.
 *
 * Results:
 *     The function sets the stored status of an associative array object.
 */
void Gua_SetStoredArray(Gua_Object *array, Gua_Stored stored)
{
    Gua_Element *element;
    
    element = (Gua_Element *)Gua_PObjectToArray(array);
    
    if (element) {
        while (true) {
            Gua_SetObjectStoredState(element->key, stored);
            Gua_SetObjectStoredState(element->object, stored);
            
            if (element->next) {
                element = (Gua_Element *)element->next;
            } else {
                break;
            }
        }
    }
    
    Gua_SetPObjectStoredState(array, stored);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CopyArray(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
 *
 * Description:
 *     Copy an associative array to other.
 *
 * Arguments:
 *     target,    the target associative array;
 *     source,    the source associative array;
 *     stored,    if TRUE the object of the target associative array is stored in a variable.
 *
 * Results:
 *     The function makes a copy of an associative array to other.
 */
Gua_Status Gua_CopyArray(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
{
    Gua_Element *element;
    Gua_Element *previous;
    Gua_Element *newElement;
    
    element = (Gua_Element *)Gua_PObjectToArray(source);
    
    if (element) {
        Gua_FreeObject(target);
        
        /* Create the first element. */
        newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
        
        Gua_ClearObject(newElement->key);
        Gua_ClearObject(newElement->object);
        
        /* The element key. */
        if (Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) {
            if (Gua_IsObjectStored(element->key)) {
                Gua_ByteArrayToObject(newElement->key, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
            } else {
                Gua_LinkByteArrayToObject(newElement->key, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
                Gua_SetStoredObject(element->key);
            }
        } else {
            Gua_LinkObjects(newElement->key, element->key);
        }
        Gua_SetObjectStoredState(newElement->key, stored);
        /* The element object. */
        if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
            if (Gua_IsObjectStored(element->object)) {
                Gua_ByteArrayToObject(newElement->object, Gua_ObjectToString(element->object), Gua_ObjectLength(element->object));
            } else {
                Gua_LinkByteArrayToObject(newElement->object, Gua_ObjectToString(element->object), Gua_ObjectLength(element->object));
                Gua_SetStoredObject(element->object);
            }
        } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
            if (Gua_IsObjectStored(element->object)) {
                Gua_CopyFile(&(newElement->object), &(element->object), stored);
            } else {
                Gua_LinkObjects(newElement->object, element->object);
                Gua_SetStoredObject(element->object);
            }
        } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
            if (Gua_IsObjectStored(element->object)) {
                Gua_CopyHandle(&(newElement->object), &(element->object), stored);
            } else {
                Gua_LinkObjects(newElement->object, element->object);
                Gua_SetStoredObject(element->object);
            }
        } else {
            Gua_LinkObjects(newElement->object, element->object);
        }
        Gua_SetObjectStoredState(newElement->object, stored);
        
        /* Set the target array chain. */
        newElement->previous = NULL;
        newElement->next = NULL;
        
        /* Link the first element. */
        Gua_ArrayToPObject(target, (struct Gua_Element *)newElement, 1);
        
        previous = (Gua_Element *)Gua_PObjectToArray(target);
        
        if (element->next) {
            element = (Gua_Element *)element->next;
            
            while (true) {
                /* Create a new element. */
                newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                
                Gua_ClearObject(newElement->key);
                Gua_ClearObject(newElement->object);
                
                /* The element key. */
                if (Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) {
                    if (Gua_IsObjectStored(element->key)) {
                        Gua_ByteArrayToObject(newElement->key, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
                    } else {
                        Gua_LinkByteArrayToObject(newElement->key, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
                        Gua_SetStoredObject(element->key);
                    }
                } else {
                    Gua_LinkObjects(newElement->key, element->key);
                }
                Gua_SetObjectStoredState(newElement->key, stored);
                /* The element object. */
                if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
                    if (Gua_IsObjectStored(element->object)) {
                        Gua_ByteArrayToObject(newElement->object, Gua_ObjectToString(element->object), Gua_ObjectLength(element->object));
                    } else {
                        Gua_LinkByteArrayToObject(newElement->object, Gua_ObjectToString(element->object), Gua_ObjectLength(element->object));
                        Gua_SetStoredObject(element->object);
                    }
                } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
                    if (Gua_IsObjectStored(element->object)) {
                        Gua_CopyFile(&(newElement->object), &(element->object), stored);
                    } else {
                        Gua_LinkObjects(newElement->object, element->object);
                        Gua_SetStoredObject(element->object);
                    }
                } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
                    if (Gua_IsObjectStored(element->object)) {
                        Gua_CopyHandle(&(newElement->object), &(element->object), stored);
                    } else {
                        Gua_LinkObjects(newElement->object, element->object);
                        Gua_SetStoredObject(element->object);
                    }
                } else {
                    Gua_LinkObjects(newElement->object, element->object);
                }
                Gua_SetObjectStoredState(newElement->object, stored);
                
                /* Set the target array chain. */
                newElement->previous = (struct Gua_Element *)previous;
                newElement->next = NULL;
                
                previous->next = (struct Gua_Element *)newElement;
                previous = newElement;
                
                if (element->next) {
                    element = (Gua_Element *)element->next;
                } else {
                    break;
                }
            }
        }
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(target, Gua_PObjectLength(source));
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_GetArrayKeys(Gua_Object *keys, Gua_Object *array)
 *
 * Description:
 *     Copy all associative array keys to a new array.
 *
 * Arguments:
 *     keys,     the target associative array;
 *     array,    the source associative array.
 *
 * Results:
 *     The function returns all associative array keys in a new one.
 */
Gua_Status Gua_GetArrayKeys(Gua_Object *keys, Gua_Object *array)
{
    Gua_Element *element;
    Gua_Element *previous;
    Gua_Element *newElement;
    Gua_Short i;
    
    element = (Gua_Element *)Gua_PObjectToArray(array);
    
    if (element) {
        Gua_FreeObject(keys);
        
        i = 0;
        
        /* Create the first element. */
        newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
        /* The element key. */
        Gua_IntegerToObject(newElement->key, i);
        /* The element object. */
        if (Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) {
            if (Gua_IsObjectStored(element->key)) {
                Gua_ByteArrayToObject(newElement->object, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
            } else {
                Gua_LinkByteArrayToObject(newElement->object, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
                Gua_SetStoredObject(element->key);
            }
        } else {
            Gua_LinkObjects(newElement->object, element->key);
        }
        
        /* Set the target array chain. */
        newElement->previous = NULL;
        newElement->next = NULL;
        
        /* Link the first element. */
        Gua_ArrayToPObject(keys, (struct Gua_Element *)newElement, 1);
        
        previous = (Gua_Element *)Gua_PObjectToArray(keys);
        
        if (element->next) {
            element = (Gua_Element *)element->next;
            
            while (true) {
                i++;
                
                /* Create the first element. */
                newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                /* The element key. */
                Gua_IntegerToObject(newElement->key, i);
                /* The element object. */
                if (Gua_ObjectType(element->key) == OBJECT_TYPE_STRING) {
                    if (Gua_IsObjectStored(element->key)) {
                        Gua_ByteArrayToObject(newElement->object, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
                    } else {
                        Gua_LinkByteArrayToObject(newElement->object, Gua_ObjectToString(element->key), Gua_ObjectLength(element->key));
                        Gua_SetStoredObject(element->key);
                    }
                } else {
                    Gua_LinkObjects(newElement->object, element->key);
                }
                
                /* Set the target array chain. */
                newElement->previous = (struct Gua_Element *)previous;
                newElement->next = NULL;
                
                previous->next = (struct Gua_Element *)newElement;
                previous = newElement;
                
                if (element->next) {
                    element = (Gua_Element *)element->next;
                } else {
                    break;
                }
            }
        }
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(keys, Gua_PObjectLength(array));
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ArrayToString(Gua_Object *array, Gua_Object *object)
 *
 * Description:
 *     Convert an associative array object to a string object.
 *
 * Arguments:
 *     array,     a pointer to an associative array;
 *     object,    a pointer to a string object.
 *
 * Results:
 *     The function converts an associative array object to a string object.
 */
Gua_Status Gua_ArrayToString(Gua_Object *array, Gua_Object *object)
{
    Gua_Element *element;
    Gua_String string;
    Gua_String newString;
    Gua_String p;
    Gua_Length length;
    Gua_Integer i;
    Gua_Integer j;
    
    Gua_ClearPObject(object);
    
    string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    length = EXPRESSION_SIZE;
    i = 0;
    
    element = (Gua_Element *)Gua_PObjectToArray(array);
    
    if (element) {
        /* Open the brace. */
        string[i] = '{';
        i++;
        
        /* Fill the string with a representation of the array. */
        while (true) {
            if (Gua_ObjectType(element->object) == OBJECT_TYPE_INTEGER) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "%ld", Gua_ObjectToInteger(element->object));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_REAL) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                if (Gua_ObjectToReal(element->object) >= ZERO) {
                    sprintf(p, "%g", Gua_ObjectToReal(element->object));
                } else {
                    sprintf(p, "%g", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_COMPLEX) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                if (Gua_ObjectToReal(element->object) >= ZERO) {
                    sprintf(p, "%g", Gua_ObjectToReal(element->object));
                } else {
                    sprintf(p, "%g", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                if (Gua_ObjectToImaginary(element->object) >= ZERO) {
                    sprintf(p, "%+g*i", Gua_ObjectToImaginary(element->object));
                } else {
                    sprintf(p, "%+g*i", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) {
                p = Gua_ObjectToString(element->object);
                string[i] = '"';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
                while (*p) {
                    if (*p == '"') {
                        if (i == (length - 1)) {
                            length = length * 2;
                            newString = (char *)Gua_Realloc(string, length);
                            
                            if (newString == NULL) {
                                Gua_Free(string);
                                return GUA_ERROR;
                            } else {
                                string = newString;
                            }
                        }
                        string[i] = '\\';
                        i++;
                    }
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = *p;
                    i++;
                    p++;
                }
                string[i] = '"';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
            } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_FILE) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "File%lu", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(element->object)));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(element->object) == OBJECT_TYPE_HANDLE) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "%s%lu", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(element->object)), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(element->object)));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else {
                Gua_Free(string);
                return GUA_ERROR;
            }
            
            if (element->next) {
                element = (Gua_Element *)element->next;
                string[i] = ',';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
            } else {
                break;
            }
        }
        
        /* Close the brace. */
        string[i] = '}';
        i++;
        if (i == (length - 1)) {
            length = length * 2;
            newString = (char *)Gua_Realloc(string, length);
            
            if (newString == NULL) {
                Gua_Free(string);
                return GUA_ERROR;
            } else {
                string = newString;
            }
        }
        string[i] = '\0';
        
        /* Set the string object. */
        Gua_StringToPObject(object, string);
        
        Gua_Free(string);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ArgsToString(Gua_Short argc, Gua_Object *argv, Gua_Object *object)
 *
 * Description:
 *     Convert an array of arguments to a string object.
 *
 * Arguments:
 *     argc,      the number of arguments;
 *     argv,      an array containing the arguments;
 *                argv[0] is the function name;
 *     object,    a pointer to a string object.
 *
 * Results:
 *     The function converts an array of arguments to a string object.
 */
Gua_Status Gua_ArgsToString(Gua_Short argc, Gua_Object *argv, Gua_Object *object)
{
    Gua_String string;
    Gua_String newString;
    Gua_String p;
    Gua_Length length;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Short k;
    
    Gua_ClearPObject(object);
    
    string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    length = EXPRESSION_SIZE;
    i = 0;
    
    if (argc > 0) {
        /* Fill the string with a representation of the arguments. */
        for (k = 1; k < argc; k++) {
            if (Gua_ObjectType(argv[k]) == OBJECT_TYPE_INTEGER) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "%ld", Gua_ObjectToInteger(argv[k]));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(argv[k]) == OBJECT_TYPE_REAL) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                if (fabs(Gua_ObjectToReal(argv[k])) >= ZERO) {
                    sprintf(p, "%g", Gua_ObjectToReal(argv[k]));
                } else {
                    sprintf(p, "%g", 0.0);
                }
                sprintf(p, "%g", Gua_ObjectToReal(argv[k]));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(argv[k]) == OBJECT_TYPE_COMPLEX) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                if (fabs(Gua_ObjectToReal(argv[k])) >= ZERO) {
                    sprintf(p, "%g", Gua_ObjectToReal(argv[k]));
                } else {
                    sprintf(p, "%g", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                if (fabs(Gua_ObjectToImaginary(argv[k])) >= ZERO) {
                    sprintf(p, "%+g*i", Gua_ObjectToImaginary(argv[k]));
                } else {
                    sprintf(p, "%+g*i", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(argv[k]) == OBJECT_TYPE_STRING) {
                p = Gua_ObjectToString(argv[k]);
                string[i] = '"';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
                while (*p) {
                    if (*p == '"') {
                        if (i == (length - 1)) {
                            length = length * 2;
                            newString = (char *)Gua_Realloc(string, length);
                            
                            if (newString == NULL) {
                                Gua_Free(string);
                                return GUA_ERROR;
                            } else {
                                string = newString;
                            }
                        }
                        string[i] = '\\';
                        i++;
                    }
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = *p;
                    i++;
                    p++;
                }
                string[i] = '"';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
            } else if (Gua_ObjectType(argv[k]) == OBJECT_TYPE_FILE) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "File%lu", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(argv[k])));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(argv[k]) == OBJECT_TYPE_HANDLE) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "%s%lu", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(argv[k])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(argv[k])));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else {
                Gua_Free(string);
                return GUA_ERROR;
            }
            
            if (k < (argc - 1)) {
                string[i] = ',';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
            }
        }
        
        if (i == (length - 1)) {
            length = length * 2;
            newString = (char *)Gua_Realloc(string, length);
            
            if (newString == NULL) {
                Gua_Free(string);
                return GUA_ERROR;
            } else {
                string = newString;
            }
        }
        string[i] = '\0';
        
        /* Set the string object. */
        Gua_LinkStringToPObject(object, string);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_IsArrayEqual(Gua_Object *a, Gua_Object *b)
 *
 * Description:
 *     Compare two associative arrays.
 *
 * Arguments:
 *     a,    an associative array;
 *     b,    an associative array.
 *
 * Results:
 *     The function returns 1 if the arrays are equivalent, 0 otherwise.
 */
Gua_Status Gua_IsArrayEqual(Gua_Object *a, Gua_Object *b)
{
    Gua_Element *element1;
    Gua_Element *element2;
    
    if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
        return 0;
    }
    
    element1 = (Gua_Element *)Gua_PObjectToArray(a);
    element2 = (Gua_Element *)Gua_PObjectToArray(b);
    
    if (element1 && element2) {
        while (true) {
            if (Gua_ObjectType(element1->key) == Gua_ObjectType(element2->key)) {
                if (Gua_ObjectType(element1->key) == OBJECT_TYPE_INTEGER) {
                    if (Gua_ObjectToInteger(element1->key) != Gua_ObjectToInteger(element2->key)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->key) == OBJECT_TYPE_REAL) {
                    if (Gua_ObjectToReal(element1->key) != Gua_ObjectToReal(element2->key)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->key) == OBJECT_TYPE_COMPLEX) {
                    if (Gua_ObjectToReal(element1->key) != Gua_ObjectToReal(element2->key)) {
                        return 0;
                    }
                    if (Gua_ObjectToImaginary(element1->key) != Gua_ObjectToImaginary(element2->key)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->key) == OBJECT_TYPE_STRING) {
                    if (Gua_ObjectLength(element1->key) == Gua_ObjectLength(element2->key)) {
                        if (memcmp(Gua_ObjectToString(element1->key), Gua_ObjectToString(element2->key), Gua_ObjectLength(element1->key)) != 0) {
                            return 0;
                        }
                    } else {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->key) == OBJECT_TYPE_FILE) {
                    if (Gua_ObjectToFile(element1->key) != Gua_ObjectToFile(element2->key)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->key) == OBJECT_TYPE_HANDLE) {
                    if (Gua_ObjectToHandle(element1->key) != Gua_ObjectToHandle(element2->key)) {
                        return 0;
                    }
                }
            } else {
                return 0;
            }
            if (Gua_ObjectType(element1->object) == Gua_ObjectType(element2->object)) {
                if (element1->object.type == OBJECT_TYPE_INTEGER) {
                    if (Gua_ObjectToInteger(element1->object) != Gua_ObjectToInteger(element2->object)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->object) == OBJECT_TYPE_REAL) {
                    if (Gua_ObjectToReal(element1->object) != Gua_ObjectToReal(element2->object)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->object) == OBJECT_TYPE_COMPLEX) {
                    if (Gua_ObjectToReal(element1->object) != Gua_ObjectToReal(element2->object)) {
                        return 0;
                    }
                    if (Gua_ObjectToImaginary(element1->object) != Gua_ObjectToImaginary(element2->object)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->object) == OBJECT_TYPE_STRING) {
                    if (Gua_ObjectLength(element1->object) == Gua_ObjectLength(element2->object)) {
                        if (memcmp(Gua_ObjectToString(element1->object), Gua_ObjectToString(element2->object), Gua_ObjectLength(element1->object)) != 0) {
                            return 0;
                        }
                    } else {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->object) == OBJECT_TYPE_FILE) {
                    if (Gua_ObjectToFile(element1->object) != Gua_ObjectToFile(element2->object)) {
                        return 0;
                    }
                } else if (Gua_ObjectType(element1->object) == OBJECT_TYPE_HANDLE) {
                    if (Gua_ObjectToHandle(element1->object) != Gua_ObjectToHandle(element2->object)) {
                        return 0;
                    }
                }
            } else {
                return 0;
            }
            
            if (element1->next) {
                element1 = (Gua_Element *)element1->next;
            } else {
                break;
            }
            if (element2->next) {
                element2 = (Gua_Element *)element2->next;
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_SetStoredMatrix(Gua_Object *matrix, Gua_Stored stored)
 *
 * Description:
 *     Set the stored status of a matrix object.
 *
 * Arguments:
 *     matrix,    a pointer to a matrix;
 *     stored,    if TRUE the object of the matrix is stored in a variable.
 *
 * Results:
 *     The function sets the stored status of a matrix object.
 */
void Gua_SetStoredMatrix(Gua_Object *matrix, Gua_Stored stored)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Integer i;
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(matrix);
    
    if (m) {
        o = (Gua_Object *)m->object;
        
        for (i = 0; i < Gua_PObjectLength(matrix); i++) {
            Gua_SetObjectStoredState(o[i], stored);
        }
    }
    
    Gua_SetPObjectStoredState(matrix, stored);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CopyMatrix(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
 *
 * Description:
 *     Copy a matrix to other.
 *
 * Arguments:
 *     target,    the target matrix;
 *     source,    the source matrix;
 *     stored,    if TRUE the object of the target matrix is stored in a variable.
 *
 * Results:
 *     The function makes a copy of a matrix to other.
 */
Gua_Status Gua_CopyMatrix(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
{
    Gua_Matrix *mt;
    Gua_Matrix *ms;
    Gua_Object *ot;
    Gua_Object *os;
    Gua_Integer i;
    
    ms = (Gua_Matrix *)Gua_PObjectToMatrix(source);
    
    if (ms) {
        if (!Gua_IsPObjectStored(target)) {
            Gua_FreeObject(target);
        } else {
            Gua_ClearPObject(target);
        }
        
        os = (Gua_Object *)ms->object;
        
        mt = (Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix));
        
        mt->dimc = ms->dimc;
        mt->dimv = Gua_Alloc(mt->dimc * sizeof(Gua_Integer));
        
        for (i = 0; i < mt->dimc; i++) {
            mt->dimv[i] = ms->dimv[i];
        }
        
        mt->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(source) * sizeof(Gua_Object));
        ot = (Gua_Object *)mt->object;
        
        for (i = 0; i < Gua_PObjectLength(source); i++) {
            Gua_ClearObject(ot[i]);
            
            if (Gua_ObjectType(os[i]) == OBJECT_TYPE_STRING) {
                if (Gua_IsObjectStored(os[i])) {
                    Gua_ByteArrayToObject(ot[i], Gua_ObjectToString(os[i]), Gua_ObjectLength(os[i]));
                } else {
                    Gua_LinkObjects(ot[i], os[i]);
                    Gua_SetStoredObject(os[i]);
                }
            } else if (Gua_ObjectType(os[i]) == OBJECT_TYPE_FILE) {
                if (Gua_IsObjectStored(os[i])) {
                    Gua_CopyFile(&(ot[i]), &(os[i]), stored);
                } else {
                    Gua_LinkObjects(ot[i], os[i]);
                    Gua_SetStoredObject(os[i]);
                }
            } else if (Gua_ObjectType(os[i]) == OBJECT_TYPE_HANDLE) {
                if (Gua_IsObjectStored(os[i])) {
                    Gua_CopyHandle(&(ot[i]), &(os[i]), stored);
                } else {
                    Gua_LinkObjects(ot[i], os[i]);
                    Gua_SetStoredObject(os[i]);
                }
            } else {
                Gua_LinkObjects(ot[i], os[i]);
            }
            
            Gua_SetObjectStoredState(ot[i], stored);
        }
        
        /* Fix the target matrix fields. */
        Gua_MatrixToPObject(target, (struct Gua_Matrix *)mt, Gua_PObjectLength(source));
        Gua_SetPObjectStoredState(target, stored);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_GetMatrixDim(Gua_Object *array, Gua_Object *matrix)
 *
 * Description:
 *     Copy all matrix dimensions to an associative array.
 *
 * Arguments:
 *     array,     the target associative array;
 *     matrix,    the source matrix.
 *
 * Results:
 *     The function returns all matrix dimensions in an associative array.
 */
Gua_Status Gua_GetMatrixDim(Gua_Object *array, Gua_Object *matrix)
{
    Gua_Matrix *m;
    Gua_Element *previous;
    Gua_Element *newElement;
    Gua_Short i;
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(matrix);
    
    if (m) {
        Gua_FreeObject(array);
        
        i = 0;
        
        /* Create the first element. */
        newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
        /* The element key. */
        Gua_IntegerToObject(newElement->key, i);
        /* The element object. */
        Gua_IntegerToObject(newElement->object, m->dimv[i]);
        
        /* Set the target array chain. */
        newElement->previous = NULL;
        newElement->next = NULL;
        
        /* Link the first element. */
        Gua_ArrayToPObject(array, (struct Gua_Element *)newElement, 1);
            
        previous = (Gua_Element *)Gua_PObjectToArray(array);
        
        if (m->dimc > 1) {
            for (i = 1; i < m->dimc; i++) {
                /* Create a new element. */
                newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                /* The element key. */
                Gua_IntegerToObject(newElement->key, i);
                /* The element object. */
                Gua_IntegerToObject(newElement->object, m->dimv[i]);
                
                /* Set the target array chain. */
                newElement->previous = (struct Gua_Element *)previous;
                newElement->next = NULL;
                
                previous->next = (struct Gua_Element *)newElement;
                previous = newElement;
            }
        }
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(array, m->dimc);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_MatrixToString(Gua_Object *matrix, Gua_Object *object)
 *
 * Description:
 *     Convert a matrix object to a string object.
 *
 * Arguments:
 *     matrix,    a pointer to a matrix;
 *     object,    a pointer to a string object.
 *
 * Results:
 *     The function converts a matrix object to a string object.
 */
Gua_Status Gua_MatrixToString(Gua_Object *matrix, Gua_Object *object)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_String string;
    Gua_String newString;
    Gua_String p;
    Gua_Length length;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer c;
    
    Gua_ClearPObject(object);
    
    string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    length = EXPRESSION_SIZE;
    i = 0;
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(matrix);
    
    if (m) {
        /* Open the bracket. */
        string[i] = '[';
        i++;
        
        o = (Gua_Object *)m->object;
        c = 0;
        
        /* Fill the string with a representation of the matrix. */
        for (k = 0; k < Gua_PObjectLength(matrix); k++) {
            if (Gua_ObjectType(o[k]) == OBJECT_TYPE_INTEGER) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "%ld", Gua_ObjectToInteger(o[k]));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(o[k]) == OBJECT_TYPE_REAL) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                if (fabs(Gua_ObjectToReal(o[k])) >= ZERO) {
                    sprintf(p, "%g", Gua_ObjectToReal(o[k]));
                } else {
                    sprintf(p, "%g", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(o[k]) == OBJECT_TYPE_COMPLEX) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                if (fabs(Gua_ObjectToReal(o[k])) >= ZERO) {
                    sprintf(p, "%g", Gua_ObjectToReal(o[k]));
                } else {
                    sprintf(p, "%g", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                if (fabs(Gua_ObjectToImaginary(o[k])) >= ZERO) {
                    sprintf(p, "%+g*i", Gua_ObjectToImaginary(o[k]));
                } else {
                    sprintf(p, "%+g*i", 0.0);
                }
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(o[k]) == OBJECT_TYPE_STRING) {
                p = Gua_ObjectToString(o[k]);
                string[i] = '"';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
                while (*p) {
                    if (*p == '"') {
                        if (i == (length - 1)) {
                            length = length * 2;
                            newString = (char *)Gua_Realloc(string, length);
                            
                            if (newString == NULL) {
                                Gua_Free(string);
                                return GUA_ERROR;
                            } else {
                                string = newString;
                            }
                        }
                        string[i] = '\\';
                        i++;
                    }
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = *p;
                    i++;
                    p++;
                }
                string[i] = '"';
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
            } else if (Gua_ObjectType(o[k]) == OBJECT_TYPE_FILE) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "File%lu", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(o[k])));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else if (Gua_ObjectType(o[k]) == OBJECT_TYPE_HANDLE) {
                p = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(p, "%s%lu", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(o[k])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(o[k])));
                j = 0;
                while (p[j]) {
                    if (i == (length - 1)) {
                        length = length * 2;
                        newString = (char *)Gua_Realloc(string, length);
                        
                        if (newString == NULL) {
                            Gua_Free(p);
                            Gua_Free(string);
                            return GUA_ERROR;
                        } else {
                            string = newString;
                        }
                    }
                    string[i] = p[j];
                    i++;
                    j++;
                }
                Gua_Free(p);
            } else {
                Gua_Free(string);
                return GUA_ERROR;
            }
            
            if (k < (Gua_PObjectLength(matrix) - 1)) {
                if (m->dimc == 2) {
                    if (c < (m->dimv[1] - 1)) {
                        string[i] = ',';
                        c++;
                    } else {
                        string[i] = ';';
                        c = 0;
                    }
                } else {
                    string[i] = ',';
                }
                i++;
                if (i == (length - 1)) {
                    length = length * 2;
                    newString = (char *)Gua_Realloc(string, length);
                    
                    if (newString == NULL) {
                        Gua_Free(string);
                        return GUA_ERROR;
                    } else {
                        string = newString;
                    }
                }
            }
        }
        
        /* Close the bracket. */
        string[i] = ']';
        i++;
        if (i == (length - 1)) {
            length = length * 2;
            newString = (char *)Gua_Realloc(string, length);
            
            if (newString == NULL) {
                Gua_Free(string);
                return GUA_ERROR;
            } else {
                string = newString;
            }
        }
        string[i] = '\0';
        
        /* Set the string object. */
        Gua_StringToPObject(object, string);
        
        Gua_Free(string);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_IsMatrixEqual(Gua_Object *a, Gua_Object *b)
 *
 * Description:
 *     Compare two matrices.
 *
 * Arguments:
 *     a,    a matrix;
 *     b,    a matrix.
 *
 * Results:
 *     The function returns 1 if the matrices are equivalent, 0 otherwise.
 */
Gua_Status Gua_IsMatrixEqual(Gua_Object *a, Gua_Object *b)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Integer i;
    
    if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
        return 0;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (m1->dimc != m2->dimc) {
            return 0;
        }
        
        for (i = 0; i < m1->dimc; i++) {
            if (m1->dimv[i] != m2->dimv[i]) {
                return 0;
            }
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                if (Gua_ObjectToInteger(o1[i]) != Gua_ObjectToInteger(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                if (Gua_ObjectToReal(o1[i]) != Gua_ObjectToReal(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                if (Gua_ObjectToInteger(o1[i]) != Gua_ObjectToReal(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                if (Gua_ObjectToReal(o1[i]) != Gua_ObjectToInteger(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                if (Gua_ObjectToReal(o1[i]) != Gua_ObjectToReal(o2[i])) {
                    return 0;
                }
                if (Gua_ObjectToImaginary(o1[i]) != Gua_ObjectToImaginary(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_STRING) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(o1[i]) == Gua_ObjectLength(o2[i])) {
                    if (memcmp(Gua_ObjectToString(o1[i]), Gua_ObjectToString(o2[i]), Gua_ObjectLength(o1[i])) != 0) {
                        return 0;
                    }
                } else {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_FILE) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_FILE)) {
                if (Gua_ObjectToFile(o1[i]) != Gua_ObjectToFile(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_HANDLE) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_HANDLE)) {
                if (Gua_ObjectToHandle(o1[i]) != Gua_ObjectToHandle(o2[i])) {
                    return 0;
                }
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_IsMatrixApproximatelyEqual(Gua_Object *a, Gua_Object *b, Gua_Object *c)
 *
 * Description:
 *     Compare two matrices.
 *
 * Arguments:
 *     a,    a matrix;
 *     b,    a matrix;
 *     c,    tolerance.
 *
 * Results:
 *     The function returns 1 if the matrices are equivalent, 0 otherwise.
 */
Gua_Status Gua_IsMatrixApproximatelyEqual(Gua_Object *a, Gua_Object *b, Gua_Object *c)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Integer i;
    
    if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
        return 0;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (m1->dimc != m2->dimc) {
            return 0;
        }
        
        for (i = 0; i < m1->dimc; i++) {
            if (m1->dimv[i] != m2->dimv[i]) {
                return 0;
            }
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                if (Gua_PObjectType(c) == OBJECT_TYPE_INTEGER) {
                    if ((Gua_ObjectToInteger(o1[i]) < (Gua_ObjectToInteger(o2[i]) - Gua_PObjectToInteger(c))) || (Gua_ObjectToInteger(o1[i]) > (Gua_ObjectToInteger(o2[i]) + Gua_PObjectToInteger(c)))) {
                        return 0;
                    }
                }
                if (Gua_PObjectType(c) == OBJECT_TYPE_REAL) {
                    if ((Gua_ObjectToInteger(o1[i]) < (Gua_ObjectToInteger(o2[i]) - Gua_PObjectToReal(c))) || (Gua_ObjectToInteger(o1[i]) > (Gua_ObjectToInteger(o2[i]) + Gua_PObjectToReal(c)))) {
                        return 0;
                    }
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                if (Gua_PObjectType(c) == OBJECT_TYPE_INTEGER) {
                    if ((Gua_ObjectToReal(o1[i]) < (Gua_ObjectToReal(o2[i]) - Gua_PObjectToInteger(c))) || (Gua_ObjectToReal(o1[i]) > (Gua_ObjectToReal(o2[i]) + Gua_PObjectToInteger(c)))) {
                        return 0;
                    }
                }
                if (Gua_PObjectType(c) == OBJECT_TYPE_REAL) {
                    if ((Gua_ObjectToReal(o1[i]) < (Gua_ObjectToReal(o2[i]) - Gua_PObjectToReal(c))) || (Gua_ObjectToReal(o1[i]) > (Gua_ObjectToReal(o2[i]) + Gua_PObjectToReal(c)))) {
                        return 0;
                    }
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                if (Gua_PObjectType(c) == OBJECT_TYPE_INTEGER) {
                    if ((Gua_ObjectToInteger(o1[i]) < (Gua_ObjectToReal(o2[i]) - Gua_PObjectToInteger(c))) || (Gua_ObjectToInteger(o1[i]) > (Gua_ObjectToReal(o2[i]) + Gua_PObjectToInteger(c)))) {
                        return 0;
                    }
                }
                if (Gua_PObjectType(c) == OBJECT_TYPE_REAL) {
                    if ((Gua_ObjectToInteger(o1[i]) < (Gua_ObjectToReal(o2[i]) - Gua_PObjectToReal(c))) || (Gua_ObjectToInteger(o1[i]) > (Gua_ObjectToReal(o2[i]) + Gua_PObjectToReal(c)))) {
                        return 0;
                    }
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                if (Gua_PObjectType(c) == OBJECT_TYPE_INTEGER) {
                    if ((Gua_ObjectToReal(o1[i]) < (Gua_ObjectToInteger(o2[i]) - Gua_PObjectToInteger(c))) || (Gua_ObjectToReal(o1[i]) > (Gua_ObjectToInteger(o2[i]) + Gua_PObjectToInteger(c)))) {
                        return 0;
                    }
                }
                if (Gua_PObjectType(c) == OBJECT_TYPE_REAL) {
                    if ((Gua_ObjectToReal(o1[i]) < (Gua_ObjectToInteger(o2[i]) - Gua_PObjectToReal(c))) || (Gua_ObjectToReal(o1[i]) > (Gua_ObjectToInteger(o2[i]) + Gua_PObjectToReal(c)))) {
                        return 0;
                    }
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                if (Gua_PObjectType(c) == OBJECT_TYPE_REAL) {
                    if ((Gua_ObjectToReal(o1[i]) < (Gua_ObjectToReal(o2[i]) - Gua_PObjectToReal(c))) || (Gua_ObjectToReal(o1[i]) > (Gua_ObjectToReal(o2[i]) + Gua_PObjectToReal(c)))) {
                        if ((Gua_ObjectToImaginary(o1[i]) < (Gua_ObjectToImaginary(o2[i]) - Gua_PObjectToReal(c)))|| (Gua_ObjectToImaginary(o1[i]) > (Gua_ObjectToImaginary(o2[i]) + Gua_PObjectToReal(c)))) {
                            return 0;
                        }
                    } else {
                        return 0;
                    }
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_STRING) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(o1[i]) == Gua_ObjectLength(o2[i])) {
                    if (memcmp(Gua_ObjectToString(o1[i]), Gua_ObjectToString(o2[i]), Gua_ObjectLength(o1[i])) != 0) {
                        return 0;
                    }
                } else {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_FILE) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_FILE)) {
                if (Gua_ObjectToFile(o1[i]) != Gua_ObjectToFile(o2[i])) {
                    return 0;
                }
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_HANDLE) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_HANDLE)) {
                if (Gua_ObjectToHandle(o1[i]) != Gua_ObjectToHandle(o2[i])) {
                    return 0;
                }
            }
        }
    } else {
        return 0;
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_AddMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Add two matrices.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a matrix;
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix C = A + B.
 */
Gua_Status Gua_AddMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Integer i;
    Gua_String errMessage;
    
    if (!((Gua_PObjectType(a) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have the same size");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        for (i = 0; i < m1->dimc; i++) {
            if (m1->dimv[i] != m2->dimv[i]) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "the matrices do not have the same dimensions");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
        
        m3->dimc = m1->dimc;
        
        m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
        
        for (i = 0; i < m3->dimc; i++) {
            m3->dimv[i] = m1->dimv[i];
        }
        
        m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        o3 = (Gua_Object *)m3->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) + Gua_ObjectToInteger(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_RealToObject(o3[i], Gua_ObjectToReal(o1[i]) + Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) + Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o1[i]) + Gua_ObjectToImaginary(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_RealToObject(o3[i], Gua_ObjectToInteger(o1[i]) + Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToObject(o3[i], Gua_ObjectToReal(o1[i]) + Gua_ObjectToInteger(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToInteger(o1[i]) + Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) + Gua_ObjectToInteger(o2[i]), Gua_ObjectToImaginary(o1[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) + Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) + Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o1[i]));
            }
            /* Fix complex numbers converting it to real if the imaginary part is zero. */
            if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) {
                if (Gua_ObjectToImaginary(o3[i]) == 0) {
                    Gua_SetObjectType(o3[i], OBJECT_TYPE_REAL)
                }
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_SubMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Subtract two matrices.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a matrix;
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix C = A - B.
 */
Gua_Status Gua_SubMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Integer i;
    Gua_String errMessage;
    
    if (!((Gua_PObjectType(a) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have the same size");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        for (i = 0; i < m1->dimc; i++) {
            if (m1->dimv[i] != m2->dimv[i]) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "the matrices do not have the same dimensions");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
        
        m3->dimc = m1->dimc;
        
        m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
        
        for (i = 0; i < m3->dimc; i++) {
            m3->dimv[i] = m1->dimv[i];
        }
        
        m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        o3 = (Gua_Object *)m3->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) - Gua_ObjectToInteger(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_RealToObject(o3[i], Gua_ObjectToReal(o1[i]) - Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) - Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o1[i]) - Gua_ObjectToImaginary(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_RealToObject(o3[i], Gua_ObjectToInteger(o1[i]) - Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToObject(o3[i], Gua_ObjectToReal(o1[i]) - Gua_ObjectToInteger(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToInteger(o1[i]) - Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) - Gua_ObjectToInteger(o2[i]), Gua_ObjectToImaginary(o1[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) - Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) - Gua_ObjectToReal(o2[i]), Gua_ObjectToImaginary(o1[i]));
            }
            /* Fix complex numbers converting it to real if the imaginary part is zero. */
            if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) {
                if (Gua_ObjectToImaginary(o3[i]) == 0) {
                    Gua_SetObjectType(o3[i], OBJECT_TYPE_REAL)
                }
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_NegMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
 *
 * Description:
 *     Calculate the negative of a matrix.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix B = -A.
 */
Gua_Status Gua_NegMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Integer i;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if (!Gua_IsPObjectStored(b)) {
            Gua_FreeObject(b);
        } else {
            Gua_ClearPObject(b);
        }
        
        o1 = (Gua_Object *)m1->object;
        
        Gua_MatrixToPObject(b, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        
        m2->dimc = m1->dimc;
        m2->dimv = Gua_Alloc(m2->dimc * sizeof(Gua_Integer));
        
        for (i = 0; i < m2->dimc; i++) {
            m2->dimv[i] = m1->dimv[i];
        }
        
        m2->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        o2 = (Gua_Object *)m2->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) {
                Gua_IntegerToObject(o2[i], -Gua_ObjectToInteger(o1[i]));
            } else if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) {
                Gua_RealToObject(o2[i], -Gua_ObjectToReal(o1[i]));
            } else if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) {
                Gua_ComplexToObject(o2[i], -Gua_ObjectToReal(o1[i]), -Gua_ObjectToImaginary(o1[i]));
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_MulMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Multiply two matrices.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a matrix;
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix C = A * B.
 */
Gua_Status Gua_MulMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer m;
    Gua_Integer n;
    Gua_Integer p;
    Gua_String errMessage;
    
    if (!((Gua_PObjectType(a) == OBJECT_TYPE_INTEGER) || (Gua_PObjectType(a) == OBJECT_TYPE_REAL) || (Gua_PObjectType(a) == OBJECT_TYPE_COMPLEX) || (Gua_PObjectType(a) == OBJECT_TYPE_MATRIX))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (!((Gua_PObjectType(b) == OBJECT_TYPE_INTEGER) || (Gua_PObjectType(b) == OBJECT_TYPE_REAL) || (Gua_PObjectType(b) == OBJECT_TYPE_COMPLEX) || (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand 2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if ((Gua_PObjectType(a) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX)) {
        m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        
        if (m1 && m2) {
            if (!Gua_IsPObjectStored(c)) {
                Gua_FreeObject(c);
            } else {
                Gua_ClearPObject(c);
            }
            
            if (m1->dimc > 2) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            if (m2->dimc > 2) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            if (m1->dimv[1] != m2->dimv[0]) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            o1 = (Gua_Object *)m1->object;
            o2 = (Gua_Object *)m2->object;
            
            /* A(m x n), B(n x p), C(m x p) */
            m = m1->dimv[0];
            n = m1->dimv[1];
            p = m2->dimv[1];
            
            Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), m * p);
            m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
            
            m3->dimc = 2;
            m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
            
            m3->dimv[0] = m;
            m3->dimv[1] = p;
            
            m3->object = (struct Gua_Object *)Gua_Alloc(m * p * sizeof(Gua_Object));
            o3 = (Gua_Object *)m3->object;
            
            /* Clear each element of matrix C. */ 
            for (i = 0; i < m; i++) {
                for (j = 0; j < p; j++) {
                    Gua_ClearObject(o3[i * p + j]);
                }
            }

            /* C(i,j) = sum(A(i,k) * B(k,j)) */
            for (i = 0; i < m; i++) {
                for (j = 0; j < p; j++) {
                    for (k = 0; k < n; k++) {            
                        if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_INTEGER)) {
                            Gua_IntegerToObject(o3[i * p + j], Gua_ObjectToInteger(o3[i * p + j]) + Gua_ObjectToInteger(o1[i * n + k]) * Gua_ObjectToInteger(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_REAL)) {
                            Gua_RealToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_COMPLEX)) {
                            Gua_ComplexToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + (Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]) - Gua_ObjectToImaginary(o1[i * n + k]) * Gua_ObjectToImaginary(o2[k * p + j])), Gua_ObjectToImaginary(o3[i * p + j]) + (Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToImaginary(o2[k * p + j]) + Gua_ObjectToImaginary(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j])));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_REAL)) {
                            Gua_RealToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToInteger(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_INTEGER)) {
                            Gua_RealToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToInteger(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_COMPLEX)) {
                            Gua_ComplexToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToInteger(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]), Gua_ObjectToImaginary(o3[i * p + j]) + Gua_ObjectToInteger(o1[i * n + k]) * Gua_ObjectToImaginary(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_INTEGER)) {
                            Gua_ComplexToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToInteger(o2[k * p + j]), Gua_ObjectToImaginary(o3[i * p + j]) + Gua_ObjectToImaginary(o1[i * n + k]) * Gua_ObjectToInteger(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_COMPLEX)) {
                            Gua_ComplexToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]), Gua_ObjectToImaginary(o3[i * p + j]) + Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToImaginary(o2[k * p + j]));
                        } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_REAL)) {
                            Gua_ComplexToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) + Gua_ObjectToReal(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]), Gua_ObjectToImaginary(o3[i * p + j]) + Gua_ObjectToImaginary(o1[i * n + k]) * Gua_ObjectToReal(o2[k * p + j]));
                        }
                    }
                }
            }
        }
    } else if ((Gua_PObjectType(a) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) != OBJECT_TYPE_MATRIX)) {
        m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
        
        if (m1) {
            if (!Gua_IsPObjectStored(c)) {
                Gua_FreeObject(c);
            } else {
                Gua_ClearPObject(c);
            }
            
            if (m1->dimc > 2) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            o1 = (Gua_Object *)m1->object;
            
            Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
            m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
            
            m3->dimc = m1->dimc;
            m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
            
            for (i = 0; i < m3->dimc; i++) {
                m3->dimv[i] = m1->dimv[i];
            }
            
            m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
            o3 = (Gua_Object *)m3->object;
            
            for (i = 0; i < Gua_PObjectLength(a); i++) {
                if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(b) == OBJECT_TYPE_INTEGER)) {
                    Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) * Gua_PObjectToInteger(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_PObjectType(b) == OBJECT_TYPE_REAL)) {
                    Gua_RealToObject(o3[i], Gua_ObjectToReal(o1[i]) * Gua_PObjectToReal(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_PObjectType(b) == OBJECT_TYPE_COMPLEX)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) * Gua_PObjectToReal(b) - Gua_ObjectToImaginary(o1[i]) * Gua_PObjectToImaginary(b), Gua_ObjectToReal(o1[i]) * Gua_PObjectToImaginary(b) + Gua_ObjectToImaginary(o1[i]) * Gua_PObjectToReal(b));
                    
                    if (Gua_ObjectToImaginary(o3[i]) == 0) {
                        Gua_SetObjectType(o3[i], OBJECT_TYPE_REAL)
                    }
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(b) == OBJECT_TYPE_REAL)) {
                    Gua_RealToObject(o3[i], Gua_ObjectToInteger(o1[i]) * Gua_PObjectToReal(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_PObjectType(b) == OBJECT_TYPE_INTEGER)) {
                    Gua_RealToObject(o3[i], Gua_ObjectToReal(o1[i]) * Gua_PObjectToInteger(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(b) == OBJECT_TYPE_COMPLEX)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToInteger(o1[i]) * Gua_PObjectToReal(b), Gua_ObjectToInteger(o1[i]) * Gua_PObjectToImaginary(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_PObjectType(b) == OBJECT_TYPE_INTEGER)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) * Gua_PObjectToInteger(b), Gua_ObjectToImaginary(o1[i]) * Gua_PObjectToInteger(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_PObjectType(b) == OBJECT_TYPE_COMPLEX)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) * Gua_PObjectToReal(b), Gua_ObjectToReal(o1[i]) * Gua_PObjectToImaginary(b));
                } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_COMPLEX) && (Gua_PObjectType(b) == OBJECT_TYPE_REAL)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o1[i]) * Gua_PObjectToReal(b), Gua_ObjectToImaginary(o1[i]) * Gua_PObjectToReal(b));
                }
            }
        }
    } else if ((Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX)) {
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        
        if (m2) {
            if (!Gua_IsPObjectStored(c)) {
                Gua_FreeObject(c);
            } else {
                Gua_ClearPObject(c);
            }
            
            if (m2->dimc > 2) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            o2 = (Gua_Object *)m2->object;
            
            Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(b));
            m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
            
            m3->dimc = m2->dimc;
            m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
            
            for (i = 0; i < m3->dimc; i++) {
                m3->dimv[i] = m2->dimv[i];
            }
            
            m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(b) * sizeof(Gua_Object));
            o3 = (Gua_Object *)m3->object;
            
            for (i = 0; i < Gua_PObjectLength(b); i++) {
                if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(a) == OBJECT_TYPE_INTEGER)) {
                    Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o2[i]) * Gua_PObjectToInteger(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL) && (Gua_PObjectType(a) == OBJECT_TYPE_REAL)) {
                    Gua_RealToObject(o3[i], Gua_ObjectToReal(o2[i]) * Gua_PObjectToReal(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX) && (Gua_PObjectType(a) == OBJECT_TYPE_COMPLEX)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o2[i]) * Gua_PObjectToReal(a) - Gua_ObjectToImaginary(o2[i]) * Gua_PObjectToImaginary(a), Gua_ObjectToReal(o2[i]) * Gua_PObjectToImaginary(a) + Gua_ObjectToImaginary(o2[i]) * Gua_PObjectToReal(a));
                    
                    if (Gua_ObjectToImaginary(o3[i]) == 0) {
                        Gua_SetObjectType(o3[i], OBJECT_TYPE_REAL)
                    }
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(a) == OBJECT_TYPE_REAL)) {
                    Gua_RealToObject(o3[i], Gua_ObjectToInteger(o2[i]) * Gua_PObjectToReal(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL) && (Gua_PObjectType(a) == OBJECT_TYPE_INTEGER)) {
                    Gua_RealToObject(o3[i], Gua_ObjectToReal(o2[i]) * Gua_PObjectToInteger(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) && (Gua_PObjectType(a) == OBJECT_TYPE_COMPLEX)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToInteger(o2[i]) * Gua_PObjectToReal(a), Gua_ObjectToInteger(o2[i]) * Gua_PObjectToImaginary(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX) && (Gua_PObjectType(a) == OBJECT_TYPE_INTEGER)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o2[i]) * Gua_PObjectToInteger(a), Gua_ObjectToImaginary(o2[i]) * Gua_PObjectToInteger(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL) && (Gua_PObjectType(a) == OBJECT_TYPE_COMPLEX)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o2[i]) * Gua_PObjectToReal(a), Gua_ObjectToReal(o2[i]) * Gua_PObjectToImaginary(a));
                } else if ((Gua_ObjectType(o2[i]) == OBJECT_TYPE_COMPLEX) && (Gua_PObjectType(a) == OBJECT_TYPE_REAL)) {
                    Gua_ComplexToObject(o3[i], Gua_ObjectToReal(o2[i]) * Gua_PObjectToReal(a), Gua_ObjectToImaginary(o2[i]) * Gua_PObjectToReal(a));
                }
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_IdentMatrix(Gua_Object *a, Gua_Integer n, Gua_String error)
 *
 * Description:
 *     Returns an identity matrix.
 *
 * Arguments:
 *     a,        a matrix;
 *     n,        the matrix dimension;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix A = I.
 */
Gua_Status Gua_IdentMatrix(Gua_Object *a, Gua_Integer n, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Object *o1;
    Gua_Integer i;
    Gua_Integer j;
    
    if (!Gua_IsPObjectStored(a)) {
        Gua_FreeObject(a);
    } else {
        Gua_ClearPObject(a);
    }
    
    Gua_MatrixToPObject(a, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), n * n);
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    m1->dimc = 2;
    m1->dimv = Gua_Alloc(n * n * sizeof(Gua_Length));
    
    m1->dimv[0] = n;
    m1->dimv[1] = n;
    
    m1->object = (struct Gua_Object *)Gua_Alloc(n * n * sizeof(Gua_Object));
    o1 = (Gua_Object *)m1->object;
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i==j) {
                Gua_RealToObject(o1[i * n + j], 1.0);
            } else {
                Gua_RealToObject(o1[i * n + j], 0.0);
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_InvMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
 *
 * Description:
 *     Calculate the matrix b inverse of a.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix b inverse of a
 */
Gua_Status Gua_InvMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
{
    Gua_Object c;
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Real scale;
    Gua_Real det;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer m;
    Gua_Integer n;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return 0;
    }
    
    Gua_ClearObject(c);
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if (!Gua_IsPObjectStored(b)) {
            Gua_FreeObject(b);
        } else {
            Gua_ClearPObject(b);
        }
        
        Gua_CopyMatrix(&c, a, false);
        Gua_IdentMatrix(b, m1->dimv[0], error);
        
        m2 = (Gua_Matrix *)Gua_ObjectToMatrix(c);
        o2 = (Gua_Object *)m2->object;
        
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        o3 = (Gua_Object *)m3->object;
        
        m = m2->dimv[0];
        n = m2->dimv[1];
        
        /* Convert the matrix to real. */
        for (i = 0; i < Gua_PObjectLength(b); i++) {
            if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o2[i]);
            } else {
                if (Gua_ObjectType(o2[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o2[i]);
                    Gua_RealToObject(o2[i], 0.0);
                }
            }
        }
        
        /* Convert to the diagonal equivalent matrix. */
        for (j = 0; j < m; j++) {
            if (Gua_ObjectToReal(o2[j * n + j]) != 0) {
                for (i = 0; i < m; i++) {
                    if (i != j) {
                        scale = -Gua_ObjectToReal(o2[i * n + j]) / Gua_ObjectToReal(o2[j * n + j]);
                        for (k = j; k < n; k++) {
                            Gua_RealToObject(o2[i * n + k], Gua_ObjectToReal(o2[i * n + k]) + scale * Gua_ObjectToReal(o2[j * n + k]));
                        }
                        for (k = 0; k < n; k++) {
                            Gua_RealToObject(o3[i * n + k], Gua_ObjectToReal(o3[i * n + k]) + scale * Gua_ObjectToReal(o3[j * n + k]));
                        }
                    }
                }
            }
        }
        
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++) {
                Gua_RealToObject(o3[i * n + j], Gua_ObjectToReal(o3[i * n + j]) / Gua_ObjectToReal(o2[i * n + i]));
            }
        }
        
        det = 1;
        for (i = 0; i < m; i++) {
            det = det * Gua_ObjectToReal(o2[i * n + i]);
        }
        
        Gua_FreeObject(&c);
        
        if (det == 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrix is singular");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_PowMatrix(Gua_Object *a, Gua_Integer n, Gua_Object *b, Gua_String error)
 *
 * Description:
 *     Calculate the power of the matrix a by n.
 *
 * Arguments:
 *     a,        a matrix;
 *     n,        a positive integer power, or 0 or -1;
 *     b,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix B = A ** n.
 */
Gua_Status Gua_PowMatrix(Gua_Object *a, Gua_Integer n, Gua_Object *b, Gua_String error)
{
    Gua_Object c;
    Gua_Matrix *m1;
    Gua_Integer i;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    Gua_ClearObject(c);
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if (!Gua_IsPObjectStored(b)) {
            Gua_FreeObject(b);
        } else {
            Gua_ClearPObject(b);
        }
        
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((n < 0) && (n != -1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "illegal operand");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (n == 0) {
            Gua_IdentMatrix(b, m1->dimv[0], error);
            return GUA_OK;
        }
        if (n == -1) {
            Gua_InvMatrix(a, b, error);
            return GUA_OK;
        }
        
        Gua_CopyMatrix(&c, a, false);
        
        for (i = 1; i < n; i++) {
            if (Gua_MulMatrix(a, &c, b, error) != GUA_OK) {
                if (!Gua_IsObjectStored(c)) {
                    Gua_FreeObject(&c);
                }
                return GUA_ERROR;
            }
            if (!Gua_IsObjectStored(c)) {
                Gua_FreeObject(&c);
            }
            Gua_CopyMatrix(&c, b, false);
        }
        
        if (!Gua_IsObjectStored(c)) {
            Gua_FreeObject(&c);
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_AndMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Perform boolean AND between two matrices.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a matrix;
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix C = A && B.
 */
Gua_Status Gua_AndMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Integer i;
    Gua_String errMessage;
    
    if (!((Gua_PObjectType(a) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have the same size");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        for (i = 0; i < m1->dimc; i++) {
            if (m1->dimv[i] != m2->dimv[i]) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "the matrices do not have the same dimensions");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
        
        m3->dimc = m1->dimc;
        
        m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
        
        for (i = 0; i < m3->dimc; i++) {
            m3->dimv[i] = m1->dimv[i];
        }
        
        m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        o3 = (Gua_Object *)m3->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) && Gua_ObjectToInteger(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToReal(o1[i]) && Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) && Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToReal(o1[i]) && Gua_ObjectToInteger(o2[i]));
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_OrMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Perform boolean OR between two matrices.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a matrix;
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix C = A && B.
 */
Gua_Status Gua_OrMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Integer i;
    Gua_String errMessage;
    
    if (!((Gua_PObjectType(a) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(b) == OBJECT_TYPE_MATRIX))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (Gua_PObjectLength(a) != Gua_PObjectLength(b)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have the same size");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        for (i = 0; i < m1->dimc; i++) {
            if (m1->dimv[i] != m2->dimv[i]) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "the matrices do not have the same dimensions");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
        
        m3->dimc = m1->dimc;
        
        m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
        
        for (i = 0; i < m3->dimc; i++) {
            m3->dimv[i] = m1->dimv[i];
        }
        
        m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        o3 = (Gua_Object *)m3->object;
        
        for (i = 0; i < Gua_PObjectLength(a); i++) {
            if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) || Gua_ObjectToInteger(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToReal(o1[i]) || Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToInteger(o1[i]) || Gua_ObjectToReal(o2[i]));
            } else if ((Gua_ObjectType(o1[i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToObject(o3[i], Gua_ObjectToReal(o1[i]) || Gua_ObjectToInteger(o2[i]));
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_AndOrMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Multiply two boolean matrices.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a matrix;
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix C = A * B(boolean multiplication).
 */
Gua_Status Gua_AndOrMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer m;
    Gua_Integer n;
    Gua_Integer p;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_PObjectType(b) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal operand 2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if (!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (m1->dimv[1] != m2->dimv[0]) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o1 = (Gua_Object *)m1->object;
        o2 = (Gua_Object *)m2->object;
        
        /* A(m x n), B(n x p), C(m x p) */
        m = m1->dimv[0];
        n = m1->dimv[1];
        p = m2->dimv[1];
        
        Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), m * p);
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
        
        m3->dimc = 2;
        m3->dimv = Gua_Alloc(m3->dimc * sizeof(Gua_Integer));
        
        m3->dimv[0] = m;
        m3->dimv[1] = p;
        
        m3->object = (struct Gua_Object *)Gua_Alloc(m * p * sizeof(Gua_Object));
        o3 = (Gua_Object *)m3->object;
        
        /* Clear each element of matrix C. */ 
        for (i = 0; i < m; i++) {
            for (j = 0; j < p; j++) {
                Gua_ClearObject(o3[i * p + j]);
            }
        }

        /* C(i,j) = sum(A(i,k) * B(k,j)) */
        for (i = 0; i < m; i++) {
            for (j = 0; j < p; j++) {
                for (k = 0; k < n; k++) {            
                    if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_INTEGER)) {
                        Gua_IntegerToObject(o3[i * p + j], Gua_ObjectToInteger(o3[i * p + j]) || (Gua_ObjectToInteger(o1[i * n + k]) && Gua_ObjectToInteger(o2[k * p + j])));
                    } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_REAL)) {
                        Gua_RealToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) || (Gua_ObjectToReal(o1[i * n + k]) && Gua_ObjectToReal(o2[k * p + j])));
                    } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_REAL)) {
                        Gua_RealToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) || (Gua_ObjectToInteger(o1[i * n + k]) && Gua_ObjectToReal(o2[k * p + j])));
                    } else if ((Gua_ObjectType(o1[i * n + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2[k * p + j]) == OBJECT_TYPE_INTEGER)) {
                        Gua_RealToObject(o3[i * p + j], Gua_ObjectToReal(o3[i * p + j]) || (Gua_ObjectToReal(o1[i * n + k]) && Gua_ObjectToInteger(o2[k * p + j])));
                    }
                }
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CopyFile(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
 *
 * Description:
 *     Copy a handle to other.
 *
 * Arguments:
 *     target,    the target handle;
 *     source,    the source handle;
 *     stored,    if TRUE the object of the target handle is stored in a variable.
 *
 * Results:
 *     The function makes a copy of a handle to other.
 */
Gua_Status Gua_CopyFile(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
{
    Gua_File *ht;
    Gua_File *hs;
    
    hs = (Gua_File *)Gua_PObjectToFile(source);
    
    if (hs) {
        if (!Gua_IsPObjectStored(target)) {
            Gua_FreeObject(target);
        } else {
            Gua_ClearPObject(target);
        }
        
        Gua_NewFile(ht, hs->file);
        Gua_LinkFilesFunctions(ht, hs);
        
        Gua_FileToPObject(target, (struct Gua_File *)ht);
        Gua_SetPObjectStoredState(target, stored);
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CopyHandle(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
 *
 * Description:
 *     Copy a handle to other.
 *
 * Arguments:
 *     target,    the target handle;
 *     source,    the source handle;
 *     stored,    if TRUE the object of the target handle is stored in a variable.
 *
 * Results:
 *     The function makes a copy of a handle to other.
 */
Gua_Status Gua_CopyHandle(Gua_Object *target, Gua_Object *source, Gua_Stored stored)
{
    Gua_Handle *ht;
    Gua_Handle *hs;
    
    hs = (Gua_Handle *)Gua_PObjectToHandle(source);
    
    if (hs) {
        if (!Gua_IsPObjectStored(target)) {
            Gua_FreeObject(target);
        } else {
            Gua_ClearPObject(target);
        }
        
        Gua_NewHandle(ht, hs->type, hs->pointer);
        
        Gua_HandleToPObject(target, (struct Gua_Handle *)ht);
        Gua_SetPObjectStoredState(target, stored);
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_BuiltInFunction(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Built in functions wrapper.
 *
 * Arguments:
 *     nspace,    a pointer to a structure Gua_Namespace. Must do a cast before use it;
 *     argc,      the number of arguments to pass to the function;
 *     argv,      an array containing the arguments to the function,
 *                argv[0] is the function name;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The return object of the wrapped funcion.
 */
Gua_Status Gua_BuiltInFunction(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Status status;
    Gua_Object key;
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Object v1;
    Gua_Function f1;
    Gua_String string;
    Gua_Length length;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer n;
    Gua_Integer p;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "array") == 0) {
        if (argc < 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        for (i = 1; i < argc; i++) {
            Gua_IntegerToObject(key, i - 1);
            
            status = Gua_SetArrayElement(object, &key, &argv[i], false);
            
            if (status != GUA_OK) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...", Gua_StatusTable[GUA_ERROR_ILLEGAL_ASSIGNMENT]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "arrayToString") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_ARRAY) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ArrayToString(&argv[1], object) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "break") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "\"break\" outside of a loop");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_BREAK;
    } else if (strcmp(Gua_ObjectToString(argv[0]), "complex") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_ComplexToPObject(object, Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2]));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_ComplexToPObject(object, Gua_ObjectToReal(argv[1]), Gua_ObjectToReal(argv[2]));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_ComplexToPObject(object, Gua_ObjectToInteger(argv[1]), Gua_ObjectToReal(argv[2]));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_ComplexToPObject(object, Gua_ObjectToReal(argv[1]), Gua_ObjectToInteger(argv[2]));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "continue") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "\"continue\" outside of a loop");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_CONTINUE;
    } else if (strcmp(Gua_ObjectToString(argv[0]), "dim") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_GetMatrixDim(object, &argv[1]) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "error") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_ObjectToString(argv[1]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    } else if (strcmp(Gua_ObjectToString(argv[0]), "eval") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_Evaluate((Gua_Namespace *)nspace, Gua_ObjectToString(argv[1]), object, &status, error);
        
        if (status != GUA_OK) {
            return status;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "exists") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, 0);
        
        if (Gua_SearchVariable(nspace, Gua_ObjectToString(argv[1]), &v1, SCOPE_STACK) != OBJECT_TYPE_UNKNOWN) {
            Gua_IntegerToPObject(object, 1);
        }
        
        i = 0;
        
        while (strlen(Gua_ConstantTable[i].name) > 0) {
            if (strcmp(Gua_ConstantTable[i].name, Gua_ObjectToString(argv[1])) == 0) {
                Gua_IntegerToPObject(object, 1);
            }
            i++;
        }
        
        if (Gua_SearchFunction(nspace, Gua_ObjectToString(argv[1]), &f1) == GUA_OK) {
            Gua_IntegerToPObject(object, 1);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "exit") == 0) {
        if (argc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc == 2) {
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
                Gua_LinkToPObject(object, argv[1]);
            } else {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        return GUA_EXIT;
    } else if (strcmp(Gua_ObjectToString(argv[0]), "expr") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_Expression((Gua_Namespace *)nspace, Gua_ObjectToString(argv[1]), object, &status, error);
        
        if (status != GUA_OK) {
            return status;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "getMatrixElement") == 0) {
        if (argc < 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        m = (Gua_Matrix *)Gua_ObjectToMatrix(argv[1]);
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if ((Gua_ObjectToInteger(argv[2]) < 0) || (Gua_ObjectToInteger(argv[2]) >= Gua_ObjectLength(argv[1]))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argv[2]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        } else {
            if ((argc - 2) != m->dimc) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "index out of bound");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            for (i = 2; i < argc; i++) {
                if (Gua_ObjectType(argv[i]) != OBJECT_TYPE_INTEGER) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %ld %s %-.20s...\n", "illegal argument", i, "for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
                if ((Gua_ObjectToInteger(argv[i]) < 0) || (Gua_ObjectToInteger(argv[i]) >= m->dimv[i - 2])) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argv[i]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
            }
        }
        
        o = (Gua_Object *)m->object;
        
        if (argc == 3) {
            n = Gua_ObjectToInteger(argv[2]);
            
            Gua_LinkToPObject(object, o[n]);
        } else {
            n = Gua_ObjectToInteger(argv[2]) * m->dimv[1] + argv[3].integer;
            
            if (argc > 4) {
                p = m->dimv[0];
                j = 1;
                for (i = 4; i < argc; i++) {
                    p = p * m->dimv[j];
                    n = n + Gua_ObjectToInteger(argv[i]) * p;
                    j++;
                }
            }
            
            Gua_LinkToPObject(object, o[n]);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ident") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_IdentMatrix(object, Gua_ObjectToInteger(argv[1]), error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "inv") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_InvMatrix(&argv[1], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isMatrixApproximatelyEqual") == 0) {
        if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_REAL) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_IsMatrixApproximatelyEqual(&argv[1], &argv[2], &argv[3])) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "keys") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_ARRAY) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_GetArrayKeys(object, &argv[1]) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "length") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, Gua_ObjectLength(argv[1]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "matrix2D") == 0) {
        if (argc < 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), 0);
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 2;
        m->dimv = Gua_Alloc(2 * sizeof(Gua_Integer));
        m->dimv[0] = Gua_ObjectToInteger(argv[1]);
        m->dimv[1] = Gua_ObjectToInteger(argv[2]);
        
        length = Gua_ObjectToInteger(argv[1]) * Gua_ObjectToInteger(argv[2]);
        
        if (length != (argc - 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_FreeObject(object);
            
            return GUA_ERROR;
        }
        
        if (length == 0) {
            Gua_FreeObject(object);
            return GUA_OK;
        }
        
        for (i = 3; i < argc; i++) {
            if (!((Gua_ObjectType(argv[i]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[i]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[i]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[i]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[i]) == OBJECT_TYPE_FILE) || (Gua_ObjectType(argv[i]) == OBJECT_TYPE_HANDLE))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %ld %s %-.20s...\n", "illegal argument", i, "for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_FreeObject(object);
                
                return GUA_ERROR;
            }
        }
        
        m->object = (struct Gua_Object *)Gua_Alloc(length * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        for (i = 0; i < length; i++) {
            Gua_ClearObject(o[i]);
            
            if (Gua_ObjectType(argv[i + 3]) == OBJECT_TYPE_STRING) {
                if (Gua_IsObjectStored(argv[i + 3])) {
                    Gua_ByteArrayToObject(o[i], Gua_ObjectToString(argv[i + 3]), Gua_ObjectLength(argv[i + 3]));
                } else {
                    Gua_LinkByteArrayToObject(o[i], Gua_ObjectToString(argv[i + 3]), Gua_ObjectLength(argv[i + 3]));
                    Gua_SetStoredObject(argv[i + 3]);
                }
            } else if (Gua_ObjectType(argv[i + 3]) == OBJECT_TYPE_FILE) {
                if (Gua_IsObjectStored(argv[i + 3])) {
                    Gua_CopyFile(&(o[i]), &(argv[i + 3]), false);
                } else {
                    Gua_LinkObjects(o[i], argv[i + 3]);
                    Gua_SetStoredObject(argv[i + 3]);
                }
            } else if (Gua_ObjectType(argv[i + 3]) == OBJECT_TYPE_HANDLE) {
                if (Gua_IsObjectStored(argv[i + 3])) {
                    Gua_CopyHandle(&(o[i]), &(argv[i + 3]), false);
                } else {
                    Gua_LinkObjects(o[i], argv[i + 3]);
                    Gua_SetStoredObject(argv[i + 3]);
                }
            } else {
                Gua_LinkObjects(o[i], argv[i + 3]);
            }
            Gua_SetNotStoredObject(o[i]);
        }
        
        Gua_SetPObjectLength(object, length);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "matrix") == 0) {
        if (argc < 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), 0);
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        if (argc > 3) {
            m->dimc = 0;
            length = 1;
            
            for (i = 2; i < argc; i++) {
                if (Gua_ObjectType(argv[i]) != OBJECT_TYPE_INTEGER) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %ld %s %-.20s...\n", "illegal argument", i, "for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    Gua_FreeObject(object);
                    
                    return GUA_ERROR;
                }
                
                m->dimc++;
                length = length * Gua_ObjectToInteger(argv[i]);
            }
            
            m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
            
            for (i = 2; i < argc; i++) {
                if (Gua_ObjectType(argv[i]) != OBJECT_TYPE_INTEGER) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %ld %s %-.20s...\n", "illegal argument", i, "for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    Gua_FreeObject(object);
                    
                    return GUA_ERROR;
                }
                
                m->dimv[i - 2] = Gua_ObjectToInteger(argv[i]);
            }
        } else {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_FreeObject(object);
                
                return GUA_ERROR;
            }
            
            m->dimc = 1;
            m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
            m->dimv[0] = Gua_ObjectToInteger(argv[2]);
            length = Gua_ObjectToInteger(argv[2]);
        }
        
        if (length == 0) {
            Gua_FreeObject(object);
            return GUA_OK;
        }
        
        m->object = (struct Gua_Object *)Gua_Alloc(length * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;

        for (j = 0; j < length; j++) {
            Gua_ClearObject(o[j]);
            
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                if (Gua_IsObjectStored(argv[1])) {
                    Gua_ByteArrayToObject(o[j], Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                } else {
                    Gua_LinkByteArrayToObject(o[j], Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                    Gua_SetStoredObject(argv[1]);
                }
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) {
                if (Gua_IsObjectStored(argv[1])) {
                    Gua_CopyFile(&(o[j]), &(argv[1]), false);
                } else {
                    Gua_LinkObjects(o[j], argv[1]);
                    Gua_SetStoredObject(argv[1]);
                }
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
                if (Gua_IsObjectStored(argv[1])) {
                    Gua_CopyHandle(&(o[j]), &(argv[1]), false);
                } else {
                    Gua_LinkObjects(o[j], argv[1]);
                    Gua_SetStoredObject(argv[1]);
                }
            } else {
                Gua_LinkObjects(o[j], argv[1]);
            }
            Gua_SetNotStoredObject(o[j]);
        }
        
        Gua_SetPObjectLength(object, length);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "matrixToString") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_MatrixToString(&argv[1], object) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "return") == 0) {
        if (argc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc == 2) {
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                Gua_ByteArrayToPObject(object, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) {
                Gua_CopyArray(object, &(argv[1]), false);
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
                Gua_CopyMatrix(object, &(argv[1]), false);
            } else {
                Gua_LinkToPObject(object, argv[1]);
            }
        }
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "\"return\" outside of a function");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_RETURN;
    } else if (strcmp(Gua_ObjectToString(argv[0]), "setMatrixElement") == 0) {
        if (argc < 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[2]) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_UNKNOWN)) {
            return GUA_ERROR_ILLEGAL_ASSIGNMENT;
        }
        
        m = (Gua_Matrix *)Gua_ObjectToMatrix(argv[1]);
        
        if (argc == 4) {
            if ((Gua_ObjectToInteger(argv[3]) < 0) || (Gua_ObjectToInteger(argv[3]) >= Gua_ObjectLength(argv[1]))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argv[3]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        } else {
            if ((argc - 3) != m->dimc) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "index out of bound");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            for (i = 3; i < argc; i++) {
                if (Gua_ObjectType(argv[i]) != OBJECT_TYPE_INTEGER) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %ld %s %-.20s...\n", "illegal argument", i, "for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
                if ((Gua_ObjectToInteger(argv[i]) < 0) || (Gua_ObjectToInteger(argv[i]) >= m->dimv[i - 3])) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argv[i]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
            }
        }
        
        o = (Gua_Object *)m->object;
        
        if (argc == 4) {
            n = Gua_ObjectToInteger(argv[3]);
            
            if (Gua_ObjectType(o[n]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(o[n]));
            }
            
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_STRING) {
                if (Gua_IsObjectStored(argv[2])) {
                    Gua_ByteArrayToObject(o[n], Gua_ObjectToString(argv[2]), Gua_ObjectLength(argv[2]));
                } else {
                    Gua_LinkByteArrayToObject(o[n], Gua_ObjectToString(argv[2]), Gua_ObjectLength(argv[2]));
                    Gua_SetStoredObject(argv[2]);
                }
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_FILE) {
                if (Gua_IsObjectStored(argv[2])) {
                    Gua_CopyFile(&(o[n]), &(argv[2]), true);
                } else {
                    Gua_LinkObjects(o[n], argv[2]);
                    Gua_SetStoredObject(argv[2]);
                }
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE) {
                if (Gua_IsObjectStored(argv[2])) {
                    Gua_CopyHandle(&(o[n]), &(argv[2]), true);
                } else {
                    Gua_LinkObjects(o[n], argv[2]);
                    Gua_SetStoredObject(argv[2]);
                }
            } else {
                Gua_LinkObjects(o[n], argv[2]);
            }
            Gua_SetStoredObject(o[n]);
            
            Gua_LinkToPObject(object, argv[2]);
            Gua_SetStoredPObject(object);
        } else {
            n = Gua_ObjectToInteger(argv[3]) * m->dimv[1] + Gua_ObjectToInteger(argv[4]);
            
            if (argc > 5) {
                p = m->dimv[0];
                j = 1;
                for (i = 5; i < argc; i++) {
                    p = p * m->dimv[j];
                    n = n + Gua_ObjectToInteger(argv[i]) * p;
                    j++;
                }
            }
            
            if (Gua_ObjectType(o[n]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(o[n]));
            }
            
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_STRING) {
                if (Gua_IsObjectStored(argv[2])) {
                    Gua_ByteArrayToObject(o[n], Gua_ObjectToString(argv[2]), Gua_ObjectLength(argv[2]));
                } else {
                    Gua_LinkByteArrayToObject(o[n], Gua_ObjectToString(argv[2]), Gua_ObjectLength(argv[2]));
                    Gua_SetStoredObject(argv[2]);
                }
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_FILE) {
                if (Gua_IsObjectStored(argv[2])) {
                    Gua_CopyFile(&(o[n]), &(argv[2]), true);
                } else {
                    Gua_LinkObjects(o[n], argv[2]);
                    Gua_SetStoredObject(argv[2]);
                }
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE) {
                if (Gua_IsObjectStored(argv[2])) {
                    Gua_CopyHandle(&(o[n]), &(argv[2]), true);
                } else {
                    Gua_LinkObjects(o[n], argv[2]);
                    Gua_SetStoredObject(argv[2]);
                }
            } else {
                Gua_LinkObjects(o[n], argv[2]);
            }
            Gua_SetStoredObject(o[n]);
            
            Gua_LinkToPObject(object, argv[2]);
            Gua_SetStoredPObject(object);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "toString") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
            sprintf(string, "%ld", Gua_ObjectToInteger(argv[1]));
            Gua_StringToPObject(object, string);
            Gua_Free(string);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
            sprintf(string, "%g", Gua_ObjectToReal(argv[1]));
            Gua_StringToPObject(object, string);
            Gua_Free(string);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
            Gua_StringToPObject(object, Gua_ObjectToString(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
            sprintf(string, "%g%+g*i", Gua_ObjectToReal(argv[1]), Gua_ObjectToImaginary(argv[1]));
            Gua_StringToPObject(object, string);
            Gua_Free(string);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) {
            if (Gua_ArrayToString(&argv[1], object) != GUA_OK) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
            if (Gua_MatrixToString(&argv[1], object) != GUA_OK) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) {
            string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
            sprintf(string, "File%lu", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(argv[1])));
            Gua_StringToPObject(object, string);
            Gua_Free(string);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
            string = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
            sprintf(string, "%s%lu\n", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(argv[1])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(argv[1])));
            Gua_StringToPObject(object, string);
            Gua_Free(string);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "type") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, Gua_ObjectType(argv[1]));
    } else {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "unknown function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CountArguments(Gua_String start)
 *
 * Description:
 *     Parse the expression and count the number o arguments separated by comma.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse.
 *
 * Results:
 *     The function returns the number o arguments separated by comma.
 *
 */
Gua_Status Gua_CountArguments(Gua_String start)
{
    Gua_String p;
    Gua_Short argc;
    Gua_Token token;
    
    p = start;
    
    argc = 0;
    
    if (strlen(start) > 0) {
        argc++;
        
        while (*p != EXPRESSION_END) {
            if (*p == SINGLE_QUOTE) {
                p = Gua_ScanSingleQuotes(p, &token);
                p--;
            } else if (*p == DOUBLE_QUOTE) {
                p = Gua_ScanDoubleQuotes(p, &token);
                p--;
            } else if (*p == PARENTHESIS_OPEN) {
                p = Gua_ScanParenthesis(p, &token);
                p--;
            } else if (*p == BRACKET_OPEN) {
                p = Gua_ScanBracket(p, &token);
                p--;
            } else if (*p == BRACE_OPEN) {
                p = Gua_ScanBrace(p, &token);
                p--;
            } else if (*p == COMMA) {
                argc++;
            }
            p++;
        }
    }
    
    return argc;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ParseArguments(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Short first, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return a vector containing each argument.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     argc,      the number of arguments;
 *     argv,      an array containing the arguments;
 *     first,     the first element in array to fill;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns a vector containing each argument in expression.
 *
 */
Gua_Status Gua_ParseArguments(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Short first, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String s;
    Gua_String expression;
    Gua_Token token;
    Gua_Short i;
    Gua_String errMessage;
    
    p = start;
    
    expression = (char *)Gua_Alloc(sizeof(char) * (strlen(start) + 1));
    memset(expression, '\0', sizeof(char) * (strlen(start) + 1));
    
    i = first;
    
    s = p;
    
    if (argc > 1) {
        while (true) {
            p = Gua_NextToken(nspace, p, &token);
            
            if (token.status != GUA_OK) {
                *status = token.status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token.status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(expression);
                
                return GUA_ERROR;
            }
            
            if (token.type == TOKEN_TYPE_ARG_SEPARATOR) {
                strncpy(expression, s, (Gua_Length)(p - s - 1));
                
                Gua_Evaluate(nspace, expression, &argv[i], status, error);
                
                if (*status != GUA_OK) {
                    Gua_Free(expression);
                    return GUA_ERROR;
                }
                
                s = p;
                
                if (i < (argc - 1)) {
                    memset(expression, '\0', sizeof(char) * (strlen(start) + 1));
                    i++;
                } else {
                    break;
                }
            } else if (token.type == TOKEN_TYPE_END) {
                break;
            }
        }
    }
    
    strcpy(expression, s);
    
    Gua_Evaluate(nspace, expression, &argv[i], status, error);
    
    Gua_Free(expression);
    
    if (*status != GUA_OK) {
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CountCommands(Gua_String start)
 *
 * Description:
 *     Parse the expression and count the number o arguments separated by comma.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse.
 *
 * Results:
 *     The function returns the number o arguments separated by comma.
 *
 */
Gua_Status Gua_CountCommands(Gua_String start)
{
    Gua_String p;
    Gua_Short argc;
    Gua_Token token;
    
    p = start;
    
    argc = 0;
    
    if (strlen(start) > 0) {
        argc++;
        
        while (*p != EXPRESSION_END) {
            if (*p == SINGLE_QUOTE) {
                p = Gua_ScanSingleQuotes(p, &token);
                p--;
            } else if (*p == DOUBLE_QUOTE) {
                p = Gua_ScanDoubleQuotes(p, &token);
                p--;
            } else if (*p == PARENTHESIS_OPEN) {
                p = Gua_ScanParenthesis(p, &token);
                p--;
            } else if (*p == BRACKET_OPEN) {
                p = Gua_ScanBracket(p, &token);
                p--;
            } else if (*p == BRACE_OPEN) {
                p = Gua_ScanBrace(p, &token);
                p--;
            } else if (*p == SEMI_COLON) {
                argc++;
            }
            p++;
        }
    }
    
    return argc;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ParseCommands(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return a vector containing each argument.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     argc,      the number of arguments;
 *     argv,      an array containing the arguments;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns a vector containing each argument in expression.
 *
 */
Gua_Status Gua_ParseCommands(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String s;
    Gua_Token token;
    Gua_Short i;
    
    p = start;
    
    i = 0;
    
    s = p;
    
    if (argc > 1) {
        while (true) {
            p = Gua_NextToken(nspace, p, &token);
            
            if (token.type == TOKEN_TYPE_SEPARATOR) {
                Gua_ByteArrayToObject(argv[i], s, (Gua_Length)(p - s - 1));
                
                s = p;
                
                if (i < (argc - 1)) {
                    i++;
                } else {
                    break;
                }
            } else if (token.type == TOKEN_TYPE_END) {
                break;
            }
        }
    }
    
    Gua_ByteArrayToObject(argv[i], s, (Gua_Length)(p - s + 1));
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ParseFormalArguments(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return a vector containing each argument.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     argc,      the number of arguments;
 *     argv,      an array containing the arguments;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns a vector containing each argument in expression.
 *
 */
Gua_Status Gua_ParseFormalArguments(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String s;
    Gua_Token token;
    Gua_Short i;
    
    p = start;
    
    i = 0;
    
    s = p;
    
    if (argc > 1) {
        while (true) {
            p = Gua_NextToken(nspace, p, &token);
            
            if (token.type == TOKEN_TYPE_ARG_SEPARATOR) {
                Gua_ByteArrayToObject(argv[i], s, (Gua_Length)(p - s - 1));
                
                s = p;
                
                if (i < (argc - 1)) {
                    i++;
                } else {
                    break;
                }
            } else if (token.type == TOKEN_TYPE_END) {
                break;
            }
        }
    }
    
    Gua_StringToObject(argv[i], s);
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_CountMatrixElements(Gua_String start)
 *
 * Description:
 *     Parse the expression and count the number o arguments separated by comma.
 *
 * Arguments:
 *     start,    a pointer to the start point of the expression to parse.
 *
 * Results:
 *     The function returns the number o arguments separated by comma.
 *
 */
Gua_Status Gua_CountMatrixElements(Gua_String start)
{
    Gua_String p;
    Gua_Short argc;
    Gua_Token token;
    
    p = start;
    
    argc = 0;
    
    if (strlen(start) > 0) {
        argc++;
        
        while (*p != EXPRESSION_END) {
            if (*p == SINGLE_QUOTE) {
                p = Gua_ScanSingleQuotes(p, &token);
                p--;
            } else if (*p == DOUBLE_QUOTE) {
                p = Gua_ScanDoubleQuotes(p, &token);
                p--;
            } else if (*p == PARENTHESIS_OPEN) {
                p = Gua_ScanParenthesis(p, &token);
                p--;
            } else if (*p == BRACKET_OPEN) {
                p = Gua_ScanBracket(p, &token);
                p--;
            } else if (*p == BRACE_OPEN) {
                p = Gua_ScanBrace(p, &token);
                p--;
            } else if (*p == COMMA) {
                argc++;
            } else if (*p == SEMI_COLON) {
                argc++;
            }
            p++;
        }
    }
    
    return argc;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ParseMatrixElements(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return a vector containing each argument.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     argc,      the number of arguments;
 *     argv,      an array containing the arguments, where
 *                argv[0] will be the function name,
 *                argv[1] will be the m size(m x n),
 *                argv[2] will be the n size(m x n);
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns a vector containing each argument in expression.
 *
 */
Gua_Status Gua_ParseMatrixElements(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String s;
    Gua_String expression;
    Gua_Token token;
    Gua_Short i;
    Gua_Integer n;
    Gua_String errMessage;
    
    p = start;
    
    expression = (char *)Gua_Alloc(sizeof(char) * (strlen(start) + 1));
    memset(expression, '\0', sizeof(char) * (strlen(start) + 1));
    
    i = 3;
    
    Gua_IntegerToObject(argv[1], 1);
    Gua_IntegerToObject(argv[2], 1);
    
    n = 1;
    
    s = p;
    
    if (argc > 4) {       
        while (true) {
            p = Gua_NextToken(nspace, p, &token);
            
            if (token.status != GUA_OK) {
                *status = token.status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token.status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(expression);
                
                return GUA_ERROR;
            }
            
            if (token.type == TOKEN_TYPE_SEPARATOR) {
                if (Gua_ObjectToInteger(argv[2]) == 1) {
                    Gua_IntegerToObject(argv[2], n);
                } else {
                    if (Gua_ObjectToInteger(argv[2]) != n) {
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", "the matrix has rows with diferent number of elements");
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        Gua_Free(expression);
                        
                        return GUA_ERROR;
                    }
                }
                
                Gua_IntegerToObject(argv[1], Gua_ObjectToInteger(argv[1]) + 1);
                n = 1;
                
                strncpy(expression, s, (Gua_Length)(p - s - 1));
                
                Gua_Evaluate(nspace, expression, &argv[i], status, error);
                
                if (*status != GUA_OK) {
                    Gua_Free(expression);
                    return GUA_ERROR;
                }
                
                s = p;
                
                if (i < (argc - 1)) {
                    memset(expression, '\0', sizeof(char) * (strlen(start) + 1));
                    i++;
                } else {
                    break;
                }
           } else if (token.type == TOKEN_TYPE_ARG_SEPARATOR) {
                n++;
                
                strncpy(expression, s, (Gua_Length)(p - s - 1));
                
                Gua_Evaluate(nspace, expression, &argv[i], status, error);
                
                if (*status != GUA_OK) {
                    Gua_Free(expression);
                    return GUA_ERROR;
                }
                
                s = p;
                
                if (i < (argc - 1)) {
                    memset(expression, '\0', sizeof(char) * (strlen(start) + 1));
                    i++;
                } else {
                    break;
                }
            } else if (token.type == TOKEN_TYPE_END) {
                if (Gua_ObjectToInteger(argv[2]) == 1) {
                    Gua_IntegerToObject(argv[2], n);
                }
                break;
            }
        }
    }
    
    if (Gua_ObjectToInteger(argv[2]) != n) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "the matrix has rows with diferent number of elements");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(expression);
        
        return GUA_ERROR;
    }
    
    strcpy(expression, s);
    
    Gua_Evaluate(nspace, expression, &argv[i], status, error);
    
    if (*status != GUA_OK) {
        Gua_Free(expression);
        return GUA_ERROR;
    }
    
    Gua_Free(expression);
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_FreeArguments(Gua_Short argc, Gua_Object *argv)
 *
 * Description:
 *     Free argument's memory.
 *
 * Arguments:
 *     argc,    the number of arguments;
 *     argv,    an array containing the arguments;
 *              argv[0] is the function name.
 *
 * Results:
 *     The memory used by the arguments is Gua_Freed.
 */
void Gua_FreeArguments(Gua_Short argc, Gua_Object *argv)
{
    Gua_Short i;
    
    for (i = 0; i < argc; i++) {
        if (!Gua_IsObjectStored(argv[i])) {
            Gua_FreeObject(&argv[i]);
        }
    }
    
    Gua_Free(argv);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseObject(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses objects: integer, real, string, parenthesis, variables,
 *     functions, macros, indirection, comments
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseObject(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String lastStart;
    Gua_String expression;
    Gua_Object operand;
    Gua_String name;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object argObject;
    Gua_Object strObject;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand);
    Gua_ClearPObject(object);
    Gua_ClearObject(argObject);
    Gua_ClearObject(strObject);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        if (token->status == GUA_ERROR_UNEXPECTED_TOKEN) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
            strcat(error, errMessage);
            Gua_Free(errMessage);
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
        
        return p;
    }
    
    /* Parse an INTEGER object. */
    if (token->type == TOKEN_TYPE_INTEGER) {
        Gua_IntegerToPObject(object, token->integer);
    /* Parse a REAL object. */
    } else if (token->type == TOKEN_TYPE_REAL) {
        Gua_RealToPObject(object, token->real);
    /* Parse a STRING object. */
    } else if (token->type == TOKEN_TYPE_STRING) {
        Gua_SetPObjectType(object, OBJECT_TYPE_STRING);
        object->string = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(object->string, '\0', sizeof(char) * (token->length + 1));
        Gua_ScanString(object->string, token->start, token->length);
        Gua_SetPObjectLength(object, strlen(Gua_PObjectToString(object)));
    /* Parse a SCRIPT object. */
    } else if (token->type == TOKEN_TYPE_SCRIPT) {
        Gua_SetPObjectType(object, OBJECT_TYPE_STRING);
        object->string = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(object->string, '\0', sizeof(char) * (token->length + 1));
        memcpy(object->string, token->start, token->length);
        Gua_SetPObjectLength(object, strlen(Gua_PObjectToString(object)));
    /* Parse PARENTHESIS. */
    } else if (token->type == TOKEN_TYPE_PARENTHESIS) {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
            
            Gua_Evaluate(nspace, expression, object, status, error);
            
            Gua_Free(expression);
        }
    /* Parse BRACKETS. */
    } else if (token->type == TOKEN_TYPE_BRACKET) {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
            
            argc = Gua_CountMatrixElements(expression);
            argc += 3;
            argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
            
            Gua_ClearArguments(argc, argv);
            
            Gua_LinkStringToObject(argv[0], "matrix2D");
            Gua_SetStoredObject(argv[0]);
            
            if (Gua_ParseMatrixElements(nspace, expression, argc, argv, status, error) == GUA_OK) {
                if (Gua_BuiltInFunction(nspace, argc, argv, object, error) != GUA_OK) {
                    Gua_FreeObject(object);
                    *status = GUA_ERROR;
                }
            } else {
                *status = GUA_ERROR;
            }
            
            Gua_FreeArguments(argc, argv);
            Gua_Free(expression);
        }
    /* Parse BRACES. */
    } else if (token->type == TOKEN_TYPE_BRACE) {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
            
            argc = Gua_CountArguments(expression);
            argc++;
            argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
            
            Gua_ClearArguments(argc, argv);
            
            Gua_LinkStringToObject(argv[0], "array");
            Gua_SetStoredObject(argv[0]);
            
            if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                if (Gua_BuiltInFunction(nspace, argc, argv, object, error) != GUA_OK) {
                    Gua_FreeObject(object);
                    *status = GUA_ERROR;
                }
            } else {
                *status = GUA_ERROR;
            }
            
            Gua_FreeArguments(argc, argv);
            Gua_Free(expression);
        }
    /* Parse a VARIABLE. */
    } else if (token->type == TOKEN_TYPE_VARIABLE) {
        name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(name, '\0', sizeof(char) * (token->length + 1));
        strncpy(name, token->start, token->length);
        
        if (Gua_GetVariable(nspace, name, object, SCOPE_STACK) == OBJECT_TYPE_UNKNOWN) {
            if (strcmp(name, LEXEME_NULL) != 0) {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "unknown variable", name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        }
        
        lastStart = p;
        
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(name);
            
            return p;
        }
        
        /* Parse a STRING, ARRAY or MATRIX. */
        if (token->type == TOKEN_TYPE_BRACKET) {
            if (token->length > 0) {
                /* Parse an ARRAY. */
                if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                    strncpy(expression, token->start, token->length);
                    
                    argc = Gua_CountArguments(expression);
                    argc++;
                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                    
                    Gua_ClearArguments(argc, argv);
                    
                    Gua_LinkFromPObject(argv[0], object);
                    
                    if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                        if (argc > 2) {
                            Gua_ArgsToString(argc, argv, &argObject);
                        } else if (argc == 2) {
                            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                            } else {
                                Gua_LinkObjects(argObject, argv[1]);
                            }
                            Gua_SetStoredObject(argObject);
                        }
                        
                        if (Gua_GetArrayElement(&argv[0], &argObject, object) == OBJECT_TYPE_UNKNOWN) {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    }
                    
                    Gua_FreeArguments(argc, argv);
                    Gua_FreeObject(&argObject);
                    Gua_Free(expression);
                /* Parse a MATRIX. */
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                    strncpy(expression, token->start, token->length);
                    
                    argc = Gua_CountArguments(expression);
                    argc += 2;
                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                    
                    Gua_ClearArguments(argc, argv);
                    
                    Gua_LinkStringToObject(argv[0], "getMatrixElement");
                    Gua_SetStoredObject(argv[0]);
                    
                    Gua_LinkFromPObject(argv[1], object);
                    
                    if (Gua_ParseArguments(nspace, expression, argc, argv, 2, status, error) == GUA_OK) {
                        if (Gua_BuiltInFunction(nspace, argc, argv, object, error) != GUA_OK) {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    }
                    
                    Gua_FreeArguments(argc, argv);
                    Gua_Free(expression);
                /* Parse a STRING. */
                } else if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                    strncpy(expression, token->start, token->length);
                    
                    Gua_Evaluate(nspace, expression, &argObject, status, error);
                    
                    if (*status == GUA_OK) {
                        if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                            if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_PObjectLength(object))) {
                                Gua_ByteArrayToObject(strObject, (Gua_String)(Gua_PObjectToString(object) + Gua_ObjectToInteger(argObject)), 1);
                                Gua_LinkToPObject(object, strObject);
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    }
                    Gua_Free(expression);
                    Gua_FreeObject(&argObject);
                }
            /* An empty BRACKET. */
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "no index specified");
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        } else {
            p = lastStart;
        }
        
        Gua_Free(name);
    /* Parse a FUNCTION. */
    } else if (token->type == TOKEN_TYPE_FUNCTION) {
        name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(name, '\0', sizeof(char) * (token->length + 1));
        strncpy(name, token->start, token->length);
                        
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(name);
            
            return p;
        }
        
        /* Parse FUNCTION arguments. */
        if (token->type == TOKEN_TYPE_PARENTHESIS) {
            if (token->length > 0) {
                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(expression, '\0', sizeof(char) * (token->length + 1));
                strncpy(expression, token->start, token->length);
                
                argc = Gua_CountArguments(expression);
                argc++;
                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                
                Gua_ClearArguments(argc, argv);
                
                Gua_LinkStringToObject(argv[0], name);
                
                if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                    if (Gua_GetFunction(nspace, name, &function) == GUA_OK) {
                        if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                            if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                if (!Gua_IsPObjectStored(object)) {
                                    Gua_FreeObject(object);
                                }
                            }
                        }
                    } else {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "undefined function", name);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                    }
                }
                
                Gua_Free(expression);
                Gua_FreeArguments(argc, argv);
            /* The FUNCTION has no arguments. */
            } else {
                argc = 1;
                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                
                Gua_ClearArguments(argc, argv);
                
                Gua_LinkStringToObject(argv[0], name);
                
                if (Gua_GetFunction(nspace, name, &function) == GUA_OK) {
                    if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                        if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                            if (!Gua_IsPObjectStored(object)) {
                                Gua_FreeObject(object);
                            }
                        }
                    }
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "undefined function", name);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
                
                Gua_FreeArguments(argc, argv);
            }
        /* The FUNCTION has no arguments. */
        } else {
            argc = 1;
            argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
            
            Gua_ClearArguments(argc, argv);
            
            Gua_LinkStringToObject(argv[0], name);
            
            if (Gua_GetFunction(nspace, name, &function) == GUA_OK) {
                if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                        if (!Gua_IsPObjectStored(object)) {
                            Gua_FreeObject(object);
                        }
                    }
                }
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "undefined function", name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            
            Gua_FreeArguments(argc, argv);
        }
    /* Parse a MACRO substitution. */
    } else if (token->type == TOKEN_TYPE_MACRO) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            if (token->status == GUA_ERROR_UNEXPECTED_TOKEN) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            } else {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            
            return p;
        }
        
        /* Get MACRO substitution expression. */
        if (token->type == TOKEN_TYPE_PARENTHESIS) {
            if (token->length > 0) {
                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(expression, '\0', sizeof(char) * (token->length + 1));
                strncpy(expression, token->start, token->length);
                
                Gua_Evaluate(nspace, expression, &operand, status, error);
                
                Gua_Free(expression);
                
                if (*status != GUA_OK) {
                    return p;
                }
                
                if (Gua_ObjectType(operand) == OBJECT_TYPE_STRING) {
                    if (Gua_GetVariable(nspace, Gua_ObjectToString(operand), object, SCOPE_STACK) != OBJECT_TYPE_UNKNOWN) {
                        lastStart = p;
                        
                        p = Gua_NextToken(nspace, p, token);
                        
                        if (token->status != GUA_OK) {
                            *status = token->status;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                            
                            if (!Gua_IsObjectStored(operand)) {
                                Gua_FreeObject(&operand);
                            }
                            
                            return p;
                        }
                        
                        /* A MACRO substitution can be performed to reach an array or matrix element. */
                        if (token->type == TOKEN_TYPE_BRACKET) {
                            if (token->length > 0) {
                                /* An ARRAY MACRO substitution. */
                                if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                                    strncpy(expression, token->start, token->length);
                                    
                                    argc = Gua_CountArguments(expression);
                                    argc++;
                                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                    
                                    Gua_ClearArguments(argc, argv);
                                    
                                    Gua_LinkFromPObject(argv[0], object);
                                    
                                    if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                                        if (argc > 2) {
                                            Gua_ArgsToString(argc, argv, &argObject);
                                        } else if (argc == 2) {
                                            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                                Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                                            } else {
                                                Gua_LinkObjects(argObject, argv[1]);
                                            }
                                            Gua_SetStoredObject(argObject);
                                        }
                                        
                                        if (Gua_GetArrayElement(&argv[0], &argObject, object) == OBJECT_TYPE_UNKNOWN) {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    }
                                    
                                    Gua_Free(expression);
                                    Gua_FreeArguments(argc, argv);
                                    Gua_FreeObject(&argObject);
                                /* A MATRIX MACRO substitution. */
                                } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                                    strncpy(expression, token->start, token->length);
                                    
                                    argc = Gua_CountArguments(expression);
                                    argc += 2;
                                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                    
                                    Gua_ClearArguments(argc, argv);
                                    
                                    Gua_LinkStringToObject(argv[0], "getMatrixElement");
                                    Gua_SetStoredObject(argv[1]);
                                    
                                    Gua_LinkFromPObject(argv[1], object);
                                    
                                    if (Gua_ParseArguments(nspace, expression, argc, argv, 2, status, error) == GUA_OK) {
                                        if (Gua_BuiltInFunction(nspace, argc, argv, object, error) != GUA_OK) {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    }
                                    
                                    Gua_Free(expression);
                                    Gua_FreeArguments(argc, argv);
                                /* A STRING MACRO substitution. */
                                } else if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                                    strncpy(expression, token->start, token->length);
                                    
                                    Gua_Evaluate(nspace, expression, &argObject, status, error);
                                    
                                    if (*status == GUA_OK) {
                                        if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                                            if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_PObjectLength(object))) {
                                                Gua_ByteArrayToObject(strObject, (Gua_String)(Gua_PObjectToString(object) + Gua_ObjectToInteger(argObject)), 1);
                                                Gua_LinkToPObject(object, strObject);
                                            } else {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    }
                                    
                                    Gua_Free(expression);
                                    Gua_FreeObject(&argObject);
                                }
                            /* An empty BRACKET. */
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", "no index specified");
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        } else {
                            p = lastStart;
                        }
                        
                        if (!Gua_IsObjectStored(operand)) {
                            Gua_FreeObject(&operand);
                        }
                    /* Parse a FUNCTION INDIRECTION. */
                    } else {
                        if (Gua_GetFunction(nspace, Gua_ObjectToString(operand), &function) == GUA_OK) {
                            lastStart = p;
                            
                            p = Gua_NextToken(nspace, p, token);
                            
                            if (token->status != GUA_OK) {
                                *status = token->status;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                                
                                if (!Gua_IsObjectStored(operand)) {
                                    Gua_FreeObject(&operand);
                                }
                                
                                return p;
                            }
                            
                            /* Parse FUNCTION arguments. */
                            if (token->type == TOKEN_TYPE_PARENTHESIS) {
                                if (token->length > 0) {
                                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                                    strncpy(expression, token->start, token->length);
                                    
                                    argc = Gua_CountArguments(expression);
                                    argc++;
                                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                    
                                    Gua_ClearArguments(argc, argv);
                                    
                                    Gua_LinkObjects(argv[0], operand);
                                    
                                    if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                                        if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                                            if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                                if (!Gua_IsPObjectStored(object)) {
                                                    Gua_FreeObject(object);
                                                }
                                            }
                                        }
                                    }
                                    
                                    Gua_Free(expression);
                                    Gua_FreeArguments(argc, argv);
                                /* The FUNCTION has no arguments. */
                                } else {
                                    argc = 1;
                                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                    
                                    Gua_ClearArguments(argc, argv);
                                    
                                    Gua_LinkObjects(argv[0], operand);
                                    
                                    if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                                        if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                            if (!Gua_IsPObjectStored(object)) {
                                                Gua_FreeObject(object);
                                            }
                                        }
                                    }
                                    
                                    Gua_FreeArguments(argc, argv);
                                }
                            /* The FUNCTION has no arguments. */
                            } else {
                                argc = 1;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                Gua_LinkObjects(argv[0], operand);
                                
                                if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                                    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                        if (!Gua_IsPObjectStored(object)) {
                                            Gua_FreeObject(object);
                                        }
                                    }
                                }
                                
                                Gua_FreeArguments(argc, argv);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %-.20s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], Gua_ObjectToString(operand));
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                            
                            if (!Gua_IsObjectStored(operand)) {
                                Gua_FreeObject(&operand);
                            }
                        }
                    }
                /* A MACRO substitution requires an STRING expression. */
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", "illegal variable name");
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    if (Gua_ObjectType(operand) == OBJECT_TYPE_ARRAY) {
                        if (!Gua_IsObjectStored(operand)) {
                            Gua_FreeArray((Gua_Element *)Gua_ObjectToArray(operand));
                        }
                    }
                }
            }
        /* Parse a VARIABLE MACRO substitution. */
        } else if (token->type == TOKEN_TYPE_VARIABLE) {
            name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(name, '\0', sizeof(char) * (token->length + 1));
            strncpy(name, token->start, token->length);
            
            if (Gua_GetVariable(nspace, name, object, SCOPE_GLOBAL) == OBJECT_TYPE_UNKNOWN) {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "unknown variable", name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            
            lastStart = p;
            
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(name);
                
                return p;
            }
            
            /* Parse a STRING, ARRAY or MATRIX VARIABLE MACRO substitution. */
            if (token->type == TOKEN_TYPE_BRACKET) {
                if (token->length > 0) {
                    /* Parse an ARRAY VARIABLE. */
                    if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        argc = Gua_CountArguments(expression);
                        argc++;
                        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                        
                        Gua_ClearArguments(argc, argv);
                        
                        Gua_LinkFromPObject(argv[0], object);
                        
                        if(Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                            if (argc > 2) {
                                Gua_ArgsToString(argc, argv, &argObject);
                            } else if (argc == 2) {
                                if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                    Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                                } else {
                                    Gua_LinkObjects(argObject, argv[1]);
                                }
                                Gua_SetStoredObject(argObject);
                            }
                            
                            if (Gua_GetArrayElement(&argv[0], &argObject, object) == OBJECT_TYPE_UNKNOWN) {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                        Gua_FreeObject(&argObject);
                    /* Parse a MATRIX VARIABLE. */
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        argc = Gua_CountArguments(expression);
                        argc += 2;
                        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                        
                        Gua_ClearArguments(argc, argv);
                        
                        Gua_LinkStringToObject(argv[0], "getMatrixElement");
                        Gua_SetStoredObject(argv[0]);
                        
                        Gua_LinkFromPObject(argv[1], object);
                        
                        if(Gua_ParseArguments(nspace, expression, argc, argv, 2, status, error) == GUA_OK) {
                            if (Gua_BuiltInFunction(nspace, argc, argv, object, error) != GUA_OK) {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                    /* A STRING MACRO substitution. */
                    } else if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        Gua_Evaluate(nspace, expression, &argObject, status, error);
                        
                        if (*status == GUA_OK) {
                            if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                                if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_PObjectLength(object))) {
                                    Gua_ByteArrayToObject(strObject, (Gua_String)(Gua_PObjectToString(object) + Gua_ObjectToInteger(argObject)), 1);
                                    Gua_LinkToPObject(object, strObject);
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeObject(&argObject);
                    }
                /* An empty BRACKET. */
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", "no index specified");
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            /* It is not an ARRAY or MATRIX VARIABLE MACRO substitution. */
            } else {
                p = lastStart;
            }
            
            Gua_Free(name);
        /* The MACRO substitution resulted in an unexpected token. */
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
    /* Parse an INDIRECTION. */
    } else if (token->type == TOKEN_TYPE_INDIRECTION) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            if (token->status == GUA_ERROR_UNEXPECTED_TOKEN) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            } else {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            
            return p;
        }

        /* Parse a VARIABLE. */
        if (token->type == TOKEN_TYPE_VARIABLE) {
            name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(name, '\0', sizeof(char) * (token->length + 1));
            strncpy(name, token->start, token->length);
            
            if (Gua_GetVariable(nspace, name, &operand, SCOPE_STACK) == OBJECT_TYPE_UNKNOWN) {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "unknown variable", name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(name);
                
                return p;            
            }
            
            Gua_Free(name);
            
            /* A VARIABLE INDIRECTION must be performed from an STRING expression. */
            if (Gua_ObjectType(operand) == OBJECT_TYPE_STRING) {
                if (Gua_GetVariable(nspace, Gua_ObjectToString(operand), object, SCOPE_GLOBAL) != OBJECT_TYPE_UNKNOWN) {
                    lastStart = p;
                    
                    p = Gua_NextToken(nspace, p, token);
                    
                    if (token->status != GUA_OK) {
                        *status = token->status;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        if (!Gua_IsObjectStored(operand)) {
                            Gua_FreeObject(&operand);
                        }
                        
                        return p;
                    }
                    
                    /* Parse a STRING, ARRAY or MATRIX VARIABLE INDIRECTION. */
                    if (token->type == TOKEN_TYPE_BRACKET) {
                        if (token->length > 0) {
                            /* Parse an ARRAY VARIABLE INDIRECTION. */
                            if (Gua_PObjectType(object) == OBJECT_TYPE_ARRAY) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                argc = Gua_CountArguments(expression);
                                argc++;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                Gua_LinkFromPObject(argv[0], object);
                                
                                if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                                    if (argc > 2) {
                                        Gua_ArgsToString(argc, argv, &argObject);
                                    } else if (argc == 2) {
                                        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                            Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                                        } else {
                                            Gua_LinkObjects(argObject, argv[1]);
                                        }
                                        Gua_SetStoredObject(argObject);
                                    }
                                    
                                    if (Gua_GetArrayElement(&argv[0], &argObject, object) == OBJECT_TYPE_UNKNOWN) {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                }
                                
                                Gua_FreeArguments(argc, argv);
                                Gua_FreeObject(&argObject);
                                Gua_Free(expression);
                            /* Parse a MATRIX VARIABLE INDIRECTION. */
                            } else if (Gua_PObjectType(object) == OBJECT_TYPE_MATRIX) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                argc = Gua_CountArguments(expression);
                                argc += 2;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                Gua_LinkStringToObject(argv[0], "getMatrixElement");
                                Gua_SetStoredObject(argv[0]);
                                
                                Gua_LinkFromPObject(argv[1], object);
                                
                                if (Gua_ParseArguments(nspace, expression, argc, argv, 2, status, error) == GUA_OK) {
                                    if (Gua_BuiltInFunction(nspace, argc, argv, object, error) != GUA_OK) {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "index out of bound", expression);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                }
                                
                                Gua_FreeArguments(argc, argv);
                                Gua_Free(expression);
                            /* A STRING MACRO substitution. */
                            } else if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                Gua_Evaluate(nspace, expression, &argObject, status, error);
                                
                                if (*status == GUA_OK) {
                                    if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                                        if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_PObjectLength(object))) {
                                            Gua_ByteArrayToObject(strObject, (Gua_String)(Gua_PObjectToString(object) + Gua_ObjectToInteger(argObject)), 1);
                                            Gua_LinkToPObject(object, strObject);
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                }
                                
                                Gua_Free(expression);
                                Gua_FreeObject(&argObject);
                            }
                        /* No index was specified for an ARRAY or MATRIX VARIABLE INDIRECTION. */
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", "no index specified");
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    /* It is not an ARRAY or MATRIX VARIABLE INDIRECTION. */
                    } else {
                        p = lastStart;
                    }
                    
                    if (!Gua_IsObjectStored(operand)) {
                        Gua_FreeObject(&operand);
                    }
                /* Parse a FUNCTION INDIRECTION. */
                } else {
                    if (Gua_GetFunction(nspace, Gua_ObjectToString(operand), &function) == GUA_OK) {
                        lastStart = p;
                        
                        p = Gua_NextToken(nspace, p, token);
                        
                        if (token->status != GUA_OK) {
                            *status = token->status;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                            
                            if (!Gua_IsObjectStored(operand)) {
                                Gua_FreeObject(&operand);
                            }
                            
                            return p;
                        }
                        
                        /* Parse FUNCTION arguments. */
                        if (token->type == TOKEN_TYPE_PARENTHESIS) {
                            if (token->length > 0) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                argc = Gua_CountArguments(expression);
                                argc++;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                Gua_LinkObjects(argv[0], operand);
                                
                                if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                                    if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                                        if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                            if (!Gua_IsPObjectStored(object)) {
                                                Gua_FreeObject(object);
                                            }
                                        }
                                    }
                                }
                                
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                            /* The FUNCTION has no arguments. */
                            } else {
                                argc = 1;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                Gua_LinkObjects(argv[0], operand);
                                
                                if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                                    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                        if (!Gua_IsPObjectStored(object)) {
                                            Gua_FreeObject(object);
                                        }
                                    }
                                }
                                
                                Gua_FreeArguments(argc, argv);
                            }
                        /* The FUNCTION has no arguments. */
                        } else {
                            argc = 1;
                            argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                            
                            Gua_ClearArguments(argc, argv);
                            
                            Gua_LinkObjects(argv[0], operand);
                            
                            if ((*status = function.pointer(nspace, argc, argv, object, error)) != GUA_OK) {
                                if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                                    if (!Gua_IsPObjectStored(object)) {
                                        Gua_FreeObject(object);
                                    }
                                }
                            }
                            
                            Gua_FreeArguments(argc, argv);
                        }
                    } else {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], Gua_ObjectToString(operand));
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        if (!Gua_IsObjectStored(operand)) {
                            Gua_FreeObject(&operand);
                        }
                    }
                }
            /* The VARIABLE INDIRECTION resulted in an illegal variable name. */
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "illegal variable name");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (!Gua_IsObjectStored(operand)) {
                    Gua_FreeObject(&operand);
                }
            }
        }
    /* Nothin to do with a SEPARATORS or COMMENTS. */
    } else if (token->type == TOKEN_TYPE_SEPARATOR) {
    } else if (token->type == TOKEN_TYPE_COMMENT) {
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseNot(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: !, ~
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseNot(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand);
    
    op = token->type;
    
    if (op == TOKEN_TYPE_NOT) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        p = Gua_ParseObject(nspace, p, token, &operand, status, error);
        
        if (*status != GUA_OK) {
            if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                Gua_LinkToPObject(object, operand);
            }
            return p;
        }
        
        if (Gua_ObjectType(operand) != OBJECT_TYPE_INTEGER) {
            if (!Gua_IsObjectStored(operand)) {
                Gua_FreeObject(&operand);
            }
            
            *status = GUA_ERROR_ILLEGAL_OPERAND;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        Gua_IntegerToPObject(object, !Gua_ObjectToInteger(operand));
    } else if (op == TOKEN_TYPE_BIT_NOT) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        p = Gua_ParseObject(nspace, p, token, &operand, status, error);
        
        if (*status != GUA_OK) {
            if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                Gua_LinkToPObject(object, operand);
            }
            return p;
        }
        
        if (Gua_ObjectType(operand) != OBJECT_TYPE_INTEGER) {
            if (!Gua_IsObjectStored(operand)) {
                Gua_FreeObject(&operand);
            }
            
            *status = GUA_ERROR_ILLEGAL_OPERAND;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        Gua_IntegerToPObject(object, ~Gua_ObjectToInteger(operand));
    } else {
        p = Gua_ParseObject(nspace, p, token, &operand, status, error);
        
        Gua_LinkToPObject(object, operand);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseUnaryPlus(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: unary +, -
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseUnaryPlus(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand);
    
    op = token->type;
    
    if (op == TOKEN_TYPE_PLUS) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        p = Gua_ParseNot(nspace, p, token, &operand, status, error);
        
        if (*status != GUA_OK) {
            if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                Gua_LinkToPObject(object, operand);
            }
            return p;
        }
        
        if (!((Gua_ObjectType(operand) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand) == OBJECT_TYPE_MATRIX))) {
            if (!Gua_IsObjectStored(operand)) {
                Gua_FreeObject(&operand);
            }
            
            *status = GUA_ERROR_ILLEGAL_OPERAND;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (Gua_ObjectType(operand) == OBJECT_TYPE_INTEGER) {
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand));
        } else if (Gua_ObjectType(operand) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, Gua_ObjectToReal(operand));
        } else if (Gua_ObjectType(operand) == OBJECT_TYPE_COMPLEX) {
            Gua_ComplexToPObject(object, Gua_ObjectToReal(operand), Gua_ObjectToImaginary(operand));
        } else if (Gua_ObjectType(operand) == OBJECT_TYPE_MATRIX) {
            Gua_LinkToPObject(object, operand);
        }
    } else if (op == TOKEN_TYPE_MINUS) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        p = Gua_ParseNot(nspace, p, token, &operand, status, error);
        
        if (*status != GUA_OK) {
            if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                Gua_LinkToPObject(object, operand);
            }
            return p;
        }
        
        if (!((Gua_ObjectType(operand) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand) == OBJECT_TYPE_MATRIX))) {
            if (!Gua_IsObjectStored(operand)) {
                Gua_FreeObject(&operand);
            }
            
            *status = GUA_ERROR_ILLEGAL_OPERAND;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (Gua_ObjectType(operand) == OBJECT_TYPE_INTEGER) {
            Gua_IntegerToPObject(object, -Gua_ObjectToInteger(operand));
        } else if (Gua_ObjectType(operand) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, -Gua_ObjectToReal(operand));
        } else if (Gua_ObjectType(operand) == OBJECT_TYPE_COMPLEX) {
            Gua_ComplexToPObject(object, -Gua_ObjectToReal(operand), -Gua_ObjectToImaginary(operand));
        } else if (Gua_ObjectType(operand) == OBJECT_TYPE_MATRIX) {
            Gua_ClearPObject(object);

            if ((*status = Gua_NegMatrix(&operand, object, error)) != GUA_OK) {
                Gua_LinkToPObject(object, operand);
                return p;
            }

            if (!Gua_IsObjectStored(operand)) {
                Gua_FreeObject(&operand);
            }
        }
    } else {
        p = Gua_ParseNot(nspace, p, token, &operand, status, error);

        Gua_LinkToPObject(object, operand);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParsePower(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: **
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParsePower(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Real a;
    Gua_Real r;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseUnaryPlus(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_POWER) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_POWER) {
            p = Gua_ParseUnaryPlus(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL))) {
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, pow(Gua_ObjectToInteger(operand1), Gua_ObjectToInteger(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, pow(Gua_ObjectToReal(operand1), Gua_ObjectToReal(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, pow(Gua_ObjectToInteger(operand1), Gua_ObjectToReal(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToPObject(object, pow(Gua_ObjectToReal(operand1), Gua_ObjectToInteger(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                object->type = OBJECT_TYPE_COMPLEX;
                r = sqrt(Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand1) + Gua_ObjectToImaginary(operand1) * Gua_ObjectToImaginary(operand1));
                a = asin(Gua_ObjectToImaginary(operand1) / r);
                
                Gua_ComplexToPObject(object, pow(r, Gua_ObjectToInteger(operand2)) * cos(a * Gua_ObjectToInteger(operand2)), pow(r, Gua_ObjectToInteger(operand2)) * sin(a * Gua_ObjectToInteger(operand2)))
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                r = sqrt(Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand1) + Gua_ObjectToImaginary(operand1) * Gua_ObjectToImaginary(operand1));
                a = asin(Gua_ObjectToImaginary(operand1) / r);
                
                Gua_ComplexToPObject(object, pow(r, Gua_ObjectToReal(operand2)) * cos(a * Gua_ObjectToReal(operand2)), pow(r, Gua_ObjectToReal(operand2)) * sin(a * Gua_ObjectToReal(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_ClearPObject(object);
                
                if ((*status = Gua_PowMatrix(&operand1, Gua_ObjectToInteger(operand2), object, error)) != GUA_OK) {
                    Gua_ClearPObject(object);
                    return p;
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else {
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseMult(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: *,/,%
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseMult(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Object inverse;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    Gua_ClearObject(inverse);
    
    p = Gua_ParsePower(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while ((op == TOKEN_TYPE_MULT) || (op == TOKEN_TYPE_DIV) || (op == TOKEN_TYPE_MOD)) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_MULT) {
            p = Gua_ParsePower(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX))) {
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) * Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToInteger(operand1) * Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) * Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand2) - Gua_ObjectToImaginary(operand1) * Gua_ObjectToImaginary(operand2), Gua_ObjectToReal(operand1) * Gua_ObjectToImaginary(operand2) + Gua_ObjectToImaginary(operand1) * Gua_ObjectToReal(operand2));
                
                if (Gua_PObjectToImaginary(object) == 0) {
                    Gua_SetPObjectType(object, OBJECT_TYPE_REAL);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand1) * Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand2), Gua_ObjectToReal(operand1) * Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) * Gua_ObjectToInteger(operand2), Gua_ObjectToImaginary(operand1) * Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToInteger(operand1) * Gua_ObjectToReal(operand2), Gua_ObjectToInteger(operand1) * Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                
                if ((*status = Gua_MulMatrix(&operand1, &operand2, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    return p;
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_DIV) {
            p = Gua_ParsePower(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX))) {
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) {
                if (Gua_ObjectToInteger(operand2) == 0) {
                    Gua_ClearPObject(object);
                    
                    *status = GUA_ERROR_DIVISION_BY_ZERO;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", "division by zero");
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return p;
                }
            } else if (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) {
                if (Gua_ObjectToReal(operand2) == 0.0) {
                    Gua_ClearPObject(object);
                    
                    *status = GUA_ERROR_DIVISION_BY_ZERO;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", "division by zero");
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return p;
                }
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) / Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) / Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToInteger(operand1) / Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) / Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, (Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand1) * Gua_ObjectToImaginary(operand2)) / (Gua_ObjectToReal(operand2) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand2) * Gua_ObjectToImaginary(operand2)), (Gua_ObjectToImaginary(operand1) * Gua_ObjectToReal(operand2) - Gua_ObjectToReal(operand1) * Gua_ObjectToImaginary(operand2)) / (Gua_ObjectToReal(operand2) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand2) * Gua_ObjectToImaginary(operand2)));

                if (Gua_PObjectToImaginary(object) == 0) {
                    Gua_SetPObjectType(object, OBJECT_TYPE_REAL);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) / Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand1) / Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, (Gua_ObjectToReal(operand1) * Gua_ObjectToReal(operand2)) / (Gua_ObjectToReal(operand2) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand2) * Gua_ObjectToImaginary(operand2)), -(Gua_ObjectToReal(operand1) * Gua_ObjectToImaginary(operand2)) / (Gua_ObjectToReal(operand2) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand2) * Gua_ObjectToImaginary(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) / Gua_ObjectToInteger(operand2), Gua_ObjectToImaginary(operand1) / Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, (Gua_ObjectToInteger(operand1) * Gua_ObjectToReal(operand2)) / (Gua_ObjectToReal(operand2) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand2) * Gua_ObjectToImaginary(operand2)), -(Gua_ObjectToInteger(operand1) * Gua_ObjectToImaginary(operand2)) / (Gua_ObjectToReal(operand2) * Gua_ObjectToReal(operand2) + Gua_ObjectToImaginary(operand2) * Gua_ObjectToImaginary(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                
                if ((*status = Gua_InvMatrix(&operand2, &inverse, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    if (!Gua_IsObjectStored(inverse)) {
                        Gua_FreeObject(&inverse);
                    }
                    return p;
                }
                if ((*status = Gua_MulMatrix(&inverse, &operand1, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    if (!Gua_IsObjectStored(inverse)) {
                        Gua_FreeObject(&inverse);
                    }
                    return p;
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                if (!Gua_IsObjectStored(inverse)) {
                    Gua_FreeObject(&inverse);
                }
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_MOD) {
            p = Gua_ParsePower(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if (operand2.integer == 0) {
                Gua_ClearPObject(object);
                
                *status = GUA_ERROR_DIVISION_BY_ZERO;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", "division by zero");
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) % Gua_ObjectToInteger(operand2));
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParsePlus(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: +,-
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParsePlus(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_String buffer1;
    Gua_String buffer2;
    Gua_String strptr;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseMult(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }

    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while ((op == TOKEN_TYPE_PLUS) || (op == TOKEN_TYPE_MINUS)) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_PLUS) {
            p = Gua_ParseMult(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                if (Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) {
                    if (!Gua_IsObjectStored(operand1)) {
                        Gua_FreeObject(&operand1);
                    }
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX))) {
                if (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY) {
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) + Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) + Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) + Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand1) + Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToInteger(operand1) + Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) + Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToInteger(operand1) + Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) + Gua_ObjectToInteger(operand2), Gua_ObjectToImaginary(operand1));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) + Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) + Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand1));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer1 = (char *)Gua_Alloc(sizeof(char) * (strlen(Gua_ObjectToString(operand1)) + strlen(Gua_ObjectToString(operand2)) + 1));
                memset(buffer1, '\0', sizeof(char) * (Gua_ObjectLength(operand1) + Gua_ObjectLength(operand2) + 1));
                
                memcpy(buffer1, Gua_ObjectToString(operand1), Gua_ObjectLength(operand1));
                strptr = buffer1 + Gua_ObjectLength(operand1);
                memcpy(strptr, Gua_ObjectToString(operand2), Gua_ObjectLength(operand2));
                strptr += Gua_ObjectLength(operand2);
                *strptr = '\0';
                
                Gua_ByteArrayToPObject(object, buffer1, (Gua_ObjectLength(operand1) + Gua_ObjectLength(operand2)));
                
                Gua_Free(buffer1);
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer1 = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer1, "%ld", Gua_ObjectToInteger(operand2));
                
                buffer2 = (char *)Gua_Alloc(sizeof(char) * (strlen(buffer1) + Gua_ObjectLength(operand1) + 1));
                memset(buffer2, '\0', sizeof(char) * (strlen(buffer1) + Gua_ObjectLength(operand1) + 1));
                
                memcpy(buffer2, Gua_ObjectToString(operand1), Gua_ObjectLength(operand1));
                strptr = buffer2 + Gua_ObjectLength(operand1);
                memcpy(strptr, buffer1, strlen(buffer1));
                strptr += strlen(buffer1);
                *strptr = '\0';
                
                Gua_ByteArrayToPObject(object, buffer2, Gua_ObjectLength(operand1) + strlen(buffer1));
                
                Gua_Free(buffer1);
                Gua_Free(buffer2);
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer1 = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer1, "%g", Gua_ObjectToReal(operand2));
                
                buffer2 = (char *)Gua_Alloc(sizeof(char) * (strlen(buffer1) + Gua_ObjectLength(operand1) + 1));
                memset(buffer2, '\0', sizeof(char) * (strlen(buffer1) + Gua_ObjectLength(operand1) + 1));
                
                memcpy(buffer2, Gua_ObjectToString(operand1), Gua_ObjectLength(operand1));
                strptr = buffer2 + Gua_ObjectLength(operand1);
                memcpy(strptr, buffer1, strlen(buffer1));
                strptr += strlen(buffer1);
                *strptr = '\0';
                
                Gua_ByteArrayToPObject(object, buffer2, Gua_ObjectLength(operand1) + strlen(buffer1));
                
                Gua_Free(buffer1);
                Gua_Free(buffer2);
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer1 = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer1, "%ld", Gua_ObjectToInteger(operand1));
                
                buffer2 = (char *)Gua_Alloc(sizeof(char) * (strlen(buffer1) + Gua_ObjectLength(operand2) + 1));
                memset(buffer2, '\0', sizeof(char) * (strlen(buffer1) + Gua_ObjectLength(operand2) + 1));
                
                memcpy(buffer2, buffer1, strlen(buffer1));
                strptr = buffer2 + strlen(buffer1);
                memcpy(strptr, Gua_ObjectToString(operand2), Gua_ObjectLength(operand2));
                strptr += Gua_ObjectLength(operand2);
                *strptr = '\0';
                
                Gua_ByteArrayToPObject(object, buffer2, strlen(buffer1) + Gua_ObjectLength(operand2));
                
                Gua_Free(buffer1);
                Gua_Free(buffer2);
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer1 = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer1, "%g", Gua_ObjectToReal(operand1));
                
                buffer2 = (char *)Gua_Alloc(sizeof(char) * (strlen(buffer1) + strlen(Gua_ObjectToString(operand2)) + 1));
                memset(buffer2, '\0', sizeof(char) * (strlen(buffer1) + strlen(Gua_ObjectToString(operand2)) + 1));
                
                memcpy(buffer2, buffer1, strlen(buffer1));
                strptr = buffer2 + strlen(buffer1);
                memcpy(strptr, Gua_ObjectToString(operand2), Gua_ObjectLength(operand2));
                strptr += Gua_ObjectLength(operand2);
                *strptr = '\0';
                
                Gua_ByteArrayToPObject(object, buffer2, strlen(buffer1) + Gua_ObjectLength(operand2));
                
                Gua_Free(buffer1);
                Gua_Free(buffer2);
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                
                if ((*status = Gua_AddMatrix(&operand1, &operand2, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    return p;
                }
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_MINUS) {
            p = Gua_ParseMult(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                if (Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) {
                    Gua_FreeObject(&operand1);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX))) {
                if (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }

            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) - Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) - Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) - Gua_ObjectToReal(operand2), Gua_ObjectToImaginary(operand1) - Gua_ObjectToImaginary(operand2));
                
                if (Gua_PObjectToImaginary(object) == 0) {
                    Gua_SetPObjectType(object, OBJECT_TYPE_REAL);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_RealToPObject(object, Gua_ObjectToInteger(operand1) - Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_RealToPObject(object, Gua_ObjectToReal(operand1) - Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToInteger(operand1) - Gua_ObjectToReal(operand2), -Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) - Gua_ObjectToInteger(operand2), -Gua_ObjectToImaginary(operand1));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) - Gua_ObjectToReal(operand2), -Gua_ObjectToImaginary(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_ComplexToPObject(object, Gua_ObjectToReal(operand1) - Gua_ObjectToReal(operand2), -Gua_ObjectToImaginary(operand1));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                
                if ((*status = Gua_SubMatrix(&operand1, &operand2, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    return p;
                }
                
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseLeftShift(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: <<,>>
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseLeftShift(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParsePlus(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while ((op == TOKEN_TYPE_LEFT_SHIFT) || (op == TOKEN_TYPE_RIGHT_SHIFT)) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_LEFT_SHIFT) {
            p = Gua_ParsePlus(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            Gua_IntegerToPObject(object, operand1.integer << operand2.integer);
            
            op = token->type;
        } else if (op == TOKEN_TYPE_RIGHT_SHIFT) {
            p = Gua_ParsePlus(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            Gua_IntegerToPObject(object, operand1.integer >> operand2.integer);
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseLess(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: <,<=,>,>=
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseLess(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_String buffer;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseLeftShift(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while ((op == TOKEN_TYPE_LESS) || (op == TOKEN_TYPE_LE) || (op == TOKEN_TYPE_GREATER) || (op == TOKEN_TYPE_GE)) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_LESS) {
            p = Gua_ParseLeftShift(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX))) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) < Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) < Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) < Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) < Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(operand1) < Gua_ObjectLength(operand2)) {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand1)) < 0);
                } else {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand2)) < 0);
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) < 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) < 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) < 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) < 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) < Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) != OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) < Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_LE) {
            p = Gua_ParseLeftShift(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) <= Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) <= Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) <= Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) <= Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(operand1) < Gua_ObjectLength(operand2)) {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand1)) <= 0);
                } else {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand2)) <= 0);
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) <= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) <= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) <= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) <= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) <= Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) != OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) <= Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_GREATER) {
            p = Gua_ParseLeftShift(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) > Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) > Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) > Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) > Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(operand1) < Gua_ObjectLength(operand2)) {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand1)) > 0);
                } else {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand2)) > 0);
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) > 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) > 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) > 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) > 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) > Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) != OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) > Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_GE) {
            p = Gua_ParseLeftShift(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 1 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            if (!((Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL) || (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING) || (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX))) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) >= Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) >= Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) >= Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) >= Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(operand1) < Gua_ObjectLength(operand2)) {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand1)) >= 0);
                } else {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand2)) >= 0);
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) >= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) >= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) >= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) >= 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) >= Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) != OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, Gua_ObjectLength(operand1) >= Gua_ObjectLength(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseEqual(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: ==,!=
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseEqual(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_String buffer;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseLess(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while ((op == TOKEN_TYPE_EQ) || (op == TOKEN_TYPE_NE)) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_EQ) {
            p = Gua_ParseLess(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) == Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) == Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) == Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) == Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, (Gua_ObjectToReal(operand1) == Gua_ObjectToReal(operand2)) && (Gua_ObjectToImaginary(operand1) == Gua_ObjectToImaginary(operand2)));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(operand1) == Gua_ObjectLength(operand2)) {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand1)) == 0);
                } else {
                    Gua_IntegerToPObject(object, 0);
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) == 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) == 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) == 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) == 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, Gua_IsArrayEqual(&operand1, &operand2));
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) != OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, Gua_IsMatrixEqual(&operand1, &operand2));
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, 0);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand1);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        } else if (op == TOKEN_TYPE_NE) {
            p = Gua_ParseLess(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) != Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) != Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) != Gua_ObjectToReal(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToReal(operand1) != Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, !((Gua_ObjectToReal(operand1) == Gua_ObjectToReal(operand2)) && (Gua_ObjectToImaginary(operand1) == Gua_ObjectToImaginary(operand2))));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                if (Gua_ObjectLength(operand1) == Gua_ObjectLength(operand2)) {
                    Gua_IntegerToPObject(object, memcmp(Gua_ObjectToString(operand1), Gua_ObjectToString(operand2), Gua_ObjectLength(operand1)) != 0);
                } else {
                    Gua_IntegerToPObject(object, 1);
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) != 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand2));
                Gua_IntegerToPObject(object, strcmp(Gua_ObjectToString(operand1), buffer) != 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%ld", Gua_ObjectToInteger(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) != 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
                sprintf(buffer, "%g", Gua_ObjectToReal(operand1));
                Gua_IntegerToPObject(object, strcmp(buffer, Gua_ObjectToString(operand2)) != 0);
                Gua_Free(buffer);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 0);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_REAL)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_COMPLEX)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_STRING)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_UNKNOWN) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_UNKNOWN)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, !Gua_IsArrayEqual(&operand1, &operand2));
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) != OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_ARRAY) && (Gua_ObjectType(operand2) == OBJECT_TYPE_ARRAY)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, !Gua_IsMatrixEqual(&operand1, &operand2));
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) != OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
            } else if ((Gua_ObjectType(operand1) != OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_IntegerToPObject(object, 1);
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand1);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseBitAnd(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: &
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseBitAnd(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseEqual(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_BIT_AND) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_BIT_AND) {
            p = Gua_ParseEqual(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) & Gua_ObjectToInteger(operand2));
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseBitXOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: ^
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseBitXOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseBitAnd(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_BIT_XOR) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_BIT_XOR) {
            p = Gua_ParseBitAnd(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) ^ Gua_ObjectToInteger(operand2));
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseBitOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: |
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseBitOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseBitXOr(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_BIT_OR) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_BIT_OR) {
            p = Gua_ParseBitXOr(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if (!((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER))) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) | Gua_ObjectToInteger(operand2));
            
            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseLogicAndOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: &|
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseLogicAndOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseBitOr(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_AND_OR) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_AND_OR) {
            p = Gua_ParseBitOr(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                
                if ((*status = Gua_AndOrMatrix(&operand1, &operand2, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    return p;
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }

            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseLogicAnd(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: &&
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseLogicAnd(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseLogicAndOr(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_AND) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_AND) {
            p = Gua_ParseLogicAndOr(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) && Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                if ((*status = Gua_AndMatrix(&operand1, &operand2, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    return p;
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }

            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseLogicOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: ||
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseLogicOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object operand1;
    Gua_Object operand2;
    Gua_Short op;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(operand1);
    Gua_ClearObject(operand2);
    
    p = Gua_ParseLogicAnd(nspace, p, token, &operand1, status, error);
    
    if (*status != GUA_OK) {
        if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_LinkToPObject(object, operand1);
        }
        return p;
    }
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(operand1)) {
            Gua_FreeObject(&operand1);
        }
        
        return p;
    }
    
    Gua_LinkToPObject(object, operand1);
    
    op = token->type;
    
    while (op == TOKEN_TYPE_OR) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (op == TOKEN_TYPE_OR) {
            p = Gua_ParseLogicAnd(nspace, p, token, &operand2, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
                    Gua_LinkToPObject(object, operand2);
                }
                return p;
            }
            
            if ((Gua_ObjectType(operand1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(operand2) == OBJECT_TYPE_INTEGER)) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(operand1) || Gua_ObjectToInteger(operand2));
            } else if ((Gua_ObjectType(operand1) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(operand2) == OBJECT_TYPE_MATRIX)) {
                Gua_ClearPObject(object);
                if ((*status = Gua_OrMatrix(&operand1, &operand2, object, error)) != GUA_OK) {
                    Gua_LinkToPObject(object, operand1);
                    if (!Gua_IsObjectStored(operand2)) {
                        Gua_FreeObject(&operand2);
                    }
                    return p;
                }
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
            } else {
                if (!Gua_IsObjectStored(operand1)) {
                    Gua_FreeObject(&operand1);
                }
                if (!Gua_IsObjectStored(operand2)) {
                    Gua_FreeObject(&operand2);
                }
                
                *status = GUA_ERROR_ILLEGAL_OPERAND;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal operand for operator", Gua_TokenTable[op].symbol);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }

            op = token->type;
        }
        
        Gua_LinkFromPObject(operand1, object);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseAssignIndirection(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: =
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseAssignIndirection(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String expression;
    Gua_Object operand;
    Gua_Token firstToken;
    Gua_String name;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object argObject;
    Gua_Object variableObject;
    Gua_Short objectType;
    Gua_String errMessage;
    
    p = start;
    firstToken = *token;
    
    Gua_ClearObject(operand);
    Gua_ClearObject(argObject);
    Gua_ClearObject(variableObject);
    
    *status = GUA_OK;
    *error = '\0';
    
    /* Parses INDIRECTIONS. */
    if (token->type == TOKEN_TYPE_INDIRECTION) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            if (token->status == GUA_ERROR_UNEXPECTED_TOKEN) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            } else {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            
            return p;
        }

        if (token->type == TOKEN_TYPE_VARIABLE) {
            name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(name, '\0', sizeof(char) * (token->length + 1));
            strncpy(name, token->start, token->length);
            
            if (Gua_GetVariable(nspace, name, &operand, SCOPE_STACK) != OBJECT_TYPE_UNKNOWN) {
                Gua_Free(name);
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "unknown variable", name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(name);
                
                return p;
            }
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        if (Gua_ObjectType(operand) == OBJECT_TYPE_STRING) {
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (!Gua_IsObjectStored(operand)) {
                    Gua_FreeObject(&operand);
                }
                
                return p;
            }

            /* A simple VARIABLE name INDIRECTION assignment. */
            if (token->type == TOKEN_TYPE_ASSIGN) {
                p = Gua_NextToken(nspace, p, token);

                p = Gua_ParseAssign(nspace, p, token, object, status, error);

                if (*status != GUA_OK) {
                    if (!Gua_IsObjectStored(operand)) {
                        Gua_FreeObject(&operand);
                    }
                    return p;
                }

                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                    /* Try to set the VARIABLE. */
                    if (Gua_SetVariable(nspace, Gua_ObjectToString(operand), object, SCOPE_STACK) != GUA_OK) {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(operand));
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                    }
                } else {
                    /* Try to unset the VARIABLE. */
                    if (Gua_UnsetVariable(nspace, Gua_ObjectToString(operand), SCOPE_STACK) != GUA_OK) {
                        if (Gua_UnsetFunction(nspace, Gua_ObjectToString(operand)) != GUA_OK) {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %-.20s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], Gua_ObjectToString(operand));
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    }
                }
            /* It is a STRING, ARRAY or a MATRIX. */
            } else if (token->type == TOKEN_TYPE_BRACKET) {
                if (token->length > 0) {
                    objectType = Gua_GetVariable(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK);
                    
                    /* It is an ARRAY VARIABLE.*/
                    if ((objectType == OBJECT_TYPE_UNKNOWN) || (objectType == OBJECT_TYPE_ARRAY)) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        argc = Gua_CountArguments(expression);
                        argc++;
                        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                        
                        Gua_ClearArguments(argc, argv);
                        
                        if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            if (token->status != GUA_OK) {
                                *status = token->status;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                                
                                if (!Gua_IsObjectStored(operand)) {
                                    Gua_FreeObject(&operand);
                                }
                                Gua_Free(expression);
                                
                                return p;
                            }
                            
                            if (argc > 2) {
                                Gua_ArgsToString(argc, argv, &argObject);
                                Gua_SetStoredObject(argObject);
                            } else if (argc == 2) {
                                if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                    Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                                } else {
                                    Gua_LinkObjects(argObject, argv[1]);
                                }
                                Gua_SetStoredObject(argObject);
                            }
                            
                            if (token->type == TOKEN_TYPE_ASSIGN) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                
                                if (*status != GUA_OK) {
                                    if (!Gua_IsObjectStored(operand)) {
                                        Gua_FreeObject(&operand);
                                    }
                                    Gua_Free(expression);
                                    Gua_FreeObject(&argObject);
                                    return p;
                                }
                                
                                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                    if (objectType != OBJECT_TYPE_UNKNOWN) {
                                        if (Gua_SetArrayElement(&variableObject, &argObject, object, true) == GUA_OK) {
                                            if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        if (Gua_SetArrayElement(&variableObject, &argObject, object, false) == GUA_OK) {
                                            if (Gua_SetVariable(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    }
                                } else {
                                    if (objectType != OBJECT_TYPE_UNKNOWN) {
                                        if (Gua_UnsetArrayElement(&variableObject, &argObject) == GUA_OK) {
                                            if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                            if (Gua_ObjectLength(variableObject) == 0) {
                                                if (Gua_UnsetVariable(nspace, Gua_ObjectToString(operand), SCOPE_STACK) != GUA_OK) {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset array", Gua_ObjectToString(operand));
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                }
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't unset array", Gua_ObjectToString(operand));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't unset array", Gua_ObjectToString(operand));
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                }
                            } else {
                                *token = firstToken;
                                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                        Gua_FreeObject(&argObject);
                    /* It is a MATRIX VARIABLE.*/
                    } else if (objectType == OBJECT_TYPE_MATRIX) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        argc = Gua_CountArguments(expression);
                        argc += 3;
                        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                        
                        Gua_ClearArguments(argc, argv);
                        
                        Gua_LinkStringToObject(argv[0], "setMatrixElement");
                        Gua_SetStoredObject(argv[0]);
                        
                        if (Gua_ParseArguments(nspace, expression, argc, argv, 3, status, error) == GUA_OK) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            if (token->status != GUA_OK) {
                                *status = token->status;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                                
                                if (!Gua_IsObjectStored(operand)) {
                                    Gua_FreeObject(&operand);
                                }
                                if (!Gua_IsObjectStored(variableObject)) {
                                    Gua_FreeObject(&variableObject);
                                }
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                                
                                return p;
                            }
                            
                            if (token->type == TOKEN_TYPE_ASSIGN) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                
                                if (*status != GUA_OK) {
                                    if (!Gua_IsObjectStored(operand)) {
                                        Gua_FreeObject(&operand);
                                    }
                                    if (!Gua_IsObjectStored(variableObject)) {
                                        Gua_FreeObject(&variableObject);
                                    }
                                    Gua_Free(expression);
                                    Gua_FreeArguments(argc, argv);
                                    return p;
                                }
                                
                                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                    if (objectType != OBJECT_TYPE_UNKNOWN) {
                                        Gua_LinkObjects(argv[1], variableObject);
                                        Gua_LinkFromPObject(argv[2], object);
                                        
                                        if (Gua_BuiltInFunction(nspace, argc, argv, object, error) == GUA_OK) {
                                            if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", Gua_ObjectToString(operand));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", Gua_ObjectToString(operand));
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset matrix", Gua_ObjectToString(operand));
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                *token = firstToken;
                                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                    /* It is a STRING VARIABLE.*/
                    } else if (objectType == OBJECT_TYPE_STRING) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        Gua_Evaluate(nspace, expression, &argObject, status, error);
                        
                        if (*status == GUA_OK) {
                            if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                if (token->status != GUA_OK) {
                                    *status = token->status;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                    
                                    Gua_Free(expression);
                                    Gua_FreeObject(&argObject);
                                    
                                    return p;
                                }
                                
                                if (token->type == TOKEN_TYPE_ASSIGN) {
                                    p = Gua_NextToken(nspace, p, token);
                                    
                                    p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                    
                                    if (*status != GUA_OK) {
                                        Gua_Free(expression);
                                        Gua_FreeObject(&argObject);
                                        return p;
                                    }
                                    
                                    if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                        if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                                            if (Gua_PObjectLength(object) != 1) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[TOKEN_TYPE_ASSIGN].symbol);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                                
                                                Gua_Free(expression);
                                                Gua_FreeObject(&argObject);
                                                Gua_FreeObject(object);
                                                
                                                return p;
                                            }
                                        }
                                        
                                        if (objectType != OBJECT_TYPE_UNKNOWN) {
                                            if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_ObjectLength(variableObject))) {
                                                if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_STRING)) {
                                                    variableObject.string[Gua_ObjectToInteger(argObject)] = object->string[0];
                                                    if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set string", Gua_ObjectToString(operand));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_INTEGER)) {
                                                    variableObject.string[Gua_ObjectToInteger(argObject)] = Gua_PObjectToInteger(object);
                                                    if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set string", Gua_ObjectToString(operand));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                    
                                                    Gua_FreeObject(object);
                                                }
                                            } else {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                                
                                                Gua_FreeObject(object);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(operand));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                            
                                            Gua_FreeObject(object);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't unset string", Gua_ObjectToString(operand));
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                } else {
                                    *token = firstToken;
                                    p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                                }
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeObject(&argObject);
                    }
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            } else {
                if (!Gua_IsObjectStored(operand)) {
                    Gua_FreeObject(&operand);
                }
                
                *token = firstToken;
                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
            }
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal variable name", Gua_ObjectToString(operand));
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
        
        if (!Gua_IsObjectStored(operand)) {
            Gua_FreeObject(&operand);
        }
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseAssignMacro(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: =
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseAssignMacro(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String expression;
    Gua_Object operand;
    Gua_Token firstToken;
    Gua_String name;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object argObject;
    Gua_Object variableObject;
    Gua_Short objectType;
    Gua_String errMessage;
    
    p = start;
    firstToken = *token;
    
    Gua_ClearObject(operand);
    Gua_ClearObject(argObject);
    Gua_ClearObject(variableObject);

    *status = GUA_OK;
    *error = '\0';
    
    /* Parses MACRO substitutions. */
    if (token->type == TOKEN_TYPE_MACRO) {
        p = Gua_NextToken(nspace, p, token);
        
        /* It is a global VARIABLE. */
        if ((token->type == TOKEN_TYPE_UNKNOWN) || (token->type == TOKEN_TYPE_VARIABLE)) {
            name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(name, '\0', sizeof(char) * (token->length + 1));
            strncpy(name, token->start, token->length);
            
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(name);
                
                return p;
            }
            
            /* A simple global VARIABLE assignment. */
            if (token->type == TOKEN_TYPE_ASSIGN) {
                p = Gua_NextToken(nspace, p, token);
                
                p = Gua_ParseAssign(nspace, p, token, object, status, error);
                
                if (*status != GUA_OK) {
                    Gua_Free(name);
                    return p;
                }
                
                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                    /* Try to set the VARIABLE. */
                    if (Gua_SetVariable(nspace, name, object, SCOPE_GLOBAL) != GUA_OK) {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", name);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                    }
                } else {
                    /* Try to unset VARIABLE. */
                    if (Gua_UnsetVariable(nspace, name, SCOPE_GLOBAL) != GUA_OK) {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "can't unset variable", name);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                    }
                }
            /* It is a STRING, ARRAY or a MATRIX. */
            } else if (token->type == TOKEN_TYPE_BRACKET) {
                if (token->length > 0) {
                    objectType = Gua_GetVariable(nspace, name, &variableObject, SCOPE_GLOBAL);
                    
                    /* It is an ARRAY VARIABLE.*/
                    if ((objectType == OBJECT_TYPE_UNKNOWN) || (objectType == OBJECT_TYPE_ARRAY)) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        argc = Gua_CountArguments(expression);
                        argc++;
                        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                        
                        Gua_ClearArguments(argc, argv);
                        
                        if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            if (token->status != GUA_OK) {
                                *status = token->status;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                                
                                Gua_Free(name);
                                Gua_Free(expression);
                                
                                return p;
                            }
                            
                            if (argc > 2) {
                                Gua_ArgsToString(argc, argv, &argObject);
                                Gua_SetStoredObject(argObject);
                            } else if (argc == 2) {
                                if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                    Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                                } else {
                                    Gua_LinkObjects(argObject, argv[1]);
                                }
                                Gua_SetStoredObject(argObject);
                            }
                            
                            if (token->type == TOKEN_TYPE_ASSIGN) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                
                                if (*status != GUA_OK) {
                                    Gua_Free(name);
                                    Gua_Free(expression);
                                    Gua_FreeObject(&argObject);
                                    return p;
                                }
                                
                                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                    if (objectType != OBJECT_TYPE_UNKNOWN) {
                                        if (Gua_SetArrayElement(&variableObject, &argObject, object, true) == GUA_OK) {
                                            if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_GLOBAL) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        if (Gua_SetArrayElement(&variableObject, &argObject, object, false) == GUA_OK) {
                                            if (Gua_SetVariable(nspace, name, &variableObject, SCOPE_GLOBAL) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    }
                                } else {
                                    if (objectType != OBJECT_TYPE_UNKNOWN) {
                                        if (Gua_UnsetArrayElement(&variableObject, &argObject) == GUA_OK) {
                                            if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_GLOBAL) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                            if (Gua_ObjectLength(variableObject) == 0) {
                                                if (Gua_UnsetVariable(nspace, name, SCOPE_GLOBAL) != GUA_OK) {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset array", name);
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                }
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't unset array", name);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't unset array", name);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                }
                            } else {
                                *token = firstToken;
                                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                        Gua_FreeObject(&argObject);
                    /* It is a MATRIX VARIABLE.*/
                    } else if (objectType == OBJECT_TYPE_MATRIX) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        argc = Gua_CountArguments(expression);
                        argc += 3;
                        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                        
                        Gua_ClearArguments(argc, argv);
                        
                        Gua_LinkStringToObject(argv[0], "setMatrixElement");
                        Gua_SetStoredObject(argv[0]);
                        
                        if (Gua_ParseArguments(nspace, expression, argc, argv, 3, status, error) == GUA_OK) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            if (token->status != GUA_OK) {
                                *status = token->status;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                                
                                Gua_Free(name);
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                                
                                return p;
                            }
                            
                            if (token->type == TOKEN_TYPE_ASSIGN) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                
                                if (*status != GUA_OK) {
                                    Gua_Free(name);
                                    Gua_Free(expression);
                                    Gua_FreeArguments(argc, argv);
                                    return p;
                                }
                                
                                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                    if (objectType != OBJECT_TYPE_UNKNOWN) {
                                        Gua_LinkObjects(argv[1], variableObject);
                                        Gua_LinkFromPObject(argv[2], object);

                                        if (Gua_BuiltInFunction(nspace, argc, argv, object, error) == GUA_OK) {
                                            if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_GLOBAL) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", name);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "index out of bound", name);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", name);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset matrix", name);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                *token = firstToken;
                                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                    /* It is a STRING VARIABLE.*/
                    } else if (objectType == OBJECT_TYPE_STRING) {
                        expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(expression, '\0', sizeof(char) * (token->length + 1));
                        strncpy(expression, token->start, token->length);
                        
                        Gua_Evaluate(nspace, expression, &argObject, status, error);
                        
                        if (*status == GUA_OK) {
                            if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                if (token->status != GUA_OK) {
                                    *status = token->status;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                    
                                    Gua_Free(expression);
                                    Gua_FreeObject(&argObject);
                                    
                                    return p;
                                }
                                
                                if (token->type == TOKEN_TYPE_ASSIGN) {
                                    p = Gua_NextToken(nspace, p, token);
                                    
                                    p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                    
                                    if (*status != GUA_OK) {
                                        Gua_Free(expression);
                                        Gua_FreeObject(&argObject);
                                        return p;
                                    }
                                    
                                    if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                        if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                                            if (Gua_PObjectLength(object) != 1) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[TOKEN_TYPE_ASSIGN].symbol);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                                
                                                Gua_Free(expression);
                                                Gua_FreeObject(&argObject);
                                                Gua_FreeObject(object);
                                                
                                                return p;
                                            }
                                        }
                                        
                                        if (objectType != OBJECT_TYPE_UNKNOWN) {
                                            if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_ObjectLength(variableObject))) {
                                                if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_STRING)) {
                                                    variableObject.string[Gua_ObjectToInteger(argObject)] = object->string[0];
                                                    if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_GLOBAL) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set string", name);
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_INTEGER)) {
                                                    variableObject.string[Gua_ObjectToInteger(argObject)] = Gua_PObjectToInteger(object);
                                                    if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_GLOBAL) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set string", name);
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                    
                                                    Gua_FreeObject(object);
                                                }
                                            } else {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                                
                                                Gua_FreeObject(object);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't set variable", name);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                            
                                            Gua_FreeObject(object);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't unset string", name);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                } else {
                                    *token = firstToken;
                                    p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                                }
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        }
                        
                        Gua_Free(expression);
                        Gua_FreeObject(&argObject);
                    }
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            } else {
                *token = firstToken;
                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
            }
            
            Gua_Free(name);
        /* It is a MACRO VARIABLE name substitution. */
        } else if (token->type == TOKEN_TYPE_PARENTHESIS) {
            if (token->length > 0) {
                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(expression, '\0', sizeof(char) * (token->length + 1));
                strncpy(expression, token->start, token->length);
                
                Gua_Evaluate(nspace, expression, &operand, status, error);
                Gua_Free(expression);
                
                if (*status != GUA_OK) {
                    return p;
                }
                
                if (Gua_ObjectType(operand) == OBJECT_TYPE_STRING) {
                    p = Gua_NextToken(nspace, p, token);
                    
                    if (token->status != GUA_OK) {
                        *status = token->status;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        if (!Gua_IsObjectStored(operand)) {
                            Gua_FreeObject(&operand);
                        }
                        
                        return p;
                    }
                    
                    /* A simple MACRO VARIABLE assignment. */
                    if (token->type == TOKEN_TYPE_ASSIGN) {
                        p = Gua_NextToken(nspace, p, token);
                        
                        p = Gua_ParseAssign(nspace, p, token, object, status, error);
                        
                        if (*status != GUA_OK) {
                            if (!Gua_IsObjectStored(operand)) {
                                Gua_FreeObject(&operand);
                            }
                            return p;
                        }
                        
                        if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                            /* Try to set the VARIABLE. */
                            if (Gua_SetVariable(nspace, Gua_ObjectToString(operand), object, SCOPE_STACK) != GUA_OK) {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(operand));
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        } else {
                            /* Try to unset the VARIABLE. */
                            if (Gua_UnsetVariable(nspace, Gua_ObjectToString(operand), SCOPE_STACK) != GUA_OK) {
                                if (Gua_UnsetFunction(nspace, Gua_ObjectToString(operand)) != GUA_OK) {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], Gua_ObjectToString(operand));
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            }
                        }
                    /* It is a STRING, ARRAY or a MATRIX. */
                    } else if (token->type == TOKEN_TYPE_BRACKET) {
                        if (token->length > 0) {
                            objectType = Gua_GetVariable(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK);
                            
                            /* It is an ARRAY VARIABLE.*/
                            if ((objectType == OBJECT_TYPE_UNKNOWN) || (objectType == OBJECT_TYPE_ARRAY)) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                argc = Gua_CountArguments(expression);
                                argc++;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                                    p = Gua_NextToken(nspace, p, token);
                                    
                                    if (token->status != GUA_OK) {
                                        *status = token->status;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                        
                                        if (!Gua_IsObjectStored(operand)) {
                                            Gua_FreeObject(&operand);
                                        }
                                        Gua_Free(expression);
                                        Gua_FreeArguments(argc, argv);
                                        
                                        return p;
                                    }
                                    
                                    if (argc > 2) {
                                        Gua_ArgsToString(argc, argv, &argObject);
                                        Gua_SetStoredObject(argObject)
                                    } else if (argc == 2) {
                                        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                            Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                                        } else {
                                            Gua_LinkObjects(argObject, argv[1]);
                                        }
                                        Gua_SetStoredObject(argObject);
                                    }
                                    
                                    if (token->type == TOKEN_TYPE_ASSIGN) {
                                        p = Gua_NextToken(nspace, p, token);
                                        
                                        p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                        
                                        if (*status != GUA_OK) {
                                            if (!Gua_IsObjectStored(operand)) {
                                                Gua_FreeObject(&operand);
                                            }
                                            Gua_Free(expression);
                                            Gua_FreeArguments(argc, argv);
                                            Gua_FreeObject(&argObject);
                                            return p;
                                        }
                                        
                                        if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                            if (objectType != OBJECT_TYPE_UNKNOWN) {
                                                if (Gua_SetArrayElement(&variableObject, &argObject, object, true) == GUA_OK) {
                                                    if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                }
                                            } else {
                                                if (Gua_SetArrayElement(&variableObject, &argObject, object, false) == GUA_OK) {
                                                    if (Gua_SetVariable(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                }
                                            }
                                        } else {
                                            if (objectType != OBJECT_TYPE_UNKNOWN) {
                                                if (Gua_UnsetArrayElement(&variableObject, &argObject) == GUA_OK) {
                                                    if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set array", Gua_ObjectToString(operand));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                    if (Gua_ObjectLength(variableObject) == 0) {
                                                        if (Gua_UnsetVariable(nspace, Gua_ObjectToString(operand), SCOPE_STACK) != GUA_OK) {
                                                            *status = GUA_ERROR;
                                                            
                                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                            sprintf(errMessage, "%s %-.20s...\n", "can't unset array", Gua_ObjectToString(operand));
                                                            strcat(error, errMessage);
                                                            Gua_Free(errMessage);
                                                        }
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset array", Gua_ObjectToString(operand));
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                }
                                            } else {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't unset array", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        }
                                    } else {
                                        *token = firstToken;
                                        p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                                
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                                Gua_FreeObject(&argObject);
                            /* It is a MATRIX VARIABLE.*/
                            } else if (objectType == OBJECT_TYPE_MATRIX) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                argc = Gua_CountArguments(expression);
                                argc += 3;
                                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                                
                                Gua_ClearArguments(argc, argv);
                                
                                Gua_LinkStringToObject(argv[0], "setMatrixElement");
                                Gua_SetStoredObject(argv[0]);
                                
                                if (Gua_ParseArguments(nspace, expression, argc, argv, 3, status, error) == GUA_OK) {
                                    p = Gua_NextToken(nspace, p, token);
                                    
                                    if (token->status != GUA_OK) {
                                        *status = token->status;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                        
                                        Gua_Free(expression);
                                        Gua_FreeArguments(argc, argv);
                                        
                                        return p;
                                    }
                                    
                                    if (token->type == TOKEN_TYPE_ASSIGN) {
                                        p = Gua_NextToken(nspace, p, token);
                                        
                                        p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                        
                                        if (*status != GUA_OK) {
                                            Gua_Free(expression);
                                            Gua_FreeArguments(argc, argv);
                                            return p;
                                        }
                                        
                                        if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                            if (objectType != OBJECT_TYPE_UNKNOWN) {
                                                Gua_LinkObjects(argv[1], variableObject);
                                                Gua_LinkFromPObject(argv[2], object);
                                                
                                                if (Gua_BuiltInFunction(nspace, argc, argv, object, error) == GUA_OK) {
                                                    if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", Gua_ObjectToString(operand));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "index out of bound", Gua_ObjectToString(operand));
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                }
                                            } else {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't unset matrix", Gua_ObjectToString(operand));
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    } else {
                                        *token = firstToken;
                                        p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                                
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                            /* It is a STRING VARIABLE.*/
                            } else if (objectType == OBJECT_TYPE_STRING) {
                                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                                memset(expression, '\0', sizeof(char) * (token->length + 1));
                                strncpy(expression, token->start, token->length);
                                
                                Gua_Evaluate(nspace, expression, &argObject, status, error);
                                
                                if (*status == GUA_OK) {
                                    if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                                        p = Gua_NextToken(nspace, p, token);
                                        
                                        if (token->status != GUA_OK) {
                                            *status = token->status;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                            
                                            Gua_Free(expression);
                                            Gua_FreeObject(&argObject);
                                            
                                            return p;
                                        }
                                        
                                        if (token->type == TOKEN_TYPE_ASSIGN) {
                                            p = Gua_NextToken(nspace, p, token);
                                            
                                            p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                            
                                            if (*status != GUA_OK) {
                                                Gua_Free(expression);
                                                Gua_FreeObject(&argObject);
                                                return p;
                                            }
                                            
                                            if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                                if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                                                    if (Gua_PObjectLength(object) != 1) {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[TOKEN_TYPE_ASSIGN].symbol);
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                        
                                                        Gua_Free(expression);
                                                        Gua_FreeObject(&argObject);
                                                        Gua_FreeObject(object);
                                                        
                                                        return p;
                                                    }
                                                }
                                                
                                                if (objectType != OBJECT_TYPE_UNKNOWN) {
                                                    if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_ObjectLength(variableObject))) {
                                                        if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_STRING)) {
                                                            variableObject.string[Gua_ObjectToInteger(argObject)] = object->string[0];
                                                            if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                                *status = GUA_ERROR;
                                                                
                                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                                sprintf(errMessage, "%s %-.20s...\n", "can't set string", Gua_ObjectToString(operand));
                                                                strcat(error, errMessage);
                                                                Gua_Free(errMessage);
                                                            }
                                                        } else if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_INTEGER)) {
                                                            variableObject.string[Gua_ObjectToInteger(argObject)] = Gua_PObjectToInteger(object);
                                                            if (Gua_UpdateObject(nspace, Gua_ObjectToString(operand), &variableObject, SCOPE_STACK) != GUA_OK) {
                                                                *status = GUA_ERROR;
                                                                
                                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                                sprintf(errMessage, "%s %-.20s...\n", "can't set string", Gua_ObjectToString(operand));
                                                                strcat(error, errMessage);
                                                                Gua_Free(errMessage);
                                                            }
                                                        } else {
                                                            *status = GUA_ERROR;
                                                            
                                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                                            strcat(error, errMessage);
                                                            Gua_Free(errMessage);
                                                            
                                                            Gua_FreeObject(object);
                                                        }
                                                    } else {
                                                        *status = GUA_ERROR;
                                                        
                                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                        sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                                        strcat(error, errMessage);
                                                        Gua_Free(errMessage);
                                                        
                                                        Gua_FreeObject(object);
                                                    }
                                                } else {
                                                    *status = GUA_ERROR;
                                                    
                                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                    sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(operand));
                                                    strcat(error, errMessage);
                                                    Gua_Free(errMessage);
                                                    
                                                    Gua_FreeObject(object);
                                                }
                                            } else {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't unset string", Gua_ObjectToString(operand));
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *token = firstToken;
                                            p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                }
                                
                                Gua_Free(expression);
                                Gua_FreeObject(&argObject);
                            /* The VARIABLE is not a STRING, ARRAY or MATRIX.*/
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    } else {
                       *token = firstToken;
                        p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                    }
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "illegal variable name", Gua_ObjectToString(operand));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
                
                if (!Gua_IsObjectStored(operand)) {
                    Gua_FreeObject(&operand);
                }
            }
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseAssignUnknown(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: =
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseAssignUnknown(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String expression;
    Gua_Token firstToken;
    Gua_String name;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object argObject;
    Gua_Object variableObject;
    Gua_String cmdStart;
    Gua_String cmdEnd;
    Gua_Integer cmdLength;
    Gua_String arguments;
    Gua_String script;
    Gua_String errMessage;
    
    p = start;
    firstToken = *token;
    
    Gua_ClearObject(argObject);
    Gua_ClearObject(variableObject);

    *status = GUA_OK;
    *error = '\0';
    
    /* Parses new VARIABLES and FUNCTIONS. */
    if (token->type == TOKEN_TYPE_UNKNOWN) {
        name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(name, '\0', sizeof(char) * (token->length + 1));
        strncpy(name, token->start, token->length);
        
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(name);
            
            return p;
        }
        
        /* A simple VARIABLE assignment. */
        if (token->type == TOKEN_TYPE_ASSIGN) {
            p = Gua_NextToken(nspace, p, token);
            
            p = Gua_ParseAssign(nspace, p, token, object, status, error);
            
            if (*status != GUA_OK) {
                Gua_Free(name);
                return p;
            }
            
            if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                /* Try to set the VARIABLE. */
                if (Gua_SetVariable(nspace, name, object, SCOPE_LOCAL) != GUA_OK) {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "can't set variable", name);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "can't unset variable", name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        /* The new VARIABLE is an ARRAY. */
        } else if (token->type == TOKEN_TYPE_BRACKET) {
            if (token->length > 0) {
                expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(expression, '\0', sizeof(char) * (token->length + 1));
                strncpy(expression, token->start, token->length);
                
                argc = Gua_CountArguments(expression);
                argc++;
                argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                
                Gua_ClearArguments(argc, argv);
                
                if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                    p = Gua_NextToken(nspace, p, token);
                    
                    if (token->status != GUA_OK) {
                        *status = token->status;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        Gua_Free(name);
                        Gua_Free(expression);
                        Gua_FreeArguments(argc, argv);
                        
                        return p;
                    }
                    
                    if (argc > 2) {
                        Gua_ArgsToString(argc, argv, &argObject);
                        Gua_SetStoredObject(argObject);
                    } else if (argc == 2) {
                        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                            Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                        } else {
                            Gua_LinkObjects(argObject, argv[1]);
                        }
                        Gua_SetStoredObject(argObject);
                    }
                    
                    if (token->type == TOKEN_TYPE_ASSIGN) {
                        p = Gua_NextToken(nspace, p, token);
                        
                        p = Gua_ParseAssign(nspace, p, token, object, status, error);
                        
                        if (*status != GUA_OK) {
                            Gua_Free(name);
                            Gua_Free(expression);
                            Gua_FreeArguments(argc, argv);
                            Gua_FreeObject(&argObject);
                            return p;
                        }
                        
                        if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                            if (Gua_SetArrayElement(&variableObject, &argObject, object, false) == GUA_OK) {
                                if (Gua_SetVariable(nspace, name, &variableObject, SCOPE_LOCAL) != GUA_OK) {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %-.20s...\n", "can't unset array", name);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    } else {
                        *token = firstToken;
                        p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                    }
                } else {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
                
                Gua_Free(expression);
                Gua_FreeArguments(argc, argv);
                Gua_FreeObject(&argObject);
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        /* A FUNCTION assignment. */
        } else if (token->type == TOKEN_TYPE_PARENTHESIS) {
            if (token->length > 0) {
                arguments = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(arguments, '\0', sizeof(char) * (token->length + 1));
                strncpy(arguments, token->start, token->length);
            } else {
                arguments = (char *)Gua_Alloc(sizeof(char) * 1);
                memset(arguments, '\0', sizeof(char) * 1);
                arguments[0] = '\0';
            }
            
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(name);
                Gua_Free(arguments);
                
                return p;
            }
            
            if (token->type == TOKEN_TYPE_ASSIGN) {
                /* Parses the command separator. */
                p = Gua_NextToken(nspace, p, token);
                
                /* Get the beginning of the command. */
                cmdStart = token->start;
                
                /* Search for the end of the command. */
                cmdEnd = cmdStart;
                
                while (!(Gua_IsSeparator(*cmdEnd) || (*cmdEnd == EXPRESSION_END))) {
                    cmdEnd++;
                }
                
                p = cmdEnd;
                
                /* Parses the command separator. */
                p = Gua_NextToken(nspace, p, token);
                
                if (token->status != GUA_OK) {
                    *status = token->status;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    Gua_Free(name);
                    Gua_Free(arguments);
                    
                    return p;
                }
                
                /* Calculates the command length. */
                cmdLength = (Gua_Length)(cmdEnd - cmdStart);
                
                if (cmdLength > 0) {
                    script = (char *)Gua_Alloc(sizeof(char) * (cmdLength + 1));
                    memset(script, '\0', sizeof(char) * (cmdLength + 1));
                    strncpy(script, cmdStart, cmdLength);
                } else {
                    script = (char *)Gua_Alloc(sizeof(char) * 1);
                    memset(script, '\0', sizeof(char) * 1);
                    script[0] = '\0';
                }
                
                expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
                memset(expression, '\0', sizeof(char) * EXPRESSION_SIZE);
                sprintf(expression, "function %s(%s) {return(%s)}\n", name, arguments, script);
                
                Gua_Evaluate(nspace, expression, object, status, error);
                
                Gua_Free(arguments);
                Gua_Free(script);
                Gua_Free(expression);
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(arguments);
            }
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], name);
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
        
        Gua_Free(name);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseAssignFunction(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: =
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseAssignFunction(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String expression;
    Gua_Token firstToken;
    Gua_String name;
    Gua_String cmdStart;
    Gua_String cmdEnd;
    Gua_Integer cmdLength;
    Gua_String arguments;
    Gua_String script;
    Gua_String errMessage;
    
    p = start;
    firstToken = *token;
    
    *status = GUA_OK;
    *error = '\0';
    
    /* Parses FUNCTIONS. */
    if (token->type == TOKEN_TYPE_FUNCTION) {
        name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(name, '\0', sizeof(char) * (token->length + 1));
        strncpy(name, token->start, token->length);
        
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(name);
            
            return p;
        }
        
        /* A simple FUNCTION assignment. */
        if (token->type == TOKEN_TYPE_ASSIGN) {
            p = Gua_NextToken(nspace, p, token);
            
            p = Gua_ParseAssign(nspace, p, token, object, status, error);
            
            if (*status != GUA_OK) {
                Gua_Free(name);
                return p;
            }
            
            if (Gua_PObjectType(object) == OBJECT_TYPE_UNKNOWN) {
                /* Try to unset the FUNCTION. */
                if (Gua_UnsetFunction(nspace, name) != GUA_OK) {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "can't unset function", name);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s after function %-.20s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], name);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        /* A FUNCTION assignment. */
        } else if (token->type == TOKEN_TYPE_PARENTHESIS) {
            if (token->length > 0) {
                arguments = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(arguments, '\0', sizeof(char) * (token->length + 1));
                strncpy(arguments, token->start, token->length);
            } else {
                arguments = (char *)Gua_Alloc(sizeof(char) * 1);
                memset(arguments, '\0', sizeof(char) * 1);
                arguments[0] = '\0';
            }
            
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                Gua_Free(name);
                Gua_Free(arguments);
                
                return p;
            }
            
            if (token->type == TOKEN_TYPE_ASSIGN) {
                /* Get the beginning of the command. */
                cmdStart = p;
                
                while (Gua_IsSpace(*cmdStart)) {
                    cmdStart++;
                }
                
                /* Search for the end of the command. */
                cmdEnd = cmdStart;
                
                while (!(Gua_IsSeparator(*cmdEnd) || (*cmdEnd == EXPRESSION_END))) {
                    cmdEnd++;
                }
                
                p = cmdEnd;
                
                /* Parses the command separator. */
                p = Gua_NextToken(nspace, p, token);
                
                if (token->status != GUA_OK) {
                    *status = token->status;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    Gua_Free(name);
                    Gua_Free(arguments);
                    
                    return p;
                }
                
                /* Calculates the command length. */
                cmdLength = (Gua_Length)(cmdEnd - cmdStart);
                
                if (cmdLength > 0) {
                    script = (char *)Gua_Alloc(sizeof(char) * (cmdLength + 1));
                    memset(script, '\0', sizeof(char) * (cmdLength + 1));
                    strncpy(script, cmdStart, cmdLength);
                } else {
                    script = (char *)Gua_Alloc(sizeof(char) * 1);
                    memset(script, '\0', sizeof(char) * 1);
                    script[0] = '\0';
                }
                
                expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
                memset(expression, '\0', sizeof(char) * EXPRESSION_SIZE);
                sprintf(expression, "function %s(%s) {return(%s)}\n", name, arguments, script);
                
                Gua_Evaluate(nspace, expression, object, status, error);
                
                Gua_Free(arguments);
                Gua_Free(script);
                Gua_Free(expression);
            } else {
                Gua_Free(arguments);
                
                *token = firstToken;
                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
            }
        } else {
            *token = firstToken;
            p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
        }
        
        Gua_Free(name);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseAssignVariable(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: =
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseAssignVariable(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String expression;
    Gua_Token firstToken;
    Gua_String name;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object argObject;
    Gua_Object variableObject;
    Gua_Short objectType;
    Gua_String errMessage;
    
    p = start;
    firstToken = *token;
    
    Gua_ClearObject(argObject);
    Gua_ClearObject(variableObject);

    *status = GUA_OK;
    *error = '\0';
    
    /* Parses VARIABLES. */
    if (token->type == TOKEN_TYPE_VARIABLE) {
        name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
        memset(name, '\0', sizeof(char) * (token->length + 1));
        strncpy(name, token->start, token->length);
        
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(name);
            
            return p;
        }
        
        /* A simple VARIABLE assignment. */
        if (token->type == TOKEN_TYPE_ASSIGN) {
            p = Gua_NextToken(nspace, p, token);
            
            p = Gua_ParseAssign(nspace, p, token, object, status, error);
            
            if (*status != GUA_OK) {
                Gua_Free(name);
                return p;
            }
            
            if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                /* Try to set the VARIABLE. */
                if (Gua_SetVariable(nspace, name, object, SCOPE_LOCAL) != GUA_OK) {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "can't set variable", name);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            } else {
                /* Try to unset the VARIABLE. */
                if (Gua_UnsetVariable(nspace, name, SCOPE_LOCAL) != GUA_OK) {
                    *status = GUA_ERROR;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "can't unset variable", name);
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                }
            }
        /* The VARIABLE is a STRING, ARRAY or a MATRIX. */
        } else if (token->type == TOKEN_TYPE_BRACKET) {
            if (token->length > 0) {
                objectType = Gua_GetVariable(nspace, name, &variableObject, SCOPE_LOCAL);
                
                /* The VARIABLE is an ARRAY.*/
                if (objectType == OBJECT_TYPE_ARRAY) {
                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                    strncpy(expression, token->start, token->length);
                    
                    argc = Gua_CountArguments(expression);
                    argc++;
                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                    
                    Gua_ClearArguments(argc, argv);
                    
                    if (Gua_ParseArguments(nspace, expression, argc, argv, 1, status, error) == GUA_OK) {
                        p = Gua_NextToken(nspace, p, token);
                        
                        if (token->status != GUA_OK) {
                            *status = token->status;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                            
                            Gua_Free(name);
                            Gua_Free(expression);
                            Gua_FreeArguments(argc, argv);
                            
                            return p;
                        }
                        
                        if (argc > 2) {
                            Gua_ArgsToString(argc, argv, &argObject);
                            Gua_SetStoredObject(argObject);
                        } else if (argc == 2) {
                            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
                                Gua_ByteArrayToObject(argObject, Gua_ObjectToString(argv[1]), Gua_ObjectLength(argv[1]));
                            } else {
                                Gua_LinkObjects(argObject, argv[1]);
                            }
                            Gua_SetStoredObject(argObject);
                        }
                        
                        if (token->type == TOKEN_TYPE_ASSIGN) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            p = Gua_ParseAssign(nspace, p, token, object, status, error);
                            
                            if (*status != GUA_OK) {
                                Gua_Free(name);
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                                Gua_FreeObject(&argObject);
                                return p;
                            }
                            
                            if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                if (Gua_SetArrayElement(&variableObject, &argObject, object, true) == GUA_OK) {
                                    if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_LOCAL) != GUA_OK) {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                if (Gua_UnsetArrayElement(&variableObject, &argObject) == GUA_OK) {
                                    if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_LOCAL) != GUA_OK) {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't set array", name);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                    if (Gua_ObjectLength(variableObject) == 0) {
                                        if (Gua_UnsetVariable(nspace, name, SCOPE_LOCAL) != GUA_OK) {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "can't unset array", name);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                        }
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset array", name);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            }
                        } else {
                            *token = firstToken;
                            p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                        }
                    } else {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                    }
                    
                    Gua_Free(expression);
                    Gua_FreeArguments(argc, argv);
                    Gua_FreeObject(&argObject);
                /* The VARIABLE is a MATRIX.*/
                } else if (objectType == OBJECT_TYPE_MATRIX) {
                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                    strncpy(expression, token->start, token->length);
                    
                    argc = Gua_CountArguments(expression);
                    argc += 3;
                    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
                    
                    Gua_ClearArguments(argc, argv);
                    
                    Gua_LinkStringToObject(argv[0], "setMatrixElement");
                    Gua_SetStoredObject(argv[0]);
                    
                    if (Gua_ParseArguments(nspace, expression, argc, argv, 3, status, error) == GUA_OK) {
                        p = Gua_NextToken(nspace, p, token);
                        
                        if (token->status != GUA_OK) {
                            *status = token->status;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                            
                            Gua_Free(name);
                            Gua_Free(expression);
                            Gua_FreeArguments(argc, argv);
                            
                            return p;
                        }
                        
                        if (token->type == TOKEN_TYPE_ASSIGN) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            p = Gua_ParseAssign(nspace, p, token, object, status, error);
                            
                            if (*status != GUA_OK) {
                                Gua_Free(name);
                                Gua_Free(expression);
                                Gua_FreeArguments(argc, argv);
                                return p;
                            }
                            
                            if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                Gua_LinkObjects(argv[1], variableObject);
                                Gua_LinkFromPObject(argv[2], object);
                                
                                if (Gua_BuiltInFunction(nspace, argc, argv, object, error) == GUA_OK) {
                                    if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_LOCAL) != GUA_OK) {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %-.20s...\n", "can't set matrix", name);
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "index out of bound", name);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                *status = GUA_ERROR;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s %-.20s...\n", "can't unset matrix", name);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                            }
                        } else {
                            *token = firstToken;
                            p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                        }
                    } else {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                    }
                    
                    Gua_Free(expression);
                    Gua_FreeArguments(argc, argv);
                /* The VARIABLE is a STRING.*/
                } else if (objectType == OBJECT_TYPE_STRING) {
                    expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                    memset(expression, '\0', sizeof(char) * (token->length + 1));
                    strncpy(expression, token->start, token->length);
                    
                    Gua_Evaluate(nspace, expression, &argObject, status, error);
                    
                    if (*status == GUA_OK) {
                        if (Gua_ObjectType(argObject) == OBJECT_TYPE_INTEGER) {
                            p = Gua_NextToken(nspace, p, token);
                            
                            if (token->status != GUA_OK) {
                                *status = token->status;
                                
                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                                strcat(error, errMessage);
                                Gua_Free(errMessage);
                                
                                Gua_Free(name);
                                Gua_Free(expression);
                                Gua_FreeObject(&argObject);
                                
                                return p;
                            }
                            
                            if (token->type == TOKEN_TYPE_ASSIGN) {
                                p = Gua_NextToken(nspace, p, token);
                                
                                p = Gua_ParseAssign(nspace, p, token, object, status, error);
                                
                                if (*status != GUA_OK) {
                                    Gua_Free(name);
                                    Gua_Free(expression);
                                    Gua_FreeObject(&argObject);
                                    return p;
                                }
                                
                                if (Gua_PObjectType(object) != OBJECT_TYPE_UNKNOWN) {
                                    if (Gua_PObjectType(object) == OBJECT_TYPE_STRING) {
                                        if (Gua_PObjectLength(object) != 1) {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s %-.20s...\n", "illegal operand 2 for operator", Gua_TokenTable[TOKEN_TYPE_ASSIGN].symbol);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                            
                                            Gua_Free(name);
                                            Gua_Free(expression);
                                            Gua_FreeObject(&argObject);
                                            Gua_FreeObject(object);
                                            
                                            return p;
                                        }
                                    }
                                    
                                    if ((Gua_ObjectToInteger(argObject) >= 0) && (Gua_ObjectToInteger(argObject) < Gua_ObjectLength(variableObject))) {
                                        if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_STRING)) {
                                            variableObject.string[Gua_ObjectToInteger(argObject)] = object->string[0];
                                            if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_LOCAL) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set string", name);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else if ((Gua_ObjectType(variableObject) == OBJECT_TYPE_STRING) && (Gua_PObjectType(object) == OBJECT_TYPE_INTEGER)) {
                                            variableObject.string[Gua_ObjectToInteger(argObject)] = Gua_PObjectToInteger(object);
                                            if (Gua_UpdateObject(nspace, name, &variableObject, SCOPE_LOCAL) != GUA_OK) {
                                                *status = GUA_ERROR;
                                                
                                                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                                sprintf(errMessage, "%s %-.20s...\n", "can't set string", name);
                                                strcat(error, errMessage);
                                                Gua_Free(errMessage);
                                            }
                                        } else {
                                            *status = GUA_ERROR;
                                            
                                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                                            strcat(error, errMessage);
                                            Gua_Free(errMessage);
                                            
                                            Gua_FreeObject(object);
                                        }
                                    } else {
                                        *status = GUA_ERROR;
                                        
                                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                        sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argObject));
                                        strcat(error, errMessage);
                                        Gua_Free(errMessage);
                                        
                                        Gua_FreeObject(object);
                                    }
                                } else {
                                    *status = GUA_ERROR;
                                    
                                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                                    sprintf(errMessage, "%s %-.20s...\n", "can't unset string", name);
                                    strcat(error, errMessage);
                                    Gua_Free(errMessage);
                                }
                            } else {
                                *token = firstToken;
                                p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                            }
                        } else {
                            *status = GUA_ERROR;
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                        }
                    }
                    
                    Gua_Free(expression);
                    Gua_FreeObject(&argObject);
                /* The VARIABLE is not a STRING, ARRAY or MATRIX, or it is in the GLOBAL namespace.*/
                } else {
                    *token = firstToken;
                    p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
                }
            } else {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        } else {
            *token = firstToken;
            p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
        }
        
        Gua_Free(name);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseAssign(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: =
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseAssign(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String errMessage;
    
    p = start;

    *status = GUA_OK;
    *error = '\0';
    
    /* Parses VARIABLES, MACROS, INDIRECTIONS and FUNCTIONS. */
    if ((token->type == TOKEN_TYPE_UNKNOWN) || (token->type == TOKEN_TYPE_VARIABLE) || (token->type == TOKEN_TYPE_MACRO) || (token->type == TOKEN_TYPE_INDIRECTION) || (token->type == TOKEN_TYPE_FUNCTION)) {
        /* Parses VARIABLES. */
        if (token->type == TOKEN_TYPE_VARIABLE) {
            p = Gua_ParseAssignVariable(nspace, start, token, object, status, error);
        /* Parses FUNCTION. */
        } else if (token->type == TOKEN_TYPE_FUNCTION) {
            p = Gua_ParseAssignFunction(nspace, start, token, object, status, error);
        /* Parses UNKNOWN. */
        } else if (token->type == TOKEN_TYPE_UNKNOWN) {
            p = Gua_ParseAssignUnknown(nspace, start, token, object, status, error);
        /* Parses MACRO substitutions. */
        } else if (token->type == TOKEN_TYPE_MACRO) {
            p = Gua_ParseAssignMacro(nspace, start, token, object, status, error);
        /* Parses INDIRECTIONS. */
        } else if (token->type == TOKEN_TYPE_INDIRECTION) {
            p = Gua_ParseAssignIndirection(nspace, start, token, object, status, error);
        }
    } else {
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
        
        p = Gua_ParseLogicOr(nspace, start, token, object, status, error);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_EvalFunction(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Evaluate a scripted function.
 *
 * Arguments:
 *     nspace,    a pointer to a structure Gua_Namespace. Must do a cast before use it;
 *     argc,      the number of arguments to pass to the function;
 *     argv,      an array containing the arguments to the function, where
 *                argv[0] is the function name;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the return object of the scripted funcion.
 */
Gua_Status Gua_EvalFunction(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Namespace *previous;
    Gua_Namespace *local;
    Gua_Function function;
    Gua_Argument *args;
    Gua_Status status;
    Gua_Short i;
    Gua_Short j;
    Gua_String errMessage;
    
    Gua_LinkStringToPObject(object, "");
    Gua_SetStoredPObject(object);
    
    /* Get the function arguments. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(argv[0]), &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "unknown function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if ((argc - 1) > function.argc) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    previous = (Gua_Namespace *)nspace;
    
    /* Create the local namespace. */
    local = (Gua_Namespace *)Gua_Alloc(sizeof(Gua_Namespace));
    
    Gua_ClearNamespace(local);
    
    local->previous = nspace;
    
    previous->next = (struct Gua_Namespace *)local;
    
    if (argc > 0) {
        /* Set the local variables from the arguments. */
        args = (Gua_Argument *)function.argv;
        for (i = 1; i < argc; i++) {
            if (Gua_SetVariable(local, Gua_ArgName(args[i - 1]), &argv[i], SCOPE_LOCAL) != GUA_OK) {
                Gua_FreeNamespace(local);
                previous->next = NULL;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ArgName(args[i - 1]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        if ((i - 1) < function.argc) {
            for (j = i - 1; j < function.argc; j++) {
                if (Gua_ObjectType(Gua_ArgObject(args[j])) != OBJECT_TYPE_UNKNOWN) {
                    if (Gua_SetVariable(local, Gua_ArgName(args[j]), &(Gua_ArgObject(args[j])), SCOPE_LOCAL) != GUA_OK) {
                        Gua_FreeNamespace(local);
                        previous->next = NULL;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ArgName(args[j]));
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return GUA_ERROR;
                    }
                } else {
                    Gua_FreeNamespace(local);
                    previous->next = NULL;
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
            }
        }
    }
    
    /* Now run the script. */
    Gua_Evaluate(local, function.script, object, &status, error);
    
    Gua_FreeNamespace(local);
    previous->next = NULL;
    
    if ((status == GUA_RETURN) || (status == GUA_EXIT)) {
        *error = '\0';
    } else {
        if (status != GUA_OK) {
            return GUA_ERROR_FUNCTION;
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseFunction(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: function
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseFunction(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_String expression;
    Gua_String name;
    Gua_String code;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Function function;
    Gua_Argument *args;
    Gua_String varName;
    Gua_String varObject;
    Gua_Short i;
    Gua_Short parseOk;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearPObject(object);
    
    *status = GUA_OK;
    *error = '\0';
    
    p = Gua_NextToken(nspace, p, token);
    
    /* The first token after the FUNCTION statement must be the FUNCTION name. */
    if (!((token->type == TOKEN_TYPE_FUNCTION) || (token->type == TOKEN_TYPE_UNKNOWN))) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            name = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(name, '\0', sizeof(char) * (token->length + 1));
            strncpy(name, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(name);
        
        return p;
    }
    
    /* The next token after the FUNCTION name must be parenthesis. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(name);
        
        return p;
    } else {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
        }
    }
    
    /* Get the formal arguments. */
    if (token->length > 0) {
        argc = Gua_CountArguments(expression);
        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
        
        if (Gua_ParseFormalArguments(nspace, expression, argc, argv, status, error) != GUA_OK) {
            Gua_Free(name);
            Gua_Free(expression);
            
            return p;
        }
        
        Gua_Free(expression);
    } else {
        argc = 0;
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(name);
        if (argc > 0) {
            Gua_FreeArguments(argc, argv);
        }
        
        return p;
    }
    
    /* After the arguments must exists some code to evaluate. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(name);
        if (argc > 0) {
            Gua_FreeArguments(argc, argv);
        }
        
        return p;
    } else {
        if (token->length > 0) {
            code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(code, '\0', sizeof(char) * (token->length + 1));
            strncpy(code, token->start, token->length);
        } else {
            code = (char *)Gua_Alloc(sizeof(char));
            code[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    varName = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
    varObject = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
    
    parseOk = true;
    
    /* Sets the structure of the FUNCTION. */
    function.type = FUNCTION_TYPE_SCRIPT;
    function.name = NULL;
    function.argc = argc;
    /* Stores the formal arguments into the structure of arguments of the FUNCTION. */
    if (argc > 0) {
        function.argv = (struct Gua_Argument *)Gua_Alloc(sizeof(Gua_Argument) * argc);
        args = (Gua_Argument *)function.argv;
        for (i = 0; i < argc; i++) {
            Gua_ParseVarAssignment(argv[i].string, varName, varObject);
            
            Gua_Evaluate(nspace, varObject, &(args[i].object), status, error);
            
            if (*status == GUA_OK) {
                Gua_SetStoredObject(args[i].object);
                args[i].name = (char *)Gua_Alloc(sizeof(char) * (strlen(varName) + 1));
                memset(args[i].name, '\0', sizeof(char) * (strlen(varName) + 1));
                strncpy(args[i].name, varName, strlen(varName));
            } else {
                parseOk = false;
            }
        }
    } else {
        function.argv = NULL;
    }
    function.script = code;
    function.previous = NULL;
    function.next = NULL;
    
    /* Sets the FUNCTION evaluator. */
    if (parseOk) {
        function.pointer = Gua_EvalFunction;
        if (Gua_SetFunction((Gua_Namespace *)nspace, name, &function) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "can't set function", name);
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
    } else {
        *status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        for (i = 0; i < argc; i++) {
            Gua_Free(args[i].name);
            Gua_FreeObject(&(args[i].object));
        }
    }
    
    if (argc > 0) {
        Gua_FreeArguments(argc, argv);
    }
    
    Gua_Free(name);
    Gua_Free(varName);
    Gua_Free(varObject);
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_ElapsedTime(Gua_Time *result, Gua_Time *tv1, Gua_Time *tv2)
 *
 * Description:
 *     Calculate the diference between two times.
 *
 * Arguments:
 *     result,    the result object;
 *     tv1,       the first time object;
 *     tv2,       the second time object.
 *
 * Results:
 *     The function returns 1 if tv1 is smaller than tv2,
 *     0 otherwise.
 *
 */
Gua_Status Gua_ElapsedTime(Gua_Time *result, Gua_Time *tv1, Gua_Time *tv2)
{
    Gua_Short nsec;

    if (tv1->tv_usec < tv2->tv_usec) {
        nsec = (tv2->tv_usec - tv1->tv_usec) / 1000000 + 1;
        tv2->tv_usec -= 1000000 * nsec;
        tv2->tv_sec += nsec;
    }

    if (tv1->tv_usec - tv2->tv_usec > 1000000) {
        nsec = (tv1->tv_usec - tv2->tv_usec) / 1000000;
        tv2->tv_usec += 1000000 * nsec;
        tv2->tv_sec -= nsec;
    }

    result->tv_sec = tv1->tv_sec - tv2->tv_sec;
    result->tv_usec = tv1->tv_usec - tv2->tv_usec;

    return tv1->tv_sec < tv2->tv_sec;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseTest(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: test
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseTest(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object triesObject;
    Gua_Object desiredObject;
    Gua_Object toleranceObject;
    Gua_Object testObject;
    Gua_Object catchObject;
    Gua_Object errorObject;
    Gua_Object timeObject;
    Gua_Object avgObject;
    Gua_Object deviationObject;
    Gua_String expression;
    Gua_String tries;
    Gua_String desired;
    Gua_String tolerance;
    Gua_String test;
    Gua_String catch;
    Gua_Short match;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Integer i;
    Gua_Real sx;
    Gua_Real sx2;
    Gua_String errMessage;
    struct timeval tv1;
    struct timeval tv2;
    struct timeval elapsed;
    struct timezone tz;
   
    p = start;
    
    Gua_ClearObject(triesObject);
    Gua_ClearObject(desiredObject);
    Gua_ClearObject(testObject);
    Gua_ClearObject(catchObject);
    Gua_ClearObject(errorObject);
    Gua_ClearObject(timeObject);
    Gua_ClearObject(avgObject);
    Gua_ClearObject(deviationObject);
    Gua_ClearPObject(object);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        strcpy(error, Gua_StatusTable[token->status]);
        return p;
    }
    
    /* After the TEST statement we must supply some arguments. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
    }
    
    argc = Gua_CountCommands(expression);
    
    /* At least the tries argument must be supplied. */
    if ((argc < 1) || (argc > 3)) {
        *status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for statement", "test");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(expression);
        
        return p;
    }
    
    if (argc > 1) {
        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
        
        if (Gua_ParseCommands(nspace, expression, argc, argv, status, error) != GUA_OK) {
            Gua_Free(expression);
            return p;
        }
        Gua_Free(expression);
        
        tries = Gua_ObjectToString(argv[0]);
        Gua_SetStoredObject(argv[0]);
    } else {
        tries = expression;
    }
    
    if (argc >= 2) {
        desired = Gua_ObjectToString(argv[1]);
        Gua_SetStoredObject(argv[1])
    } else {
        desired = (char *)Gua_Alloc(sizeof(char));
        desired[0] = '\0';
    }
    
    if (argc == 3) {
        tolerance = Gua_ObjectToString(argv[2]);
        Gua_SetStoredObject(argv[2]);
    } else {
        tolerance = (char *)Gua_Alloc(sizeof(char));
        tolerance[0] = '\0';
    }
    
    if (argc > 1) {
        Gua_FreeArguments(argc, argv);
    }
    
    /* Parses the arguments. */
    Gua_Evaluate(nspace, tries, &triesObject, status, error);
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        Gua_Free(tries);
        Gua_Free(desired);
        Gua_Free(tolerance);
        return p;
    }
    Gua_Evaluate(nspace, desired, &desiredObject, status, error);
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        if (!Gua_IsObjectStored(desiredObject)) {
            Gua_FreeObject(&desiredObject);
        }
        Gua_Free(tries);
        Gua_Free(desired);
        Gua_Free(tolerance);
        return p;
    }
    Gua_Evaluate(nspace, tolerance, &toleranceObject, status, error);
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        if (!Gua_IsObjectStored(desiredObject)) {
            Gua_FreeObject(&desiredObject);
        }
        if (!Gua_IsObjectStored(toleranceObject)) {
            Gua_FreeObject(&toleranceObject);
        }
        Gua_Free(tries);
        Gua_Free(desired);
        Gua_Free(tolerance);
        return p;
    }
    Gua_Free(tries);
    Gua_Free(desired);
    Gua_Free(tolerance);
    if (Gua_ObjectType(triesObject) != OBJECT_TYPE_INTEGER) {
        *status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "tries must be integer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        if (!Gua_IsObjectStored(desiredObject)) {
            Gua_FreeObject(&desiredObject);
        }
        if (!Gua_IsObjectStored(toleranceObject)) {
            Gua_FreeObject(&toleranceObject);
        }
        return p;
    }
    /* Only integer and real tolerance object is supported. */
    if (!((Gua_ObjectType(toleranceObject) == OBJECT_TYPE_UNKNOWN) || (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_REAL))) {
        *status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "tolerance must be integer or real");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        if (!Gua_IsObjectStored(desiredObject)) {
            Gua_FreeObject(&desiredObject);
        }
        if (!Gua_IsObjectStored(toleranceObject)) {
            Gua_FreeObject(&toleranceObject);
        }
        return p;
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        if (!Gua_IsObjectStored(desiredObject)) {
            Gua_FreeObject(&desiredObject);
        }
        if (!Gua_IsObjectStored(toleranceObject)) {
            Gua_FreeObject(&toleranceObject);
        }
        return p;
    }
    
    /* We must have some code to TEST. So get it. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_ObjectType(triesObject)) {
            Gua_FreeObject(&triesObject);
        }
        if (!Gua_ObjectType(desiredObject)) {
            Gua_FreeObject(&desiredObject);
        }
        if (!Gua_ObjectType(toleranceObject)) {
            Gua_FreeObject(&toleranceObject);
        }
        return p;
    } else {
        if (token->length > 0) {
            test = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(test, '\0', sizeof(char) * (token->length + 1));
            strncpy(test, token->start, token->length);
        } else {
            test = (char *)Gua_Alloc(sizeof(char));
            test[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    /* Optionaly, we can have a CATCH code to evaluate if the TEST fail. */
    if (token->type == TOKEN_TYPE_CATCH) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            if (!Gua_IsObjectStored(triesObject)) {
                Gua_FreeObject(&triesObject);
            }
            if (!Gua_IsObjectStored(desiredObject)) {
                Gua_FreeObject(&desiredObject);
            }
            if (!Gua_IsObjectStored(toleranceObject)) {
                Gua_FreeObject(&toleranceObject);
            }
            Gua_Free(test);
            return p;
        }
        
        if (token->type != TOKEN_TYPE_BRACE) {
            *status = GUA_ERROR_UNEXPECTED_TOKEN;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            if (!Gua_IsObjectStored(triesObject)) {
                Gua_FreeObject(&triesObject);
            }
            if (!Gua_IsObjectStored(desiredObject)) {
                Gua_FreeObject(&desiredObject);
            }
            if (!Gua_IsObjectStored(toleranceObject)) {
                Gua_FreeObject(&toleranceObject);
            }
            Gua_Free(test);
            return p;
        } else {
            if (token->length > 0) {
                catch = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(catch, '\0', sizeof(char) * (token->length + 1));
                strncpy(catch, token->start, token->length);
            } else {
                catch = (char *)Gua_Alloc(sizeof(char));
                catch[0] = '\0';
            }
        }
        
        p = Gua_NextToken(nspace, p, token);
    } else {
        catch = (char *)Gua_Alloc(sizeof(char));
        catch[0] = '\0';
    }
    
    /* This is to calculate the statistics. */
    sx = 0.0;
    sx2 = 0.0;
    
    /* Reset the global variables. */
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_TRIES", &triesObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_DESIRED", &desiredObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_TOLERANCE", &toleranceObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_TIME", &timeObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_AVG", &avgObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_DEVIATION", &deviationObject, SCOPE_GLOBAL);
    
    /* Start the stopwatch. */
    gettimeofday(&tv1, &tz);
    
    /* Do the TEST. */
    match = 0;
    
    for (i = 1; i <=  Gua_ObjectToInteger(triesObject); i++) {
        Gua_Evaluate(nspace, test, &testObject, status, error);
        
        if (*status == GUA_OK) {
            Gua_LinkStringToObject(errorObject, error);
            Gua_SetStoredObject(errorObject);
            
            /* Calculate the statistics. */
            if (Gua_ObjectType(testObject) == OBJECT_TYPE_INTEGER) {
                sx = sx + Gua_ObjectToInteger(testObject);
                sx2 = sx2 + Gua_ObjectToInteger(testObject) * Gua_ObjectToInteger(testObject);
            } else if (Gua_ObjectType(testObject) == OBJECT_TYPE_REAL) {
                sx = sx + Gua_ObjectToReal(testObject);
                sx2 = sx2 + Gua_ObjectToReal(testObject) * Gua_ObjectToReal(testObject);
            }
            
            Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &testObject, SCOPE_GLOBAL);
            Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ERROR", &errorObject, SCOPE_GLOBAL);
            
            if (Gua_ObjectType(desiredObject) != OBJECT_TYPE_UNKNOWN) {
                /* Check if the object is exactly the expected. */
                if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_UNKNOWN) {
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_INTEGER)) {
                        if (Gua_ObjectToInteger(testObject) == Gua_ObjectToInteger(desiredObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_REAL) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_REAL)) {
                        if (Gua_ObjectToReal(testObject) == Gua_ObjectToReal(desiredObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_COMPLEX)) {
                        if ((Gua_ObjectToReal(testObject) == Gua_ObjectToReal(desiredObject)) && (Gua_ObjectToImaginary(testObject) == Gua_ObjectToImaginary(desiredObject))) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_STRING) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_STRING)) {
                        if (Gua_ObjectLength(testObject) == Gua_ObjectLength(desiredObject)) {
                            if (memcmp(Gua_ObjectToString(testObject), Gua_ObjectToString(desiredObject), Gua_ObjectLength(testObject)) == 0) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_ARRAY)) {
                        if (Gua_IsArrayEqual(&testObject, &desiredObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_MATRIX)) {
                        if (Gua_IsMatrixEqual(&testObject, &desiredObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_REAL)) {
                        if (Gua_ObjectToInteger(testObject) == Gua_ObjectToReal(desiredObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_REAL) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_INTEGER)) {
                        if (Gua_ObjectToReal(testObject) == Gua_ObjectToInteger(desiredObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                /* Check if the result object is out of range. */
                } else {
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_INTEGER)) {
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_INTEGER) {
                            if ((Gua_ObjectToInteger(testObject) >= (Gua_ObjectToInteger(desiredObject) - Gua_ObjectToInteger(toleranceObject))) && (Gua_ObjectToInteger(testObject) <= (Gua_ObjectToInteger(desiredObject) + Gua_ObjectToInteger(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_REAL) {
                            if ((Gua_ObjectToInteger(testObject) >= (Gua_ObjectToInteger(desiredObject) - Gua_ObjectToReal(toleranceObject))) && (Gua_ObjectToInteger(testObject) <= (Gua_ObjectToInteger(desiredObject) + Gua_ObjectToReal(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_REAL) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_REAL)) {
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_INTEGER) {
                            if ((Gua_ObjectToReal(testObject) >= (Gua_ObjectToReal(desiredObject) - Gua_ObjectToInteger(toleranceObject))) && (Gua_ObjectToReal(testObject) <= (Gua_ObjectToReal(desiredObject) + Gua_ObjectToInteger(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_REAL) {
                            if ((Gua_ObjectToReal(testObject) >= (Gua_ObjectToReal(desiredObject) - Gua_ObjectToReal(toleranceObject))) && (Gua_ObjectToReal(testObject) <= (Gua_ObjectToReal(desiredObject) + Gua_ObjectToReal(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_COMPLEX)) {
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_REAL) {
                            if ((Gua_ObjectToReal(testObject) >= (Gua_ObjectToReal(desiredObject) - Gua_ObjectToReal(toleranceObject))) && (Gua_ObjectToReal(testObject) <= (Gua_ObjectToReal(desiredObject) + Gua_ObjectToReal(toleranceObject)))) {
                                if ((Gua_ObjectToImaginary(testObject) >= (Gua_ObjectToImaginary(desiredObject) - Gua_ObjectToReal(toleranceObject))) && (Gua_ObjectToImaginary(testObject) <= (Gua_ObjectToImaginary(desiredObject) + Gua_ObjectToReal(toleranceObject)))) {
                                    match = 1;
                                } else {
                                    match = 0;
                                }
                            } else {
                                match = 0;
                            }
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_STRING) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_STRING)) {
                        match = 0;
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_ARRAY) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_ARRAY)) {
                        match = 0;
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_MATRIX) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_MATRIX)) {
                        if (Gua_IsMatrixApproximatelyEqual(&testObject, &desiredObject, &toleranceObject)) {
                            match = 1;
                        } else {
                            match = 0;
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_REAL)) {
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_INTEGER) {
                            if ((Gua_ObjectToInteger(testObject) >= (Gua_ObjectToReal(desiredObject) - Gua_ObjectToInteger(toleranceObject))) && (Gua_ObjectToInteger(testObject) <= (Gua_ObjectToReal(desiredObject) + Gua_ObjectToInteger(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_REAL) {
                            if ((Gua_ObjectToInteger(testObject) >= (Gua_ObjectToReal(desiredObject) - Gua_ObjectToReal(toleranceObject))) && (Gua_ObjectToInteger(testObject) <= (Gua_ObjectToReal(desiredObject) + Gua_ObjectToReal(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                    }
                    if ((Gua_ObjectType(testObject) == OBJECT_TYPE_REAL) && (Gua_ObjectType(desiredObject) == OBJECT_TYPE_INTEGER)) {
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_INTEGER) {
                            if ((Gua_ObjectToReal(testObject) >= (Gua_ObjectToInteger(desiredObject) - Gua_ObjectToInteger(toleranceObject))) && (Gua_ObjectToReal(testObject) <= (Gua_ObjectToInteger(desiredObject) + Gua_ObjectToInteger(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                        if (Gua_ObjectType(toleranceObject) == OBJECT_TYPE_REAL) {
                            if ((Gua_ObjectToReal(testObject) >= (Gua_ObjectToInteger(desiredObject) - Gua_ObjectToReal(toleranceObject))) && (Gua_ObjectToReal(testObject) <= (Gua_ObjectToInteger(desiredObject) + Gua_ObjectToReal(toleranceObject)))) {
                                match = 1;
                            } else {
                                match = 0;
                            }
                        }
                    }
                }
                /* The object is not the expected one. */
                if (!match) {
                    Gua_LinkStringToObject(errorObject, error);
                    Gua_SetStoredObject(errorObject);
                    
                    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &testObject, SCOPE_GLOBAL);
                    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ERROR", &errorObject, SCOPE_GLOBAL);
                    
                    Gua_Evaluate(nspace, catch, &catchObject, status, error);
                    
                    if (*status == GUA_OK) {
                        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &catchObject, SCOPE_GLOBAL);
                        break;
                    } else {
                        if (!Gua_IsObjectStored(triesObject)) {
                            Gua_FreeObject(&triesObject);
                        }
                        if (!Gua_IsObjectStored(desiredObject)) {
                            Gua_FreeObject(&desiredObject);
                        }
                        if (!Gua_IsObjectStored(toleranceObject)) {
                            Gua_FreeObject(&toleranceObject);
                        }
                        if (!Gua_IsObjectStored(testObject)) {
                            Gua_FreeObject(&testObject);
                        }
                        if (!Gua_IsObjectStored(catchObject)) {
                            Gua_FreeObject(&catchObject);
                        }
                        
                        Gua_Free(test);
                        Gua_Free(catch);
                        
                        return p;
                    }
                }
            }
        /* The TEST failed. */
        } else {
            Gua_LinkStringToObject(errorObject, error);
            Gua_SetStoredObject(errorObject);
            
            Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &catchObject, SCOPE_GLOBAL);
            Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ERROR", &errorObject, SCOPE_GLOBAL);
            
            Gua_Evaluate(nspace, catch, &catchObject, status, error);
            
            if (*status == GUA_OK) {
                Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &catchObject, SCOPE_GLOBAL);
                break;
            } else {
                if (!Gua_IsObjectStored(triesObject)) {
                    Gua_FreeObject(&triesObject);
                }
                if (!Gua_IsObjectStored(desiredObject)) {
                    Gua_FreeObject(&desiredObject);
                }
                if (!Gua_IsObjectStored(toleranceObject)) {
                    Gua_FreeObject(&toleranceObject);
                }
                if (!Gua_IsObjectStored(testObject)) {
                    Gua_FreeObject(&testObject);
                }
                if (!Gua_IsObjectStored(catchObject)) {
                    Gua_FreeObject(&catchObject);
                }
                
                Gua_Free(test);
                Gua_Free(catch);
                
                return p;
            }
        }
    }
    i--;
    if (i == 0) {
        i = 1;
    }
    
    /* Stop the stopwatch. */
    gettimeofday(&tv2, &tz);
    
    /* Calculate the elapsed time. */
    Gua_ElapsedTime(&elapsed, &tv2, &tv1);
    
    /* Calculate the statistics. */
    Gua_IntegerToObject(triesObject, i);
    
    Gua_RealToObject(timeObject, elapsed.tv_sec + (elapsed.tv_usec / 1000000.0));
    
    Gua_RealToObject(avgObject, sx / Gua_ObjectToInteger(triesObject));
    
    if (Gua_ObjectToInteger(triesObject) > 1) {
        Gua_RealToObject(deviationObject, sqrt((sx2 - (sx * sx) / Gua_ObjectToInteger(triesObject)) / (Gua_ObjectToInteger(triesObject) - 1)));
    } else {
        Gua_RealToObject(deviationObject, 0.0);
    }
    
    /* Save the results. */
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_TIME", &timeObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_AVG", &avgObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_DEVIATION", &deviationObject, SCOPE_GLOBAL);
    Gua_SetVariable((Gua_Namespace *)nspace, "GUA_TRIES", &triesObject, SCOPE_GLOBAL);
    
    Gua_ClearPObject(object);
    
    if (!Gua_IsObjectStored(triesObject)) {
        Gua_FreeObject(&triesObject);
    }
    if (!Gua_IsObjectStored(desiredObject)) {
        Gua_FreeObject(&desiredObject);
    }
    if (!Gua_IsObjectStored(toleranceObject)) {
        Gua_FreeObject(&toleranceObject);
    }
    if (!Gua_IsObjectStored(testObject)) {
        Gua_FreeObject(&testObject);
    }
    if (!Gua_IsObjectStored(catchObject)) {
        Gua_FreeObject(&catchObject);
    }
    
    Gua_Free(test);
    Gua_Free(catch);
    
    *status = GUA_OK;
    *error = '\0';
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseTry(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: try
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseTry(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object tryObject;
    Gua_Object catchObject;
    Gua_Object errorObject;
    Gua_String try;
    Gua_String catch;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(tryObject);
    Gua_ClearObject(catchObject);
    Gua_ClearObject(errorObject);
    Gua_ClearPObject(object);
    
    *status = GUA_OK;
    *error = '\0';

    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    }
    
    /* The first token after the TRY statement must be the code to try. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            try = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(try, '\0', sizeof(char) * (token->length + 1));
            strncpy(try, token->start, token->length);
        } else {
            try = (char *)Gua_Alloc(sizeof(char));
            try[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    /* After the code we can have an optional CATCH statement. */
    if (token->type == TOKEN_TYPE_CATCH) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(try);
            
            return p;
        }
        
        /* After CATCH we must have some code to avaluate. */
        if (token->type != TOKEN_TYPE_BRACE) {
            *status = GUA_ERROR_UNEXPECTED_TOKEN;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(try);
            
            return p;
        } else {
            if (token->length > 0) {
                catch = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(catch, '\0', sizeof(char) * (token->length + 1));
                strncpy(catch, token->start, token->length);
            } else {
                catch = (char *)Gua_Alloc(sizeof(char));
                catch[0] = '\0';
            }
        }
        
        p = Gua_NextToken(nspace, p, token);
    } else {
        catch = (char *)Gua_Alloc(sizeof(char));
        catch[0] = '\0';
    }
    
    /* Now run the code. */
    Gua_Evaluate(nspace, try, &tryObject, status, error);
        
    if (*status == GUA_OK) {
        Gua_LinkStringToObject(errorObject, error);
        Gua_SetStoredObject(errorObject);
        
        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &tryObject, SCOPE_GLOBAL);
        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ERROR", &errorObject, SCOPE_GLOBAL);
        
        Gua_LinkToPObject(object, tryObject);
    } else if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
        Gua_LinkStringToObject(errorObject, error);
        Gua_SetStoredObject(errorObject);
        
        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &tryObject, SCOPE_GLOBAL);
        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ERROR", &errorObject, SCOPE_GLOBAL);
        
        Gua_LinkToPObject(object, tryObject);
    } else {
        Gua_LinkStringToObject(errorObject, error);
        Gua_SetStoredObject(errorObject);
        
        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &catchObject, SCOPE_GLOBAL);
        Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ERROR", &errorObject, SCOPE_GLOBAL);
        
        /* The TRY fail. So run the CATCH code. */
        Gua_Evaluate(nspace, catch, &catchObject, status, error);
        
        if (*status == GUA_OK) {
            Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &catchObject, SCOPE_GLOBAL);
            
            Gua_LinkToPObject(object, catchObject);
        } else if ((*status == GUA_RETURN) || (*status == GUA_EXIT)) {
            Gua_SetVariable((Gua_Namespace *)nspace, "GUA_RESULT", &catchObject, SCOPE_GLOBAL);
        } else {
            if (!Gua_IsObjectStored(tryObject)) {
                Gua_FreeObject(&tryObject);
            }
            if (!Gua_IsObjectStored(catchObject)) {
                Gua_FreeObject(&catchObject);
            }
            
            Gua_Free(try);
            Gua_Free(catch);
            
            return p;
        }
    }
    
    if (!Gua_IsObjectStored(tryObject)) {
        Gua_FreeObject(&tryObject);
    }
    if (!Gua_IsObjectStored(catchObject)) {
        Gua_FreeObject(&catchObject);
    }
    
    Gua_Free(try);
    Gua_Free(catch);
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseForeach(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: foreach
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseForeach(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object array;
    Gua_Object keyObject;
    Gua_Object objectObject;
    Gua_Object codeObject;
    Gua_Element *element;
    Gua_String expression;
    Gua_String arrayExpression;
    Gua_String keyVariable;
    Gua_String objectVariable;
    Gua_String code;
    Gua_String s;
    Gua_String t;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(array);
    Gua_ClearObject(keyObject);
    Gua_ClearObject(objectObject);
    Gua_ClearObject(codeObject);
    Gua_ClearPObject(object);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    }
    
    /* The first token after the FOREACH statement must be parenthesis. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
    }
    
    /* Get the arguments. */
    argc = Gua_CountCommands(expression);
    
    if (argc != 3) {
        *status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for statement", "foreach");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(expression);
        
        return p;
    }
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    if (Gua_ParseCommands(nspace, expression, argc, argv, status, error) != GUA_OK) {
        Gua_Free(expression);
        return p;
    }
    Gua_Free(expression);
    
    arrayExpression = (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[0]) + 1));
    memset(arrayExpression, '\0', sizeof(char) * (Gua_ObjectLength(argv[0]) + 1));
    s = Gua_ObjectToString(argv[0]);    
    t = arrayExpression;
    while (*s) {
        if (!Gua_IsSpace(*s)) {
            *t = *s;
            t++;
        };
        s++;
    }
    *t = '\0';
    
    keyVariable = (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));
    memset(keyVariable, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));
    s = Gua_ObjectToString(argv[1]);
    t = keyVariable;
    while (*s) {
        if (!Gua_IsSpace(*s)) {
            *t = *s;
            t++;
        };
        s++;
    }
    *t = '\0';
    
    objectVariable = (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[2]) + 1));
    memset(objectVariable, '\0', sizeof(char) * (Gua_ObjectLength(argv[2]) + 1));
    s = Gua_ObjectToString(argv[2]);
    t = objectVariable;
    while (*s) {
        if (!Gua_IsSpace(*s)) {
            *t = *s;
            t++;
        };
        s++;
    }
    *t = '\0';
    
    Gua_FreeArguments(argc, argv);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(arrayExpression);
        Gua_Free(keyVariable);
        Gua_Free(objectVariable);
        
        return p;
    }
    
    /* After the arguments must exists some code to evaluate. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(arrayExpression);
        Gua_Free(keyVariable);
        Gua_Free(objectVariable);
        
        return p;
    } else {
        if (token->length > 0) {
            code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(code, '\0', sizeof(char) * (token->length + 1));
            strncpy(code, token->start, token->length);
        } else {
            code = (char *)Gua_Alloc(sizeof(char));
            code[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    /* Get the array expression to interact through. */
    Gua_Evaluate(nspace, arrayExpression, &array, status, error);
    
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(array)) {
            Gua_FreeObject(&array);
        }
        
        Gua_Free(arrayExpression);
        Gua_Free(keyVariable);
        Gua_Free(objectVariable);
        Gua_Free(code);
        
        return p;
    }
    
    Gua_Free(arrayExpression);
    
    element = (Gua_Element *)array.array;
    
    if (element) {
        /* Now do the loop. */
        while (true) {
            Gua_LinkObjects(keyObject, element->key);
            Gua_SetStoredObject(keyObject);
            
            Gua_LinkObjects(objectObject, element->object);
            Gua_SetStoredObject(objectObject);
            
            /* Set the loop variables. */
            if (Gua_SetVariable(nspace, keyVariable, &keyObject, SCOPE_LOCAL) != GUA_OK) {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "can't set variable", keyVariable);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (!Gua_IsObjectStored(array)) {
                    Gua_FreeObject(&array);
                }
                if (!Gua_IsObjectStored(keyObject)) {
                    Gua_FreeObject(&keyObject);
                }
                if (!Gua_IsObjectStored(objectObject)) {
                    Gua_FreeObject(&objectObject);
                }
                
                Gua_Free(keyVariable);
                Gua_Free(objectVariable);
                Gua_Free(code);
                
                return p;
            }
            if (Gua_SetVariable(nspace, objectVariable, &objectObject, SCOPE_LOCAL) != GUA_OK) {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "can't set variable", objectVariable);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (!Gua_IsObjectStored(array)) {
                    Gua_FreeObject(&array);
                }
                if (!Gua_IsObjectStored(keyObject)) {
                    Gua_FreeObject(&keyObject);
                }
                if (!Gua_IsObjectStored(objectObject)) {
                    Gua_FreeObject(&objectObject);
                }
                
                Gua_Free(keyVariable);
                Gua_Free(objectVariable);
                Gua_Free(code);
                
                return p;
            }
            
            if (!Gua_IsObjectStored(codeObject)) {
                Gua_FreeObject(&codeObject);
            }
            
            /* Evaluate the code. */
            Gua_Evaluate(nspace, code, &codeObject, status, error);
            
            if (*status == GUA_CONTINUE) {
                continue;
            } else if (*status == GUA_BREAK) {
                break;
            } else if (*status == GUA_RETURN) {
                break;
            } else if (*status == GUA_EXIT) {
                break;
            } else if (*status == GUA_OK) {
                if (element->next) {
                    element = (Gua_Element *)element->next;
                } else {
                    break;
                }
            } else {
                if (!Gua_IsObjectStored(array)) {
                    Gua_FreeObject(&array);
                }
                if (!Gua_IsObjectStored(keyObject)) {
                    Gua_FreeObject(&keyObject);
                }
                if (!Gua_IsObjectStored(objectObject)) {
                    Gua_FreeObject(&objectObject);
                }
                
                Gua_Free(keyVariable);
                Gua_Free(objectVariable);
                Gua_Free(code);
                
                return p;
            }
        }
    }
    
    Gua_LinkToPObject(object, codeObject);
    
    if (!Gua_IsObjectStored(array)) {
        Gua_FreeObject(&array);
    }
    if (!Gua_IsObjectStored(keyObject)) {
        Gua_FreeObject(&keyObject);
    }
    if (!Gua_IsObjectStored(objectObject)) {
        Gua_FreeObject(&objectObject);
    }
    
    Gua_Free(keyVariable);
    Gua_Free(objectVariable);
    Gua_Free(code);
    
    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
        *status = GUA_OK;
        *error = '\0';
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseFor(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: for
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseFor(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object initialisationObject;
    Gua_Object conditionObject;
    Gua_Object incrementObject;
    Gua_Object codeObject;
    Gua_String expression;
    Gua_String initialisation;
    Gua_String condition;
    Gua_String increment;
    Gua_String code;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(initialisationObject);
    Gua_ClearObject(conditionObject);
    Gua_ClearObject(incrementObject);
    Gua_ClearObject(codeObject);
    Gua_ClearPObject(object);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    }
    
    /* The first token after the FOR statement must be parenthesis. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            expression = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(expression, '\0', sizeof(char) * (token->length + 1));
            strncpy(expression, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
    }
    
    /* Get the initialization, condition and increment arguments. */
    argc = Gua_CountCommands(expression);
    
    if (argc != 3) {
        *status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for statement", "for");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(expression);
        
        return p;
    }
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    if (Gua_ParseCommands(nspace, expression, argc, argv, status, error) != GUA_OK) {
        Gua_Free(expression);
        return p;
    }
    Gua_Free(expression);
    
    initialisation = Gua_ObjectToString(argv[0]);
    condition = Gua_ObjectToString(argv[1]);
    increment = Gua_ObjectToString(argv[2]);
    
    Gua_Free(argv);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(initialisation);
        Gua_Free(condition);
        Gua_Free(increment);
        
        return p;
    }
    
    /* After the arguments must exists some code to evaluate. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(initialisation);
        Gua_Free(condition);
        Gua_Free(increment);
        
        return p;
    } else {
        if (token->length > 0) {
            code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(code, '\0', sizeof(char) * (token->length + 1));
            strncpy(code, token->start, token->length);
        } else {
            code = (char *)Gua_Alloc(sizeof(char));
            code[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    /* Do the initialization. */
    Gua_Evaluate(nspace, initialisation, &initialisationObject, status, error);
    
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(initialisationObject)) {
            Gua_FreeObject(&initialisationObject);
        }
        
        Gua_Free(initialisation);
        Gua_Free(condition);
        Gua_Free(increment);
        Gua_Free(code);
        
        return p;
    }
    
    if (!Gua_IsObjectStored(initialisationObject)) {
        Gua_FreeObject(&initialisationObject);
    }
    
    Gua_Free(initialisation);
    
    /* Evaluate the condition. */
    Gua_Evaluate(nspace, condition, &conditionObject, status, error);
    
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(conditionObject)) {
            Gua_FreeObject(&conditionObject);
        }
        
        Gua_Free(condition);
        Gua_Free(increment);
        Gua_Free(code);
        
        return p;
    }
    
    /* Now enter the loop. */
    while (Gua_ObjectToInteger(conditionObject)) {
        if (!Gua_IsObjectStored(codeObject)) {
            Gua_FreeObject(&codeObject);
        }
        
        Gua_Evaluate(nspace, code, &codeObject, status, error);
        
        /* The CONTINUE function was called. */
        if (*status == GUA_CONTINUE) {
            if (!Gua_IsObjectStored(incrementObject)) {
                Gua_FreeObject(&incrementObject);
            }
            
            Gua_Evaluate(nspace, increment, &incrementObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(incrementObject)) {
                    Gua_FreeObject(&incrementObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(increment);
                Gua_Free(code);
                
                return p;
            }
            
            if (!Gua_IsObjectStored(incrementObject)) {
                Gua_FreeObject(&incrementObject);
            }
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            
            Gua_Evaluate(nspace, condition, &conditionObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(incrementObject)) {
                    Gua_FreeObject(&incrementObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(increment);
                Gua_Free(code);
                
                return p;
            }
            
            continue;
        /* The BREAK function was called. */
        } else if (*status == GUA_BREAK) {
            break;
        } else if (*status == GUA_RETURN) {
            break;
        } else if (*status == GUA_EXIT) {
            break;
        } else if (*status == GUA_OK) {
            if (!Gua_IsObjectStored(incrementObject)) {
                Gua_FreeObject(&incrementObject);
            }
            
            /* Do the increment. */
            Gua_Evaluate(nspace, increment, &incrementObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(incrementObject)) {
                    Gua_FreeObject(&incrementObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(increment);
                Gua_Free(code);
                
                return p;
            }
            
            if (!Gua_IsObjectStored(incrementObject)) {
                Gua_FreeObject(&incrementObject);
            }
            
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            
            /* Reevaluate the condition to the next interaction. */
            Gua_Evaluate(nspace, condition, &conditionObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(incrementObject)) {
                    Gua_FreeObject(&incrementObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(increment);
                Gua_Free(code);
                
                return p;
            }
        } else {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            if (!Gua_IsObjectStored(incrementObject)) {
                Gua_FreeObject(&incrementObject);
            }
            if (!Gua_IsObjectStored(codeObject)) {
                Gua_FreeObject(&codeObject);
            }
            
            Gua_Free(condition);
            Gua_Free(increment);
            Gua_Free(code);
            
            return p;
        }
    }
    
    Gua_LinkToPObject(object, codeObject);
    
    if (!Gua_IsObjectStored(conditionObject)) {
        Gua_FreeObject(&conditionObject);
    }
    if (!Gua_IsObjectStored(incrementObject)) {
        Gua_FreeObject(&incrementObject);
    }
    
    Gua_Free(condition);
    Gua_Free(increment);
    Gua_Free(code);
    
    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
        *status = GUA_OK;
        *error = '\0';
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseDo(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: do
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseDo(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object conditionObject;
    Gua_Object codeObject;
    Gua_String condition;
    Gua_String code;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(conditionObject);
    Gua_ClearObject(codeObject);
    Gua_ClearPObject(object);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    }
    
    /* The first code after the DO statement is the code. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(code, '\0', sizeof(char) * (token->length + 1));
            strncpy(code, token->start, token->length);
        } else {
            code = (char *)Gua_Alloc(sizeof(char));
            code[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(code);
        
        return p;
    }
    
    /* After the code must exists an WHILE statement and its condition. */
    if (token->type != TOKEN_TYPE_WHILE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(code);
        
        return p;
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(code);
        
        return p;
    }
    
    /* Here is the condition. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(code);
        
        return p;
    } else {
        if (token->length > 0) {
            condition = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(condition, '\0', sizeof(char) * (token->length + 1));
            strncpy(condition, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(code);
            
            return p;
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    /* Now enter the loop. */
    do {
        if (!Gua_IsObjectStored(codeObject)) {
            Gua_FreeObject(&codeObject);
        }
        
        /* Evaluate the code first. */
        Gua_Evaluate(nspace, code, &codeObject, status, error);
        
        /* The CONTINUE function was called. */
        if (*status == GUA_CONTINUE) {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            
            Gua_Evaluate(nspace, condition, &conditionObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(code);
                
                return p;
            }
            
            continue;
        /* The BREAK function was called. */
        } else if (*status == GUA_BREAK) {
            break;
        } else if (*status == GUA_RETURN) {
            break;
        } else if (*status == GUA_EXIT) {
            break;
        } else if (*status == GUA_OK) {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            
            Gua_Evaluate(nspace, condition, &conditionObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(code);
                
                return p;
            }
        } else {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            if (!Gua_IsObjectStored(codeObject)) {
                Gua_FreeObject(&codeObject);
            }
            
            Gua_Free(condition);
            Gua_Free(code);
            
            return p;
        }
    /* Now lets evaluate the condition. */
    } while (Gua_ObjectToInteger(conditionObject));
    
    Gua_LinkToPObject(object, codeObject);
    
    if (!Gua_IsObjectStored(conditionObject)) {
        Gua_FreeObject(&conditionObject);
    }
    
    Gua_Free(condition);
    Gua_Free(code);
    
    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
        *status = GUA_OK;
        *error = '\0';
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_ParseWhile(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: while
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_ParseWhile(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object conditionObject;
    Gua_Object codeObject;
    Gua_String condition;
    Gua_String code;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(conditionObject);
    Gua_ClearObject(codeObject);
    Gua_ClearPObject(object);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    }
    
    /* The first token after the WHILE statement must be parenthesis. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            condition = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(condition, '\0', sizeof(char) * (token->length + 1));
            strncpy(condition, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(condition);
        
        return p;
    }
    
    /* Atfer the condition must have some code to evaluate. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(condition);
        
        return p;
    } else {
        if (token->length > 0) {
            code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(code, '\0', sizeof(char) * (token->length + 1));
            strncpy(code, token->start, token->length);
        } else {
            code = (char *)Gua_Alloc(sizeof(char));
            code[0] = '\0';
        }
    }
    
    p = Gua_NextToken(nspace, p, token);
    
    /* This is a WHILE statement so we must evaluate the condition before
     * run the code.
     */
    Gua_Evaluate(nspace, condition, &conditionObject, status, error);
    
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(conditionObject)) {
            Gua_FreeObject(&conditionObject);
        }
        
        Gua_Free(condition);
        Gua_Free(code);
        
        return p;
    }
    
    /* Now enter the loop. */
    while (Gua_ObjectToInteger(conditionObject)) {
        if (!Gua_IsObjectStored(codeObject)) {
            Gua_FreeObject(&codeObject);
        }
        
        /* We evaluate the code here. */
        Gua_Evaluate(nspace, code, &codeObject, status, error);
        
        /* The CONTINUE function was called. */
        if (*status == GUA_CONTINUE) {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            
            /* Now reevaluate the condition to the next interaction. */
            Gua_Evaluate(nspace, condition, &conditionObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(code);
                
                return p;
            }
            
            continue;
        /* The BREAK function was called. */
        } else if (*status == GUA_BREAK) {
            break;
        } else if (*status == GUA_RETURN) {
            break;
        } else if (*status == GUA_EXIT) {
            break;
        } else if (*status == GUA_OK) {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            
            Gua_Evaluate(nspace, condition, &conditionObject, status, error);
            
            if (*status != GUA_OK) {
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                
                Gua_Free(condition);
                Gua_Free(code);
                
                return p;
            }
        } else {
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            if (!Gua_IsObjectStored(codeObject)) {
                Gua_FreeObject(&codeObject);
            }
            
            Gua_Free(condition);
            Gua_Free(code);
            
            return p;
        }
    }
    
    Gua_LinkToPObject(object, codeObject);
    
    if (!Gua_IsObjectStored(conditionObject)) {
        Gua_FreeObject(&conditionObject);
    }
    
    Gua_Free(condition);
    Gua_Free(code);

    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
        *status = GUA_OK;
        *error = '\0';
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_Parseif (Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses the structure: if
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     token,     a pointer to a structure containing the last token found;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise;
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_Parseif (Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Object conditionObject;
    Gua_Object codeObject;
    Gua_String condition;
    Gua_String code;
    Gua_Short found;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearObject(conditionObject);
    Gua_ClearObject(codeObject);
    Gua_ClearPObject(object);
    
    /* found will be true if a condition returns true. */
    found = false;
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    }
    
    /* The first token after IF must be parenthesis. */
    if (token->type != TOKEN_TYPE_PARENTHESIS) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return p;
    } else {
        if (token->length > 0) {
            condition = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
            memset(condition, '\0', sizeof(char) * (token->length + 1));
            strncpy(condition, token->start, token->length);
        } else {
            *status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return p;
        }
    }
    Gua_Evaluate(nspace, condition, &conditionObject, status, error);
    if (*status != GUA_OK) {
        if (!Gua_IsObjectStored(conditionObject)) {
            Gua_FreeObject(&conditionObject);
        }
        Gua_Free(condition);
        return p;
    }
    Gua_Free(condition);
    
    p = Gua_NextToken(nspace, p, token);
    
    if (token->status != GUA_OK) {
        *status = token->status;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        Gua_Free(condition);
        
        return p;
    }
    
    /* After parenthesis must exists some code to evalute. */
    if (token->type != TOKEN_TYPE_BRACE) {
        *status = GUA_ERROR_UNEXPECTED_TOKEN;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        if (!Gua_IsObjectStored(conditionObject)) {
            Gua_FreeObject(&conditionObject);
        }
        return p;
    } else {
        if (conditionObject.integer) {
            if (token->length > 0) {
                code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                memset(code, '\0', sizeof(char) * (token->length + 1));
                strncpy(code, token->start, token->length);
            } else {
                code = (char *)Gua_Alloc(sizeof(char));
                code[0] = '\0';
            }
            if (!Gua_IsObjectStored(conditionObject)) {
                Gua_FreeObject(&conditionObject);
            }
            found = true;
        }
    }
    
    /* If the condition to the IF statement was true,
     * we just clean up the script removing any ELSEIF or ELSE statement.
     * Otherwise, we look at the ELSEIF conditions or execute the ELSE
     * statement if it exists.
     */
    while (true) {
        p = Gua_NextToken(nspace, p, token);
        
        if (token->status != GUA_OK) {
            *status = token->status;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(code);
            
            return p;
        }
        
        /* Have a look at the ELSEIF statement. */
        if (token->type == TOKEN_TYPE_ELSEIF) {
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return p;
            }
            
            if (token->type != TOKEN_TYPE_PARENTHESIS) {
                *status = GUA_ERROR_UNEXPECTED_TOKEN;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (found) {
                    Gua_Free(code);
                }
                return p;
            } else {
                if (!found) {
                    if (token->length > 0) {
                        condition = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(condition, '\0', sizeof(char) * (token->length + 1));
                        strncpy(condition, token->start, token->length);
                    } else {
                        *status = GUA_ERROR;
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return p;
                    }
                }
            }
            
            /* Just evaluate this condition if no other true condition was found yet. */
            if (!found) {
                Gua_Evaluate(nspace, condition, &conditionObject, status, error);
                if (*status != GUA_OK) {
                    if (!Gua_IsObjectStored(conditionObject)) {
                        Gua_FreeObject(&conditionObject);
                    }
                    Gua_Free(condition);
                    return p;
                }
                Gua_Free(condition);
            }
            
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (found) {
                    Gua_Free(code);
                }
                return p;
            }
            
            if (token->type != TOKEN_TYPE_BRACE) {
                *status = GUA_ERROR_UNEXPECTED_TOKEN;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (!Gua_IsObjectStored(conditionObject)) {
                    Gua_FreeObject(&conditionObject);
                }
                if (found) {
                    Gua_Free(code);
                }
                return p;
            } else {
                /* Just evaluate this code if no other true condition was found yet. */
                if (!found) {
                    if (conditionObject.integer) {
                        if (token->length > 0) {
                            code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                            memset(code, '\0', sizeof(char) * (token->length + 1));
                            strncpy(code, token->start, token->length);
                        } else {
                            code = (char *)Gua_Alloc(sizeof(char));
                            code[0] = '\0';
                        }
                        if (!Gua_IsObjectStored(conditionObject)) {
                            Gua_FreeObject(&conditionObject);
                        }
                        found = true;
                    }
                }
            }
        /* Have a look at the ELSE statement. */
        } else if (token->type == TOKEN_TYPE_ELSE) {
            p = Gua_NextToken(nspace, p, token);
            
            if (token->status != GUA_OK) {
                *status = token->status;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[token->status]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (found) {
                    Gua_Free(code);
                }
                return p;
            }
            
            if (token->type != TOKEN_TYPE_BRACE) {
                *status = GUA_ERROR_UNEXPECTED_TOKEN;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %*.*s...\n", Gua_StatusTable[GUA_ERROR_UNEXPECTED_TOKEN], (int)token->length, (int)token->length, token->start);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                if (found) {
                    Gua_Free(code);
                }
                return p;
            } else {
                if (!found) {
                    if (token->length > 0) {
                        code = (char *)Gua_Alloc(sizeof(char) * (token->length + 1));
                        memset(code, '\0', sizeof(char) * (token->length + 1));
                        strncpy(code, token->start, token->length);
                    } else {
                        code = (char *)Gua_Alloc(sizeof(char));
                        code[0] = '\0';
                    }
                    found = true;
                }
            }
        /* No more elseif or else statements to process. */
        } else {
            break;
        }
    }
    
    /* Some condition was evaluated as true. So lets evaluate it. */
    if (found) {
        Gua_Evaluate(nspace, code, &codeObject, status, error);
        if (*status != GUA_OK) {
            if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
                if (!Gua_IsObjectStored(codeObject)) {
                    Gua_FreeObject(&codeObject);
                }
                Gua_Free(code);
                return p;
            }
        }
        Gua_Free(code);
    }
    
    Gua_LinkToPObject(object, codeObject);
    
    if (!((*status == GUA_RETURN) || (*status == GUA_EXIT))) {
        *status = GUA_OK;
        *error = '\0';
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_Expression(Gua_Namespace *nspace, Gua_String start, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: separator and comments
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise.
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_Expression(Gua_Namespace *nspace, Gua_String start, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Token token;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearPObject(object);
    
    *status = GUA_OK;
    *error = '\0';
    
    p = Gua_NextToken(nspace, p, &token);
    
    while (true) {
        p = Gua_ParseAssign(nspace, p, &token, object, status, error);
        
        if (*status == GUA_CONTINUE) {
            break;
        }
        if (*status == GUA_BREAK) {
            break;
        }
        if (*status == GUA_RETURN) {
            break;
        }
        if (*status == GUA_EXIT) {
            break;
        }
        
        if (token.type == TOKEN_TYPE_SEPARATOR) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
        } else if (token.type == TOKEN_TYPE_END) {
            break;
        } else {
            if (*status == GUA_OK) {
                *status = GUA_ERROR;
                
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s...\n", Gua_StatusTable[GUA_ERROR]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
            break;
        }
        
        p = Gua_NextToken(nspace, p, &token);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Gua_Evaluate(Gua_Namespace *nspace, Gua_String start, Gua_Object *object, Gua_Status *status, Gua_String error)
 *
 * Description:
 *     Parse the expression and return its numeric or string object.
 *     There is a parser to each precedence level. One calling each other.
 *
 *     This parser parses operators: separator and comments
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     start,     a pointer to the start point of the expression to parse;
 *     object,    a structure containing the return object of the expression;
 *     status,    the parse status. GUA_OK if no error has occurred,
 *                a parse error number otherwise.
 *     error,     the error message if any.
 *
 * Results:
 *     The function returns the next start point to search tokens in
 *     the expression.
 *
 */
Gua_String Gua_Evaluate(Gua_Namespace *nspace, Gua_String start, Gua_Object *object, Gua_Status *status, Gua_String error)
{
    Gua_String p;
    Gua_Token token;
    Gua_Token cmdToken;
    Gua_String cmdStart;
    Gua_String cmdEnd;
    Gua_Integer cmdLength;
    Gua_String errMessage;
    
    p = start;
    
    Gua_ClearPObject(object);
    
    *status = GUA_OK;
    *error = '\0';
    
    p = Gua_NextToken(nspace, p, &token);
    
    while (true) {
        cmdToken = token;
        
        if (token.type == TOKEN_TYPE_SEPARATOR) {
            p = Gua_NextToken(nspace, p, &token);
            continue;
        } else if (token.type == TOKEN_TYPE_COMMENT) {
            p = Gua_NextToken(nspace, p, &token);
            continue;
        } else if (token.type == TOKEN_TYPE_END) {
            break;
        }
        
        if (token.type == TOKEN_TYPE_IF) {
            if (!object->stored) {
                Gua_FreeObject(object);
            }
            p = Gua_Parseif (nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_WHILE) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseWhile(nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_DO) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseDo(nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_FOR) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseFor(nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_FOREACH) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseForeach(nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_DEFINE_FUNCTION  ) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseFunction(nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_TRY) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseTry(nspace, p, &token, object, status, error);
        } else if (token.type == TOKEN_TYPE_TEST) {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseTest(nspace, p, &token, object, status, error);
        } else {
            if (!Gua_IsPObjectStored(object)) {
                Gua_FreeObject(object);
            }
            p = Gua_ParseAssign(nspace, p, &token, object, status, error);
        }
        
        if (*status >= GUA_ERROR) {
            if (strlen(error) > 0) {
                /* Get the beginning of the command. */
                cmdStart = cmdToken.start;
                
                /* Search for the end of the command. */
                cmdEnd = cmdStart;
                
                while (!(Gua_IsSeparator(*cmdEnd) || (*cmdEnd == EXPRESSION_END))) {
                    if (cmdEnd != p) {
                        cmdEnd++;
                    } else {
                        break;
                    }
                }
                
                /* Calculates the command length. */
                cmdLength = (Gua_Length)(cmdEnd - cmdStart);
                
                /* Displays the error message and the command that caused the problem. */
                if (cmdLength > 64) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s at %64.64s...\n", error, cmdStart);
                    strcpy(error, errMessage);
                    Gua_Free(errMessage);
                } else {
                    if (cmdLength > 0) {
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s at %.*s...\n", error, (int)cmdLength, cmdStart);
                        strcpy(error, errMessage);
                        Gua_Free(errMessage);
                    }
                }
            }
            
            break;
        }
        
        if (*status == GUA_CONTINUE) {
            break;
        }
        if (*status == GUA_BREAK) {
            break;
        }
        if (*status == GUA_RETURN) {
            break;
        }
        if (*status == GUA_EXIT) {
            break;
        }
        
        if (token.type == TOKEN_TYPE_COMMENT) {
            continue;
        } else if (token.type == TOKEN_TYPE_END) {
            break;
        }
        
        p = Gua_NextToken(nspace, p, &token);
    }
    
    return p;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_ParseVarAssignment(Gua_String expression, Gua_String variable, Gua_String value)
 *
 * Description:
 *     Parses a variable assignment like VARIABLE=VALUE.
 *
 * Arguments:
 *     expression,    the resource string;
 *     variable,      the variable name;
 *     value,         the variable value.
 *
 * Results:
 *     The function returns the variable name and its object.
 */
void Gua_ParseVarAssignment(Gua_String expression, Gua_String variable, Gua_String value)
{
    Gua_String p;
    Gua_String n;
    Gua_String v;
    Gua_Short token;
    
    #define VARIABLE 0
    #define VALUE    1
    
    p = expression;
    n = variable;
    v = value;
    
    token = VARIABLE;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    while (*p) {
        if ((*p == '=') && (token == VARIABLE)) {
            token = VALUE;
            p++;
            while (Gua_IsSpace(*p)) {
                p++;
            }
        }
        if (token == VARIABLE) {
            if (!Gua_IsSpace(*p)) {
                *n = *p;
                n++;
            }
        }
        if (token == VALUE) {
            *v = *p;
            v++;
        }
        if (*p) {
            p++;
        }
    }
    
    *n = '\0';
    *v = '\0';
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_KeyValuePairsToArray(Gua_Short n, Gua_String *key, Gua_String *value, Gua_Object *object)
 *
 * Description:
 *     Create an array from key, value pairs.
 *
 * Arguments:
 *     n,         the number of elements;
 *     key,       array keys;
 *     value,     array values;
 *     object,    a pointer to an array object.
 *
 * Results:
 *     The function create an array from key, value pairs.
 */
void Gua_KeyValuePairsToArray(Gua_Short n, Gua_String *key, Gua_String *value, Gua_Object *object)
{
    Gua_Short i;
    Gua_Element *previous;
    Gua_Element *newElement;
    
    Gua_ClearPObject(object);
    
    for (i = 0; i < n; i++) {
        if (i == 0) {
            /* Create the first element. */
            newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
            /* The element key. */
            Gua_StringToObject(newElement->key, key[i]);
            /* The element object. */
            if (value[i] != NULL) {
                Gua_StringToObject(newElement->object, value[i]);
            } else {
                Gua_StringToObject(newElement->object, "");
            }
            
            /* Set the target array chain. */
            newElement->previous = NULL;
            newElement->next = NULL;
            
            /* Link the first element. */
            Gua_ArrayToPObject(object, (struct Gua_Element *)newElement, 1);
            
            previous = (Gua_Element *)Gua_PObjectToArray(object);
        } else {
            /* Create a new element. */
            newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
            /* The element key. */
            Gua_StringToObject(newElement->key, key[i]);
            /* The element object. */
            if (value[i] != NULL) {
                Gua_StringToObject(newElement->object, value[i]);
            } else {
                Gua_StringToObject(newElement->object, "");
            }
            
            /* Set the target array chain. */
            newElement->previous = (struct Gua_Element *)previous;
            newElement->next = NULL;
            
            previous->next = (struct Gua_Element *)newElement;
            previous = newElement;
        }
    }
    
    /* Update the array length entry. */
    Gua_SetPObjectLength(object, n);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Gua_ArgvToArray(int argc, char **argv, Gua_Object *object)
 *
 * Description:
 *     Create an array from an argv like array.
 *
 * Arguments:
 *     argc,      the number of elements;
 *     argv,      array keys;
 *     object,    a pointer to a string object.
 *
 * Results:
 *     The function create an array from key, value pairs.
 */
void Gua_ArgvToArray(int argc, char **argv, Gua_Object *object)
{
    Gua_Short i;
    Gua_Element *previous;
    Gua_Element *newElement;
    
    Gua_ClearPObject(object);
    
    for (i = 0; i < argc; i++) {
        if (i == 0) {
            /* Create the first element. */
            newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
            /* The element key. */
            Gua_IntegerToObject(newElement->key, i);
            /* The element object. */
            Gua_StringToObject(newElement->object, argv[i]);
            
            /* Set the target array chain. */
            newElement->previous = NULL;
            newElement->next = NULL;
            
            /* Link the first element. */
            Gua_ArrayToPObject(object, (struct Gua_Element *)newElement, 1);
            
            previous = (Gua_Element *)Gua_PObjectToArray(object);
        } else {
            /* Create a new element. */
            newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
            /* The element key. */
            Gua_IntegerToObject(newElement->key, i);
            /* The element object. */
            Gua_StringToObject(newElement->object, argv[i]);
            
            /* Set the target array chain. */
            newElement->previous = (struct Gua_Element *)previous;
            newElement->next = NULL;
            
            previous->next = (struct Gua_Element *)newElement;
            previous = newElement;
        }
    }
    
    /* Update the array length entry. */
    Gua_SetPObjectLength(object, argc);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gua_Init(Gua_Namespace *nspace, int argc, char *argv[], char **env)
 *
 * Description:
 *     Create the built in variables and functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns GUA_OK is the system is setup successfully.
 *
 */
Gua_Status Gua_Init(Gua_Namespace *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String expression;
    Gua_String p;
    Gua_Status status;
    char **environment;
    Gua_String varName;
    Gua_String varObject;
    Gua_Short i;
    Gua_Short j;
    Gua_String realPath;
    Gua_String unixPath;
    Gua_String errMessage;
    
    /* Install the built in functions. */
    Gua_LinkCFunctionToFunction(function, Gua_BuiltInFunction);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "array", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "array");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "arrayToString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "arrayToString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "break", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "break");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "complex", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "complex");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "continue", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "continue");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "dim", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "dim");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "error", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "error");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "eval", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "eval");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "exists", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "exists");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "exit", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "exit");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "expr", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "expr");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ident", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ident");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "inv", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "inv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isMatrixApproximatelyEqual", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isMatrixApproximatelyEqual");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "keys", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "keys");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "length", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "length");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "matrix", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "matrix");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "matrix2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "matrix2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "matrixToString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "matrixToString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "return", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "return");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "toString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "toString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "type", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "type");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Install the built in variables. */
    Gua_IntegerToObject(object, OBJECT_TYPE_ARRAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_ARRAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_ARRAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_COMPLEX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_COMPLEX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_COMPLEX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_FILE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_FILE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_FILE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_HANDLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_HANDLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_HANDLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_INTEGER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_INTEGER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_INTEGER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_MATRIX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_MATRIX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_MATRIX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_NAMESPACE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_NAMESPACE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_NAMESPACE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_REAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_REAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_REAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, OBJECT_TYPE_STRING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_STRING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_STRING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Setup argc and argv variables. */
    Gua_IntegerToObject(object, argc);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "argc", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "argc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    Gua_ClearObject(object);
    
    /* Get the command line arguments. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    error = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
    
    for (i = 0; i < argc; i++) {
        /* Convert Windows path to Unix path. */
        if ((i == 0) || (i == 1)) {
            realPath = argv[i];
            
            unixPath = (Gua_String)malloc(sizeof(Gua_Char) * strlen(realPath) + 1);
            for (j = 0; j < strlen(realPath); j++) {
                unixPath[j] = realPath[j] == '\\' ? '/' : realPath[j];
            }
            unixPath[j] = '\0';
            
            sprintf(expression, "argv[%d] = \"%s\";", i, unixPath);
            
            Gua_Free(unixPath);
        } else {
            sprintf(expression, "argv[%d] = \"%s\";", i, argv[i]);
        }
        
        p = expression;
        p = Gua_Expression(nspace, p, &object, &status, error);
        if (!Gua_IsObjectStored(object)) {
            Gua_FreeObject(&object);
        }
    }
    
    /* Get the environment variables. */
    varName = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
    varObject = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
    
    environment = env;
    
    while (*environment) {
        Gua_ParseVarAssignment(*environment, varName, varObject);
        sprintf(expression, "env[\"%s\"] = \"%s\";", varName, varObject);
        p = expression;
        p = Gua_Expression(nspace, p, &object, &status, error);
        if (!Gua_IsObjectStored(object)) {
            Gua_FreeObject(&object);
        }
        environment++;
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    Gua_Free(varName);
    Gua_Free(varObject);
    
    return GUA_OK;
}

