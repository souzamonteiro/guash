/**
 * File:
 *     matrix.c
 *
 * Package:
 *    Matrix
 *
 * Description:
 *     This file implements the matrix functions for an expression interpreter.
 *
 * Copyright:
 *     Copyright (c) 2017 Roberto Luiz Souza Monteiro, Hernane B. B. Pereira, Marcelo A. Moret.
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
 * RCS: @(#) $Id: matrix.c,v 2.3 2017/01/07 17:49:00 monteiro Exp $
 * 
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "matrix.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Real Matrix_GaussMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
 *
 * Description:
 *     Calculate the matrix b triangular equivalent to a.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix b triangular equivalent to a and
 *     the determinant.
 */
Gua_Real Matrix_GaussMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o2;
    Gua_Real scale;
    Gua_Real det;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer m;
    Gua_Integer n;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return 0;
    }
    
    det = 0;
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if(!Gua_IsPObjectStored(b)) {
            Gua_FreeObject(b);
        } else {
            Gua_ClearPObject(b);
        }
        
        Gua_CopyMatrix(b, a, false);
        
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        o2 = (Gua_Object *)m2->object;
        
        m = m2->dimv[0];
        n = m2->dimv[1];
        
        /* Convert the matrix to real. */
        for (i = 0; i < Gua_PObjectLength(b); i++) {
            if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o2[i]);
            } else {
                if (Gua_ObjectType(o2[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o2[i]);
                    Gua_RealToObject(o2[i], 0.0);
                }
            }
        }
        
        /* Convert to the triangular equivalent matrix. */
        for (j = 0; j < m; j++) {
            if (Gua_ObjectToReal(o2[j * n + j]) != 0.0) {
                for (i = j + 1; i < m; i++) {
                    scale = -Gua_ObjectToReal(o2[i * n + j]) / Gua_ObjectToReal(o2[j * n + j]);
                    for (k = j; k < n; k++) {
                        Gua_RealToObject(o2[i * n + k], (Gua_ObjectToReal(o2[i * n + k]) + scale * Gua_ObjectToReal(o2[j * n + k])));
                    }
                }
            }
        }
        
        det = 1;
        for (i = 0; i < m; i++) {
            det = det * Gua_ObjectToReal(o2[i * n + i]);
        }
        
        if (det == 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrix is singular");
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
    }
    
    return det;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Real Matrix_JordanMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
 *
 * Description:
 *     Calculate the matrix b diagonal equivalent to a.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix b diagonal equivalent to a and
 *     the determinant.
 */
