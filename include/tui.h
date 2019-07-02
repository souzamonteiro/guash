/*
 * tui.h
 *
 *     This library implements a Terminal User Interface for an expression interpreter.
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
 * RCS: @(#) $Id: tui.h,v 1.4 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#define TUI_VERSION "1.4"

/* Color definitions. */
#define TUI_BLACK    0
#define TUI_RED      1
#define TUI_GREEN    2
#define TUI_YELLOW   3
#define TUI_BLUE     4
#define TUI_MAGENTA  5
#define TUI_CYAN     6
#define TUI_WHITE    7

void Tui_Clreol(void);
void Tui_Clrscr(void);
int Tui_Getch(void);
int Tui_Getche(void);
int Tui_Getkey(void);
void Tui_Gotoxy(int x, int y);
void Tui_Textcolor(int color);
void Tui_Textbackground(int color);
int Tui_Entry(int x, int y, char *buffer, int size, int forecolor, int backcolor);
void Tui_Box(int x1, int y1, int x2, int y2, int bordercolor, int backcolor);
Gua_Status Tui_TuiFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Tui_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

