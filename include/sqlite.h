/*
 * sqlite.h
 *
 *     This library implements the SQLite functions wrapper.
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
 * RCS: @(#) $Id: sqlite.h,v 2.1 2015/12/14 15:46:00 monteiro Exp $
 * 
 */

#define GUA_SQLITE_VERSION "2.1"

Gua_Status Sqlite_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Sqlite_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

