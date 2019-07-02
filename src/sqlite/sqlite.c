/**
 * File:
 *     sqlite.c
 *
 * Package:
 *     Sqlite
 *
 * Description:
 *     This library implements the SQLite functions wrapper.
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
 * RCS: @(#) $Id: sqlite.c,v 2.1 2015/12/14 15:46:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "sqlite.h"
#include "sqlite3.h"

#define ERROR_SIZE  65536

#define GUA_SQLITE_FUNCTION_EVAL        0
#define GUA_SQLITE_FUNCTION_EXECUTE     1
#define GUA_SQLITE_FUNCTION_EXISTS      2
#define GUA_SQLITE_FUNCTION_ONE_COLUMN  3

typedef struct {
    Gua_Namespace *nspace;
    Gua_String script;
    Gua_Object object;
    Gua_Integer function;
    Gua_String error;
    Gua_Status status;
} Sqlite_CallbackArgs;

/**
 * Group:
 *     C
 *
 * Function:
 *     int Sqlite_Callback(void *callback, int n, char **key, char **value)
 *
 * Description:
 *     SQLite callback.
 *
 * Arguments:
 *     argument,    the Sqlite callback argument;
 *     n,           the number of columns;
 *     key,         an array containing the columns names;
 *     value,       an array containing the columns values.
 *
 * Results:
 *     This function is the SQLite callback.
 */
