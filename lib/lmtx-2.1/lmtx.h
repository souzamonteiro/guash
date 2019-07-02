/*
 * lmtx.h
 *
 *     This script implements the learning matrix artificial neural network
 *     train algorithm.
 *
 * Copyright (C) 2017 Roberto Luiz Souza Monteiro,
 *                    Hernane Borges de Barros Pereira.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * RCS: @(#) $Id: lmtx.gua,v 2.0 2017/01/07 16:25:00 monteiro Exp $
 * 
 */

#define LMTX_VERSION "2.0"

#define LMTX_LINEAR_ACTIVATION_FUNCTION    0
#define LMTX_LOGISTIC_ACTIVATION_FUNCTION  1
#define LMTX_TANH_ACTIVATION_FUNCTION      2

#define LMTX_NO_OUTPUT_FUNCTION            0
#define LMTX_LINEAR_OUTPUT_FUNCTION        1
#define LMTX_STEP_OUTPUT_FUNCTION          2

Gua_Status Lmtx_Learn(Gua_Object *nn, Gua_Object *in, Gua_Object *out, Gua_Integer ni, Gua_Integer no, Gua_Real lrate, Gua_Integer af, Gua_Integer oaf);
Gua_Status Lmtx_Process(Gua_Object *nn, Gua_Object *in, Gua_Object *out, Gua_Integer ni, Gua_Integer no, Gua_Integer af, Gua_Integer oaf, Gua_Integer of, Gua_Object *c);

Gua_Status Lmtx_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Lmtx_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

