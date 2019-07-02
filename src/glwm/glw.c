/**
 * File:
 *     glw.c
 *
 * Package:
 *    Glw
 *
 * Description:
 *     This library implements an OpenGL GLWM wrapper
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
 * RCS: @(#) $Id: glw.c,v 1.0 2013/10/24 10:36:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "unix/glwm.h"
#include "glw.h"

#define EXPRESSION_SIZE  4096
#define ERROR_SIZE  65536

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_DisplayFunc(void *window)
 *
 * Description:
 *     GLWM display window callback.
 *
 * Arguments:
 *     window,    the window that received the event.
 *
 * Results:
 *     This function is a wrapper to the display window callback.
 */
void Glw_DisplayFunc(void *window)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_DISPLAYFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_ReshapeFunc(void *window)
 *
 * Description:
 *     GLWM reshape window callback.
 *
 * Arguments:
 *     window,    the window that received the event.
 *
 * Results:
 *     This function is a wrapper to the reshape window callback.
 */
void Glw_ReshapeFunc(void *window)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_RESHAPEFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_IdleFunc(void *window)
 *
 * Description:
 *     GLWM idle window callback.
 *
 * Arguments:
 *     window,    the window that received the event.
 *
 * Results:
 *     This function is a wrapper to the idle window callback.
 */
void Glw_IdleFunc(void *window)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_IDLEFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_KeyPressFunc(void *window, unsigned char c, int x, int y)
 *
 * Description:
 *     GLWM key press callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     c,         the key code;
 *     x,         pointer x coordinate;
 *     y,         pointer y coordinate.
 *
 * Results:
 *     This function is a wrapper to the key press callback.
 */
