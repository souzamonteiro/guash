/**
 * File:
 *     printf.c
 *
 * Package:
 *     Printf
 *
 * Description:
 *     This file implements the printf and sprintf functions for an expression interpreter.
 *     This release does not support the * modifier.
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
 * RCS: @(#) $Id: printf.c,v 1.6 2015/12/15 22:00:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "printf.h"

#define BUFFER_SIZE 65536

#define SPECIFIERS         "diuoxXcsfeEgG"
#define INTEGER_SPECIFIERS "diuoxXc"
#define REAL_SPECIFIERS    "feEgG"
#define STRING_SPECIFIERS  "s"

#define Printf_IsConversionSpecifier(c) (strchr(SPECIFIERS, c) != NULL)
#define Printf_IsIntegerSpecifier(c) (strchr(INTEGER_SPECIFIERS, c) != NULL)
#define Printf_IsRealSpecifier(c) (strchr(REAL_SPECIFIERS, c) != NULL)
#define Printf_IsStringSpecifier(c) (strchr(STRING_SPECIFIERS, c) != NULL)

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Printf_SprintfFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Sprintf function wrapper.
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
 *     Return a formatted string.
 */
Gua_Status Printf_SprintfFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String buffer;
    Gua_String format;
    Gua_String sprintfBuffer;
    Gua_String a;
    Gua_String f;
    char conversionSpecifier;
    Gua_Short isLong;
    Gua_Short arg;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (argc < 2) {
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
    
    if (argc == 2) {
        Gua_LinkToPObject(object, argv[1]);
        Gua_SetStoredObject(argv[1]);
    } else {
        buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
        buffer[0] = '\0';
        
        sprintfBuffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
        format = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
        
        a = Gua_ObjectToString(argv[1]);
        f = format;
        
        arg = 1;
        
        while (*a != '\0') {
            sprintfBuffer[0] = '\0';
            format[0] = '\0';
            
            f = format;
            
            if (*a == '\\') {
                *f = *a;
                a++;
                f++;
                
                *f = *a;
                a++;
                f++;
            }
            
            if (*a == '%') {
                conversionSpecifier = '\0';
                isLong = false;
                
                *f = *a;
                a++;
                f++;
                
                while (*a != '\0') {
                    *f = *a;
                    
                    if (*a == 'l') {
                        isLong = true;
                    }
                    
                    if (Printf_IsConversionSpecifier(*a)) {
                        conversionSpecifier = *a;
                        
                        arg++;
                        if (arg >= argc) {
                            Gua_Free(buffer);
                            Gua_Free(format);
                            Gua_Free(sprintfBuffer);
                            
                            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                            sprintf(errMessage, "%s %-.20s...\n", "illegal number of arguments for function", Gua_ObjectToString(argv[0]));
                            strcat(error, errMessage);
                            Gua_Free(errMessage);
                            
                            return GUA_ERROR;
                        }
                        
                        a++;
                        f++;
                        break;
                    }
                    
                    a++;
                    f++;
                }
                *f = '\0';
                
                if (Printf_IsIntegerSpecifier(conversionSpecifier)) {
                    if (Gua_ObjectType(argv[arg]) != OBJECT_TYPE_INTEGER) {
                        Gua_Free(buffer);
                        Gua_Free(format);
                        Gua_Free(sprintfBuffer);
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "bad format string for function", Gua_ObjectToString(argv[0]));
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return GUA_ERROR;
                    }
                    
                    if (isLong) {
                        sprintf(sprintfBuffer, format, Gua_ObjectToInteger(argv[arg]));
                    } else {
                        sprintf(sprintfBuffer, format, (int)(Gua_ObjectToInteger(argv[arg])));
                    }
                } else if (Printf_IsRealSpecifier(conversionSpecifier)) {
                    if (!((Gua_ObjectType(argv[arg]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[arg]) == OBJECT_TYPE_REAL))) {
                        Gua_Free(buffer);
                        Gua_Free(format);
                        Gua_Free(sprintfBuffer);
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "bad format string for function", Gua_ObjectToString(argv[0]));
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return GUA_ERROR;
                    }
                    
                    if (Gua_ObjectType(argv[arg]) == OBJECT_TYPE_REAL) {
                        sprintf(sprintfBuffer, format, Gua_ObjectToReal(argv[arg]));
                    } else {
                        sprintf(sprintfBuffer, format, (double)(Gua_ObjectToInteger(argv[arg])));
                    }
                } else if (Printf_IsStringSpecifier(conversionSpecifier)) {
                    if (Gua_ObjectType(argv[arg]) != OBJECT_TYPE_STRING) {
                        Gua_Free(buffer);
                        Gua_Free(format);
                        Gua_Free(sprintfBuffer);
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s %-.20s...\n", "bad format string for function", Gua_ObjectToString(argv[0]));
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return GUA_ERROR;
                    }
                    
                    sprintf(sprintfBuffer, format, Gua_ObjectToString(argv[arg]));
                } else {
                    Gua_Free(buffer);
                    Gua_Free(format);
                    Gua_Free(sprintfBuffer);
                    
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "bad format string for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
            } else {
                while (!((*a == '%') || (*a == '\0'))) {
                    *f = *a;
                    a++;
                    f++;
                }
                *f = '\0';
                strcpy(sprintfBuffer, format);
            }
            
            strcat(buffer, sprintfBuffer);
        }
        
        Gua_StringToPObject(object, buffer);

        Gua_Free(buffer);
        Gua_Free(format);
        Gua_Free(sprintfBuffer);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Printf_PrintfFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Printf function wrapper.
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
 *     Print a formated object on stdout.
 */
Gua_Status Printf_PrintfFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
    
    if (argc < 2) {
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
    
    if (argc == 2) {
        printf("%s", Gua_ObjectToString(argv[1]));
    } else {
        if (Printf_SprintfFunctionWrapper(nspace, argc, argv, object, error) == GUA_OK) {
            printf("%s", Gua_PObjectToString(object));
            Gua_Free(Gua_PObjectToString(object));
        } else {
            return GUA_ERROR;
        }
    }
    
    Gua_LinkStringToPObject(object, " ");
    Gua_SetStoredPObject(object);
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Printf_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the printf and sprintf functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the printf and sprintf functions.
 */
Gua_Status Printf_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Printf_PrintfFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "printf", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "printf");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Printf_SprintfFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sprintf", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sprintf");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     PRINTF_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, PRINTF_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "PRINTF_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "PRINTF_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

