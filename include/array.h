/*
 * array.h
 *
 *     This library implements the array functions for an expression interpreter.
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
 * RCS: @(#) $Id: array.h,v 1.7 2015/10/08 21:51:00 monteiro Exp $
 * 
 */

#define ARRAY_ASCENDING_ORDER   0
#define ARRAY_DESCENDING_ORDER  1

#define ARRAY_VERSION "1.7"

Gua_Status Array_Intersection(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Array_Sort(Gua_Object *target, Gua_Object *source, Gua_Integer order, Gua_String error);
Gua_Status Array_ArrayFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Array_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

