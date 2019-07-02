/**
 * File:
 *     tui.c
 *
 * Package:
 *     Tui
 *
 * Description:
 *     This library implements a Terminal User Interface for an expression interpreter.
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
 * RCS: @(#) $Id: tui.c,v 1.4 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "interp.h"
#include "tui.h"

#define BUFFER_SIZE 255

/**
 * Group:
 *     C
 *
 * Function:
 *     void Tui_Clreol(void)
 *
 * Description:
 *     Clear from cursor to the end of line.
 *
 * Results:
 *     Clear from cursor to the end of line.
 */
void Tui_Clreol(void) {
    printf("\x1b[K");
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Tui_Clrscr(void)
 *
 * Description:
 *     Clear the entire screen.
 *
 * Results:
 *     Clear the entire screen.
 */
void Tui_Clrscr(void) {
    printf("\x1b[2J");
    Tui_Gotoxy(1,1);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Tui_Getch(void)
 *
 * Description:
 *     Return a single character from the standard input, without a buffer.
 *     The character is not echoed to the screen.
 *
 * Results:
 *     The function returns a caracter typed by the user.
 */
int Tui_Getch(void) {
    struct termios old;
    struct termios new;
    int ch;
    
    tcgetattr(0, &old);

    new = old;

    new.c_lflag &= ~(ICANON | ECHO);
    new.c_cc[VTIME] = 0;
    new.c_cc[VMIN] = 1;

    tcsetattr(0, TCSANOW, &new);
    
    ch = getchar();
    
    tcsetattr(0, TCSANOW, &old);
    
    return(ch);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Tui_Getche(void)
 *
 * Description:
 *     Return a single character from the standard input, without a buffer.
 *     The character is echoed to the screen.
 *
 * Results:
 *     The function returns a caracter typed by the user.
 */
int Tui_Getche(void) {
    struct termios old;
    struct termios new;
    int ch;

    tcgetattr(0, &old);

    new = old;

    new.c_lflag &= ~(ICANON);
    new.c_cc[VTIME] = 0;
    new.c_cc[VMIN] = 1;

    tcsetattr(0, TCSANOW, &new);

    ch = getchar();

    tcsetattr(0, TCSANOW, &old);

    return(ch);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Tui_Getkey(void)
 *
 * Description:
 *     Return a single character from the standard input, without a buffer, and without block.
 *     The character is not echoed to the screen.
 *
 * Results:
 *     The function returns a caracter typed by the user.
 */
int Tui_Getkey(void) {
    struct termios old;
    struct termios new;
    int ch;
    
    tcgetattr(0, &old);

    new = old;

    new.c_lflag &= ~(ICANON | ECHO);
    new.c_cc[VTIME] = 0;
    new.c_cc[VMIN] = 0;

    tcsetattr(0, TCSANOW, &new);
    
    ch = getchar();
    
    tcsetattr(0, TCSANOW, &old);
    
    return(ch);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Tui_Gotoxy(int x, int y)
 *
 * Description:
 *     Move the cursor to the column x and line y.
 *
 * Arguments:
 *     x,    the screen column;
 *     y,    the screen row.
 *
 * Results:
 *     Move the cursor to the column x and line y.
 */
void Tui_Gotoxy(int x, int y) {
    printf("\x1b[%d;%dH", y, x);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Tui_Textcolor(int color)
 *
 * Description:
 *     Sets the foreground text color.
 *
 * Arguments:
 *     color,    the foreground text color.
 *
 * Results:
 *     Sets the foreground text color.
 */
void Tui_Textcolor(int color) {
    printf("\x1b[%dm", 30 + color);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Tui_Textbackground(int color)
 *
 * Description:
 *     Sets the background text color.
 *
 * Arguments:
 *     color,    the background text color.
 *
 * Results:
 *     Sets the background text color.
 */
void Tui_Textbackground(int color) {
    printf("\x1b[%dm", 40 + color);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Tui_Entry(int x, int y, char *buffer, int size, int forecolor, int backcolor)
 *
 * Description:
 *     Get an entry at column x and line y.
 *
 * Arguments:
 *     x,            the screen column;
 *     y,            the screen row;
 *     buffer,       the buffer to the typed text;
 *     size,         the field length;
 *     forecolor,    the foreground color;
 *     backcolor,    the background color.
 *
 * Results:
 *     The function returns an entry typed at column x and line y.
 */
int Tui_Entry(int x, int y, char *buffer, int size, int forecolor, int backcolor) {
    char *entry;
    int length;
    int i;
    
    entry = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
    memset(entry, '\0', BUFFER_SIZE);
    
    /* Set the forecolor and the backcolor for this field. */
    Tui_Textcolor(forecolor);
    Tui_Textbackground(backcolor);

    /* Paint the area of the field with the specified colors. */
    Tui_Gotoxy(x, y);
    
    for (i = 0; i < size; i++) {
        putchar(' ');
    }
    
    /* Get the entry. */
    Tui_Gotoxy(x, y);
    
    fgets(entry, BUFFER_SIZE, stdin);
    /* Remove '\n' from the end of the entry. */
    if (strlen(entry) > 0) {
        entry[strlen(entry) - 1] = '\0';
    }
    length = strlen(entry);
    
    if (length <= size) {
        strcpy(buffer, entry);
    } else {
        strncpy(buffer, entry, size);
        length = size;
    }
    
    Gua_Free(entry);
    
    return(length);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Tui_Box(int x1, int y1, int x2, int y2, int bordercolor, int backcolor)
 *
 * Description:
 *     Draw a box on the screen.
 *
 * Arguments:
 *     x1,             the top left x corner;
 *     y1,             the top left y corner;
 *     x2,             the bottom right x corner;
 *     y2,             the bottom right y corner;
 *     bordercolor,    the border color;
 *     backcolor,      the background color.
 *
 * Results:
 *     Draw a box on the screen.
 */
void Tui_Box(int x1, int y1, int x2, int y2, int bordercolor, int backcolor) {
    int column;
    int row;

    /* Draw the border of the box. */
    Tui_Textcolor(bordercolor);
    Tui_Textbackground(bordercolor);

    for (column = x1; column <= x2; column++) {
        Tui_Gotoxy(column, y1);
        putchar(' ');

        Tui_Gotoxy(column, y2);
        putchar(' ');
    }

    for (row = y1; row <= y2; row++) {
        Tui_Gotoxy(x1, row);
        putchar(' ');

        Tui_Gotoxy(x2, row);
        putchar(' ');
    }

    /* Paint the inside of the box. */
    Tui_Textcolor(backcolor);
    Tui_Textbackground(backcolor);

    for (row = y1 + 1; row <= y2 - 1; row++) {
        for (column = x1 + 1; column <= x2 - 1; column++) {
            Tui_Gotoxy(column, row);
            putchar(' ');
        }
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Tui_TuiFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Terminal User Interface functions wrapper.
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
Gua_Status Tui_TuiFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String buffer;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "box") == 0) {
        /* Draw a box on the screen. */
        if (argc != 7) {
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
        if (Gua_ObjectType(argv[4]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[5]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[6]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 6 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Tui_Box(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2]), Gua_ObjectToInteger(argv[3]), Gua_ObjectToInteger(argv[4]), Gua_ObjectToInteger(argv[5]), Gua_ObjectToInteger(argv[6]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "clreol") == 0) {
        /* Clear from cursor to the end of line. */
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Tui_Clreol();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "clrscr") == 0) {
        /* Clear the entire screen. */
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Tui_Clrscr();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "entry") == 0) {
        /* Get an entry. */
        if (argc != 6) {
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
        if (Gua_ObjectType(argv[4]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[5]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (Gua_String)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
        memset(buffer, '\0', BUFFER_SIZE);
        
        Tui_Entry(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2]), buffer, Gua_ObjectToInteger(argv[3]), Gua_ObjectToInteger(argv[4]), Gua_ObjectToInteger(argv[5]));
        
        Gua_StringToPObject(object, buffer);
        
        Gua_Free(buffer);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "getch") == 0) {
        /*
         * Return a single character from the standard input, without a buffer.
         * The character is not echoed to the screen.
         */
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (Gua_String)Gua_Alloc(sizeof(char) * 2);
        
        buffer[0] = Tui_Getch();
        buffer[1] = '\0';
        
        Gua_StringToPObject(object, buffer);
        
        Gua_Free(buffer);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "getche") == 0) {
        /*
         * Return a single character from the standard input, without a buffer.
         * The character is echoed to the screen.
         */
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (Gua_String)Gua_Alloc(sizeof(char) * 2);
        
        buffer[0] = Tui_Getche();
        buffer[1] = '\0';
        
        Gua_StringToPObject(object, buffer);

        Gua_Free(buffer);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "getkey") == 0) {
        /*
         * Return a single character from the standard input, as an integer, without a buffer.
         * The character is not echoed to the screen.
         */
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, Tui_Getkey());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "gotoxy") == 0) {
        /* Clear the entire screen. */
        if (argc != 3) {
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
        
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Tui_Gotoxy((Gua_Short)Gua_ObjectToInteger(argv[1]), (Gua_Short)Gua_ObjectToInteger(argv[2]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "textbackground") == 0) {
        /* Clear the entire screen. */
        if (argc != 2) {
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
        
        Tui_Textbackground((Gua_Short)Gua_ObjectToInteger(argv[1]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "textcolor") == 0) {
        /* Clear the entire screen. */
        if (argc != 2) {
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
        
        Tui_Textcolor((Gua_Short)Gua_ObjectToInteger(argv[1]));
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Tui_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the Terminal User Interface functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the Terminal User Interface functions.
 */
Gua_Status Tui_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Tui_TuiFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "box", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "box");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "clreol", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "clreol");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "clrscr", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "clrscr");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "entry", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "entry");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "getch", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "getch");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "getche", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "getche");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "getkey", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "getkey");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gotoxy", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gotoxy");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "textbackground", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "textbackground");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "textcolor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "textcolor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    Gua_IntegerToObject(object, TUI_BLACK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "BLACK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "BLACK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_BLUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "BLUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "BLUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_CYAN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "CYAN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "CYAN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_GREEN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GREEN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GREEN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_MAGENTA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "MAGENTA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "MAGENTA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_RED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "RED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "RED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_WHITE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "WHITE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "WHITE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, TUI_YELLOW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "YELLOW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "YELLOW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     TUI_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, TUI_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "TUI_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "TUI_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}
