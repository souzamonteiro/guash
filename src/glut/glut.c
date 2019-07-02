/**
 * File:
 *     glut.c
 *
 * Package:
 *    Glut
 *
 * Description:
 *     This library implements an OpenGL GLUT wrapper
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
 * RCS: @(#) $Id: glut.c,v 1.0 2013/09/16 18:31:00 monteiro Exp $
 * 
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WINDOWS_
    #include <windows.h>
#endif
#ifdef _MAC_OS_X_
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include "interp.h"
#include "glut.h"

#define EXPRESSION_SIZE  4096
#define ERROR_SIZE  65536

Gua_Namespace *globalNameSpace;

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_ButtonBoxFunc(int button, int state)
 *
 * Description:
 *     GLUT dial and button box callback.
 *
 * Arguments:
 *     button,    the button number;
 *     state,     the button state.
 *
 * Results:
 *     This function is a wrapper to the dial and button box callback.
 */
void Glut_ButtonBoxFunc(int button, int state)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_BUTTONBOXFUNC[%ld] + \"(%d, %d);\")", currentWindow, button, state);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_CreateMenu(int value)
 *
 * Description:
 *     GLUT menu callback.
 *
 * Arguments:
 *     value,    the selected menu item.
 *
 * Results:
 *     This function is a wrapper to the menu callback.
 */
void Glut_CreateMenu(int value)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_CREATEMENU[%ld] + \"(%d);\")", currentWindow, value);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_DialsFunc(int dial, int value)
 *
 * Description:
 *     GLUT dial callback.
 *
 * Arguments:
 *     dial,     the dial number;
 *     value,    the dial value.
 *
 * Results:
 *     This function is a wrapper to the dial callback.
 */
void Glut_DialsFunc(int dial, int value)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_DIALSFUNC[%ld] + \"(%d, %d);\")", currentWindow, dial, value);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_DisplayFunc(void)
 *
 * Description:
 *     GLUT display callback.
 *
 * Results:
 *     This function is a wrapper to the display callback.
 */
void Glut_DisplayFunc(void)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_DISPLAYFUNC[%ld] + \"();\")", currentWindow);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_EntryFunc(int state)
 *
 * Description:
 *     GLUT mouse entry callback.
 *
 * Arguments:
 *     state,    the mouse pointer state on the selected window.
 *
 * Results:
 *     This function is a wrapper to the mouse entry callback.
 */
void Glut_EntryFunc(int state)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_ENTRYFUNC[%ld] + \"(%d);\")", currentWindow, state);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_IdleFunc(void)
 *
 * Description:
 *     GLUT idle callback.
 *
 * Results:
 *     This function is a wrapper to the idle callback.
 */
void Glut_IdleFunc(void)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_IDLEFUNC[%ld] + \"();\")", currentWindow);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_KeyboardFunc(unsigned char key, int x, int y)
 *
 * Description:
 *     GLUT keyboard callback.
 *
 * Arguments:
 *     key,    the key ASCII code;
 *     x,      the x position of the mouse pointer on the selected window;
 *     y,      the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the keyboard callback.
 */
void Glut_KeyboardFunc(unsigned char key, int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_KEYBOARDFUNC[%ld] + \"(%d, %d, %d);\")", currentWindow, key, x, y);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_MenuStateFunc(int s)
 *
 * Description:
 *     GLUT menu state callback.
 *
 * Arguments:
 *     s,    the menu status.
 *
 * Results:
 *     This function is a wrapper to the menu state callback.
 */
void Glut_MenuStateFunc(int s)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_MENUSTATEFUNC[%ld] + \"(%d);\")", currentWindow, s);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_MenuStatusFunc(int s, int x, int y)
 *
 * Description:
 *     GLUT menu status callback.
 *
 * Arguments:
 *     s,    the menu status;
 *     x,    the x position of the mouse pointer on the selected window;
 *     y,    the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the menu status callback.
 */
void Glut_MenuStatusFunc(int s, int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_MENUSTATUSFUNC[%ld] + \"(%d, %d, %d);\")", currentWindow, s, x, y);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_MotionFunc(int x, int y)
 *
 * Description:
 *     GLUT mouse motion callback.
 *
 * Arguments:
 *     x,    the x position of the mouse pointer on the selected window;
 *     y,    the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the mouse motion callback.
 */
void Glut_MotionFunc(int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_MOTIONFUNC[%ld] + \"(%d, %d);\")", currentWindow, x, y);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_MouseFunc(int button, int state, int x, int y)
 *
 * Description:
 *     GLUT mouse callback.
 *
 * Arguments:
 *     button,    the mouse button;
 *     state,     the mouse button state;
 *     x,         the x position of the mouse pointer on the selected window;
 *     y,         the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the mouse callback.
 */
