/**
 * File:
 *     cna.c
 *
 * Package:
 *     Cna
 *
 * Description:
 *     This file implements the complex network analysis functions for an expression interpreter.
 *
 * Copyright:
 *     Copyright (C) 2015 Roberto Luiz Souza Monteiro,
 *                        Hernane Borges de Barros Pereira.
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
 * RCS: @(#) $Id: cna.c,v 2.0 2015/12/16 16:04:00 monteiro Exp $
 *
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "matrix.h"
#include "cna.h"

#define ERROR_SIZE 65536
#define GUA_SIZE  65536

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Cna_FloydWarshallShortestPath(Gua_Object *adj, Gua_Object *geodesic, Gua_String error)
 *
 * Description:
 *     Calculate the shortest path between each pair of vertices of the network, using
 *     the Floyd-Warshall Shortest Path algorithm.
 *
 * Arguments:
 *     adj,         the adjacency matrix;
 *     geodesic,    a matrix containing the shortest path between each pair of vertices of the network;
 *     path,        a matrix containing the path between each pair of vertices of the network;
 *     error,       a pointer to the error message.
 *
 * Results:
 *     The function calculates the shortest path between each pair of vertices of the network.
 */
Gua_Status Cna_FloydWarshallShortestPath(Gua_Object *adj, Gua_Object *geodesic, Gua_Object *path, Gua_String error)
{
    Gua_Matrix *madj;
    Gua_Matrix *mgeodesic;
    Gua_Matrix *mpath;
    Gua_Object *ogeodesic;
    Gua_Object *opath;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer l;
    Gua_Integer dim_i;
    Gua_Integer dim_j;
    
    if (Gua_PObjectType(adj) != OBJECT_TYPE_MATRIX) {
        return GUA_ERROR;
    }
    
    madj = (Gua_Matrix *)Gua_PObjectToMatrix(adj);
    
    if (madj) {
        if (!Gua_IsPObjectStored(geodesic)) {
            Gua_FreeObject(geodesic);
        } else {
            Gua_ClearPObject(geodesic);
        }
        if (!Gua_IsPObjectStored(path)) {
            Gua_FreeObject(path);
        } else {
            Gua_ClearPObject(path);
        }
        
        dim_i = madj->dimv[0];
        dim_j = madj->dimv[1];
        
        /* Create matrices zero and one. */
        l = dim_i * dim_j;
        
        /* Create a matrix containing only -1. */
        Gua_MatrixToPObject(path, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        mpath = (Gua_Matrix *)Gua_PObjectToMatrix(path);
        
        mpath->dimc = 2;
        mpath->dimv = Gua_Alloc(mpath->dimc * sizeof(Gua_Integer));
        mpath->dimv[0] = dim_i;
        mpath->dimv[1] = dim_j;
        
        mpath->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        opath = (Gua_Object *)mpath->object;
        
        for (i = 0; i < l; i++) {
            Gua_RealToObject(opath[i], -1.0);
        }
        
        /* Calculate the shortest paths. */
        Gua_CopyMatrix(geodesic, adj, 0);
        
        mgeodesic = (Gua_Matrix *)Gua_PObjectToMatrix(geodesic);
        ogeodesic = (Gua_Object *)mgeodesic->object;
        
        for (i = 1; i < dim_i; i = i + 1) {
            for (j = 1; j < dim_j; j = j + 1) {
                for (k = 1; k < dim_i; k = k + 1) {
                    if (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL) {
                        if (Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + k]) == 0.0) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], 999999999);
                        }
                    } else if (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER) {
                        if (Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + k]) == 0) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], 999999999);
                        }
                    }
                    
                    if (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_REAL) {
                        if (Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) == 0.0) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + i], 999999999);
                        }
                    } else if (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_INTEGER) {
                        if (Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) == 0) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + i], 999999999);
                        }
                    }
                    
                    if (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL) {
                        if (Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k]) == 0.0) {
                            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + k], 999999999);
                        }
                    } else if (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER) {
                        if (Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k]) == 0) {
                            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + k], 999999999);
                        }
                    }
                    
                    if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL)) {
                        if ((Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER)) {
                        if ((Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER)) {
                        if ((Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER)) {
                        if ((Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL)) {
                        if ((Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL)) {
                        if ((Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL)) {
                        if ((Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    } else if ((Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + i]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[j * mgeodesic->dimv[1] + k]) == OBJECT_TYPE_INTEGER)) {
                        if ((Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k])) < Gua_ObjectToInteger(ogeodesic[j * mgeodesic->dimv[1] + k])) {
                            Gua_RealToObject(ogeodesic[j * mgeodesic->dimv[1] + k], Gua_ObjectToReal(ogeodesic[j * mgeodesic->dimv[1] + i]) + Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + k]));
                            Gua_RealToObject(opath[j * mgeodesic->dimv[1] + k], i);
                        }
                    }
                }
            }
        }
        
        for (i = 0; i < dim_i; i = i + 1) {
            Gua_RealToObject(ogeodesic[0 * mgeodesic->dimv[1] + i], 0.0);
            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + 0], 0.0);
            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + i], 0.0);
        }
        
        /* Fix geodesics. */
        for (i = 0; i < dim_i; i = i + 1) {
            for (j = 0; j < dim_j; j = j + 1) {
                if (Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + j]) == 999999999) {
                    Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + j], 0.0);
                }
            }
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Cna_ShortestPath(Gua_Object *adj, Gua_Object *geodesic, Gua_String error)
 *
 * Description:
 *     Calculate the shortest path between each pair of vertices of the network.
 *
 * Arguments:
 *     adj,         the adjacency matrix;
 *     geodesic,    a matrix containing the shortest path between each pair of vertices of the network;
 *     error,       a pointer to the error message.
 *
 * Results:
 *     The function calculates the shortest path between each pair of vertices of the network.
 */