void Glw_KeyPressFunc(void *window, unsigned char c, int x, int y)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_KEYPRESSFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 5;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], c);
    Gua_IntegerToObject(argv[3], x);
    Gua_IntegerToObject(argv[4], y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_KeyReleaseFunc(void *window, unsigned char c, int x, int y)
 *
 * Description:
 *     GLWM key release callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     c,         the key code;
 *     x,         pointer x coordinate;
 *     y,         pointer y coordinate.
 *
 * Results:
 *     This function is a wrapper to the key release callback.
 */
void Glw_KeyReleaseFunc(void *window, unsigned char c, int x, int y)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_KEYRELEASEFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 5;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], c);
    Gua_IntegerToObject(argv[3], x);
    Gua_IntegerToObject(argv[4], y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_MouseEnterFunc(void *window)
 *
 * Description:
 *     GLWM mouse enter callback.
 *
 * Arguments:
 *     window,    the window that received the event.
 *
 * Results:
 *     This function is a wrapper to the mouse enter callback.
 */
void Glw_MouseEnterFunc(void *window)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_MOUSEENTERFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_MouseLeaveFunc(void *window)
 *
 * Description:
 *     GLWM mouse leave callback.
 *
 * Arguments:
 *     window,    the window that received the event.
 *
 * Results:
 *     This function is a wrapper to the mouse leave callback.
 */
void Glw_MouseLeaveFunc(void *window)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_MOUSELEAVEFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_MouseMotionFunc(void *window, int x, int y)
 *
 * Description:
 *     GLWM mouse motion callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     x,         pointer x coordinate;
 *     y,         pointer y coordinate.
 *
 * Results:
 *     This function is a wrapper to the mouse motion callback.
 */
void Glw_MouseMotionFunc(void *window, int x, int y)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_MOUSEMOTIONFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 4;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], x);
    Gua_IntegerToObject(argv[3], y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_MouseButtonPressFunc(void *window, unsigned int button, unsigned int state, int x, int y)
 *
 * Description:
 *     GLWM mouse button press callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     button,    pointer x coordinate;
 *     state,     pointer y coordinate.
 *     x,         pointer x coordinate;
 *     y,         pointer y coordinate.
 *
 * Results:
 *     This function is a wrapper to the mouse button press callback.
 */
void Glw_MouseButtonPressFunc(void *window, unsigned int button, unsigned int state, int x, int y)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_MOUSEBUTTONPRESSFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 6;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], button);
    Gua_IntegerToObject(argv[3], state);
    Gua_IntegerToObject(argv[4], x);
    Gua_IntegerToObject(argv[5], y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_MouseButtonReleaseFunc(void *window, unsigned int button, unsigned int state, int x, int y)
 *
 * Description:
 *     GLWM mouse button release callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     button,    pointer x coordinate;
 *     state,     pointer y coordinate.
 *     x,         pointer x coordinate;
 *     y,         pointer y coordinate.
 *
 * Results:
 *     This function is a wrapper to the mouse button release callback.
 */
void Glw_MouseButtonReleaseFunc(void *window, unsigned int button, unsigned int state, int x, int y)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_MOUSEBUTTONRELEASEFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 6;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], button);
    Gua_IntegerToObject(argv[3], state);
    Gua_IntegerToObject(argv[4], x);
    Gua_IntegerToObject(argv[5], y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glw_CloseFunc(void *window)
 *
 * Description:
 *     GLWM close window callback.
 *
 * Arguments:
 *     window,    the window that received the event.
 *
 * Results:
 *     This function is a wrapper to the close window callback.
 */
void Glw_CloseFunc(void *window)
{
    Gua_Namespace *nspace;
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    nspace = Glwm_WindowUserData((Glwm_Window *)window);
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLWM_CLOSEFUNC[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(nspace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "Glwm_Window", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(nspace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(nspace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glw_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Glw_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Namespace *top;
    Gua_Handle *h;
    void *arg1p;
    Glwm_Window *window;
    Gua_String expression;
    Gua_String p;
    Gua_Status status;
    Gua_String errMessage;
    
    top = (Gua_Namespace *)nspace;
    while (top->previous) {
        top = (Gua_Namespace *)top->previous;
    }
    
    arg1p = NULL;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "glwmCloseWindow") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Gua_IntegerToPObject(object, Glwm_CloseWindow((Glwm_Window *)arg1p));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmCreateWindow") == 0) {
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
        
        window = (Glwm_Window *)Glwm_CreateWindow(Gua_ObjectToString(argv[1]));
        Glwm_SetUserData(window, top);
        
        if (window != NULL) {
            Gua_NewHandle(h, "Glwm_Window", window);
            
            Gua_HandleToPObject(object, (struct Gua_Handle *)h);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmDestroyWindow") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_DestroyWindow((Glwm_Window *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmExitMainLoop") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Glwm_ExitMainLoop();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmGetTime") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_RealToPObject(object, Glwm_GetTime());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmHideWindow") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_HideWindow((Glwm_Window *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmMainIteration") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Glwm_MainIteration();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmMainLoop") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Glwm_MainLoop();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmMakeCurrent") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_MakeCurrent((Glwm_Window *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterCloseFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_CLOSEFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterCloseFunc((Glwm_Window *)arg1p, &Glw_CloseFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterDisplayFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_DISPLAYFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterDisplayFunc((Glwm_Window *)arg1p, &Glw_DisplayFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterIdleFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_IDLEFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterIdleFunc((Glwm_Window *)arg1p, &Glw_IdleFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterKeyPressFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_KEYPRESSFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterKeyPressFunc((Glwm_Window *)arg1p, &Glw_KeyPressFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterKeyReleaseFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_KEYRELEASEFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterKeyReleaseFunc((Glwm_Window *)arg1p, &Glw_KeyReleaseFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterMouseButtonPressFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_MOUSEBUTTONPRESSFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterMouseButtonPressFunc((Glwm_Window *)arg1p, &Glw_MouseButtonPressFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterMouseButtonReleaseFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_MOUSEBUTTONRELEASEFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterMouseButtonReleaseFunc((Glwm_Window *)arg1p, &Glw_MouseButtonReleaseFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterMouseEnterFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_MOUSEENTERFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterMouseEnterFunc((Glwm_Window *)arg1p, &Glw_MouseEnterFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterMouseLeaveFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_MOUSELEAVEFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterMouseLeaveFunc((Glwm_Window *)arg1p, &Glw_MouseLeaveFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterMouseMotionFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_MOUSEMOTIONFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterMouseMotionFunc((Glwm_Window *)arg1p, &Glw_MouseMotionFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmRegisterReshapeFunc") == 0) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLWM_RESHAPEFUNC[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Glwm_RegisterReshapeFunc((Glwm_Window *)arg1p, &Glw_ReshapeFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmSetCloseWindow") == 0) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_SetCloseWindow((Glwm_Window *)arg1p, Gua_ObjectToInteger(argv[2]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmSetWindowPos") == 0) {
        if (argc < 4) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_SetWindowPos((Glwm_Window *)arg1p, Gua_ObjectToInteger(argv[2]), Gua_ObjectToInteger(argv[3]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmSetWindowSize") == 0) {
        if (argc < 4) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_SetWindowSize((Glwm_Window *)arg1p, Gua_ObjectToInteger(argv[2]), Gua_ObjectToInteger(argv[3]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmSetWindowTitle") == 0) {
        if (argc < 3) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_SetWindowTitle((Glwm_Window *)arg1p, Gua_ObjectToString(argv[2]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmShowWindow") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_ShowWindow((Glwm_Window *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmSwapBuffers") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Glwm_SwapBuffers((Glwm_Window *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmTerminate") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Glwm_Terminate();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmWindowHeight") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Gua_IntegerToPObject(object, Glwm_WindowHeight((Glwm_Window *)arg1p));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmWindowWidth") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Gua_IntegerToPObject(object, Glwm_WindowWidth((Glwm_Window *)arg1p));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmWindowXPos") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Gua_IntegerToPObject(object, Glwm_WindowXPos((Glwm_Window *)arg1p));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glwmWindowYPos") == 0) {
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
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Glwm_Window") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Gua_IntegerToPObject(object, Glwm_WindowYPos((Glwm_Window *)arg1p));
    }

    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glw_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Glw_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmCloseWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmCloseWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmCreateWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmCreateWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmDestroyWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmDestroyWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmExitMainLoop", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmExitMainLoop");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmHideWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmHideWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmGetTime", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmGetTime");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmMainIteration", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmMainIteration");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmMainLoop", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmMainLoop");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmMakeCurrent", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmMakeCurrent");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterCloseFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterCloseFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterDisplayFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterDisplayFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterIdleFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterIdleFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterKeyPressFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterKeyPressFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterKeyReleaseFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterKeyReleaseFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterMouseButtonPressFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterMouseButtonPressFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterMouseButtonReleaseFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterMouseButtonReleaseFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterMouseEnterFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterMouseEnterFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterMouseLeaveFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterMouseLeaveFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterMouseMotionFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterMouseMotionFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmRegisterReshapeFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmRegisterReshapeFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmSetCloseWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmSetCloseWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmSetWindowPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmSetWindowPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmSetWindowSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmSetWindowSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmSetWindowTitle", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmSetWindowTitle");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmShowWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmShowWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmSwapBuffers", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmSwapBuffers");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmTerminate", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmTerminate");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmWindowHeight", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmWindowHeight");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmWindowWidth", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmWindowWidth");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmWindowXPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmWindowXPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glwmWindowYPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glwmWindowYPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }

    /* Define each extension constant... */
    Gua_IntegerToObject(object, GLWM_BUTTON_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_BUTTON_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_BUTTON_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_BUTTON_UP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_BUTTON_UP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_BUTTON_UP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_LEFT_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_LEFT_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_LEFT_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_MIDDLE_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_MIDDLE_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_MIDDLE_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_MOD_ALT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_MOD_ALT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_MOD_ALT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_MOD_CONTROL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_MOD_CONTROL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_MOD_CONTROL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_MOD_SHIFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_MOD_SHIFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_MOD_SHIFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_MOD_SUPER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_MOD_SUPER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_MOD_SUPER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_RIGHT_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_RIGHT_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_RIGHT_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_SCROLL_DOWN_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_SCROLL_DOWN_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_SCROLL_DOWN_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLWM_SCROLL_UP_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLWM_SCROLL_UP_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLWM_SCROLL_UP_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_GLWM_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_GLWM_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_GLWM_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_GLWM_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Call the GLWM Initializer. */
    if (!Glwm_Init(&argc, argv, env)) {
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