void Glut_MouseFunc(int button, int state, int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_MOUSEFUNC[%ld] + \"(%d, %d, %d, %d);\")", currentWindow, button, state, x, y);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_OverlayDisplayFunc(void)
 *
 * Description:
 *     GLUT overlay display callback.
 *
 * Results:
 *     This function is a wrapper to the overlay display callback.
 */
void Glut_OverlayDisplayFunc(void)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_OVERLAYDISPLAYFUNC[%ld] + \"();\")", currentWindow);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_PassiveMotionFunc(int x, int y)
 *
 * Description:
 *     GLUT mouse passive motion callback.
 *
 * Arguments:
 *     x,    the x position of the mouse pointer on the selected window;
 *     y,    the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the mouse passive motion callback.
 */
void Glut_PassiveMotionFunc(int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_PASSIVEMOTIONFUNC[%ld] + \"(%d, %d);\")", currentWindow, x, y);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_ReshapeFunc(int width, int height)
 *
 * Description:
 *     GLUT reshape callback.
 *
 * Arguments:
 *     width,     window width;
 *     height,    window height.
 *
 * Results:
 *     This function is a wrapper to the reshape callback.
 */
void Glut_ReshapeFunc(int width, int height)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_RESHAPEFUNC[%ld] + \"(%d, %d);\")", currentWindow, width, height);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_SpaceballButtonFunc(int button, int state)
 *
 * Description:
 *     GLUT space ball button callback.
 *
 * Arguments:
 *     button,    the button number;
 *     state,     the button state.
 *
 * Results:
 *     This function is a wrapper to the space ball button callback.
 */
void Glut_SpaceballButtonFunc(int button, int state)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_SPACEBALLBUTTONFUNC[%ld] + \"(%d, %d);\")", currentWindow, button, state);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_SpaceballMotionFunc(int x, int y, int z)
 *
 * Description:
 *     GLUT space ball motion callback.
 *
 * Arguments:
 *     x,    the x position of the space ball on the selected window;
 *     y,    the y position of the space ball on the selected window;
 *     z,    the z position of the space ball on the selected window.
 *
 * Results:
 *     This function is a wrapper to the space ball motion callback.
 */
void Glut_SpaceballMotionFunc(int x, int y, int z)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_SPACEBALLMOTIONFUNC[%ld] + \"(%d, %d, %d);\")", currentWindow, x, y, z);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_SpaceballRotateFunc(int x, int y, int z)
 *
 * Description:
 *     GLUT space ball rotate callback.
 *
 * Arguments:
 *     x,    the x position of the space ball on the selected window;
 *     y,    the y position of the space ball on the selected window;
 *     z,    the z position of the space ball on the selected window.
 *
 * Results:
 *     This function is a wrapper to the space ball rotate callback.
 */
void Glut_SpaceballRotateFunc(int x, int y, int z)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_SPACEBALLROTATEFUNC[%ld] + \"(%d, %d, %d);\")", currentWindow, x, y, z);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_SpecialFunc(unsigned char key, int x, int y)
 *
 * Description:
 *     GLUT special keyboard callback.
 *
 * Arguments:
 *     key,    the key code;
 *     x,      the x position of the mouse pointer on the selected window;
 *     y,      the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the special keyboard callback.
 */
void Glut_SpecialFunc(int key, int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_SPECIALFUNC[%ld] + \"(%d, %d, %d);\")", currentWindow, key, x, y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_TabletButtonFunc(int button, int state, int x, int y)
 *
 * Description:
 *     GLUT tablet button callback.
 *
 * Arguments:
 *     button,    the button number;
 *     state,     the button state;
 *     x,         the x position of the mouse pointer on the selected window;
 *     y,         the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the tablet button callback.
 */
void Glut_TabletButtonFunc(int button, int state, int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_TABLETBUTTONFUNC[%ld] + \"(%d, %d, %d, %d);\")", currentWindow, button, state, x, y);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_TabletMotionFunc(int x, int y)
 *
 * Description:
 *     GLUT tablet motion callback.
 *
 * Arguments:
 *     x,    the x position of the mouse pointer on the selected window;
 *     y,    the y position of the mouse pointer on the selected window.
 *
 * Results:
 *     This function is a wrapper to the tablet motion callback.
 */
void Glut_TabletMotionFunc(int x, int y)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_TABLETMOTIONFUNC[%ld] + \"(%d, %d);\")", currentWindow, x, y);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_TimerFunc(int value)
 *
 * Description:
 *     GLUT timer callback.
 *
 * Arguments:
 *     value,    an integer value.
 *
 * Results:
 *     This function is a wrapper to the timer callback.
 */
