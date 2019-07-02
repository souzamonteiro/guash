/**
 * File:
 *     lmtx.c
 *
 * Package:
 *     Lmtx
 *
 * Description:
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
 * RCS: @(#) $Id: lmtx.gua,v 2.0 2017/01/10 13:08:00 monteiro Exp $
 *
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "lmtx.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Lmtx_Learn(Gua_Object *nn, Gua_Object *in, Gua_Object *out, Gua_Integer ni, Gua_Integer no, Gua_Real lrate, Gua_Integer af, Gua_Integer oaf)
 *
 * Description:
 *     Teach an artificial neural network.
 *
 * Arguments:
 *     nn,       the learning matrix;
 *     in,       input training set;
 *     out,      input training set;
 *     ni,       number of input neurons;
 *     no,       number of input neurons;
 *     lrate,    learning rate;
 *     af,       activation function;
 *     oaf,      output layer activation function.
 *
 * Results:
 *     The function returns the ANN after one step of one epoch.
 */
Gua_Status Lmtx_Learn(Gua_Object *nn, Gua_Object *in, Gua_Object *out, Gua_Integer ni, Gua_Integer no, Gua_Real lrate, Gua_Integer af, Gua_Integer oaf)
{
    Gua_Matrix *mnn;
    Gua_Matrix *min;
    Gua_Matrix *mout;
    Gua_Object *onn;
    Gua_Object *oin;
    Gua_Object *oout;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer dim_i;
    Gua_Integer dim_j;
    Gua_Integer first_out;
    
    if (!((Gua_PObjectType(nn) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(in) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(out) == OBJECT_TYPE_MATRIX))) {
        return GUA_ERROR;
    }
    
    mnn = (Gua_Matrix *)Gua_PObjectToMatrix(nn);
    min = (Gua_Matrix *)Gua_PObjectToMatrix(in);
    mout = (Gua_Matrix *)Gua_PObjectToMatrix(out);
    
    if (mnn && min && mout) {
        dim_i = mnn->dimv[0];
        dim_j = mnn->dimv[1];
        
        onn = (Gua_Object *)mnn->object;
        oin = (Gua_Object *)min->object;
        oout = (Gua_Object *)mout->object;
        
        /* Linear matrix cell access: k = i * dim_j + j */
        first_out = dim_j - 1 - no;
        
        /* Convert integer values to real. */
        for (i = 0; i < Gua_PObjectLength(nn); i++) {
            if (Gua_ObjectType(onn[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(onn[i]);
            }
        }
        for (i = 0; i < Gua_PObjectLength(in); i++) {
            if (Gua_ObjectType(oin[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(oin[i]);
            }
        }
        for (i = 0; i < Gua_PObjectLength(out); i++) {
            if (Gua_ObjectType(oout[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(oout[i]);
            }
        }
        
        /* Clear inputs and outputs. */
        for (i = 0; i < dim_i - 1; i++) {
            if (Gua_ObjectType(onn[0 * dim_j + i]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[0 * dim_j + i]));
            }
            Gua_RealToObject(onn[0 * dim_j + i], 0.0);
            
            if (Gua_ObjectType(onn[i * dim_j + 0]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[i * dim_j + 0]));
            }
            Gua_RealToObject(onn[i * dim_j + 0], 0.0);
            
            if (Gua_ObjectType(onn[i * dim_j + (dim_j - 1)]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[i * dim_j + (dim_j - 1)]));
            }
            Gua_RealToObject(onn[i * dim_j + (dim_j - 1)], 0.0);
            
            if (Gua_ObjectType(onn[(dim_i - 1) * dim_j + i]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[(dim_i - 1) * dim_j + i]));
            }
            Gua_RealToObject(onn[(dim_i - 1) * dim_j + i], 0.0);
        }
        
        /* Assign inputs. */
        for (j = 0; j < ni; j++) {
            Gua_RealToObject(onn[(j + 1) * dim_j + 0], Gua_ObjectToReal(oin[j]));
        }
        
        /* Calculate the neurons output. */
        for (j = ni + 1; j < (dim_j - 1); j++) {
            Gua_RealToObject(onn[0 * dim_j + j], 0.0);
            
            /* Weighted sums.
               x = x1 * w1 + x2 * w2 + ... */
            for (i = 1; i < (dim_i - 1 - no); i++) {
                if (i < j) {
                    if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                        Gua_RealToObject(onn[0 * dim_j + j], Gua_ObjectToReal(onn[0 * dim_j + j]) + Gua_ObjectToReal(onn[i * dim_j + j]) * (Gua_ObjectToReal(onn[i * dim_j + 0]) * 1.0));
                    }
                } else if (i == j) {
                    if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                        Gua_RealToObject(onn[0 * dim_j + j], Gua_ObjectToReal(onn[0 * dim_j + j]) + Gua_ObjectToReal(onn[i * dim_j + j]) * 1.0);
                    }
                } else {
                    break;
                }
            }
            
            /* Activation function. */
            if (j < first_out) {
                /* Linear: f(x) = x
                           df(x)/dx = 1 */
                if (af == LMTX_LINEAR_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], Gua_ObjectToReal(onn[0 * dim_j + j]));
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], 1.0);
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x))
                             df(x)/dx = f(x) * (1 - f(x)) */
                } else if (af == LMTX_LOGISTIC_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], Gua_ObjectToReal(onn[j * dim_j + 0]) * (1.0 - Gua_ObjectToReal(onn[j * dim_j + 0])));
                /* Hyperbolic tangent: f(x) = 2 / (1 + e^(-2x)) - 1
                                       df(x)/dx = 1 - f(x)^2 */
                } else if (af == LMTX_TANH_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 2.0 / (1.0 + exp(-2.0 * Gua_ObjectToReal(onn[0 * dim_j + j]))) - 1.0);
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], 1.0 - Gua_ObjectToReal(onn[j * dim_j + 0]) * Gua_ObjectToReal(onn[j * dim_j + 0]));
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x))
                             df(x)/dx = f(x) * (1 - f(x)) */
                } else {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], Gua_ObjectToReal(onn[j * dim_j + 0]) * (1.0 - Gua_ObjectToReal(onn[j * dim_j + 0])));
                }
            /* Output layer activation function. */
            } else {
                /* Linear: f(x) = x
                           df(x)/dx = 1 */
                if (oaf == LMTX_LINEAR_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], Gua_ObjectToReal(onn[0 * dim_j + j]));
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], 1.0);
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x))
                             df(x)/dx = f(x) * (1 - f(x)) */
                } else if (oaf == LMTX_LOGISTIC_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], Gua_ObjectToReal(onn[j * dim_j + 0]) * (1.0 - Gua_ObjectToReal(onn[j * dim_j + 0])));
                /* Hyperbolic tangent: f(x) = 2 / (1 + e^(-2x)) - 1
                                       df(x)/dx = 1 - f(x)^2 */
                } else if (oaf == LMTX_TANH_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 2.0 / (1.0 + exp(-2.0 * Gua_ObjectToReal(onn[0 * dim_j + j]))) - 1.0);
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], 1.0 - Gua_ObjectToReal(onn[j * dim_j + 0]) * Gua_ObjectToReal(onn[j * dim_j + 0]));
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x))
                             df(x)/dx = f(x) * (1 - f(x)) */
                } else {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                    /* Calculate df(x)/dx for backpropagation. */
                    Gua_RealToObject(onn[j * dim_j + (dim_j - 1)], Gua_ObjectToReal(onn[j * dim_j + 0]) * (1.0 - Gua_ObjectToReal(onn[j * dim_j + 0])));
                }
            }
        }
        
        /* Calculate delta for the output neurons.
           d = z - y */
        for (i = 0; i < no; i++) {
            Gua_RealToObject(onn[(dim_i - 1) * dim_j + (first_out + i)], Gua_ObjectToReal(oout[i]) - Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]));
        }
        
        /* Calculate delta for hidden neurons.
           d1 = w1 * d2 + w2 * d2 + ... */
        for (j = dim_j - 2; j > ni; j--) {
            for (i = ni + 1; i < (dim_i - 1 - no); i++) {
                if (i == j) {
                    break;
                }
                
                if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                    Gua_RealToObject(onn[(dim_i - 1) * dim_j + i], Gua_ObjectToReal(onn[(dim_i - 1) * dim_j + i]) + Gua_ObjectToReal(onn[i * dim_j + j]) * Gua_ObjectToReal(onn[(dim_i - 1) * dim_j + j]));
                }
            }
        }
        
        /* Adjust weights.
           x = x1 * w1 + x2 * w2 + ...
           w1 = w1 + n * d * df(x)/dx * x1
           w2 = w2 + n * d * df(x)/dx * x2 */
        for (j = no + 1; j < (dim_j - 1); j++) {
            for (i = 1; i < (dim_i - 1 - no); i++) {
                if (i < j) {
                    if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                        Gua_RealToObject(onn[i * dim_j + j], Gua_ObjectToReal(onn[i * dim_j + j]) + lrate * Gua_ObjectToReal(onn[(dim_i - 1) * dim_j + j]) * Gua_ObjectToReal(onn[j * dim_j + (dim_j - 1)]) * Gua_ObjectToReal(onn[i * dim_j + 0]));
                    }
                } else if (i == j) {
                    if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                        Gua_RealToObject(onn[i * dim_j + j], Gua_ObjectToReal(onn[i * dim_j + j]) + lrate * Gua_ObjectToReal(onn[(dim_i - 1) * dim_j + j]) * Gua_ObjectToReal(onn[j * dim_j + (dim_j - 1)]) * 1.0);
                    }
                } else {
                    break;
                }
            }
        }
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Lmtx_Process(Gua_Object *nn, Gua_Object *in, Gua_Object *out, Gua_Integer ni, Gua_Integer no, Gua_Integer af, Gua_Integer oaf, Gua_Integer of)
 *
 * Description:
 *     Process a pulse in an artificial neural network.
 *
 * Arguments:
 *     nn,     the learning matrix;
 *     in,     input training set;
 *     out,    input training set;
 *     ni,     number of input neurons;
 *     no,     number of input neurons;
 *     af,     activation function;
 *     oaf,    output layer activation function;
 *     of,     output function.
 *
 * Results:
 *     The function returns the ANN output after the pulse.
 */
