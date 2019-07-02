/*
 * fs.h
 *
 *     This library implements the file system access functions.
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
 * RCS: @(#) $Id: fs.h,v 2.0 2015/09/17 01:34:00 monteiro Exp $
 * 
 */

#define FS_VERSION "2.0"

Gua_Status Fs_Copy(Gua_String source, Gua_String target, Gua_String error);
Gua_Status Fs_Delete(Gua_String file, Gua_String error);
Gua_Integer Fs_Exists(Gua_String file);
void Fs_Extension(Gua_String file, Gua_String *extension);
void Fs_FullPath(Gua_String file, Gua_String *path);
Gua_Integer Fs_IsDirectory(Gua_String file);
Gua_Integer Fs_IsExecutable(Gua_String file);
Gua_Integer Fs_IsReadable(Gua_String file);
Gua_Integer Fs_IsWritable(Gua_String file);
Gua_Integer Fs_Link(Gua_String source, Gua_String target);
Gua_Status Fs_List(Gua_String directory, Gua_Object *list, Gua_String error);
Gua_Integer Fs_MkDir(Gua_String directory);
void Fs_Name(Gua_String file, Gua_String *name);
void Fs_Path(Gua_String file, Gua_String *path);
Gua_Status Fs_Rename(Gua_String source, Gua_String target, Gua_String error);
Gua_Integer Fs_Size(Gua_String file);
Gua_Real Fs_Time(Gua_String file);
Gua_Status Fs_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Fs_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

