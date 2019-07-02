/*
 * system.h
 *
 *     This library implements the system functions for an expression interpreter.
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
 * RCS: @(#) $Id: system.h,v 2.6 2017/02/09 21:17:00 monteiro Exp $
 * 
 */

#define SYSTEM_VERSION "2.6"

#ifdef _LINUX_
    #define SYSTEM_HOST "linux"
    #define SYSTEM_ARCH "x86"
#elif _MAC_OS_X_
    #define SYSTEM_HOST "macosx"
    #define SYSTEM_ARCH "x86"
#elif _WINDOWS_
    #define SYSTEM_HOST "windows"
    #define SYSTEM_ARCH "x86"
#else
    #define SYSTEM_HOST "unix"
    #define SYSTEM_ARCH "x86"
#endif

typedef struct {
    void *data;
    void *previous;
    void *next;
} System_ListItem;

/* Function to maintain the list of opened libraries. */
System_ListItem *System_AddListItem(System_ListItem **list, void *data);
void System_FreeList(System_ListItem *list);

Gua_Status System_ExecFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
void System_ToCamelCase(char *target, char *source);
Gua_Status System_LoadFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_PrintFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_PrintlnFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_SourceFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);
Gua_Status System_Finish(void *nspace, int argc, char *argv[], char **env, Gua_String error);

