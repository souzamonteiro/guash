/*
 * glwm.h
 *
 *     This file implements an OpenGL window management library.
 *
 * Copyright (c) 2017 Roberto Luiz Souza Monteiro, Hernane B. B. Pereira, Marcelo A. Moret.
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
 * RCS: @(#) $Id: glwm.h,v 1.0 2015/10/26 17:22:00 monteiro Exp $
 * 
 */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glx.h>

#define GLWM_VERSION "1.0"

#define GLWM_DEFAULT_WIDTH 320
#define GLWM_DEFAULT_HEIGHT 240
#define GLWM_DEFAULT_X 0
#define GLWM_DEFAULT_Y 0

typedef struct {
    void *data;
    void *previous;
    void *next;
} Glwm_ListItem;

typedef struct {
    int width;
    int height;
    int x;
    int y;
    Display *display;
    XVisualInfo *visual;
    Colormap colormap;
    Window window;
    GLXContext context;
    void *userData;
    int closeState;
    void (*displayFunc)(void *window);
    void (*reshapeFunc)(void *window);
    void (*idleFunc)(void *window);
    void (*keyPressFunc)(void *window, unsigned char c, int x, int y);
    void (*keyReleaseFunc)(void *window, unsigned char c, int x, int y);
    void (*mouseEnterFunc)(void *window);
    void (*mouseLeaveFunc)(void *window);
    void (*mouseMotionFunc)(void *window, int x, int y);
    void (*mouseButtonPressFunc)(void *window, unsigned int button, unsigned int state, int x, int y);
    void (*mouseButtonReleaseFunc)(void *window, unsigned int button, unsigned int state, int x, int y);
    void (*closeFunc)(void *window);
} Glwm_Window;

#define GLWM_LEFT_BUTTON         0
#define GLWM_MIDDLE_BUTTON       1
#define GLWM_RIGHT_BUTTON        2
#define GLWM_SCROLL_UP_BUTTON    3
#define GLWM_SCROLL_DOWN_BUTTON  4

#define GLWM_BUTTON_DOWN  0
#define GLWM_BUTTON_UP    1

#define GLWM_MOD_SHIFT    1
#define GLWM_MOD_CONTROL  2
#define GLWM_MOD_ALT      4
#define GLWM_MOD_SUPER    8

#define Glwm_WindowWidth(w) (w)->width
#define Glwm_WindowHeight(w) (w)->height
#define Glwm_WindowXPos(w) (w)->x
#define Glwm_WindowYPos(w) (w)->y
#define Glwm_WindowDisplay(w) (w)->display
#define Glwm_WindowVisual(w) (w)->visual
#define Glwm_WindowColormap(w) (w)->colormap
#define Glwm_WindowWindow(w) (w)->window
#define Glwm_WindowContext(w) (w)->context
#define Glwm_WindowUserData(w) (w)->userData
#define Glwm_WindowCloseState(w) (w)->closeState
#define Glwm_WindowDisplayFunc(w) (w)->displayFunc
#define Glwm_WindowReshapeFunc(w) (w)->reshapeFunc
#define Glwm_WindowIdleFunc(w) (w)->idleFunc
#define Glwm_WindowKeyPressFunc(w) (w)->keyPressFunc
#define Glwm_WindowKeyReleaseFunc(w) (w)->keyReleaseFunc
#define Glwm_WindowMouseEnterFunc(w) (w)->mouseEnterFunc
#define Glwm_WindowMouseLeaveFunc(w) (w)->mouseLeaveFunc
#define Glwm_WindowMouseMotionFunc(w) (w)->mouseMotionFunc
#define Glwm_WindowCloseFunc(w) (w)->closeFunc

#define Glwm_CloseWindow(w) Glwm_WindowCloseState(w)

#define Glwm_SetUserData(w,d) {(w)->userData = d;}
#define Glwm_SetCloseWindow(w,v) {(w)->closeState = v;}

int Glwm_ComparePointers(void *p1, void *p2);
Glwm_ListItem *Glwm_AddListItem(Glwm_ListItem **list, void *data);
int Glwm_DeleteListItem(Glwm_ListItem **list, void *data, int (*cmpfunc)(void *p1, void *p2));
void *Glwm_FindListItem(Glwm_ListItem *list, void *data, int (*cmpfunc)(void *p1, void *p2));
void Glwm_FreeList(Glwm_ListItem *list, void (*freefunc)(void *data));

void Glwm_FreeWindow(void *data);

#define Glwm_AddWindowToList(w) Glwm_AddListItem(&windowList, w)
#define Glwm_DeleteWindowFromList(w) Glwm_DeleteListItem(&windowList, w, &Glwm_ComparePointers)
#define Glwm_FindWindowInList(w) Glwm_FindListItem(windowList, w, &Glwm_ComparePointers)
#define Glwm_FreeWindowList() Glwm_FreeList(windowList, &Glwm_FreeWindow)

void Glwm_ParseVarAssignment(char *expression, char *variable, char *value);

int Glwm_Init(int *argc, char *argv[], char **env);

Glwm_Window *Glwm_CreateWindow(char *title);
void Glwm_DestroyWindow(Glwm_Window *window);

void Glwm_SetWindowTitle(Glwm_Window *window, char *title);
void Glwm_SetWindowPos(Glwm_Window *window, int x, int y);
void Glwm_SetWindowSize(Glwm_Window *window, int width, int height);
void Glwm_HideWindow(Glwm_Window *window);
void Glwm_ShowWindow(Glwm_Window *window);

void Glwm_RegisterDisplayFunc(Glwm_Window *window, void (*function)(void *window));
void Glwm_RegisterReshapeFunc(Glwm_Window *window, void (*function)(void *window));
void Glwm_RegisterIdleFunc(Glwm_Window *window, void (*function)(void *window));
void Glwm_RegisterKeyPressFunc(Glwm_Window *window, void (*function)(void *window, unsigned char c, int x, int y));
void Glwm_RegisterKeyReleaseFunc(Glwm_Window *window, void (*function)(void *window, unsigned char c, int x, int y));
void Glwm_RegisterMouseEnterFunc(Glwm_Window *window, void (*function)(void *window));
void Glwm_RegisterMouseLeaveFunc(Glwm_Window *window, void (*function)(void *window));
void Glwm_RegisterMouseMotionFunc(Glwm_Window *window, void (*function)(void *window, int x, int y));
void Glwm_RegisterMouseButtonPressFunc(Glwm_Window *window, void (*function)(void *window, unsigned int button, unsigned int state, int x, int y));
void Glwm_RegisterMouseButtonReleaseFunc(Glwm_Window *window, void (*function)(void *window, unsigned int button, unsigned int state, int x, int y));
void Glwm_RegisterCloseFunc(Glwm_Window *window, void (*function)(void *window));

void Glwm_MakeCurrent(Glwm_Window *window);
void Glwm_SwapBuffers(Glwm_Window *window);

void Glwm_MainLoop(void);
void Glwm_MainIteration(void);
void Glwm_ExitMainLoop(void);
void Glwm_Terminate(void);

double Glwm_GetTime(void);

