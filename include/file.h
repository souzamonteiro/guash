/*
 * file.h
 *
 *     This library implements a virtual file system for an expression interpreter.
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
 * RCS: @(#) $Id: file.c,v 1.3 2013/09/24 19:28:00 monteiro Exp $
 * 
 */

#define FILE_VERSION "1.3"

void File_Clearerr(void *h);
Gua_Short File_Close(void *h);
Gua_Short File_Eof(void *h);
Gua_Short File_Error(void *h);
Gua_Short File_Fileno(void *h);
Gua_Short File_Flush(void *h);
Gua_String File_Gets(Gua_String s, Gua_Short l, void *h);
Gua_File *File_Open(Gua_String p, Gua_String m);
Gua_Short File_Puts(Gua_String s, void *h);
Gua_Integer File_Read(Gua_String s, Gua_Integer l, Gua_Integer n, void *h);
void File_Rewind(void *h);
Gua_Short File_Seek(void *h, Gua_Integer o, Gua_Short w);
Gua_Integer File_Tell(void *h);
Gua_Integer File_Write(Gua_String s, Gua_Integer l, Gua_Integer n, void *h);
Gua_Status File_FileFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status File_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

