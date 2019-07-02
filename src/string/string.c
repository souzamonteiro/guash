/**
 * File:
 *     string.c
 *
 * Package:
 *     String
 *
 * Description:
 *     This library implements the string functions for an expression interpreter.
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
 * RCS: @(#) $Id: string.c,v 2.3 2014/07/21 16:09:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "string.h"

#define DEFAULT_SEPARATOR ' '

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status String_StringFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     String functions wrapper.
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
Gua_Status String_StringFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String p;
    Gua_String q;
    Gua_Object key;
    Gua_Object o;
    Gua_Integer i;
    Gua_Integer j;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
        
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "at") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectToInteger(argv[2]) < 0) || (Gua_ObjectToInteger(argv[2]) >= Gua_ObjectLength(argv[1]))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(error, "index %ld out of bound in string \"%-.20s...\"", Gua_ObjectToInteger(argv[2]), Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * 2), 1);
        memset(object->string, '\0', sizeof(char) * 2);
        object->string[0] = argv[1].string[Gua_ObjectToInteger(argv[2])];
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isalnum") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isalnum(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isalpha") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isalpha(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isascii") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isascii(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isblank") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isblank(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "iscntrl") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (iscntrl(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isdigit") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isdigit(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isgraph") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isgraph(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "islower") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (islower(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isprint") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isprint(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ispunct") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (ispunct(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isspace") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (Gua_IsSpace(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isupper") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isupper(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "isxdigit") == 0) {
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
        
        q = Gua_ObjectToString(argv[1]);
        
        if (isxdigit(*q)) {
            Gua_IntegerToPObject(object, 1);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ltrim") == 0) {
        if ((argc < 2) || (argc > 3)) {
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
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) + 1)), Gua_ObjectLength(argv[1]));
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));

        p = Gua_ObjectToString(argv[1]);
        q = Gua_ObjectToString(argv[1]);

        if (argc == 2) {
            while (Gua_IsSpace(*p)) {
                p++;
            }
            while (*q) {
                q++;
            }
            q--;
        } else {
            i = 0;
            while (strchr(Gua_ObjectToString(argv[2]), *p) != NULL) {
                p++;
                i++;
                if (i == Gua_ObjectLength(argv[1])) {
                    break;
                }
            }
            while (*q) {
                q++;
            }
            q--;
        }
        
        if (p <= q) {
            i = 0;
            while (*p) {
                object->string[i] = *p;
                if (p != q) {
                    p++;
                    i++;
                } else {
                    break;
                }
            }
        }
        
        /* Fix the object length. */
        Gua_SetPObjectLength(object, strlen(Gua_PObjectToString(object)));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "range") == 0) {
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
        
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectToInteger(argv[2]) < 0) || (Gua_ObjectToInteger(argv[2]) >= Gua_ObjectLength(argv[1]))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(error, "index %ld out of bound in string \"%-.20s...\"", Gua_ObjectToInteger(argv[2]), Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((Gua_ObjectToInteger(argv[3]) < 0) || (Gua_ObjectToInteger(argv[3]) >= Gua_ObjectLength(argv[1]))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(error, "index %ld out of bound in string \"%-.20s...\"", Gua_ObjectToInteger(argv[3]), Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectToInteger(argv[3]) - Gua_ObjectToInteger(argv[2]) + 2)), (Gua_ObjectToInteger(argv[3]) - Gua_ObjectToInteger(argv[2]) + 1));
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectToInteger(argv[3]) - Gua_ObjectToInteger(argv[2]) + 2));
        for (i = Gua_ObjectToInteger(argv[2]); i <= Gua_ObjectToInteger(argv[3]); i++) {
            object->string[i - Gua_ObjectToInteger(argv[2])] = argv[1].string[i];
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "replace") == 0) {
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
        
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_StringToPObject(object, Gua_ObjectToString(argv[1]));
        
        q = Gua_ObjectToString(argv[3]);
        
        while (true) {
            p = strstr(Gua_PObjectToString(object), Gua_ObjectToString(argv[2]));
            
            if (p == NULL)  {
                break;
            }
            
            *p = *q;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "rtrim") == 0) {
        if ((argc < 2) || (argc > 3)) {
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
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) + 1)), Gua_ObjectLength(argv[1]));
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));
        
        p = Gua_ObjectToString(argv[1]);
        q = Gua_ObjectToString(argv[1]);

        if (argc == 2) {
            while (*q) {
                q++;
            }
            q--;
            while (*q) {
                if (Gua_IsSpace(*q)) {
                    q--;
                } else {
                    break;
                }
            }
        } else {
            while (*q) {
                q++;
            }
            q--;
            i = Gua_ObjectLength(argv[1]) - 1;
            while (*q) {
                if (strchr(Gua_ObjectToString(argv[2]), *q) != NULL) {
                    q--;
                    i--;
                    if (i < 0) {
                        break;
                    }
                } else {
                    break;
                }
            }
        }
        
        if (p <= q) {
            i = 0;
            while (*p) {
                object->string[i] = *p;
                if (p != q) {
                    p++;
                    i++;
                } else {
                    break;
                }
            }
        }
        
        /* Fix the object length. */
        Gua_SetPObjectLength(object, strlen(Gua_PObjectToString(object)));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "split") == 0) {
        if ((argc < 2) || (argc > 3)) {
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
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        Gua_IntegerToObject(key, 0);

        Gua_ClearObject(o);
        
        Gua_LinkByteArrayToObject(o, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) + 1)), Gua_ObjectLength(argv[1]))
        memset(o.string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));
        
        Gua_SetStoredObject(o);
        
        j = 0;

        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            /* Parse strings. */
            if (argv[1].string[i] == DOUBLE_QUOTE) {
                o.string[j] = argv[1].string[i];
                i++;
                j++;
                while ((argv[1].string[i] != DOUBLE_QUOTE) && (i < Gua_ObjectLength(argv[1]))) {
                    if (argv[1].string[i] == '\\') {
                        if (argv[1].string[i + 1] == '"') {
                            o.string[j] = argv[1].string[i];
                            i++;
                            j++;                    
                            o.string[j] = argv[1].string[i];
                            i++;
                            j++;                    
                        }
                    } else {
                        o.string[j] = argv[1].string[i];
                        i++;
                        j++;
                    }
                }
            }
            /* Parse arrays. */
            if (argv[1].string[i] == BRACE_OPEN) {
                o.string[j] = argv[1].string[i];
                i++;
                j++;
                while ((argv[1].string[i] != BRACE_CLOSE) && (i < Gua_ObjectLength(argv[1]))) {
                    o.string[j] = argv[1].string[i];
                    i++;
                    j++;
                }
            }
            /* Parse matrices. */
            if (argv[1].string[i] == BRACKET_OPEN) {
                o.string[j] = argv[1].string[i];
                i++;
                j++;
                while ((argv[1].string[i] != BRACKET_CLOSE) && (i < Gua_ObjectLength(argv[1]))) {
                    o.string[j] = argv[1].string[i];
                    i++;
                    j++;
                }
            }

            if (argc == 2) {
                if (argv[1].string[i] == DEFAULT_SEPARATOR) {
                    while ((argv[1].string[i] == DEFAULT_SEPARATOR) && (i < Gua_ObjectLength(argv[1]))) {
                        i++;
                    }
                    if (argv[1].string[i] != DEFAULT_SEPARATOR) {
                        i--;
                    }

                    o.string[j] = '\0';
                    Gua_SetObjectLength(o, strlen(Gua_ObjectToString(o)));

                    j = 0;

                    if (Gua_SetArrayElement(object, &key, &o, false) != GUA_OK) {
                        Gua_FreeObject(&o);
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s", "can't set array");
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return GUA_ERROR;
                    }
                    
                    Gua_IntegerToObject(key, (Gua_ObjectToInteger(key) + 1));
                } else {
                    o.string[j] = argv[1].string[i];
                    j++;
                }
            } else {
                if (strchr(Gua_ObjectToString(argv[2]), argv[1].string[i]) != NULL) {
                    o.string[j] = '\0';
                    Gua_SetObjectLength(o, strlen(Gua_ObjectToString(o)));
                    j = 0;

                    if (Gua_SetArrayElement(object, &key, &o, false) != GUA_OK) {
                        Gua_FreeObject(&o);
                        
                        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                        sprintf(errMessage, "%s", "can't set array");
                        strcat(error, errMessage);
                        Gua_Free(errMessage);
                        
                        return GUA_ERROR;
                    }
                    
                    Gua_IntegerToObject(key, (Gua_ObjectToInteger(key) + 1));
                } else {
                    o.string[j] = argv[1].string[i];
                    j++;
                }
            }
        }
        
        o.string[j] = '\0';
        Gua_SetObjectLength(o, strlen(Gua_ObjectToString(o)));;
        
        if (Gua_SetArrayElement(object, &key, &o, false) != GUA_OK) {
            Gua_FreeObject(&o);
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s", "can't set array");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_FreeObject(&o);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "string") == 0) {
        if (!((argc == 2) || (argc == 3))) {
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
        
        if (argc == 3) {
            if ((Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING)) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectToInteger(argv[1]) + 1)), Gua_ObjectToInteger(argv[1]));
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectToInteger(argv[1]) + 1));
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
                memset(object->string, Gua_ObjectToInteger(argv[2]), sizeof(char) * Gua_ObjectToInteger(argv[1]));
            } else {
                if (Gua_ObjectLength(argv[2]) > 1) {
                    j = 0;
                    
                    for (i = 0; i < Gua_ObjectToInteger(argv[1]); i++) {
                        object->string[i] = argv[2].string[j];
                        
                        j++;
                        
                        if (j >= Gua_ObjectLength(argv[2])) {
                            j = 0;
                        }
                    }
                } else if (Gua_ObjectLength(argv[2]) == 1) {
                    memset(object->string, argv[2].string[0], sizeof(char) * Gua_ObjectToInteger(argv[1]));
                } else {
                    memset(object->string, ' ', sizeof(char) * Gua_ObjectToInteger(argv[1]));
                }
            }
        } else {
            memset(object->string, ' ', sizeof(char) * Gua_ObjectToInteger(argv[1]));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "strpos") == 0) {
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
        
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((p = strstr(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2])))) {
            Gua_IntegerToPObject(object, (p - Gua_ObjectToString(argv[1])));
        } else {
            Gua_IntegerToPObject(object, -1);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "tolower") == 0) {
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
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            object->string[i] = tolower(argv[1].string[i]);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "toupper") == 0) {
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
        for (i = 0; i < Gua_ObjectLength(argv[1]); i++) {
            object->string[i] = toupper(argv[1].string[i]);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "trim") == 0) {
        if ((argc < 2) || (argc > 3)) {
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
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        Gua_LinkByteArrayToPObject(object, (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectLength(argv[1]) + 1)), Gua_ObjectLength(argv[1]));
        memset(object->string, '\0', sizeof(char) * (Gua_ObjectLength(argv[1]) + 1));

        p = Gua_ObjectToString(argv[1]);
        q = Gua_ObjectToString(argv[1]);
        
        if (argc == 2) {
            while (Gua_IsSpace(*p)) {
                p++;
            }
            while (*q) {
                q++;
            }
            q--;
            while (*q) {
                if (Gua_IsSpace(*q)) {
                    q--;
                } else {
                    break;
                }
            }
        } else {
            i = 0;
            while (strchr(Gua_ObjectToString(argv[2]), *p) != NULL) {
                p++;
                i++;
                if (i == Gua_ObjectLength(argv[1])) {
                    break;
                }
            }
            while (*q) {
                q++;
            }
            q--;
            i = Gua_ObjectLength(argv[1]) - 1;
            while (*q) {
                if (strchr(Gua_ObjectToString(argv[2]), *q) != NULL) {
                    q--;
                    i--;
                    if (i < 0) {
                        break;
                    }
                } else {
                    break;
                }
            }
        }
        
        if (p <= q) {
            i = 0;
            while (*p) {
                object->string[i] = *p;
                if (p != q) {
                    p++;
                    i++;
                } else {
                    break;
                }
            }
        }
        
        /* Fix the object length. */
        Gua_SetPObjectLength(object, strlen(Gua_PObjectToString(object)));
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status String_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the string functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the string functions.
 */
Gua_Status String_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, String_StringFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "at", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "at");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isalnum", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isalnum");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isalpha", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isalpha");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isascii", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isascii");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isblank", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isblank");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "iscntrl", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "iscntrl");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isdigit", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isdigit");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isgraph", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isgraph");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "islower", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "islower");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isprint", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isprint");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ispunct", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ispunct");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isspace", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isspace");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isupper", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isupper");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "isxdigit", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "isxdigit");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ltrim", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ltrim");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "range", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "range");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "replace", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "replace");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "rtrim", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "rtrim");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "split", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "split");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "string", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "string");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "strpos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "strpos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "tolower", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "tolower");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "toupper", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "toupper");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "trim", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "trim");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     STRING_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, STRING_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "STRING_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "STRING_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}
