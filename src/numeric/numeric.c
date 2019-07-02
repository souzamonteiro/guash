/**
 * File:
 *     numeric.c
 *
 * Package:
 *     Numeric
 *
 * Description:
 *     This file implements the numeric computation functions for an expression interpreter.
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
 * RCS: @(#) $Id: numeric.c,v 1.5 2017/05/27 09:42:00 monteiro Exp $
 *
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "matrix.h"
#include "numeric.h"

#define ERROR_SIZE 65536
#define GUA_SIZE  65536

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Numeric_GaussLSS(Gua_Object *a, Gua_Object *b, Gua_Object *x, Gua_String error)
 *
 * Description:
 *     Solve a linear system in the form A * X = B.
 *
 * Arguments:
 *     a,          the a matrix;
 *     b,          the b matrix;
 *     x,          a structure containing the return object of the function;
 *     error,      a pointer to the error message.
 *
 * Results:
 *     The function returns the linear system solution.
 */
Gua_Status Numeric_GaussLSS(Gua_Object *a, Gua_Object *b, Gua_Object *x, Gua_String error)
{
    Gua_Namespace *nspace;
    Gua_Object c;
    Gua_Object d;
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Matrix *m4;
    Gua_Matrix *m5;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *o3;
    Gua_Object *o4;
    Gua_Object *o5;
    Gua_Object *argv;
    Gua_Short argc;
    Gua_Real sum;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer m;
    Gua_Integer n;
    Gua_String errMessage;
    
    Gua_ClearObject(c);
    Gua_ClearObject(d);

    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_PObjectType(b) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument 2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        nspace = (Gua_Namespace *)Gua_Alloc(sizeof(Gua_Namespace));
        
        Gua_ClearNamespace(nspace);
        
        argc = 3;
        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
        Gua_LinkStringToObject(argv[0], "zero");
        Gua_SetStoredObject(argv[0]);
        
        if(!Gua_IsPObjectStored(x)) {
            Gua_FreeObject(x);
        } else {
            Gua_ClearPObject(x);
        }
        
        o1 = (Gua_Object *)m1->object;
        
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        o2 = (Gua_Object *)m2->object;
        
        m = m1->dimv[0];
        n = m1->dimv[1];
        
        /* Create the [A|B] matrix. */
        Gua_IntegerToObject(argv[1], m);
        Gua_SetStoredObject(argv[1]);
        
        Gua_IntegerToObject(argv[2], n + 1);
        Gua_SetStoredObject(argv[2]);
        
        Matrix_MatrixFunctionWrapper(nspace, argc, argv, &c, error);
    
        m3 = (Gua_Matrix *)Gua_ObjectToMatrix(c);
        o3 = (Gua_Object *)m3->object;
    
        m = m3->dimv[0];
        n = m3->dimv[1];
    
        /* Copy A and B matrices to the [A|B] matrix. */
        /* The A matrix. */
        for (i = 0; i < m1->dimv[0]; i++) {
            for (j = 0; j < m1->dimv[1]; j++) {
                if (Gua_ObjectType(o1[i * m1->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
                    Gua_RealToObject(o3[i * n + j], Gua_ObjectToInteger(o1[i * m1->dimv[1] + j]));
                } else if (Gua_ObjectType(o1[i * m1->dimv[1] + j]) == OBJECT_TYPE_REAL) {
                    Gua_RealToObject(o3[i * n + j], Gua_ObjectToReal(o1[i * m1->dimv[1] + j]));
                } else {
                    Gua_RealToObject(o3[i * n + j], 0.0);
                }
            }
        }
        /* The B matrix. */
        j = m3->dimv[1] - 1;
        for (i = 0; i < m2->dimv[0]; i++) {
            if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o3[i * n + j], Gua_ObjectToInteger(o2[i]));
            } else if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_REAL) {
                Gua_RealToObject(o3[i * n + j], Gua_ObjectToReal(o2[i]));
            } else {
                Gua_RealToObject(o3[i * n + j], 0.0);
            }
        }
        
        /* Converts the [A|B] matrix to the triangular form. */
        if (Matrix_GaussMatrix(&c, &d, error) == 0.0) {
            Gua_FreeNamespace(nspace);
            Gua_FreeArguments(argc, argv);
            Gua_FreeObject(&c);
            
            return GUA_ERROR;
        }
    
        m4 = (Gua_Matrix *)Gua_ObjectToMatrix(d);
        o4 = (Gua_Object *)m4->object;
        
        m = m4->dimv[0];
        n = m4->dimv[1];
    
        /* Create the result matrix X. */
        Gua_IntegerToObject(argv[1], m);
        Gua_SetStoredObject(argv[1]);
        
        Gua_IntegerToObject(argv[2], 1);
        Gua_SetStoredObject(argv[2]);
        
        Matrix_MatrixFunctionWrapper(nspace, argc, argv, x, error);
    
        m5 = (Gua_Matrix *)Gua_PObjectToMatrix(x);
        o5 = (Gua_Object *)m5->object;
    
        /* Find the solution. */
        Gua_RealToObject(o5[m - 1], (Gua_ObjectToReal(o4[(m - 1) * n + n - 1]) / Gua_ObjectToReal(o4[(m - 1) * n + m - 1])));
    
        for (i = m - 2; i >= 0; i--) {
            sum = 0;
    
            for (j = n - 2; j >= i; j--) {
                sum = sum + Gua_ObjectToReal(o4[i * n + j]) * Gua_ObjectToReal(o5[j]);
            }
    
            Gua_RealToObject(o5[i], ((Gua_ObjectToReal(o4[i * n + n - 1]) - sum) / Gua_ObjectToReal(o4[i * n + i])));
        }
       
        Gua_FreeNamespace(nspace);
        Gua_FreeArguments(argc, argv);
        Gua_FreeObject(&c);
        Gua_FreeObject(&d);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Numeric_NumericFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Numeric computation functions wrapper.
 *
 * Arguments:
 *     nspace,    a pointer to a structure Gua_Namespace. Must do a cast before use it;
 *     argc,      the number of arguments to pass to the function;
 *     argv,      an array containing the arguments to the function;
 *                argv[0] is the function name;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The return object of the wrapped function.
 */