Gua_Status Lmtx_Process(Gua_Object *nn, Gua_Object *in, Gua_Object *out, Gua_Integer ni, Gua_Integer no, Gua_Integer af, Gua_Integer oaf, Gua_Integer of)
{
    Gua_Matrix *mnn;
    Gua_Matrix *min;
    Gua_Matrix *mout;
    Gua_Object *onn;
    Gua_Object *oin;
    Gua_Object *oout;
    Gua_Integer i;
    Gua_Integer j;
    Gua_Integer dim_i;
    Gua_Integer dim_j;
    Gua_Integer first_out;
    
    if (!((Gua_PObjectType(nn) == OBJECT_TYPE_MATRIX) && (Gua_PObjectType(in) == OBJECT_TYPE_MATRIX))) {
        return GUA_ERROR;
    }
    
    mnn = (Gua_Matrix *)Gua_PObjectToMatrix(nn);
    min = (Gua_Matrix *)Gua_PObjectToMatrix(in);
    
    if (mnn && min) {
        if (!Gua_IsPObjectStored(out)) {
            Gua_FreeObject(out);
        } else {
            Gua_ClearPObject(out);
        }
        
        dim_i = mnn->dimv[0];
        dim_j = mnn->dimv[1];
        
        onn = (Gua_Object *)mnn->object;
        oin = (Gua_Object *)min->object;
        
        /* Linear matrix cell access: k = i * dim_j + j */
        first_out = dim_j - 1 - no;
        
        /* Convert integer values to real. */
        for (i = 0; i < Gua_PObjectLength(nn); i++) {
            if (Gua_ObjectType(onn[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(onn[i]);
            }
        }
        for (i = 0; i < Gua_PObjectLength(in); i++) {
            if (Gua_ObjectType(oin[i]) == OBJECT_TYPE_INTEGER) {
                Gua_ConvertIntegerObjectToReal(oin[i]);
            }
        }
        
        /* Clear inputs and outputs. */
        for (i = 0; i < dim_i - 1; i++) {
            if (Gua_ObjectType(onn[0 * dim_j + i]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[0 * dim_j + i]));
            }
            Gua_RealToObject(onn[0 * dim_j + i], 0.0);
            
            if (Gua_ObjectType(onn[i * dim_j + 0]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[i * dim_j + 0]));
            }
            Gua_RealToObject(onn[i * dim_j + 0], 0.0);
            
            if (Gua_ObjectType(onn[i * dim_j + (dim_j - 1)]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[i * dim_j + (dim_j - 1)]));
            }
            Gua_RealToObject(onn[i * dim_j + (dim_j - 1)], 0.0);
            
            if (Gua_ObjectType(onn[(dim_i - 1) * dim_j + i]) == OBJECT_TYPE_STRING) {
                Gua_FreeObject(&(onn[(dim_i - 1) * dim_j + i]));
            }
            Gua_RealToObject(onn[(dim_i - 1) * dim_j + i], 0.0);
        }
        
        /* Assign inputs. */
        for (j = 0; j < ni; j++) {
            Gua_RealToObject(onn[(j + 1) * dim_j + 0], Gua_ObjectToReal(oin[j]));
        }
        
        /* Calculate the neurons output. */
        for (j = ni + 1; j < (dim_j - 1); j++) {
            Gua_RealToObject(onn[0 * dim_j + j], 0.0);
            
            /* Weighted sums.
               x = x1 * w1 + x2 * w2 + ... */
            for (i = 1; i < (dim_i - 1 - no); i++) {
                if (i < j) {
                    if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                        Gua_RealToObject(onn[0 * dim_j + j], Gua_ObjectToReal(onn[0 * dim_j + j]) + Gua_ObjectToReal(onn[i * dim_j + j]) * (Gua_ObjectToReal(onn[i * dim_j + 0]) * 1.0));
                    }
                } else if (i == j) {
                    if (Gua_ObjectToReal(onn[i * dim_j + j]) != 0.0) {
                        Gua_RealToObject(onn[0 * dim_j + j], Gua_ObjectToReal(onn[0 * dim_j + j]) + Gua_ObjectToReal(onn[i * dim_j + j]) * 1.0);
                    }
                } else {
                    break;
                }
            }
            
            /* Activation function. */
            if (j < first_out) {
                /* Linear: f(x) = x */
                if (af == LMTX_LINEAR_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], Gua_ObjectToReal(onn[0 * dim_j + j]));
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x)) */
                } else if (af == LMTX_LOGISTIC_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                /* Hyperbolic tangent: f(x) = 2 / (1 + e^(-2x)) - 1 */
                } else if (af == LMTX_TANH_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 2.0 / (1.0 + exp(-2.0 * Gua_ObjectToReal(onn[0 * dim_j + j]))) - 1.0);
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x)) */
                } else {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                }
            /* Output layer activation function. */
            } else {
                /* Linear: f(x) = x */
                if (oaf == LMTX_LINEAR_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], Gua_ObjectToReal(onn[0 * dim_j + j]));
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x)) */
                } else if (oaf == LMTX_LOGISTIC_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                /* Hyperbolic tangent: f(x) = 2 / (1 + e^(-2x)) - 1 */
                } else if (oaf == LMTX_TANH_ACTIVATION_FUNCTION) {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 2.0 / (1.0 + exp(-2.0 * Gua_ObjectToReal(onn[0 * dim_j + j]))) - 1.0);
                /* Logistic: f(x) = 1.0 / (1.0 + e^(-x)) */
                } else {
                    /* Calculate y = f(x). */
                    Gua_RealToObject(onn[j * dim_j + 0], 1.0 / (1.0 + exp(-(Gua_ObjectToReal(onn[0 * dim_j + j])))));
                }
            }
        }
        
        /* Set the output matrix. */
        Gua_MatrixToPObject(out, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), no);
        
        mout = (Gua_Matrix *)Gua_PObjectToMatrix(out);
        
        mout->dimc = 2;
        mout->dimv = Gua_Alloc(mout->dimc * sizeof(Gua_Integer));
        mout->dimv[0] = 1;
        mout->dimv[1] = no;
        
        mout->object = (struct Gua_Object *)Gua_Alloc(no * sizeof(Gua_Object));
        
        oout = (Gua_Object *)mout->object;
        
        for (i = 0; i < no; i = i + 1) {
            if (of == LMTX_STEP_OUTPUT_FUNCTION) {
                if (oaf == LMTX_LINEAR_ACTIVATION_FUNCTION) {
                    if (Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]) >= 0.0) {
                        Gua_RealToObject(oout[i], 1.0);
                    } else {
                        Gua_RealToObject(oout[i], 0.0);
                    }
                } else if (oaf == LMTX_LOGISTIC_ACTIVATION_FUNCTION) {
                    if (Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]) >= 0.5) {
                        Gua_RealToObject(oout[i], 1.0);
                    } else {
                        Gua_RealToObject(oout[i], 0.0);
                    }
                } else if (oaf == LMTX_TANH_ACTIVATION_FUNCTION) {
                    if (Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]) >= 0.0) {
                        Gua_RealToObject(oout[i], 1.0);
                    } else {
                        Gua_RealToObject(oout[i], 0.0);
                    }
                } else {
                    if (Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]) >= 0.0) {
                        Gua_RealToObject(oout[i], 1.0);
                    } else {
                        Gua_RealToObject(oout[i], 0.0);
                    }
                }
            } else if (of == LMTX_NO_OUTPUT_FUNCTION) {
                Gua_RealToObject(oout[i], Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]));
            } else {
                Gua_RealToObject(oout[i], Gua_ObjectToReal(onn[(first_out + i) * dim_j + 0]));
            }
        }
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Lmtx_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Function wrapper.
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
Gua_Status Lmtx_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String errMessage;
    Gua_Real lrate;
    Gua_Integer af;
    Gua_Integer oaf;
    Gua_Integer of;
    
    Gua_ClearPObject(object);
    
    lrate = 0.45;
    af = LMTX_LOGISTIC_ACTIVATION_FUNCTION;
    oaf = LMTX_LOGISTIC_ACTIVATION_FUNCTION;
    of = LMTX_STEP_OUTPUT_FUNCTION;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     lmtxLearn(nn, in, out, ni, no, lrate, af, oaf)
     *
     * Description:
     *      Teach an artificial neural network for step of one epoch.
     *
     * Examples:
     *     lmtx = lmtxLearn(lmtx, input, output, dim_in_j, dim_out_j, 0.45, LMTX_SIGMOID_ACTIVATION_FUNCTION).
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "lmtxLearn") == 0) {
        if (argc < 6) {
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
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_MATRIX) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[4]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[5]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc >= 7) {
            if (Gua_ObjectType(argv[6]) != OBJECT_TYPE_REAL) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 6 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            lrate = Gua_ObjectToReal(argv[6]);
        }
        
        if (argc >= 8) {
            if (Gua_ObjectType(argv[7]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            af = Gua_ObjectToInteger(argv[7]);
        }
        
        if (argc == 9) {
            if (Gua_ObjectType(argv[8]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            oaf = Gua_ObjectToInteger(argv[8]);
        } else {
            oaf = af;
        }
        
        Gua_CopyMatrix(object, &(argv[1]), false);
        
        if (Lmtx_Learn(object, &(argv[2]), &(argv[3]), Gua_ObjectToInteger(argv[4]), Gua_ObjectToInteger(argv[5]), lrate, af, oaf) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     lmtxProcess(nn, in, ni, no, af, oaf, of)
     *
     * Description:
     *      Process a pulse in an artificial neural network.
     *
     * Examples:
     *     output == lmtxProcess(lmtx, input, dim_in_j, dim_out_j, LMTX_SIGMOID_ACTIVATION_FUNCTION, LMTX_SIGMOID_ACTIVATION_FUNCTION, LMTX_STEP_OUTPUT_FUNCTION).
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "lmtxProcess") == 0) {
        if (argc < 5) {
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
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[4]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc >= 6) {
            if (Gua_ObjectType(argv[5]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            af = Gua_ObjectToInteger(argv[5]);
        }
        
        if (argc >= 7) {
            if (Gua_ObjectType(argv[6]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 6 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            oaf = Gua_ObjectToInteger(argv[6]);
        } else {
            oaf = af;
        }
        
        if (argc == 8) {
            if (Gua_ObjectType(argv[7]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            of = Gua_ObjectToInteger(argv[7]);
        }
        
        if (Lmtx_Process(&(argv[1]), &(argv[2]), object, Gua_ObjectToInteger(argv[3]), Gua_ObjectToInteger(argv[4]), af, oaf, of) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
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
 *     Gua_Status Lmtx_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the extension functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the extension functions.
 */
Gua_Status Lmtx_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Lmtx_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "lmtxLearn", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "lmtxLearn");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Lmtx_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "lmtxProcess", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "lmtxProcess");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Define each extension constant... */
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     LMTX_LINEAR_ACTIVATION_FUNCTION
     *
     * Description:
     *     Use the hiperbolic tangent as activation function for each neuron.
     */
    Gua_IntegerToObject(object, LMTX_LINEAR_ACTIVATION_FUNCTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "LMTX_LINEAR_ACTIVATION_FUNCTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "LMTX_LINEAR_ACTIVATION_FUNCTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     LMTX_LOGISTIC_ACTIVATION_FUNCTION
     *
     * Description:
     *     Use the logistic as activation function for each neuron.
     */
    Gua_IntegerToObject(object, LMTX_LOGISTIC_ACTIVATION_FUNCTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "LMTX_LOGISTIC_ACTIVATION_FUNCTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "LMTX_LOGISTIC_ACTIVATION_FUNCTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     LMTX_TANH_ACTIVATION_FUNCTION
     *
     * Description:
     *     Use the hiperbolic tangent as activation function for each neuron.
     */
    Gua_IntegerToObject(object, LMTX_TANH_ACTIVATION_FUNCTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "LMTX_TANH_ACTIVATION_FUNCTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "LMTX_TANH_ACTIVATION_FUNCTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     LMTX_NO_OUTPUT_FUNCTION
     *
     * Description:
     *     Do not change the neural network output.
     */
    Gua_IntegerToObject(object, LMTX_NO_OUTPUT_FUNCTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "LMTX_NO_OUTPUT_FUNCTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "LMTX_NO_OUTPUT_FUNCTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     LMTX_STEP_OUTPUT_FUNCTION
     *
     * Description:
     *     Use the step function on the neural network output.
     */
    Gua_IntegerToObject(object, LMTX_STEP_OUTPUT_FUNCTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "LMTX_STEP_OUTPUT_FUNCTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "LMTX_STEP_OUTPUT_FUNCTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     LMTX_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, LMTX_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "LMTX_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "LMTX_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