Gua_Status Cna_ShortestPath(Gua_Object *adj, Gua_Object *geodesic, Gua_String error)
{
    Gua_Object one;
    Gua_Object zero;
    Gua_Object old;
    Gua_Object prod;
    Gua_Object path;
    Gua_Matrix *madj;
    Gua_Matrix *mgeodesic;
    Gua_Matrix *mone;
    Gua_Matrix *mzero;
    Gua_Matrix *mprod;
    Gua_Object *oadj;
    Gua_Object *ogeodesic;
    Gua_Object *oone;
    Gua_Object *ozero;
    Gua_Object *oprod;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer l;
    Gua_Integer order;
    Gua_Integer dim_i;
    Gua_Integer dim_j;
    
    Gua_ClearObject(zero);
    Gua_ClearObject(one);
    Gua_ClearObject(old);
    Gua_ClearObject(prod);
    Gua_ClearObject(path);
    
    if (Gua_PObjectType(adj) != OBJECT_TYPE_MATRIX) {
        return GUA_ERROR;
    }
    
    madj = (Gua_Matrix *)Gua_PObjectToMatrix(adj);
    
    if (madj) {
        if (!Gua_IsPObjectStored(geodesic)) {
            Gua_FreeObject(geodesic);
        } else {
            Gua_ClearPObject(geodesic);
        }
        
        dim_i = madj->dimv[0];
        dim_j = madj->dimv[1];
        
        oadj = (Gua_Object *)madj->object;
        
        /* Create matrices zero and one. */
        l = dim_i * dim_j;
        
        /* Create a matrix containing only zeros. */
        Gua_MatrixToObject(zero, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        mzero = (Gua_Matrix *)Gua_ObjectToMatrix(zero);
        
        mzero->dimc = 2;
        mzero->dimv = Gua_Alloc(mzero->dimc * sizeof(Gua_Integer));
        mzero->dimv[0] = dim_i;
        mzero->dimv[1] = dim_j;
        
        mzero->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        ozero = (Gua_Object *)mzero->object;
        
        for (i = 0; i < l; i++) {
            Gua_RealToObject(ozero[i], 0.0);
        }
        
        /* Create a matrix containing only ones. */
        Gua_MatrixToObject(one, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        mone = (Gua_Matrix *)Gua_ObjectToMatrix(one);
        
        mone->dimc = 2;
        mone->dimv = Gua_Alloc(mone->dimc * sizeof(Gua_Integer));
        mone->dimv[0] = dim_i;
        mone->dimv[1] = dim_j;
        
        mone->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        oone = (Gua_Object *)mone->object;
        
        for (i = 0; i < l; i++) {
            Gua_RealToObject(oone[i], 1.0);
        }
        
        /* Remove vertex data from the matrices adj and one. */
        for (i = 0; i < dim_i; i = i + 1) {
            Gua_RealToObject(oadj[0 * madj->dimv[1] + i], 0.0);
            Gua_RealToObject(oadj[i * madj->dimv[1] + 0], 0.0);
            Gua_RealToObject(oone[0 * mone->dimv[1] + i], 0.0);
            Gua_RealToObject(oone[i * mone->dimv[1] + 0], 0.0);
        }
        
        /* Calculate the shortest paths. */
        Gua_CopyMatrix(geodesic, adj, 0);
        
        mgeodesic = (Gua_Matrix *)Gua_PObjectToMatrix(geodesic);
        ogeodesic = (Gua_Object *)mgeodesic->object;
        
        if (Gua_IsMatrixEqual(adj, &zero) || Gua_IsMatrixEqual(adj, &one)) {
            return GUA_OK;
        }
        
        Gua_CopyMatrix(&old, adj, 0);
        Gua_CopyMatrix(&prod, adj, 0);
        
        order = 1;
        
        while (1) {
            Gua_AndOrMatrix(adj, &old, &prod, error);
            Gua_OrMatrix(&old, &prod, &path, error);
            order = order + 1;
            
            mprod = (Gua_Matrix *)Gua_ObjectToMatrix(prod);
            oprod = (Gua_Object *)mprod->object;
            
            for (i = 1; i < dim_i; i = i + 1) {
                for (j = 1; j < dim_j; j = j + 1) {
                    if ((Gua_ObjectType(oprod[i * mprod->dimv[1] + j]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + j]) == OBJECT_TYPE_REAL)) {
                        if ((Gua_ObjectToReal(oprod[i * mprod->dimv[1] + j]) == 1.0) && (Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + j]) == 0.0) && (i != j)) {
                            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + j], order);
                        }
                    } else if ((Gua_ObjectType(oprod[i * mprod->dimv[1] + j]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + j]) == OBJECT_TYPE_INTEGER)) {
                        if ((Gua_ObjectToInteger(oprod[i * mprod->dimv[1] + j]) == 1) && (Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + j]) == 0) && (i != j)) {
                            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + j], order);
                        }
                    } else if ((Gua_ObjectType(oprod[i * mprod->dimv[1] + j]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + j]) == OBJECT_TYPE_INTEGER)) {
                        if ((Gua_ObjectToReal(oprod[i * mprod->dimv[1] + j]) == 1.0) && (Gua_ObjectToInteger(ogeodesic[i * mgeodesic->dimv[1] + j]) == 0) && (i != j)) {
                            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + j], order);
                        }
                    } else if ((Gua_ObjectType(oprod[i * mprod->dimv[1] + j]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(ogeodesic[i * mgeodesic->dimv[1] + j]) == OBJECT_TYPE_REAL)) {
                        if ((Gua_ObjectToInteger(oprod[i * mprod->dimv[1] + j]) == 1) && (Gua_ObjectToReal(ogeodesic[i * mgeodesic->dimv[1] + j]) == 0.0) && (i != j)) {
                            Gua_RealToObject(ogeodesic[i * mgeodesic->dimv[1] + j], order);
                        }
                    }
                }
            }

            if (Gua_IsMatrixEqual(&path, &one)) {
                break;
            } else if (Gua_IsMatrixEqual(&path, &old)) {
                break;
            } else if (order == dim_i) {
                break;
            }
            
            Gua_CopyMatrix(&old, &prod, 0);
        }
        
        /* Free the auxiliary matrices. */
        Gua_FreeObject(&zero);
        Gua_FreeObject(&one);
        Gua_FreeObject(&old);
        Gua_FreeObject(&prod);
        Gua_FreeObject(&path);
    }
    
    if (strlen(error) > 0) {
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Cna_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Functions wrapper.
 *
 * Arguments:
 *     nspace,    a pointer to a structure Gua_Namespace. Must do a cast before use it;
 *     argc,      the number of arguments to pass to the function;
 *     argv,      an array containing the arguments to the function;
 *                argv[0] is the function name;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The return object of the wrapped function.
 */
Gua_Status Cna_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Object path;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    Gua_ClearObject(path);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "cnaFloydWarshallShortestPath") == 0) {
        if ((argc != 2) && (argc != 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        if (Cna_FloydWarshallShortestPath(&argv[1], object, &path, error) != GUA_OK) {
            return GUA_ERROR;
        }
        
        if (argc == 2) {
            if (!Gua_IsObjectStored(path)) {
                Gua_FreeObject(&path);
            }
        } else {
            if (Gua_SetVariable((Gua_Namespace *)nspace, Gua_ObjectToString(argv[2]), &path, SCOPE_STACK) != GUA_OK) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(argv[2]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "cnaShortestPath") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Cna_ShortestPath(&argv[1], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Cna_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install library functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install library functions.
 */
Gua_Status Cna_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Cna_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "cnaFloydWarshallShortestPath", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "cnaFloydWarshallShortestPath");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Cna_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "cnaShortestPath", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "cnaShortestPath");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     CNA_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, CNA_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "CNA_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "CNA_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}
