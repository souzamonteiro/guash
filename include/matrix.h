/*
 * matrix.h
 *
 *     This library implements the matrix functions for an expression interpreter.
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
 * RCS: @(#) $Id: matrix.h,v 2.3 2017/01/07 17:49:00 monteiro Exp $
 * 
 */

#define MATRIX_VERSION "2.3"

Gua_Real Matrix_GaussMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Real Matrix_JordanMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Status Matrix_DetMatrix(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_TransMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Status Matrix_Cross(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Matrix_Dot(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Matrix_Sum(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Sum2(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_SumCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Avg(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_AvgCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Count(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_CountCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error);
Gua_Status Matrix_DelRow(Gua_Object *source, Gua_Object n, Gua_Object *object, Gua_String error);
Gua_Status Matrix_DelCol(Gua_Object *source, Gua_Object n, Gua_Object *object, Gua_String error);
Gua_Status Matrix_MatrixFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);
