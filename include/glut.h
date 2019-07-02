/*
 * glut.h
 *
 *     This library implements an OpenGL GLUT wrapper
 *     for the GuaraScript interpreter.
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
 * RCS: @(#) $Id: glut.h,v 1.0 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#define GUA_GLUT_VERSION "1.0"

void Glut_ButtonBoxFunc(int button, int state);
void Glut_CreateMenu(int value);
void Glut_DialsFunc(int dial, int value);
void Glut_DisplayFunc(void);
void Glut_EntryFunc(int state);
void Glut_IdleFunc(void);
void Glut_KeyboardFunc(unsigned char key, int x, int y);
void Glut_MenuStateFunc(int s);
void Glut_MenuStatusFunc(int s, int x, int y);
void Glut_MotionFunc(int x, int y);
void Glut_MouseFunc(int button, int state, int x, int y);
void Glut_OverlayDisplayFunc(void);
void Glut_PassiveMotionFunc(int x, int y);
void Glut_ReshapeFunc(int width, int height);
void Glut_SpaceballButtonFunc(int button, int state);
void Glut_SpaceballMotionFunc(int x, int y, int z);
void Glut_SpaceballRotateFunc(int x, int y, int z);
void Glut_SpecialFunc(int key, int x, int y);
void Glut_TabletButtonFunc(int button, int state, int x, int y);
void Glut_TabletMotionFunc(int x, int y);
void Glut_TimerFunc(int value);
void Glut_VisibilityFunc(int state);
Gua_Status Glut_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Glut_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