int Sqlite_Callback(void *argument, int n, char **value, char **key)
{
    Sqlite_CallbackArgs *args;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object array;
    Gua_Integer i;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_Short returnCode;
    Gua_String errMessage;
    
    Gua_ClearObject(array);
    Gua_ClearObject(object);
    
    args = (Sqlite_CallbackArgs *)argument;
    
    Gua_ClearObject(args->object);
    
    returnCode = 1;
    
    if (args->function == GUA_SQLITE_FUNCTION_EVAL) {
        error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
        *error = '\0';
        
        status = GUA_OK;
        
        for (i = 0; i < n; i ++) {
            Gua_LinkStringToObject(object, value[i]);
            Gua_SetStoredObject(object);
            if (Gua_SetVariable((Gua_Namespace *)(args->nspace), key[i], &object, SCOPE_GLOBAL) != GUA_OK) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "can't set variable", value[i]);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                status = GUA_ERROR;
                
                break;
            }
        }
        
        if (status != GUA_OK) {
            args->error = error;
            args->status = status;
            
            return 1;
        }
        
        p = args->script;
        p = Gua_Evaluate((Gua_Namespace *)(args->nspace), p, &object, &status, error);
        if (!Gua_IsObjectStored(object)) {
            Gua_FreeObject(&object);
        }
        
        if (status != GUA_OK) {
            args->error = error;
            args->status = status;
            
            return 1;
        }
        
        Gua_Free(error);
        
        return 0;
    } else if (args->function == GUA_SQLITE_FUNCTION_EXECUTE) {
        error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
        *error = '\0';
        
        /* Convert SQLite row data to a GuaraScript array. */
        Gua_KeyValuePairsToArray(n, key, value, &array);
        
        /* Prepare the arguments array to be passed to the callback function. */
        argc = 2;
        argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
        
        Gua_ClearArguments(argc, argv);
        
        Gua_LinkStringToObject(argv[0], (Gua_String)(args->script));
        Gua_SetStoredObject(argv[0]);
        
        Gua_LinkObjects(argv[1], array);
        
        /* Call the callback function. */
        if (Gua_GetFunction((Gua_Namespace *)(args->nspace), (Gua_String)(args->script), &function) == GUA_OK) {
            status = function.pointer((Gua_Namespace *)(args->nspace), argc, argv, &object, error);
        } else {
            status = GUA_ERROR;
            
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "undefined function", (Gua_String)(args->script));
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
        
        /* Get the return code. */
        if (Gua_ObjectType(object) == OBJECT_TYPE_INTEGER) {
            returnCode = Gua_ObjectToInteger(object);
        }
        
        Gua_FreeArguments(argc, argv);
        
        if (!Gua_IsObjectStored(object)) {
            Gua_FreeObject(&object);
        }
        
        /* Show the error message, if any. */
        if (status != GUA_OK) {
            if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
                args->error = error;
                args->status = status;
                
                Gua_Free(error);
                
                return 1;
            }
        }
        
        Gua_Free(error);
    } else if (args->function == GUA_SQLITE_FUNCTION_EXISTS) {
        if (n > 0) {
            Gua_IntegerToObject(args->object, 1);
        } else {
            Gua_IntegerToObject(args->object, 0);
        }
    } else if (args->function == GUA_SQLITE_FUNCTION_ONE_COLUMN) {
        if (n > 0) {
            if (value[0] != NULL) {
                Gua_StringToObject(args->object, value[0]);
            }
        }
        
        return 1;
    }
    
    return returnCode;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Sqlite_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Sqlite_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Handle *h;
    sqlite3 *db;
    Gua_Short returnCode;
    Gua_String errMessage;
    Sqlite_CallbackArgs args;
    
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
     *     sqliteClose(database)
     *
     * Description:
     *     Closes the database.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "sqliteClose") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        if (sqlite3_close(db) != SQLITE_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not close database");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_SetHandlePointer((Gua_Handle *)h, NULL);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     sqliteEval(database, query, script)
     *
     * Description:
     *     Executes the specified SQL query and run the defined script for each returned row.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteEval") == 0) {
        if ((argc < 3) || (argc > 4)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 4) {
            if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        args.status = GUA_OK;
        
        if (argc == 3) {
            returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_Callback, NULL, &errMessage);
        } else {
            args.nspace = nspace;
            args.script = Gua_ObjectToString(argv[3]);
            args.function = GUA_SQLITE_FUNCTION_EVAL;
            args.error = NULL;
            args.status = GUA_OK;
            
            returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_Callback, &args, &errMessage);
        }
        
        if (args.status != GUA_OK) {
            strcat(error, args.error);
            Gua_Free(args.error);
            
            if (returnCode != SQLITE_OK) {
                sqlite3_free(errMessage);
            }
            
            return GUA_ERROR;
        }
        
        if (returnCode != SQLITE_OK) {
            strcat(error, errMessage);
            sqlite3_free(errMessage);
            
            if (returnCode != SQLITE_ABORT) {
                return GUA_ERROR;
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     sqliteExecute(database, query, function)
     *
     * Description:
     *     Executes the specified SQL query and call the defined function for each returned row.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteExecute") == 0) {
        if ((argc < 3) || (argc > 4)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 4) {
            if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        args.status = GUA_OK;
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        if (argc == 3) {
            returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_Callback, NULL, &errMessage);
        } else {
            args.nspace = nspace;
            args.script = Gua_ObjectToString(argv[3]);
            args.function = GUA_SQLITE_FUNCTION_EXECUTE;
            args.error = NULL;
            args.status = GUA_OK;
            
            returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_Callback, &args, &errMessage);
        }
        
        if (args.status != GUA_OK) {
            strcat(error, args.error);
            Gua_Free(args.error);
            
            if (returnCode != SQLITE_OK) {
                sqlite3_free(errMessage);
            }
            
            return GUA_ERROR;
        }
        
        if (returnCode != SQLITE_OK) {
            strcat(error, errMessage);
            sqlite3_free(errMessage);
            
            if (returnCode != SQLITE_ABORT) {
                return GUA_ERROR;
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     sqliteExists(database, query)
     *
     * Description:
     *     Executes the specified SQL query and return 1 if any row is returned.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteExists") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        args.nspace = nspace;
        args.script = NULL;
        args.function = GUA_SQLITE_FUNCTION_EXISTS;
        args.error = NULL;
        args.status = GUA_OK;
        
        Gua_ClearObject(args.object);
        
        returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_Callback, &args, &errMessage);
        
        if (returnCode != SQLITE_OK) {
            sqlite3_free(errMessage);
        }
        
        if (Gua_ObjectType(args.object) == OBJECT_TYPE_INTEGER) {
            Gua_LinkToPObject(object, args.object);
        } else {
            Gua_IntegerToPObject(object, 0);
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     sqliteOneColumn(database, query)
     *
     * Description:
     *     Executes the specified SQL query but return only the first row.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteOneColumn") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        args.nspace = nspace;
        args.script = NULL;
        args.function = GUA_SQLITE_FUNCTION_ONE_COLUMN;
        args.error = NULL;
        args.status = GUA_OK;
        
        Gua_ClearObject(args.object);
        
        returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_Callback, &args, &errMessage);
        
        Gua_LinkToPObject(object, args.object);
        
        if (returnCode != SQLITE_OK) {
            strcat(error, errMessage);
            sqlite3_free(errMessage);
            
            if (returnCode != SQLITE_ABORT) {
                return GUA_ERROR;
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     sqliteOpen(file)
     *
     * Description:
     *     Opens the specified database file and returns a handle to use it.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteOpen") == 0) {
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
        
        if (sqlite3_open(Gua_ObjectToString(argv[1]), &db) != SQLITE_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "could not open or create database", Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_NewHandle(h, "Sqlite", db);
        
        Gua_HandleToPObject(object, (struct Gua_Handle *)h);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Sqlite_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Sqlite_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteClose", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteClose");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteEval", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteEval");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteExecute", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteExecute");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteExists", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteExists");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteOneColumn", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteOneColumn");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteOpen", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteOpen");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_SQLITE_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_SQLITE_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_SQLITE_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_SQLITE_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

