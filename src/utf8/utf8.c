/**
 * File:
 *     utf8.h
 *
 * Package:
 *     Utf8
 *
 * Description:
 *     This library implements character encoders / decoders for an expression interpreter.
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
 * RCS: @(#) $Id: utf8.h,v 1.0 2014/07/14 13:53:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "utf8.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Utf8_Utf8FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Utf8_Utf8FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer len;
    Gua_Integer pos;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
        
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "utf8at") == 0) {
        if (argc != 3) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * 7), 7);
        memset(object->string, '\0', sizeof(char) * 7);
        
        pos = 0;
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            if (pos == Gua_ObjectToInteger(argv[2])) {
                if ((unsigned char)(argv[1].string[i]) < 0xc0) {
                    memcpy(object->string, argv[1].string + i, 1);
                    /* Fix the object length. */
                    Gua_SetPObjectLength(object, 1);
                } else if (((unsigned char)(argv[1].string[i]) >= 0xc0) && ((unsigned char)(argv[1].string[i]) < 0xe0)) {
                    memcpy(object->string, argv[1].string + i, 2);
                    /* Fix the object length. */
                    Gua_SetPObjectLength(object, 2);
                } else if (((unsigned char)(argv[1].string[i]) >= 0xe0) && ((unsigned char)(argv[1].string[i]) < 0xf0)) {
                    memcpy(object->string, argv[1].string + i, 3);
                    /* Fix the object length. */
                    Gua_SetPObjectLength(object, 3);
                } else if (((unsigned char)(argv[1].string[i]) >= 0xf0) && ((unsigned char)(argv[1].string[i]) < 0xf8)) {
                    memcpy(object->string, argv[1].string + i, 4);
                    /* Fix the object length. */
                    Gua_SetPObjectLength(object, 4);
                } else if (((unsigned char)(argv[1].string[i]) >= 0xf8) && ((unsigned char)(argv[1].string[i]) < 0xfc)) {
                    memcpy(object->string, argv[1].string + i, 5);
                    /* Fix the object length. */
                    Gua_SetPObjectLength(object, 5);
                } else if ((unsigned char)(argv[1].string[i]) >= 0xfc) {
                    memcpy(object->string, argv[1].string + i, 6);
                    /* Fix the object length. */
                    Gua_SetPObjectLength(object, 6);
                }
                
                break;
            }
            
            if ((unsigned char)(argv[1].string[i]) < 0xc0) {
            } else if (((unsigned char)(argv[1].string[i]) >= 0xc0) && ((unsigned char)(argv[1].string[i]) < 0xe0)) {
                i++;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xe0) && ((unsigned char)(argv[1].string[i]) < 0xf0)) {
                i+=2;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xf0) && ((unsigned char)(argv[1].string[i]) < 0xf8)) {
                i+=3;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xf8) && ((unsigned char)(argv[1].string[i]) < 0xfc)) {
                i+=4;
            } else if ((unsigned char)(argv[1].string[i]) >= 0xfc) {
                i+=5;
            }
            pos++;
        }
        
        if (pos != Gua_ObjectToInteger(argv[2])) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argv[2]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "utf8dec") == 0) {
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
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) + 1)), Gua_ObjectLength(argv[1]));
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));
        
        j = 0;
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            if ((unsigned char)(argv[1].string[i]) < 0x80) {
                object->string[j] = argv[1].string[i];
            } else {
                object->string[j] = (unsigned char)(((unsigned char)(argv[1].string[i]) << 6) & 0xc0);
                i++;
                object->string[j] = (unsigned char)((unsigned char)(object->string[j]) | (unsigned char)((unsigned char)((argv[1].string[i]) & 0x3f)));
            }
            j++;
        }
        
        /* Fix the object length. */
        Gua_SetPObjectLength(object, j);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "utf8enc") == 0) {
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
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) * 2 + 1)), Gua_ObjectLength(argv[1]) * 2);
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) * 2 + 1));
        
        j = 0;
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            if ((unsigned char)(argv[1].string[i]) < 0x80) {
                object->string[j] = argv[1].string[i];
            } else {
                object->string[j] = (unsigned char)(((unsigned char)(argv[1].string[i]) >> 6) | 0xc0);
                j++;
                object->string[j] = (unsigned char)((unsigned char)((argv[1].string[i]) & 0x3f) | 0x80);
            }
            j++;
        }
        
        /* Fix the object length. */
        Gua_SetPObjectLength(object, j);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "utf8len") == 0) {
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
        
        len = 0;
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            if ((unsigned char)(argv[1].string[i]) < 0xc0) {
            } else if (((unsigned char)(argv[1].string[i]) >= 0xc0) && ((unsigned char)(argv[1].string[i]) < 0xe0)) {
                i++;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xe0) && ((unsigned char)(argv[1].string[i]) < 0xf0)) {
                i+=2;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xf0) && ((unsigned char)(argv[1].string[i]) < 0xf8)) {
                i+=3;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xf8) && ((unsigned char)(argv[1].string[i]) < 0xfc)) {
                i+=4;
            } else if ((unsigned char)(argv[1].string[i]) >= 0xfc) {
                i+=5;
            }
            len++;
        }
        
        Gua_IntegerToPObject(object, len);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "utf8range") == 0) {
        if (argc != 4) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) * 2 + 1)), Gua_ObjectLength(argv[1]) * 2);
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) * 2 + 1));
        
        pos = 0;
        j = 0;
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            if ((pos >= Gua_ObjectToInteger(argv[2])) && (pos <= Gua_ObjectToInteger(argv[3]))) {
                if ((unsigned char)(argv[1].string[i]) < 0xc0) {
                    memcpy(object->string + j, argv[1].string + i, 1);
                    j+=1;
                } else if (((unsigned char)(argv[1].string[i]) >= 0xc0) && ((unsigned char)(argv[1].string[i]) < 0xe0)) {
                    memcpy(object->string + j, argv[1].string + i, 2);
                    j+=2;
                } else if (((unsigned char)(argv[1].string[i]) >= 0xe0) && ((unsigned char)(argv[1].string[i]) < 0xf0)) {
                    memcpy(object->string + j, argv[1].string + i, 3);
                    j+=3;
                } else if (((unsigned char)(argv[1].string[i]) >= 0xf0) && ((unsigned char)(argv[1].string[i]) < 0xf8)) {
                    memcpy(object->string + j, argv[1].string + i, 4);
                    j+=4;
                } else if (((unsigned char)(argv[1].string[i]) >= 0xf8) && ((unsigned char)(argv[1].string[i]) < 0xfc)) {
                    memcpy(object->string + j, argv[1].string + i, 5);
                    j+=5;
                } else if ((unsigned char)(argv[1].string[i]) >= 0xfc) {
                    memcpy(object->string + j, argv[1].string + i, 6);
                    j+=6;
                }
            }
            
            if (pos == Gua_ObjectToInteger(argv[3])) {
                break;
            }
            
            if ((unsigned char)(argv[1].string[i]) < 0xc0) {
            } else if (((unsigned char)(argv[1].string[i]) >= 0xc0) && ((unsigned char)(argv[1].string[i]) < 0xe0)) {
                i++;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xe0) && ((unsigned char)(argv[1].string[i]) < 0xf0)) {
                i+=2;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xf0) && ((unsigned char)(argv[1].string[i]) < 0xf8)) {
                i+=3;
            } else if (((unsigned char)(argv[1].string[i]) >= 0xf8) && ((unsigned char)(argv[1].string[i]) < 0xfc)) {
                i+=4;
            } else if ((unsigned char)(argv[1].string[i]) >= 0xfc) {
                i+=5;
            }
            pos++;
        }
        
        /* Fix the object length. */
        Gua_SetPObjectLength(object, j);
        
        if (pos > Gua_ObjectToInteger(argv[3])) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %ld...\n", "index out of bound", Gua_ObjectToInteger(argv[2]));
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
 *     Gua_Status Utf8_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the extension functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message;
 *
 * Results:
 *     Install the extension functions.
 */
Gua_Status Utf8_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Utf8_Utf8FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "utf8at", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "utf8at");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "utf8dec", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "utf8dec");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "utf8enc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "utf8enc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "utf8len", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "utf8len");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "utf8range", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "utf8range");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     UTF8_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, UTF8_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "UTF8_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "UTF8_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

