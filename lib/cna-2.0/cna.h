/*
 * cna.h
 *
 *     This file implements the complex network analysis functions for an expression interpreter.
 *
 * Copyright (C) 2005 Roberto Luiz Souza Monteiro.
 * Copyright (C) 2013 Roberto Luiz Souza Monteiro,
 *                    Hernane Borges de Barros Pereira,
 *                    Marcelo A. Moret.
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
 * RCS: @(#) $Id: cna.h,v 2.0 2015/12/16 16:04:00 monteiro Exp $
 * 
 */

#define CNA_VERSION "2.0"

Gua_Status Cna_ShortestPath(Gua_Object *adj, Gua_Object *geodesic, Gua_String error);
Gua_Status Cna_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Cna_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

