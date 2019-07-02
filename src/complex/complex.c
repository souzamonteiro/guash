/**
 * File:
 *     complex.c
 *
 * Package:
 *     Complex
 *
 * Description:
 *     This library implements the complex functions for an expression interpreter.
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
 * RCS: @(#) $Id: complex.c,v 1.4 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "complex.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Complex_ComplexFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Complex functions wrapper.
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
 *     The return object of the wrapped funcion.
 */
Gua_Status Complex_ComplexFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     abs(value)
     *
     * Description:
     *     Returns the modulus of a complex number.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "abs") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_COMPLEX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_RealToPObject(object, (sqrt(Gua_ObjectToReal(argv[1]) * Gua_ObjectToReal(argv[1]) + Gua_ObjectToImaginary(argv[1]) * Gua_ObjectToImaginary(argv[1]))));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     arg(value)
     *
     * Description:
     *     Returns the argument of a complex number.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "arg") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_COMPLEX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_RealToPObject(object, (asin(Gua_ObjectToImaginary(argv[1]) / (sqrt(Gua_ObjectToReal(argv[1]) * Gua_ObjectToReal(argv[1]) + Gua_ObjectToImaginary(argv[1]) * Gua_ObjectToImaginary(argv[1]))))));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     conj(value)
     *
     * Description:
     *     Returns the complex conjugate number.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "conj") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_COMPLEX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_ComplexToPObject(object, Gua_ObjectToReal(argv[1]), -(Gua_ObjectToImaginary(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     imag(value)
     *
     * Description:
     *     Returns the imaginary part of a complex number.
     *
     * Examples:
     *     a=1.0+2.0*i
     *     c=imag(a) # c contains the value 2.0.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "imag") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_COMPLEX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_RealToPObject(object, Gua_ObjectToImaginary(argv[1]));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     real(value)
     *
     * Description:
     *     Returns the real part of a complex number.
     *
     * Examples:
     *     a=1.0+2.0*i
     *     b=real(a) # b contains the value 1.0.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "real") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_COMPLEX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Complex_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the complex functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the complex functions.
 */
Gua_Status Complex_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Complex_ComplexFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "abs", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "abs");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "arg", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "arg");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "conj", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "conj");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "imag", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "imag");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "real", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "real");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     COMPLEX_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, COMPLEX_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "COMPLEX_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "COMPLEX_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

