/*
 * numeric.h
 *
 *     This library implements the numeric computation functions for an expression interpreter.
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
 * RCS: @(#) $Id: numeric.h,v 1.5 2017/05/27 09:42:00 monteiro Exp $
 * 
 */

#define NUMERIC_VERSION "1.5"
#define NUMERIC_GOLDEN_NUMBER 1.6180
#define NUMERIC_X "x"
#define NUMERIC_XYZ "{\"x\", \"y\", \"z\"}"

Gua_Status Numeric_GaussLSS(Gua_Object *a, Gua_Object *b, Gua_Object *x, Gua_String error);
Gua_Status Numeric_NumericFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Numeric_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);
