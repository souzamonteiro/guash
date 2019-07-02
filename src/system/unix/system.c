/**
 * File:
 *     system.c
 *
 * Package:
 *     System
 *
 * Description:
 *     This library implements the system functions for an expression interpreter.
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
 * RCS: @(#) $Id: system.c,v 2.6 2017/05/26 21:16:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#ifdef _WINDOWS_
    #include <windows.h>
    #include <windef.h>
    #include <winbase.h>
#else
    #define _GNU_SOURCE 1
    #include <dlfcn.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "fs.h"
#include "system.h"

#define EXPRESSION_SIZE  4096
#define BUFFER_SIZE      65536

System_ListItem *libraryList;

/* Global pointers to argc, argv and env variables. */
int appArgc;
char **appArgv;
char **appEnv;

/**
 * Group:
 *     C
 *
 * Function:
 *     System_ListItem *System_AddListItem(System_ListItem **list, void *data)
 *
 * Description:
 *     Add an object to a list.
 *
 * Arguments:
 *     list,    a pointer to the list;
 *     data,    a pointer to the data to be stored in the list.
 *
 * Results:
 *     The function adds an object to a list.
 */
System_ListItem *System_AddListItem(System_ListItem **list, void *data)
{
    System_ListItem *newListItem;
    System_ListItem *item;
    
    newListItem = (System_ListItem *)malloc(sizeof(System_ListItem));
    newListItem->data = data;
    newListItem->previous = NULL;
    newListItem->next = NULL;
    
    if (*list == NULL) {
        *list = newListItem;
    } else {
        item = *list;
        while (1) {
            if (item->next) {
                item = (System_ListItem *)(item->next);
            } else {
                break;
            }
        }
        newListItem->previous = item;
        item->next = newListItem;
    }
    
    return newListItem;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void System_FreeList(System_ListItem *list)
 *
 * Description:
 *     Frees a list allocated memory.
 *
 * Arguments:
 *     list,        a pointer to the list.
 *
 * Results:
 *     The function frees the list allocated memory.
 */
void System_FreeList(System_ListItem *list)
{
    if (list == NULL) {
        return;
    } else {
        System_FreeList((System_ListItem *)(list->next));
        
        if (list->data != NULL) {
#ifdef _WINDOWS_
            FreeLibrary(list->data);
#else
            dlclose(list->data);
#endif
        }
        
        free(list);
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_ExecFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Exec function wrapper.
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
 *     Execute a system command.
 */
Gua_Status System_ExecFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
    
    Gua_IntegerToPObject(object, system(Gua_ObjectToString(argv[1])));
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void System_ToCamelCase(char *target, char *source)
 *
 * Description:
 *     Converts a string (words separated by space) to a CamelCase form.
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
 *     Returns a string converted to a CamelCase form.
 */
void System_ToCamelCase(char *target, char *source)
{
    char *p;
    char *q;
    int toUp;
    
    p = source;
    q = target;
    
    toUp = 1;
    
    while (*p) {
        if (*p == ' ') {
            toUp = 1;
            p++;
        }
        
        if (toUp) {
            *q = toupper(*p);
            toUp = 0;
        } else {
            *q = tolower(*p);
        }
        
        p++;
        q++;
    }
    *q = '\0';
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_LoadFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Load function wrapper.
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
 *     Loads a system dynamic library.
 */
Gua_Status System_LoadFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Namespace *top;
    Gua_Short status;
    Gua_String p;
    Gua_String q;
    Gua_String name;
    Gua_String prefix;
    Gua_String prefixToCamelCase;
    Gua_Integer removeLibPrefix;
    Gua_String functionName;
    
    top = (Gua_Namespace *)nspace;
    while (top->previous) {
        top = (Gua_Namespace *)top->previous;
    }
    
#ifdef _WINDOWS_
    HINSTANCE library;
    Gua_Status __cdecl (*initFunction) (void *nspace, int argc, char *argv[], char **env, Gua_String error);
#else
    void *library;
    Gua_Status (*initFunction) (void *nspace, int argc, char *argv[], char **env, Gua_String error);
#endif
    Gua_String errMessage;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
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
    
    status = GUA_OK;
    
    prefix = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE + 1);
    memset(prefix, '\0', EXPRESSION_SIZE + 1);
    
    prefixToCamelCase = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE + 1);
    memset(prefixToCamelCase, '\0', EXPRESSION_SIZE + 1);
    
    functionName = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE + 1);
    memset(functionName, '\0', EXPRESSION_SIZE + 1);
    
    Fs_Name(Gua_ObjectToString(argv[1]), &name);
    
    p = name;
    q = prefix;
    
    while (Gua_IsSpace(*p)) {
        p++;
    }
    
    removeLibPrefix = 1;
    
    while (*p) {
        /* Skip the "lib" prefix. */
        if (removeLibPrefix) {
            if (*p == 'l') {
                p++;
                if (*p == 'i') {
                    p++;
                    if (*p == 'b') {
                        p++;
                    }
                }
            } else {
                removeLibPrefix = 0;
            }
        }
        
        *q = *p;
        
        p++;
        q++;
        
        if ((*p == '.') || (*p == '-') || Gua_IsNumber(*p)) {
            break;
        }
    }
    *q = '\0';
    
    /* Converts the library name do CamelCase. */
    System_ToCamelCase(prefixToCamelCase, prefix);
    
    sprintf(functionName, "%s_Init", prefixToCamelCase);
    
#ifdef _WINDOWS_
    library = LoadLibrary(TEXT(Gua_ObjectToString(argv[1])));
    if (library != NULL) {
        initFunction = (Gua_Status __cdecl (*) (void *nspace, int argc, char *argv[], char **env, Gua_String error)) GetProcAddress(library, functionName);
        if (initFunction != NULL) {
            status = initFunction(top, appArgc, appArgv, appEnv, error);
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s \"%s\"\n", "could not load function", functionName);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            status = GUA_ERROR;
        }
        /* We can not call FreeLibrary(library) as this would remove the functions
         * from memory and the interpreter could not call them when needed. So
         * we add the library handle to a list and free it later when the application
         * exits.
         */
         System_AddListItem(&libraryList, library);
    } else {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s \"%s\"\n", "could not loading library", Gua_ObjectToString(argv[1]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        status = GUA_ERROR;
    }
#else
    library = dlopen(Gua_ObjectToString(argv[1]), RTLD_NOW | RTLD_GLOBAL);
    if (library != NULL) {
        initFunction = (Gua_Status (*) (void *nspace, int argc, char *argv[], char **env, Gua_String error)) dlsym(library, functionName);
        if (initFunction != NULL) {
            status = initFunction(top, appArgc, appArgv, appEnv, error);
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s", dlerror());
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            status = GUA_ERROR;
        }
        /* We can not call dlclose(library) as this would remove the functions
         * from memory and the interpreter could not call them when needed. So
         * we add the library handle to a list and free it later when the application
         * exits.
         */
         System_AddListItem(&libraryList, library);
    } else {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s", dlerror());
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        status = GUA_ERROR;
    }
#endif
    
    Gua_Free(name);
    Gua_Free(prefix);
    Gua_Free(prefixToCamelCase);
    Gua_Free(functionName);
    
    return status;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_PrintFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Print function wrapper.
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
 *     Print a object on stdout.
 */
Gua_Status System_PrintFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Object objectAsString;
    Gua_String errMessage;
        
    Gua_ClearObject(objectAsString);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (argc > 2) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    Gua_StringToPObject(object, " ");

    if (argc == 2) {
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            printf("%ld", Gua_ObjectToInteger(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g", 0.0);
            }
            if (fabs(Gua_ObjectToImaginary(argv[1])) >= ZERO) {
                printf("%+g*i", Gua_ObjectToImaginary(argv[1]));
            } else {
                printf("%+g*i", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
            printf("%s", Gua_ObjectToString(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) {
            Gua_ArrayToString(&argv[1], &objectAsString);
            printf("%s", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
            Gua_MatrixToString(&argv[1], &objectAsString);
            printf("%s", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) {
            printf("File%lu\n", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
            printf("%s%lu\n", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(argv[1])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(argv[1])));
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_PrintlnFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Println function wrapper.
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
 *     Print a object on stdout and the line feed character.
 */
Gua_Status System_PrintlnFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Object objectAsString;
    Gua_String errMessage;
    
    Gua_ClearObject(objectAsString);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (argc > 2) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    Gua_LinkStringToPObject(object, " ");
    Gua_SetStoredPObject(object);
    
    if (argc == 2) {
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            printf("%ld\n", Gua_ObjectToInteger(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g\n", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g\n", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g", 0.0);
            }
            if (fabs(Gua_ObjectToImaginary(argv[1])) >= ZERO) {
                printf("%+g*i\n", Gua_ObjectToImaginary(argv[1]));
            } else {
                printf("%+g*i\n", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
            printf("%s\n", Gua_ObjectToString(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) {
            Gua_ArrayToString(&argv[1], &objectAsString);
            printf("%s\n", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
            Gua_MatrixToString(&argv[1], &objectAsString);
            printf("%s\n", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) {
            printf("File%lu\n", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
            printf("%s%lu\n", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(argv[1])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(argv[1])));
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_SourceFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Source function wrapper.
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
 *     Read a script file and try to execute it.
 */
Gua_Status System_SourceFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Namespace *top;
    FILE *fp;
    Gua_String script;
    Gua_String p;
    Gua_Integer length;
    Gua_Short status;
    Gua_Object argv1;
    Gua_String expression;
    Gua_String errMessage;
    
    top = (Gua_Namespace *)nspace;
    while (top->previous) {
        top = (Gua_Namespace *)top->previous;
    }
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
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
    
    fp = fopen(Gua_ObjectToString(argv[1]), "r");
    
    if (fp == NULL) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can not open file", Gua_ObjectToString(argv[1]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) + 1;
    fseek(fp, 0, SEEK_SET);
    
    script = (char *)Gua_Alloc(sizeof(char) * length + 1);
    memset(script, '\0', length + 1);
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE + 1);
    memset(expression, '\0', EXPRESSION_SIZE + 1);
    
    status = GUA_OK;
    
    if (length > 0) {
        /* Store the main script argv[1]. */
        strcpy(expression, "$argv[1]");
        p = expression;
        p = Gua_Expression(top, p, &argv1, &status, error);

        /* Set the new argv[1]. */
        sprintf(expression, "$argv[1] = \"%s\"", Gua_ObjectToString(argv[1]));
        p = expression;
        p = Gua_Expression(top, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        /* Load the script. */
        fread(script, sizeof(char), length, fp);
        p = script;
        p = Gua_Evaluate(top, p, object, &status, error);
        
        /* Restore the main script argv[1]. */
        sprintf(expression, "$argv[1] = \"%s\"", Gua_ObjectToString(argv1));
        if (!Gua_IsObjectStored(argv1)) {
            Gua_FreeObject(&argv1);
        }
        p = expression;
        p = Gua_Expression(top, p, &argv1, &status, error);
        if (!Gua_IsObjectStored(argv1)) {
            Gua_FreeObject(&argv1);
        }
    }
    
    fclose(fp);
    
    Gua_Free(expression);
    Gua_Free(script);
    
    if (status != GUA_OK) {
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the system functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message;
 *
 * Results:
 *     Install the system functions.
 */
Gua_Status System_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String p;
    Gua_Status status;
    Gua_String errMessage;
    
    char *includeScript = "function include(package) {\n"
                          "    found = 0\n"
                          "    \n"
                          "    if (exists(\"SYS_PKG_PATH\")) {\n"
                          "        foreach ($SYS_PKG_PATH; index; path) {\n"
                          "            pkgs = \"\"\n"
                          "            \n"
                          "            pkgs = glob(path, package + \"*\")\n"
                          "            \n"
                          "            if (exists(\"pkgs\")) {\n"
                          "                sorted_pkgs = sort(pkgs, ARRAY_DESCENDING_ORDER)\n"
                          "                \n"
                          "                if (fsExists(path + \"/\" + sorted_pkgs[0] + \"/\" + \"package.gua\")) {\n"
                          "                    source(path + \"/\" + sorted_pkgs[0] + \"/\" + \"package.gua\")\n"
                          "                    \n"
                          "                    found = 1\n"
                          "                    \n"
                          "                    break\n"
                          "                }\n"
                          "            }\n"
                          "        }\n"
                          "    } else {\n"
                          "        executable_file = fsFullPath($argv[0])\n"
                          "        executable_path = fsPath(executable_file)\n"
                          "        root_path = fsPath(executable_path)\n"
                          "        library_path = root_path + \"/lib\"\n"
                          "        pkgs = \"\"\n"
                          "        \n"
                          "        pkgs = glob(library_path, package + \"*\")\n"
                          "        \n"
                          "        if (exists(\"pkgs\")) {\n"
                          "            sorted_pkgs = sort(pkgs, ARRAY_DESCENDING_ORDER)\n"
                          "            \n"
                          "            if (fsExists(library_path + \"/\" + sorted_pkgs[0] + \"/\" + \"package.gua\")) {\n"
                          "                source(library_path + \"/\" + sorted_pkgs[0] + \"/\" + \"package.gua\")\n"
                          "                \n"
                          "                found = 1\n"
                          "            }\n"
                          "        }\n"
                          "    }\n"
                          "    \n"
                          "    if(!found) {\n"
                          "        error(\"could not load package \" + package)\n"
                          "    }\n"
                          "}\n";

    Gua_LinkCFunctionToFunction(function, System_ExecFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "exec", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "exec");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_LoadFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "load", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "load");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_PrintFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "print", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "print");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_PrintlnFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "println", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "println");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_SourceFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "source", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "source");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Create the scripted functions. */
    p = includeScript;
    p = Gua_Evaluate(nspace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    if (status != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "include");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }    
    
    /* Create the package global variables. */
    Gua_LinkStringToObject(object, SYSTEM_HOST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SYS_HOST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SYS_HOST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, SYSTEM_ARCH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SYS_ARCH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SYS_ARCH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    appArgc = argc;
    appArgv = argv;
    appEnv = env;
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     SYSTEM_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, SYSTEM_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SYS_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SYS_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_Finish(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Free the system resources.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message;
 *
 * Results:
 *     Free the system resources.
 */
Gua_Status System_Finish(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    System_FreeList(libraryList);
    
    return GUA_OK;
}