Gua_Status Numeric_NumericFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Real eps;
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);

    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "gaussLSS") == 0) {
        if (argc != 3) {
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
        
        m1 = (Gua_Matrix *)Gua_ObjectToMatrix(argv[1]);
        m2 = (Gua_Matrix *)Gua_ObjectToMatrix(argv[2]);
        
        if (m1->dimv[0] != m2->dimv[0]) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m2->dimv[1] != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Numeric_GaussLSS(&argv[1], &argv[2], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "epsilon") == 0) {
        eps = 1.0;
        
        while ((eps + 1.0) > 1.0) {
           eps=eps / 2;
        }
        
        eps = eps * 2.0;
        
        Gua_RealToPObject(object, eps);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Numeric_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the numeric computation functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the numeric computation functions.
 */
Gua_Status Numeric_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String expression;
    Gua_String p;
    Gua_Status status;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Numeric_NumericFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gaussLSS", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gaussLSS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Numeric_NumericFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "epsilon", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "epsilon");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    Gua_RealToObject(object, NUMERIC_GOLDEN_NUMBER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GOLDEN_NUMBER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GOLDEN_NUMBER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, NUMERIC_X);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "NUM_X", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "NUM_X");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    Gua_ClearObject(object);
    
    /* Set the NUM_XYZ array. */
    expression = (Gua_String)Gua_Alloc(sizeof(char) * GUA_SIZE);
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    
    sprintf(expression, "NUM_XYZ = %s;", NUMERIC_XYZ);
    p = expression;
    p = Gua_Expression(nspace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     NUM_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, NUMERIC_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "NUM_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "NUM_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}