Gua_Real Matrix_JordanMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o2;
    Gua_Real scale;
    Gua_Real det;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_Integer m;
    Gua_Integer n;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return 0;
    }
    
    det = 0;
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if(!Gua_IsPObjectStored(b)) {
            Gua_FreeObject(b);
        } else {
            Gua_ClearPObject(b);
        }
        
        Gua_CopyMatrix(b, a, false);
        
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        o2 = (Gua_Object *)m2->object;
        
        m = m2->dimv[0];
        n = m2->dimv[1];
        
        /* Convert the matrix to real. */
        for (i = 0; i < Gua_PObjectLength(b); i++) {
            if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o2[i]);
            } else {
                if (Gua_ObjectType(o2[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o2[i]);
                    Gua_RealToObject(o2[i], 0.0);
                }
            }
        }
        
        /* Convert to the diagonal equivalent matrix. */
        for (j = 0; j < m; j++) {
            if (Gua_ObjectToReal(o2[j * n + j]) != 0.0) {
                for (i = 0; i < m; i++) {
                    if (i != j) {
                        scale = -Gua_ObjectToReal(o2[i * n + j]) / Gua_ObjectToReal(o2[j * n + j]);
                        for (k = j; k < n; k++) {
                            Gua_RealToObject(o2[i * n + k], (Gua_ObjectToReal(o2[i * n + k]) + scale * Gua_ObjectToReal(o2[j * n + k])));
                        }
                    }
                }
            }
        }
        
        det = 1;
        for (i = 0; i < m; i++) {
            det = det * Gua_ObjectToReal(o2[i * n + i]);
        }
        
        if (det == 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrix is singular");
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
    }
    
    return det;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_DetMatrix(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the matrix determinant.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the matrix determinant.
 */
Gua_Status Matrix_DetMatrix(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Object b;
    Gua_Matrix *m1;
    Gua_String errMessage;
    
    Gua_ClearObject(b);
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        Gua_RealToPObject(object, Matrix_GaussMatrix(a, &b, error));
        
        Gua_FreeObject(&b);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_TransMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
 *
 * Description:
 *     Calculate the transpose of a.
 *
 * Arguments:
 *     a,        a matrix;
 *     b,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the transpose of a
 */
Gua_Status Matrix_TransMatrix(Gua_Object *a, Gua_Object *b, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Integer i;
    Gua_Integer j;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m1) {
        if(!Gua_IsPObjectStored(b)) {
            Gua_FreeObject(b);
        } else {
            Gua_ClearPObject(b);
        }
        
        if (m1->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o1 = (Gua_Object *)m1->object;
        
        Gua_MatrixToPObject(b, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
        
        m2->dimc = m1->dimc;
        m2->dimv = Gua_Alloc(m2->dimc * sizeof(Gua_Integer));
        
        m2->dimv[0] = m1->dimv[1];
        m2->dimv[1] = m1->dimv[0];
        
        m2->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        o2 = (Gua_Object *)m2->object;
        
        for (i = 0; i < m1->dimv[0]; i++) {
            for (j = 0; j < m1->dimv[1]; j++) {
                if (Gua_ObjectType(o1[j * m2->dimv[1] + i]) == OBJECT_TYPE_STRING) {
                    Gua_ByteArrayToObject(o2[j * m2->dimv[1] + i], Gua_ObjectToString(o1[i * m1->dimv[1] + j]), Gua_ObjectLength(o1[i * m1->dimv[1] + j]));
                } else {
                    Gua_LinkObjects(o2[j * m2->dimv[1] + i], o1[i * m1->dimv[1] + j]);
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
 *     Gua_Status Matrix_Cross(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Calculate the cross product of two vectors a and b.
 *
 * Arguments:
 *     a,        vector a(an one row matrix);
 *     b,        vector b(an one row matrix);
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the cross product of two vectors a and b.
 */
Gua_Status Matrix_Cross(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Matrix *m3;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object *v3;
    Gua_Object u;
    Gua_Object v;
    Gua_Integer i;
    Gua_String errMessage;
    
    Gua_ClearObject(u);
    Gua_ClearObject(v);

    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_PObjectType(b) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument 2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);

    if (m1 && m2) {
        if(!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (m1->dimc != m2->dimc) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimv[0] != m2->dimv[0]) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimv[1] != m2->dimv[1]) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m2->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((m1->dimv[0] != 1) || (m2->dimv[0] != 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only 1x2 or 1x3 matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((m1->dimv[1] < 2) || (m1->dimv[1] > 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only 1x2 or 1x3 matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((m2->dimv[1] < 2) || (m2->dimv[1] > 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only 1x2 or 1x3 matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        /* We will work with copies, to not change the original matrices. */
        Gua_CopyMatrix(&u, a, false);
        Gua_CopyMatrix(&v, b, false);
        
        m1 = (Gua_Matrix *)Gua_ObjectToMatrix(u);
        o1 = (Gua_Object *)m1->object;
        
        m2 = (Gua_Matrix *)Gua_ObjectToMatrix(v);
        o2 = (Gua_Object *)m2->object;
        
        /* Convert the matrices to real. */
        for (i = 0; i < Gua_ObjectLength(u); i++) {
            if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o1[i]);
            } else {
                if (Gua_ObjectType(o1[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o1[i]);
                    Gua_RealToObject(o1[i], 0.0);
                }
            }
        }
        for (i = 0; i < Gua_ObjectLength(v); i++) {
            if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o2[i]);
            } else {
                if (Gua_ObjectType(o2[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o2[i]);
                    Gua_RealToObject(o2[i], 0.0);
                }
            }
        }
        
        
        /* Create the result matrix. */
        Gua_MatrixToPObject(c, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), Gua_PObjectLength(a));
        m3 = (Gua_Matrix *)Gua_PObjectToMatrix(c);
        
        m3->dimc = m1->dimc;
        m3->dimv = Gua_Alloc(m1->dimc * sizeof(long));
        
        for (i = 0; i < m1->dimc; i++) {
            m3->dimv[i] = m1->dimv[i];
        }
        
        m3->object = (struct Gua_Object *)Gua_Alloc(Gua_PObjectLength(a) * sizeof(Gua_Object));
        v3 = (Gua_Object *)m3->object;
        
        /* This release only supports real numbers. */
        if (m1->dimv[1] == 2) {
            Gua_RealToObject(v3[0], (Gua_ObjectToReal(o1[0]) * Gua_ObjectToReal(o2[1])));
            Gua_RealToObject(v3[1], (-Gua_ObjectToReal(o1[1]) * Gua_ObjectToReal(o2[0])));
        } else {
            Gua_RealToObject(v3[0], (Gua_ObjectToReal(o1[1]) * Gua_ObjectToReal(o2[2]) - Gua_ObjectToReal(o2[1]) * Gua_ObjectToReal(o1[2])));
            Gua_RealToObject(v3[1], (Gua_ObjectToReal(o1[2]) * Gua_ObjectToReal(o2[0]) - Gua_ObjectToReal(o2[2]) * Gua_ObjectToReal(o1[0])));
            Gua_RealToObject(v3[2], (Gua_ObjectToReal(o1[0]) * Gua_ObjectToReal(o2[1]) - Gua_ObjectToReal(o2[0]) * Gua_ObjectToReal(o1[1])));
        }
                
        Gua_FreeObject(&u);
        Gua_FreeObject(&v);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Dot(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Calculate the dot product of two vectors a and b.
 *
 * Arguments:
 *     a,        vector a(an one row matrix);
 *     b,        vector b(an one row matrix);
 *     c,        a structure containing the return object of the function;
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns the dot product of two vectors a and b.
 */
Gua_Status Matrix_Dot(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Matrix *m1;
    Gua_Matrix *m2;
    Gua_Object *o1;
    Gua_Object *o2;
    Gua_Object u;
    Gua_Object v;
    Gua_Integer i;
    Gua_String errMessage;
    
    Gua_ClearObject(u);
    Gua_ClearObject(v);

    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_PObjectType(b) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument 2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m1 = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    m2 = (Gua_Matrix *)Gua_PObjectToMatrix(b);
    
    if (m1 && m2) {
        if(!Gua_IsPObjectStored(c)) {
            Gua_FreeObject(c);
        } else {
            Gua_ClearPObject(c);
        }
        
        if (m1->dimc != m2->dimc) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimv[0] != m2->dimv[0]) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimv[1] != m2->dimv[1]) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "the matrices do not have compatible dimensions");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m1->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (m2->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((m1->dimv[0] != 1) || (m2->dimv[0] != 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only 1x2 or 1x3 matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((m1->dimv[1] < 2) || (m1->dimv[1] > 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only 1x2 or 1x3 matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((m2->dimv[1] < 2) || (m2->dimv[1] > 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only 1x2 or 1x3 matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        /* We will work with copies, to not change the original matrices. */
        Gua_CopyMatrix(&u, a, false);
        Gua_CopyMatrix(&v, b, false);
        
        m1 = (Gua_Matrix *)Gua_ObjectToMatrix(u);
        o1 = (Gua_Object *)m1->object;
        
        m2 = (Gua_Matrix *)Gua_ObjectToMatrix(v);
        o2 = (Gua_Object *)m2->object;
        
        /* Convert the matrices to real. */
        for (i = 0; i < Gua_ObjectLength(u); i++) {
            if (Gua_ObjectType(o1[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o1[i]);
            } else {
                if (Gua_ObjectType(o1[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o1[i]);
                    Gua_RealToObject(o1[i], 0.0);
                }
            }
        }
        for (i = 0; i < Gua_ObjectLength(v); i++) {
            if (Gua_ObjectType(o2[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(o2[i]);
            } else {
                if (Gua_ObjectType(o2[i]) != OBJECT_TYPE_REAL) {
                    Gua_FreeObject(&o2[i]);
                    Gua_RealToObject(o2[i], 0.0);
                }
            }
        }
        
        /* This release only supports real numbers. */
        if (m1->dimv[1] == 2) {
            Gua_RealToPObject(c, (Gua_ObjectToReal(o1[0]) * Gua_ObjectToReal(o2[0]) + Gua_ObjectToReal(o1[1]) * Gua_ObjectToReal(o2[1])));
        } else {
            Gua_RealToPObject(c, (Gua_ObjectToReal(o1[0]) * Gua_ObjectToReal(o2[0]) + Gua_ObjectToReal(o1[1]) * Gua_ObjectToReal(o2[1]) + Gua_ObjectToReal(o1[2]) * Gua_ObjectToReal(o2[2])));
        }
        
        Gua_FreeObject(&u);
        Gua_FreeObject(&v);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Sum(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the sum of all cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the sum of all cells in the matrix.
 */
Gua_Status Matrix_Sum(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Real sum;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        o = (Gua_Object *)m->object;
        
        length = Gua_PObjectLength(a);

        sum = 0;

        for (i = 0; i < length; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                sum = sum + Gua_ObjectToInteger(o[i]);
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                sum = sum + Gua_ObjectToReal(o[i]);
            }
        }

        Gua_RealToPObject(object, sum);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Sum2(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the squared sum of all cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the squared sum of all cells in the matrix.
 */
Gua_Status Matrix_Sum2(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Real sum;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        o = (Gua_Object *)m->object;
        
        length = Gua_PObjectLength(a);

        sum = 0;

        for (i = 0; i < length; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                sum = sum + Gua_ObjectToInteger(o[i]) * Gua_ObjectToInteger(o[i]);
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                sum = sum + Gua_ObjectToReal(o[i]) * Gua_ObjectToReal(o[i]);
            }
        }

        Gua_RealToPObject(object, sum);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_SumCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the sum of specified cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     x1,        row of the top left corner;
 *     y1,        column of the top left corner;
 *     x2,        row of the bottom right corner;
 *     y2,        column of the bottom right corner;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the sum of specified cells in the matrix.
 */
Gua_Status Matrix_SumCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Real sum;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (m->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x2) > (m->dimv[0] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y2) > (m->dimv[1] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o = (Gua_Object *)m->object;
        
        sum = 0;

        for (i = Gua_ObjectToInteger(x1); i <= Gua_ObjectToInteger(x2); i++) {
            for (j = Gua_ObjectToInteger(y1); j <= Gua_ObjectToInteger(y2); j++) {
                if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
                    sum = sum + Gua_ObjectToInteger(o[i * m->dimv[1] + j]);
                } else if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_REAL) {
                    sum = sum + Gua_ObjectToReal(o[i * m->dimv[1] + j]);
                }
            }
        }

        Gua_RealToPObject(object, sum);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Avg(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the average of all cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the average of all cells in the matrix.
 */
Gua_Status Matrix_Avg(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Real sum;
    Gua_Real avg;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        o = (Gua_Object *)m->object;
        
        length = Gua_PObjectLength(a);

        sum = 0;

        for (i = 0; i < length; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                sum = sum + Gua_ObjectToInteger(o[i]);
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                sum = sum + Gua_ObjectToReal(o[i]);
            }
        }

        avg = sum / length;

        Gua_RealToPObject(object, avg);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_AvgCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the average of specified cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     x1,        row of the top left corner;
 *     y1,        column of the top left corner;
 *     x2,        row of the bottom right corner;
 *     y2,        column of the bottom right corner;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the average of specified cells in the matrix.
 */
Gua_Status Matrix_AvgCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Real sum;
    Gua_Real avg;
    Gua_Integer n;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (m->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x2) > (m->dimv[0] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y2) > (m->dimv[1] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o = (Gua_Object *)m->object;
        
        sum = 0;
        n = 0;

        for (i = Gua_ObjectToInteger(x1); i <= Gua_ObjectToInteger(x2); i++) {
            for (j = Gua_ObjectToInteger(y1); j <= Gua_ObjectToInteger(y2); j++) {
                if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
                    sum = sum + Gua_ObjectToInteger(o[i * m->dimv[1] + j]);
                    n++;
                } else if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_REAL) {
                    sum = sum + Gua_ObjectToReal(o[i * m->dimv[1] + j]);
                    n++;
                }
            }
        }

        avg = sum / n;

        Gua_RealToPObject(object, avg);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Count(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the number of all cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the number of all cells in the matrix.
 */
Gua_Status Matrix_Count(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Integer count;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        o = (Gua_Object *)m->object;
        
        length = Gua_PObjectLength(a);
        
        count = 0;

        for (i = 0; i < length; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
				if (Gua_ObjectToInteger(o[i]) != 0) {
                    count = count + 1;
				}
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
				if (Gua_ObjectToReal(o[i]) != 0) {
                    count = count + 1;
				}
            } else {
				if (Gua_ObjectType(o[i]) != OBJECT_TYPE_UNKNOWN) {
					count = count + 1;
				}
			}
        }

        Gua_IntegerToPObject(object, count);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_CountCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the number of specified cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     x1,        row of the top left corner;
 *     y1,        column of the top left corner;
 *     x2,        row of the bottom right corner;
 *     y2,        column of the bottom right corner;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the number of specified cells in the matrix.
 */
Gua_Status Matrix_CountCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer count;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (m->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x2) > (m->dimv[0] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y2) > (m->dimv[1] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o = (Gua_Object *)m->object;
        
        count = 0;

        for (i = Gua_ObjectToInteger(x1); i <= Gua_ObjectToInteger(x2); i++) {
            for (j = Gua_ObjectToInteger(y1); j <= Gua_ObjectToInteger(y2); j++) {
				if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
					if (Gua_ObjectToInteger(o[i * m->dimv[1] + j]) != 0) {
	                    count = count + 1;
					}
	            } else if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_REAL) {
					if (Gua_ObjectToReal(o[i * m->dimv[1] + j]) != 0) {
	                    count = count + 1;
					}
	            } else {
					if (Gua_ObjectType(o[i * m->dimv[1] + j]) != OBJECT_TYPE_UNKNOWN) {
						count = count + 1;
					}
				}
            }
        }

        Gua_IntegerToPObject(object, count);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_DelRow(Gua_Object *source, Gua_Integer n, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Copy a matrix to other deleting the specified row.
 *
 * Arguments:
 *     source,    the source matrix;
 *     n,         the row number;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function makes a copy of a matrix to other deleting the specified row.
 */
Gua_Status Matrix_DelRow(Gua_Object *source, Gua_Object n, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *mt;
    Gua_Matrix *ms;
    Gua_Object *ot;
    Gua_Object *os;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_String errMessage;
    
    if (Gua_PObjectType(source) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_ObjectType(n) != OBJECT_TYPE_INTEGER) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    ms = (Gua_Matrix *)Gua_PObjectToMatrix(source);
    
    if (ms) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (ms->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if ((Gua_ObjectToInteger(n) < 0) || (Gua_ObjectToInteger(n) > (ms->dimv[0] - 1))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        os = (Gua_Object *)ms->object;
        
        mt = (Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix));
        
        mt->dimc = ms->dimc;
        mt->dimv = Gua_Alloc(mt->dimc * sizeof(Gua_Integer));
        
        mt->dimv[0] = ms->dimv[0] - 1;
        mt->dimv[1] = ms->dimv[1];
        
        mt->object = (struct Gua_Object *)Gua_Alloc((Gua_PObjectLength(source) - ms->dimv[1]) * sizeof(Gua_Object));
        ot = (Gua_Object *)mt->object;
        
        k = 0;
        for (i = 0; i < ms->dimv[0]; i++) {
            if (i != Gua_ObjectToInteger(n)) {
                for (j = 0; j < ms->dimv[1]; j++) {
                    if (Gua_ObjectType(os[i * ms->dimv[1] + j]) == OBJECT_TYPE_STRING) {
                        Gua_ByteArrayToObject(ot[k * mt->dimv[1] + j], Gua_ObjectToString(os[i * ms->dimv[1] + j]), Gua_ObjectLength(os[i * ms->dimv[1] + j]));
                    } else {
                        Gua_LinkObjects(ot[k * mt->dimv[1] + j], os[i * ms->dimv[1] + j]);
                    }
                }
                k++;
            }
        }
        
        /* Fix the target matrix fields. */
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)mt, (Gua_PObjectLength(source) - ms->dimv[1]));
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_DelCol(Gua_Object *source, Gua_Integer n, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Copy a matrix to other deleting the specified Column.
 *
 * Arguments:
 *     source,    the source matrix;
 *     n,         the column number;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function makes a copy of a matrix to other deleting the specified Column.
 */
Gua_Status Matrix_DelCol(Gua_Object *source, Gua_Object n, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *mt;
    Gua_Matrix *ms;
    Gua_Object *ot;
    Gua_Object *os;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer k;
    Gua_String errMessage;
    
    if (Gua_PObjectType(source) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_ObjectType(n) != OBJECT_TYPE_INTEGER) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    ms = (Gua_Matrix *)Gua_PObjectToMatrix(source);
    
    if (ms) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (ms->dimc > 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if ((Gua_ObjectToInteger(n) < 0) || (Gua_ObjectToInteger(n) > (ms->dimv[1] - 1))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        os = (Gua_Object *)ms->object;
        
        mt = (Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix));
        
        mt->dimc = ms->dimc;
        mt->dimv = Gua_Alloc(mt->dimc * sizeof(Gua_Integer));
        
        mt->dimv[0] = ms->dimv[0];
        mt->dimv[1] = ms->dimv[1] - 1;
        
        mt->object = (struct Gua_Object *)Gua_Alloc((Gua_PObjectLength(source) - ms->dimv[0]) * sizeof(Gua_Object));
        ot = (Gua_Object *)mt->object;
        
        k = 0;
        for (j = 0; j < ms->dimv[1]; j++) {
            if (j != Gua_ObjectToInteger(n)) {
                for (i = 0; i < ms->dimv[0]; i++) {
                    if (Gua_ObjectType(os[i * ms->dimv[1] + j]) == OBJECT_TYPE_STRING) {
                        Gua_ByteArrayToObject(ot[i * mt->dimv[1] + k], Gua_ObjectToString(os[i * ms->dimv[1] + j]), Gua_ObjectLength(os[i * ms->dimv[1] + j]));
                    } else {
                        Gua_LinkObjects(ot[i * mt->dimv[1] + k], os[i * ms->dimv[1] + j]);
                    }
                }
                k++;
            }
        }
        
        /* Fix the target matrix fields. */
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)mt, (Gua_PObjectLength(source) - ms->dimv[0]));
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Min(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the min object of all cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the min object of all cells in the matrix.
 */
Gua_Status Matrix_Min(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Real min;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        o = (Gua_Object *)m->object;
        
        length = Gua_PObjectLength(a);
        
        min = 0;
        
        for (i = 0; i < length; i++) {
            if (i == 0) {
                if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                    min = Gua_ObjectToInteger(o[i]);
                } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                    min = Gua_ObjectToReal(o[i]);
                }
            } else {
                if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                    if (min > Gua_ObjectToInteger(o[i])) {
                        min = Gua_ObjectToInteger(o[i]);
                    }
                } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                    if (min > Gua_ObjectToReal(o[i])) {
                        min = Gua_ObjectToReal(o[i]);
                    }
                }
            }
        }
        
        Gua_RealToPObject(object, min);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_MinCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the min of specified cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     x1,        row of the top left corner;
 *     y1,        column of the top left corner;
 *     x2,        row of the bottom right corner;
 *     y2,        column of the bottom right corner;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the min of specified cells in the matrix.
 */
Gua_Status Matrix_MinCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Real min;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (m->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x2) > (m->dimv[0] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y2) > (m->dimv[1] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o = (Gua_Object *)m->object;
        
        min = 0;
        
        for (i = Gua_ObjectToInteger(x1); i <= Gua_ObjectToInteger(x2); i++) {
            for (j = Gua_ObjectToInteger(y1); j <= Gua_ObjectToInteger(y2); j++) {
                if ((i == Gua_ObjectToInteger(x1)) && (j == Gua_ObjectToInteger(y2))) {
                    if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
                        min = Gua_ObjectToInteger(o[i * m->dimv[1] + j]);
                    } else if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_REAL) {
                        min = Gua_ObjectToReal(o[i * m->dimv[1] + j]);
                    }
                } else {
                    if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
                        if (min > Gua_ObjectToInteger(o[i * m->dimv[1] + j])) {
                            min = Gua_ObjectToInteger(o[i * m->dimv[1] + j]);
                        }
                    } else if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_REAL) {
                        if (min > Gua_ObjectToReal(o[i * m->dimv[1] + j])) {
                            min = Gua_ObjectToReal(o[i * m->dimv[1] + j]);
                        }
                    }
                }
            }
        }
        
        Gua_RealToPObject(object, min);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Max(Gua_Object *a, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the max object of all cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the max object of all cells in the matrix.
 */
Gua_Status Matrix_Max(Gua_Object *a, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Real max;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }
        
        o = (Gua_Object *)m->object;
        
        length = Gua_PObjectLength(a);

        max = 0;

        for (i = 0; i < length; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                if (max < Gua_ObjectToInteger(o[i])) {
                    max = Gua_ObjectToInteger(o[i]);
                }
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                if (max < Gua_ObjectToReal(o[i])) {
                    max = Gua_ObjectToReal(o[i]);
                }
            }
        }

        Gua_RealToPObject(object, max);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_MaxCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Calculate the max of specified cells in the matrix.
 *
 * Arguments:
 *     a,         a matrix;
 *     x1,        row of the top left corner;
 *     y1,        column of the top left corner;
 *     x2,        row of the bottom right corner;
 *     y2,        column of the bottom right corner;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function returns the max of specified cells in the matrix.
 */
Gua_Status Matrix_MaxCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Real max;
    Gua_String errMessage;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_MATRIX) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s...\n", "illegal argument");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    m = (Gua_Matrix *)Gua_PObjectToMatrix(a);
    
    if (m) {
        if(!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        } else {
            Gua_ClearPObject(object);
        }

        if (m->dimc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "only bidimensional matrices are supported");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y1) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(x2) > (m->dimv[0] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectToInteger(y2) > (m->dimv[1] - 1)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s...\n", "index out of bound");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        o = (Gua_Object *)m->object;
        
        max = 0;
        
        for (i = Gua_ObjectToInteger(x1); i <= Gua_ObjectToInteger(x2); i++) {
            for (j = Gua_ObjectToInteger(y1); j <= Gua_ObjectToInteger(y2); j++) {
                if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_INTEGER) {
                    if (max < Gua_ObjectToInteger(o[i * m->dimv[1] + j])) {
                        max = Gua_ObjectToInteger(o[i * m->dimv[1] + j]);
                    }
                } else if (Gua_ObjectType(o[i * m->dimv[1] + j]) == OBJECT_TYPE_REAL) {
                    if (max < Gua_ObjectToReal(o[i * m->dimv[1] + j])) {
                        max = Gua_ObjectToReal(o[i * m->dimv[1] + j]);
                    }
                }
            }
        }
        
        Gua_RealToPObject(object, max);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_MatrixFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Matrix functions wrapper.
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
 *     The return object of the wrapped funcion.
 */
Gua_Status Matrix_MatrixFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_Length length;
    Gua_Integer i;
    Gua_Integer j;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);

    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (strcmp(Gua_ObjectToString(argv[0]), "avg") == 0) {
        if (argc != 2) {
            if (argc != 6) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 6) {
            if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }

        if (argc == 2) {
            if (Matrix_Avg(&argv[1], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        } else if (argc == 6) {
            if (Matrix_AvgCells(&argv[1], argv[2], argv[3], argv[4], argv[5], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "count") == 0) {
        if (argc != 2) {
            if (argc != 6) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 6) {
            if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }

        if (argc == 2) {
            if (Matrix_Count(&argv[1], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        } else if (argc == 6) {
            if (Matrix_CountCells(&argv[1], argv[2], argv[3], argv[4], argv[5], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "cross") == 0) {
        if (argc != 3) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Matrix_Cross(&argv[1], &argv[2], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "delcol") == 0) {
        if (argc != 3) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Matrix_DelCol(&argv[1], argv[2], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "delrow") == 0) {
        if (argc != 3) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Matrix_DelRow(&argv[1], argv[2], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "det") == 0) {
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
        
        if (Matrix_DetMatrix(&argv[1], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "diag") == 0) {
        if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        length = Gua_ObjectToInteger(argv[1]) * Gua_ObjectToInteger(argv[2]);
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), length);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 2;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = Gua_ObjectToInteger(argv[1]);
        m->dimv[1] = Gua_ObjectToInteger(argv[2]);
        
        if (length == 0) {
            Gua_FreeObject(object);
            Gua_Free(m->dimv);
            return GUA_OK;
        }
        
        m->object = (struct Gua_Object *)Gua_Alloc(length * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        for (i = 0; i < m->dimv[0]; i++) {
            for (j = 0; j < m->dimv[1]; j++) {
                if (i == j) {
                    if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                        Gua_IntegerToObject(o[i * m->dimv[1] + j], Gua_ObjectToInteger(argv[3]));
                    } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
                        Gua_RealToObject(o[i * m->dimv[1] + j], Gua_ObjectToReal(argv[3]));
                    } else {
                        Gua_ComplexToObject(o[i * m->dimv[1] + j], Gua_ObjectToReal(argv[3]), Gua_ObjectToImaginary(argv[3]));
                    }
                } else {
                    if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                        Gua_IntegerToObject(o[i * m->dimv[1] + j], 0);
                    } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
                        Gua_RealToObject(o[i * m->dimv[1] + j], 0.0);
                    } else {
                        Gua_RealToObject(o[i * m->dimv[1] + j], 0.0);
                    }
                }
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "dot") == 0) {
        if (argc != 3) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Matrix_Dot(&argv[1], &argv[2], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "gauss") == 0) {
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
        
        if (Matrix_GaussMatrix(&argv[1], object, error) == 0) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "jordan") == 0) {
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
        
        if (Matrix_JordanMatrix(&argv[1], object, error) == 0) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "max") == 0) {
        if (argc != 2) {
            if (argc != 6) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 6) {
            if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        if (argc == 2) {
            if (Matrix_Max(&argv[1], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        } else if (argc == 6) {
            if (Matrix_MaxCells(&argv[1], argv[2], argv[3], argv[4], argv[5], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "min") == 0) {
        if (argc != 2) {
            if (argc != 6) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 6) {
            if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }

        if (argc == 2) {
            if (Matrix_Min(&argv[1], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        } else if (argc == 6) {
            if (Matrix_MinCells(&argv[1], argv[2], argv[3], argv[4], argv[5], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "one") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        length = Gua_ObjectToInteger(argv[1]) * Gua_ObjectToInteger(argv[2]);
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), length);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 2;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = Gua_ObjectToInteger(argv[1]);
        m->dimv[1] = Gua_ObjectToInteger(argv[2]);
        
        if (length == 0) {
            Gua_FreeObject(object);
            Gua_Free(m->dimv);
            return GUA_OK;
        }
        
        m->object = (struct Gua_Object *)Gua_Alloc(length * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        for (i = 0; i < length; i++) {
            Gua_RealToObject(o[i], 1.0);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "rand") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        length = Gua_ObjectToInteger(argv[1]) * Gua_ObjectToInteger(argv[2]);
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), length);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 2;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = Gua_ObjectToInteger(argv[1]);
        m->dimv[1] = Gua_ObjectToInteger(argv[2]);
        
        if (length == 0) {
            Gua_FreeObject(object);
            Gua_Free(m->dimv);
            return GUA_OK;
        }
        
        m->object = (struct Gua_Object *)Gua_Alloc(length * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        for (i = 0; i < length; i++) {
#ifdef _WINDOWS_
            Gua_RealToObject(o[i], (1.0 * (rand() / (RAND_MAX + 1.0))));
#else
            Gua_RealToObject(o[i], (1.0 * (random() / (RAND_MAX + 1.0))));
#endif
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sum") == 0) {
        if (argc != 2) {
            if (argc != 6) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 6) {
            if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }

        if (argc == 2) {
            if (Matrix_Sum(&argv[1], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        } else if (argc == 6) {
            if (Matrix_SumCells(&argv[1], argv[2], argv[3], argv[4], argv[5], object, error) != GUA_OK) {
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sum2") == 0) {
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
        
        if (Matrix_Sum2(&argv[1], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "trans") == 0) {
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
        
        if (Matrix_TransMatrix(&argv[1], object, error) != GUA_OK) {
            return GUA_ERROR;
        }    
    } else if (strcmp(Gua_ObjectToString(argv[0]), "zero") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        length = Gua_ObjectToInteger(argv[1]) * Gua_ObjectToInteger(argv[2]);
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), length);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 2;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = Gua_ObjectToInteger(argv[1]);
        m->dimv[1] = Gua_ObjectToInteger(argv[2]);
        
        if (length == 0) {
            Gua_FreeObject(object);
            Gua_Free(m->dimv);
            return GUA_OK;
        }
        
        m->object = (struct Gua_Object *)Gua_Alloc(length * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        for (i = 0; i < length; i++) {
            Gua_RealToObject(o[i], 0.0);
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Matrix_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the matrix functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the matrix functions.
 */
Gua_Status Matrix_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Matrix_MatrixFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "avg", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "avg");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "count", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "count");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "cross", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "cross");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "delcol", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "delcol");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "delrow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "delrow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "det", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "det");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "diag", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "diag");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "dot", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "dot");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gauss", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gauss");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "jordan", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "jordan");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "max", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "max");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "min", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "min");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "one", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "one");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "rand", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "rand");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sum", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sum");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sum2", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sum2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "trans", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "trans");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "zero", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "zero");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     MATRIX_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, MATRIX_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "MATRIX_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "MATRIX_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