void Glut_TimerFunc(int value)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_TIMERFUNC[%ld] + \"(%d);\")", currentWindow, value);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glut_VisibilityFunc(int state)
 *
 * Description:
 *     GLUT visibility callback.
 *
 * Arguments:
 *     state,    the window visibility state.
 *
 * Results:
 *     This function is a wrapper to the visibility callback.
 */
void Glut_VisibilityFunc(int state)
{
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    
    currentWindow = glutGetWindow();
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLUT_VISIBILITYFUNC[%ld] + \"(%d);\")", currentWindow, state);

    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, error);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glut_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Glut_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Integer arg1i;
    Gua_Integer arg2i;
    Gua_Integer arg3i;
    Gua_Integer arg4i;
    Gua_Integer arg5i;
    Gua_Real arg1d;
    Gua_Real arg2d;
    Gua_Real arg3d;
    Gua_Real arg4d;
    Gua_Integer currentWindow;
    Gua_String expression;
    Gua_String p;
    Gua_Status status;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    arg1i = 0;
    arg2i = 0;
    arg3i = 0;
    arg4i = 0;
    arg5i = 0;
    
    arg1d = 0.0;
    arg2d = 0.0;
    arg3d = 0.0;
    arg4d = 0.0;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "glutAddMenuEntry") == 0) {
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
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glutAddMenuEntry(Gua_ObjectToString(argv[1]), arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutAddSubMenu") == 0) {
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
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glutAddSubMenu(Gua_ObjectToString(argv[1]), arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutAttachMenu") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutAttachMenu(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutBitmapCharacter") == 0) {
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
        
        if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_8_BY_13") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_9_BY_15") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_HELVETICA_10") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_HELVETICA_12") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_HELVETICA_18") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_TIMES_ROMAN_10") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_BITMAP_TIMES_ROMAN_24") == 0) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Gua_ObjectToString(argv[2])[0]);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutBitmapLength") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutBitmapWidth") == 0) {
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
        
        if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_STROKE_MONO_ROMAN") == 0) {
            Gua_IntegerToPObject(object, glutBitmapWidth(GLUT_STROKE_MONO_ROMAN, Gua_ObjectToString(argv[2])[0]));
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_STROKE_ROMAN") == 0) {
            Gua_IntegerToPObject(object, glutBitmapWidth(GLUT_STROKE_ROMAN, Gua_ObjectToString(argv[2])[0]));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutButtonBoxFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_BUTTONBOXFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutButtonBoxFunc(Glut_ButtonBoxFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutChangeToMenuEntry") == 0) {
        if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glutChangeToMenuEntry(arg1i, Gua_ObjectToString(argv[2]), arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutChangeToSubMenu") == 0) {
        if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glutChangeToSubMenu(arg1i, Gua_ObjectToString(argv[2]), arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutCopyColormap") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutCopyColormap(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutCreateMenu") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_CREATEMENU[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        Gua_IntegerToPObject(object, glutCreateMenu(Glut_CreateMenu));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutCreateSubWindow") == 0) {
        if (argc != 6) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        
        Gua_IntegerToPObject(object, glutCreateSubWindow(arg1i, arg2i, arg3i, arg4i, arg5i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutCreateWindow") == 0) {
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
        
        Gua_IntegerToPObject(object, glutCreateWindow(Gua_ObjectToString(argv[1])));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutDestroyMenu") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutDestroyMenu(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutDestroyWindow") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutDestroyWindow(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutDetachMenu") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutDetachMenu(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutDeviceGet") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));           
        }
        
        Gua_IntegerToPObject(object, glutDeviceGet(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutDialsFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_DIALSFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutDialsFunc(Glut_DialsFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutDisplayFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_DISPLAYFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutDisplayFunc(Glut_DisplayFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutEnterGameMode") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutEntryFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_ENTRYFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutEntryFunc(Glut_EntryFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutEstablishOverlay") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutEstablishOverlay();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutExtensionSupported") == 0) {
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
        
        Gua_IntegerToPObject(object, glutExtensionSupported(Gua_ObjectToString(argv[1])));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutForceJoystickFunc") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutFullScreen") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutFullScreen();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGameModeGet") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGameModeString") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGet") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));           
        }
        
        Gua_IntegerToPObject(object, glutGet(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGetColor") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));           
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));           
        }
        
        Gua_RealToPObject(object, glutGetColor(arg1i, arg2i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGetMenu") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, glutGetMenu());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGetModifiers") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, glutGetModifiers());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutGetWindow") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, glutGetWindow());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutHideOverlay") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutHideOverlay();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutHideWindow") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutHideWindow();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutIconifyWindow") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutIconifyWindow();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutIdleFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_IDLEFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutIdleFunc(Glut_IdleFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutIgnoreKeyRepeat") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutInitDisplayMode") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutInitDisplayMode(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutInitDisplayString") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutInitWindowPosition") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glutInitWindowPosition(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutInitWindowSize") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glutInitWindowSize(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutJoystickFunc") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutKeyboardFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_KEYBOARDFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutKeyboardFunc(Glut_KeyboardFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutKeyboardUpFunc") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutLayerGet") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));           
        }
        
        Gua_IntegerToPObject(object, glutLayerGet(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutLeaveGameMode") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutMainLoop") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutMainLoop();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutMenuStateFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_MENUSTATEFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutMenuStateFunc(Glut_MenuStateFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutMenuStatusFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_MENUSTATUSFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutMenuStatusFunc(Glut_MenuStatusFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutMotionFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_MOTIONFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutMotionFunc(Glut_MotionFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutMouseFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_MOUSEFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutMouseFunc(Glut_MouseFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutOverlayDisplayFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_OVERLAYDISPLAYFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutOverlayDisplayFunc(Glut_OverlayDisplayFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPassiveMotionFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_PASSIVEMOTIONFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutPassiveMotionFunc(Glut_PassiveMotionFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPopWindow") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutPopWindow();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPositionWindow") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glutPositionWindow(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPostOverlayRedisplay") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutPostOverlayRedisplay();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPostRedisplay") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutPostRedisplay();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPostWindowOverlayRedisplay") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPostWindowRedisplay") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutPushWindow") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutPushWindow();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutRemoveMenuItem") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutRemoveMenuItem(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutRemoveOverlay") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutRemoveOverlay();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutReportErrors") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutReshapeFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_RESHAPEFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutReshapeFunc(Glut_ReshapeFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutReshapeWindow") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glutReshapeWindow(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetColor") == 0) {
        if (argc != 5) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));           
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
        
        glutSetColor(arg1i, arg2d, arg3d, arg4d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetCursor") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutSetCursor(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetIconTitle") == 0) {
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
        
        glutSetIconTitle(Gua_ObjectToString(argv[1]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetKeyRepeat") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetMenu") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutSetMenu(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetupVideoResizing") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetWindow") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutSetWindow(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSetWindowTitle") == 0) {
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
        
        glutSetWindowTitle(Gua_ObjectToString(argv[1]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutShowOverlay") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutShowOverlay();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutShowWindow") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutShowWindow();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidCone") == 0) {
		if (argc != 5) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        
        glutSolidCone(arg1d, arg2d, arg3i, arg4i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidCube") == 0) {
		if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        
        glutSolidCube(arg1d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidDodecahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutSolidDodecahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidIcosahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutSolidIcosahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidOctahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        glutSolidOctahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidSphere") == 0) {
		if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glutSolidSphere(arg1d, arg2i, arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidTeapot") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        
        glutSolidTeapot(arg1d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidTetrahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutSolidTetrahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSolidTorus") == 0) {
		if (argc != 5) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }

        glutSolidTorus(arg1d, arg2d, arg3i, arg4i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSpaceballButtonFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_SPACEBALLBUTTONFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutSpaceballButtonFunc(Glut_SpaceballButtonFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSpaceballMotionFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_SPACEBALLMOTIONFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutSpaceballMotionFunc(Glut_SpaceballMotionFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSpaceballRotateFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_SPACEBALLROTATEFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutSpaceballRotateFunc(Glut_SpaceballRotateFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSpecialFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_SPECIALFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutSpecialFunc(Glut_SpecialFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSpecialUpFunc") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutStopVideoResizing") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutStrokeCharacter") == 0) {
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
        
        if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_STROKE_MONO_ROMAN") == 0) {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, Gua_ObjectToString(argv[2])[0]);
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_STROKE_ROMAN") == 0) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, Gua_ObjectToString(argv[2])[0]);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutStrokeLength") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutStrokeWidth") == 0) {
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
        
        if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_STROKE_MONO_ROMAN") == 0) {
            Gua_IntegerToPObject(object, glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, Gua_ObjectToString(argv[2])[0]));
        } else if (strcmp(Gua_ObjectToString(argv[1]), "GLUT_STROKE_ROMAN") == 0) {
            Gua_IntegerToPObject(object, glutStrokeWidth(GLUT_STROKE_ROMAN, Gua_ObjectToString(argv[2])[0]));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutSwapBuffers") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutSwapBuffers();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutTabletButtonFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_TABLETBUTTONFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutTabletButtonFunc(Glut_TabletButtonFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutTabletMotionFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_TABLETMOTIONFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutTabletMotionFunc(Glut_TabletMotionFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutTimerFunc") == 0) {
        if (argc != 4) {
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
		if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
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
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_TIMERFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutTimerFunc(arg1i, Glut_TimerFunc, arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutUseLayer") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glutUseLayer(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutVideoPan") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutVideoResize") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutVideoResizeGet") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutVisibilityFunc") == 0) {
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

        currentWindow = glutGetWindow();
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLUT_VISIBILITYFUNC[%ld] = \"%s\";", currentWindow, Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glutVisibilityFunc(Glut_VisibilityFunc);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWarpPointer") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWindowStatusFunc") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireCone") == 0) {
		if (argc != 5) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        
        glutWireCone(arg1d, arg2d, arg3i, arg4i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireCube") == 0) {
		if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        
        glutWireCube(arg1d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireDodecahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutWireDodecahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireIcosahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutWireIcosahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireOctahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        glutWireOctahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireSphere") == 0) {
		if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glutWireSphere(arg1d, arg2i, arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireTeapot") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

		if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        
        glutWireTeapot(arg1d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireTetrahedron") == 0) {
		if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glutWireTetrahedron();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutWireTorus") == 0) {
		if (argc != 5) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

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
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }

        glutWireTorus(arg1d, arg2d, arg3i, arg4i);
    }

    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glut_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Glut_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    globalNameSpace = nspace;
    
    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutAddMenuEntry", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutAddMenuEntry");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutAddSubMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutAddSubMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutAttachMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutAttachMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutBitmapCharacter", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutBitmapCharacter");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutBitmapLength", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutBitmapLength");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutBitmapWidth", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutBitmapWidth");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutButtonBoxFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutButtonBoxFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutChangeToMenuEntry", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutChangeToMenuEntry");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutChangeToSubMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutChangeToSubMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutCopyColormap", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutCopyColormap");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutCreateMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutCreateMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutCreateSubWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutCreateSubWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutCreateWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutCreateWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutDestroyMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutDestroyMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutDestroyWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutDestroyWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutDetachMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutDetachMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutDeviceGet", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutDeviceGet");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutDialsFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutDialsFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutDisplayFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutDisplayFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutEnterGameMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutEnterGameMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutEntryFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutEntryFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutEstablishOverlay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutEstablishOverlay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutExtensionSupported", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutExtensionSupported");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutForceJoystickFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutForceJoystickFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutFullScreen", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutFullScreen");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGameModeGet", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGameModeGet");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGameModeString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGameModeString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGet", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGet");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGetColor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGetColor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGetMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGetMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGetModifiers", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGetModifiers");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutGetWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutGetWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutHideOverlay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutHideOverlay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutHideWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutHideWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutIconifyWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutIconifyWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutIdleFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutIdleFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutIgnoreKeyRepeat", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutIgnoreKeyRepeat");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutInitDisplayMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutInitDisplayMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutInitDisplayString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutInitDisplayString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutInitWindowPosition", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutInitWindowPosition");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutInitWindowSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutInitWindowSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutJoystickFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutJoystickFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutKeyboardFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutKeyboardFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutKeyboardUpFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutKeyboardUpFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutLayerGet", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutLayerGet");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutLeaveGameMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutLeaveGameMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutMainLoop", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutMainLoop");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutMenuStateFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutMenuStateFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutMenuStatusFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutMenuStatusFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutMotionFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutMotionFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutMouseFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutMouseFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutOverlayDisplayFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutOverlayDisplayFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPassiveMotionFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPassiveMotionFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPopWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPopWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPositionWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPositionWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPostOverlayRedisplay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPostOverlayRedisplay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPostRedisplay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPostRedisplay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPostWindowOverlayRedisplay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPostWindowOverlayRedisplay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPostWindowRedisplay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPostWindowRedisplay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutPushWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutPushWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutRemoveMenuItem", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutRemoveMenuItem");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutRemoveOverlay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutRemoveOverlay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutReportErrors", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutReportErrors");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutReshapeFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutReshapeFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutReshapeWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutReshapeWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetColor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetColor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetCursor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetCursor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetIconTitle", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetIconTitle");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetKeyRepeat", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetKeyRepeat");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetMenu", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetMenu");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetupVideoResizing", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetupVideoResizing");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSetWindowTitle", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSetWindowTitle");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutShowOverlay", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutShowOverlay");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutShowWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutShowWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidCone", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidCone");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidCube", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidCube");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidDodecahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidDodecahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidIcosahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidIcosahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidOctahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidOctahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidSphere", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidSphere");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidTeapot", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidTeapot");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidTetrahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidTetrahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSolidTorus", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSolidTorus");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSpaceballButtonFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSpaceballButtonFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSpaceballMotionFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSpaceballMotionFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSpaceballRotateFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSpaceballRotateFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSpecialFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSpecialFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSpecialUpFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSpecialUpFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutStopVideoResizing", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutStopVideoResizing");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutStrokeCharacter", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutStrokeCharacter");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutStrokeLength", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutStrokeLength");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutStrokeWidth", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutStrokeWidth");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutSwapBuffers", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutSwapBuffers");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutTabletButtonFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutTabletButtonFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutTabletMotionFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutTabletMotionFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutTimerFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutTimerFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutUseLayer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutUseLayer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutVideoPan", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutVideoPan");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutVideoResize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutVideoResize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutVideoResizeGet", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutVideoResizeGet");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutVisibilityFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutVisibilityFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWarpPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWarpPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWindowStatusFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWindowStatusFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireCone", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireCone");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireCube", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireCube");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireDodecahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireDodecahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireIcosahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireIcosahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireOctahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireOctahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireSphere", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireSphere");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireTeapot", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireTeapot");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireTetrahedron", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireTetrahedron");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutWireTorus", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutWireTorus");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glut_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }

    /* Define each extension constant... */
    Gua_IntegerToObject(object, GLUT_ACCUM);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ACCUM", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ACCUM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_ACTIVE_ALT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ACTIVE_ALT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ACTIVE_ALT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_ACTIVE_CTRL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ACTIVE_CTRL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ACTIVE_CTRL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_ACTIVE_SHIFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ACTIVE_SHIFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ACTIVE_SHIFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_API_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_API_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_API_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_8_BY_13");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_8_BY_13", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_8_BY_13");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_9_BY_15");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_9_BY_15", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_9_BY_15");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_HELVETICA_10");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_HELVETICA_10", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_HELVETICA_10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_HELVETICA_12");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_HELVETICA_12", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_HELVETICA_12");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_HELVETICA_18");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_HELVETICA_18", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_HELVETICA_18");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_TIMES_ROMAN_10");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_TIMES_ROMAN_10", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_TIMES_ROMAN_10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_BITMAP_TIMES_ROMAN_24");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BITMAP_TIMES_ROMAN_24", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BITMAP_TIMES_ROMAN_24");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_BLUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_BLUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_BLUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_BOTTOM_LEFT_CORNER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_BOTTOM_LEFT_CORNER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_BOTTOM_LEFT_CORNER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_BOTTOM_RIGHT_CORNER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_BOTTOM_RIGHT_CORNER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_BOTTOM_RIGHT_CORNER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_BOTTOM_SIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_BOTTOM_SIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_BOTTOM_SIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_CROSSHAIR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_CROSSHAIR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_CROSSHAIR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_CYCLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_CYCLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_CYCLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_DESTROY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_DESTROY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_DESTROY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_FULL_CROSSHAIR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_FULL_CROSSHAIR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_FULL_CROSSHAIR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_HELP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_HELP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_HELP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_INFO);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_INFO", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_INFO");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_INHERIT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_INHERIT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_INHERIT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_LEFT_ARROW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_LEFT_ARROW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_LEFT_ARROW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_LEFT_RIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_LEFT_RIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_LEFT_RIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_LEFT_SIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_LEFT_SIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_LEFT_SIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_NONE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_NONE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_NONE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_RIGHT_ARROW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_RIGHT_ARROW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_RIGHT_ARROW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_RIGHT_SIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_RIGHT_SIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_RIGHT_SIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_SPRAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_SPRAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_SPRAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_TEXT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_TEXT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_TEXT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_TOP_LEFT_CORNER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_TOP_LEFT_CORNER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_TOP_LEFT_CORNER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_TOP_RIGHT_CORNER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_TOP_RIGHT_CORNER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_TOP_RIGHT_CORNER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_TOP_SIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_TOP_SIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_TOP_SIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_UP_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_UP_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_UP_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_CURSOR_WAIT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_CURSOR_WAIT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_CURSOR_WAIT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_DEVICE_IGNORE_KEY_REPEAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_DEVICE_IGNORE_KEY_REPEAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_DEVICE_IGNORE_KEY_REPEAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_DEVICE_KEY_REPEAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_DEVICE_KEY_REPEAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_DEVICE_KEY_REPEAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_DISPLAY_MODE_POSSIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_DISPLAY_MODE_POSSIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_DISPLAY_MODE_POSSIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_DOUBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_DOUBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_DOUBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_ELAPSED_TIME);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ELAPSED_TIME", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ELAPSED_TIME");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_ENTERED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_ENTERED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_ENTERED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_FULLY_COVERED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_FULLY_COVERED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_FULLY_COVERED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_FULLY_RETAINED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_FULLY_RETAINED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_FULLY_RETAINED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_ACTIVE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_ACTIVE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_ACTIVE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_DISPLAY_CHANGED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_DISPLAY_CHANGED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_DISPLAY_CHANGED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_HEIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_HEIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_HEIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_PIXEL_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_PIXEL_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_PIXEL_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_POSSIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_POSSIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_POSSIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_REFRESH_RATE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_REFRESH_RATE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_REFRESH_RATE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GAME_MODE_WIDTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GAME_MODE_WIDTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GAME_MODE_WIDTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_GREEN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_GREEN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_GREEN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_DIAL_AND_BUTTON_BOX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_DIAL_AND_BUTTON_BOX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_DIAL_AND_BUTTON_BOX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_JOYSTICK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_JOYSTICK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_JOYSTICK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_KEYBOARD);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_KEYBOARD", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_KEYBOARD");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_MOUSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_MOUSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_MOUSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_OVERLAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_OVERLAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_OVERLAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_SPACEBALL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_SPACEBALL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_SPACEBALL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HAS_TABLET);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HAS_TABLET", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HAS_TABLET");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_HIDDEN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_HIDDEN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_HIDDEN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_INDEX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_INDEX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_INDEX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_INIT_DISPLAY_MODE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_INIT_DISPLAY_MODE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_INIT_DISPLAY_MODE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_INIT_WINDOW_HEIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_INIT_WINDOW_HEIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_INIT_WINDOW_HEIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_INIT_WINDOW_WIDTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_INIT_WINDOW_WIDTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_INIT_WINDOW_WIDTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_INIT_WINDOW_X);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_INIT_WINDOW_X", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_INIT_WINDOW_X");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_INIT_WINDOW_Y);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_INIT_WINDOW_Y", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_INIT_WINDOW_Y");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_AXES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_AXES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_AXES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_BUTTON_A);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_BUTTON_A", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_BUTTON_A");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_BUTTON_B);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_BUTTON_B", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_BUTTON_B");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_BUTTON_C);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_BUTTON_C", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_BUTTON_C");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_BUTTON_D);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_BUTTON_D", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_BUTTON_D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_BUTTONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_BUTTONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_BUTTONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_JOYSTICK_POLL_RATE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_JOYSTICK_POLL_RATE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_JOYSTICK_POLL_RATE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_END);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_END", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_END");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F10);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F10", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F11);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F11", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F11");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F12);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F12", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F12");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_F9);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_F9", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_F9");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_HOME);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_HOME", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_HOME");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_INSERT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_INSERT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_INSERT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_LEFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_LEFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_LEFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_PAGE_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_PAGE_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_PAGE_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_PAGE_UP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_PAGE_UP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_PAGE_UP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_REPEAT_DEFAULT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_REPEAT_DEFAULT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_REPEAT_DEFAULT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_REPEAT_OFF);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_REPEAT_OFF", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_REPEAT_OFF");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_REPEAT_ON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_REPEAT_ON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_REPEAT_ON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_RIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_RIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_RIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_KEY_UP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_KEY_UP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_KEY_UP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_LAYER_IN_USE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_LAYER_IN_USE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_LAYER_IN_USE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_LEFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_LEFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_LEFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_LEFT_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_LEFT_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_LEFT_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_LUMINANCE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_LUMINANCE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_LUMINANCE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_MENU_IN_USE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_MENU_IN_USE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_MENU_IN_USE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_MENU_NOT_IN_USE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_MENU_NOT_IN_USE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_MENU_NOT_IN_USE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_MENU_NUM_ITEMS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_MENU_NUM_ITEMS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_MENU_NUM_ITEMS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_MIDDLE_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_MIDDLE_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_MIDDLE_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_MULTISAMPLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_MULTISAMPLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_MULTISAMPLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NORMAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NORMAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NORMAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NORMAL_DAMAGED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NORMAL_DAMAGED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NORMAL_DAMAGED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NOT_VISIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NOT_VISIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NOT_VISIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NUM_BUTTON_BOX_BUTTONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NUM_BUTTON_BOX_BUTTONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NUM_BUTTON_BOX_BUTTONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NUM_DIALS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NUM_DIALS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NUM_DIALS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NUM_MOUSE_BUTTONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NUM_MOUSE_BUTTONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NUM_MOUSE_BUTTONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NUM_SPACEBALL_BUTTONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NUM_SPACEBALL_BUTTONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NUM_SPACEBALL_BUTTONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_NUM_TABLET_BUTTONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_NUM_TABLET_BUTTONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_NUM_TABLET_BUTTONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_OVERLAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_OVERLAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_OVERLAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_OVERLAY_DAMAGED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_OVERLAY_DAMAGED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_OVERLAY_DAMAGED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_OVERLAY_POSSIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_OVERLAY_POSSIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_OVERLAY_POSSIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_OWNS_JOYSTICK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_OWNS_JOYSTICK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_OWNS_JOYSTICK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_PARTIALLY_RETAINED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_PARTIALLY_RETAINED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_PARTIALLY_RETAINED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_RED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_RED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_RED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_RGBA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_RGBA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_RGBA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_RIGHT_BUTTON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_RIGHT_BUTTON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_RIGHT_BUTTON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_SCREEN_HEIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_SCREEN_HEIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_SCREEN_HEIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_SCREEN_HEIGHT_MM);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_SCREEN_HEIGHT_MM", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_SCREEN_HEIGHT_MM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_SCREEN_WIDTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_SCREEN_WIDTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_SCREEN_WIDTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_SCREEN_WIDTH_MM);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_SCREEN_WIDTH_MM", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_SCREEN_WIDTH_MM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_SINGLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_SINGLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_SINGLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_STENCIL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_STENCIL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_STENCIL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_STEREO);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_STEREO", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_STEREO");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_STROKE_MONO_ROMAN");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_STROKE_MONO_ROMAN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_STROKE_MONO_ROMAN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, "GLUT_STROKE_ROMAN");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_STROKE_ROMAN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_STROKE_ROMAN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_TRANSPARENT_INDEX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_TRANSPARENT_INDEX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_TRANSPARENT_INDEX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_UP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_UP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_UP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_HEIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_HEIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_HEIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_HEIGHT_DELTA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_HEIGHT_DELTA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_HEIGHT_DELTA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_IN_USE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_IN_USE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_IN_USE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_POSSIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_POSSIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_POSSIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_WIDTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_WIDTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_WIDTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_WIDTH_DELTA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_WIDTH_DELTA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_WIDTH_DELTA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_X);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_X", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_X");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_X_DELTA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_X_DELTA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_X_DELTA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_Y);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_Y", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_Y");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VIDEO_RESIZE_Y_DELTA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VIDEO_RESIZE_Y_DELTA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VIDEO_RESIZE_Y_DELTA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_VISIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_VISIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_VISIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_ACCUM_ALPHA_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_ACCUM_ALPHA_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_ACCUM_ALPHA_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_ACCUM_BLUE_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_ACCUM_BLUE_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_ACCUM_BLUE_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_ACCUM_GREEN_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_ACCUM_GREEN_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_ACCUM_GREEN_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_ACCUM_RED_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_ACCUM_RED_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_ACCUM_RED_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_ALPHA_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_ALPHA_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_ALPHA_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_BLUE_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_BLUE_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_BLUE_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_BUFFER_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_BUFFER_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_BUFFER_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_COLORMAP_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_COLORMAP_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_COLORMAP_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_DEPTH_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_DEPTH_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_DEPTH_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_DOUBLEBUFFER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_DOUBLEBUFFER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_DOUBLEBUFFER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_FORMAT_ID);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_FORMAT_ID", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_FORMAT_ID");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_GREEN_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_GREEN_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_GREEN_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_HEIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_HEIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_HEIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_NUM_CHILDREN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_NUM_CHILDREN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_NUM_CHILDREN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_NUM_SAMPLES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_NUM_SAMPLES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_NUM_SAMPLES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_PARENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_PARENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_PARENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_RED_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_RED_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_RED_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_RGBA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_RGBA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_RGBA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_STENCIL_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_STENCIL_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_STENCIL_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_STEREO);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_STEREO", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_STEREO");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_WIDTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_WIDTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_WIDTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_X);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_X", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_X");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_WINDOW_Y);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_WINDOW_Y", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_WINDOW_Y");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLUT_XLIB_IMPLEMENTATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUT_XLIB_IMPLEMENTATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUT_XLIB_IMPLEMENTATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_GLUT_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_GLUT_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_GLUT_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_GLUT_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Call the GLUT Initializer. */
    glutInit(&argc, argv);
    
    return GUA_OK;
}

