/**
 * File:
 *     glu.c
 *
 * Package:
 *     Glu
 *
 * Description:
 *     This library implements an OpenGL GLU wrapper
 *     for the GuaraScript interpreter.
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
 * RCS: @(#) $Id: glu.c,v 1.0 2013/09/23 16:41:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#ifdef _MAC_OS_X_
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif
#include "interp.h"
#include "glu.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glu_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Function wrapper.
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
Gua_Status Glu_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    GLdouble arg1d;
    GLdouble arg2d;
    GLdouble arg3d;
    GLdouble arg4d;
    GLdouble arg5d;
    GLdouble arg6d;
    GLdouble arg7d;
    GLdouble arg8d;
    GLdouble arg9d;
    Gua_String errMessage;
    
    arg1d = 0.0;
    arg2d = 0.0;
    arg3d = 0.0;
    arg4d = 0.0;
    arg5d = 0.0;
    arg6d = 0.0;
    arg7d = 0.0;
    arg8d = 0.0;
    arg9d = 0.0;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "gluLookAt") == 0) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 6 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[9]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[9]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2d = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2d = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3d = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3d = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4d = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4d = Gua_ObjectToReal(argv[4]);            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5d = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5d = Gua_ObjectToReal(argv[5]);            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6d = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6d = Gua_ObjectToReal(argv[6]);            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7d = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7d = Gua_ObjectToReal(argv[7]);            
        }
        if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) {
            arg8d = Gua_ObjectToInteger(argv[8]);
        } else if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL) {
            arg8d = Gua_ObjectToReal(argv[8]);            
        }
        if (Gua_ObjectType(argv[9]) == OBJECT_TYPE_INTEGER) {
            arg9d = Gua_ObjectToInteger(argv[9]);
        } else if (Gua_ObjectType(argv[9]) == OBJECT_TYPE_REAL) {
            arg9d = Gua_ObjectToReal(argv[9]);            
        }
        
        gluLookAt(arg1d, arg2d, arg3d, arg4d, arg5d, arg6d, arg7d, arg8d, arg9d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "gluOrtho2D") == 0) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2d = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2d = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3d = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3d = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4d = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4d = Gua_ObjectToReal(argv[4]);            
        }
        
        gluOrtho2D(arg1d, arg2d, arg3d, arg4d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "gluPerspective") == 0) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2d = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2d = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3d = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3d = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4d = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4d = Gua_ObjectToReal(argv[4]);            
        }
        
        gluPerspective(arg1d, arg2d, arg3d, arg4d);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glu_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the extension functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the extension functions.
 */
Gua_Status Glu_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Glu_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gluLookAt", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gluLookAt");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glu_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gluOrtho2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gluOrtho2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glu_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gluPerspective", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gluPerspective");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_GLU_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_GLU_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_GLU_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_GLU_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

