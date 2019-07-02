/*
 * File:
 *     match.c
 *
 * Package:
 *     Match
 *
 * Description:
 *     This library implements a glob like library wrapper
 *     for the GuaraScript interpreter.
 *
 *     Based on public domain source code developed by Ozan S. Yigit.
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
 * RCS: @(#) $Id: match.c,v 1.0 2015/10/24 14:58:00 monteiro Exp $
 * 
 */

#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "match.h"

#ifndef NEGATE
#define NEGATE  '^'
#endif

#define TRUE   1
#define FALSE  0

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Match_Match(Gua_String string, Gua_String pattern)
 *
 * Description:
 *     Verify if a string matches a specified pattern.
 *
 * Arguments:
 *     string,    string to verify;
 *     pattern,   match pattern.
 *
 * Results:
 *     Return true if the string matches a specified pattern.
 */
Gua_Integer Match_Match(Gua_String string, Gua_String pattern)
{
    int negate;
    int match;
    int c;
    
    while (*pattern) {
        if (!*string && *pattern != '*') {
            return FALSE;
        }
        
        switch (c = *pattern++) {
            case '*':
                while (*pattern == '*') {
                    pattern++;
                }
                if (!*pattern) {
                    return TRUE;
                }
                if (*pattern != '?' && *pattern != '[' && *pattern != '\\') {
                    while (*string && *pattern != *string) {
                        string++;
                    }
                }
                while (*string) {
                    if (Match_Match(string, pattern)) {
                        return TRUE;
                    }
                    string++;
                }
                return FALSE;
            case '?':
                if (*string) {
                    break;
                }
                return FALSE;
            case '[':
                if (*pattern != NEGATE) {
                    negate = FALSE;
                } else {
                    negate = TRUE;
                    pattern++;
                }
                
                match = FALSE;
                
                while (!match && (c = *pattern++)) {
                    if (!*pattern) {
                        return FALSE;
                    }
                    if (*pattern == '-') {
                        if (!*++pattern) {
                            return FALSE;
                        }
                        if (*pattern != ']') {
                            if ((*string == c || *string == *pattern) || (*string > c && *string < *pattern)) {
                                match = TRUE;
                            }
                        } else {
                            if (*string >= c) {
                                match = TRUE;
                            }
                            break;
                        }
                    } else {
                        if (c == *string) {
                            match = TRUE;
                        }
                        if (*pattern != ']') {
                            if (*pattern == *string) {
                                match = TRUE;
                            }
                        } else {
                            break;
                        }
                    }
                }

                if (negate == match) {
                    return FALSE;
                }
                
                while (*pattern && *pattern != ']') {
                    pattern++;
                }
                if (!*pattern++) {
                    return FALSE;
                }
                break;
            case '\\':
                if (*pattern) {
                    c = *pattern++;
                }
            default:
                if (c != *string) {
                    return FALSE;
                }
                break;
        }
        string++;
    }
    
    return !*string;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Match_Glob(Gua_String directory, Gua_String pattern, Gua_Object *list, Gua_Integer max, Gua_String error)
 *
 * Description:
 *     Get the directory file list, matching the specified pattern.
 *
 * Arguments:
 *     directory,    the directory name;
 *     pattern,      match pattern;
 *     list,         directory file list;
 *     max,          maximum number of elements to return;
 *     error,        a pointer to the error message.
 *
 * Results:
 *     Return the directory file list.
 */
Gua_Status Match_Glob(Gua_String directory, Gua_String pattern, Gua_Object *list, Gua_Integer max, Gua_String error)
{
    Gua_Short i;
    DIR *dirPointer;
    struct dirent *dirEntry;
    Gua_Element *previous;
    Gua_Element *newElement;
    Gua_String errMessage;
    
    Gua_ClearPObject(list);
    
    if (max == 0) {
        return GUA_OK;
    }
    
    if (strlen(directory) > 0) {
        if ((dirPointer = opendir(directory)) == NULL) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "could not open directory", directory);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        i = 0;
        
        while ((dirEntry = readdir(dirPointer)) != NULL) {
            if (Match_Match(dirEntry->d_name, pattern)) {
                if (i == 0) {
                    /* Create the first element. */
                    newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                    /* The element key. */
                    Gua_IntegerToObject(newElement->key, i);
                    /* The element object. */
                    Gua_StringToObject(newElement->object, dirEntry->d_name);
                    
                    /* Set the target array chain. */
                    newElement->previous = NULL;
                    newElement->next = NULL;
                    
                    /* Link the first element. */
                    Gua_ArrayToPObject(list, (struct Gua_Element *)newElement, 1);
                    
                    previous = (Gua_Element *)Gua_PObjectToArray(list);
                } else {
                    /* Create a new element. */
                    newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                    /* The element key. */
                    Gua_IntegerToObject(newElement->key, i);
                    /* The element object. */
                    Gua_StringToObject(newElement->object, dirEntry->d_name);
                    
                    /* Set the target array chain. */
                    newElement->previous = (struct Gua_Element *)previous;
                    newElement->next = NULL;
                    
                    previous->next = (struct Gua_Element *)newElement;
                    previous = newElement;
                }
                
                i++;
                
                if (max > 0) {
                    if (i == max) {
                        break;
                    }
                }
            }
        }
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(list, i);
        
        closedir(dirPointer);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Match_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Match_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
     *     glob(directory, pattern)
     *
     * Description:
     *     Return the directory file list, matching the specified pattern.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "glob") == 0) {
        if ((argc < 3) || (argc > 4)) {
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
        
        if (argc == 4) {
            if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        if (argc == 3) {
            return Match_Glob(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2]), object, -1, error);
        } else {
            return Match_Glob(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2]), object, Gua_ObjectToInteger(argv[3]), error);
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     match(string, pattern)
     *
     * Description:
     *     Return true if the string contents matches the specified pattern.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "match") == 0) {
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
        
        Gua_IntegerToPObject(object, Match_Match(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2])));
    }

    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Match_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Match_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Match_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glob", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glob");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Match_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "match", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "match");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     MATCH_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, MATCH_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "MATCH_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "MATCH_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

