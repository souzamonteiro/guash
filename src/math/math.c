/**
 * File:
 *     math.c
 * 
 * Package:
 *    Math
 *
 * Description:
 *     This file implements the math functions for an expression interpreter.
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
 * RCS: @(#) $Id: math.c,v 2.2 2013/11/14 15:35:00 monteiro Exp $
 * 
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "math.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Math_MathFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Math functions wrapper.
 *
 * Arguments:
 *     nspace,    a pointer to a structure Gua_Namespace. Must do a cast before use it;
 *     argc,      the number of arguments to pass to the function;
 *     argv,      an array containing the arguments to the function;
 *                argv[0] is the function name;
 *     object,    a structure containing the return value of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The return value of the wrapped funcion.
 */
Gua_Status Math_MathFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Object o;
    Gua_Real a;
    Gua_Imaginary b;
    Gua_Real c;
    Gua_Imaginary d;
    Gua_Real r;
    Gua_Real t;
    Gua_Real n;
    Gua_Object sine;
    Gua_Object cosine;
    Gua_Integer i;
    Gua_String errMessage;
    
    Gua_ClearObject(o);
    
    Gua_ClearPObject(object);

    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    /*
     * Complex numbers functions:
     *
     * (a+b*i)/(c+d*i)=(a*c+b*d)/(c*c+d*d)+i*(b*c-a*d)/(c*c+d*d)
     *
     * cos(a+b*i)=cos(a)*cosh(b)-i*sin(a)*sinh(b)
     * sin(a+b*i)=sin(a)*cosh(b)+i*cos(a)*sinh(b)
     * tan(a+b*i)=sin(a+b*i)/cos(a+b*i)
     *
     * cosh(a+b*i)=cosh(a)*cos(b)+i*sinh(a)sin(b)
     * sinh(a+b*i)=sinh(a)*cos(b)+i*cosh(a)sin(b)
     * tanh(a+b*i)=sinh(a+b*i)/cosh(a+b*i)
     *
     * r=abs(a+b*i)=sqrt(a*a+b*b)
     * t=arg(a+b*i)=atan(b/a)
     *
     * exp(a+b*i)=exp(a)*cos(b)+i*sin(b)
     * log(a+b*i)=log(r)+i*t
     *
     * pow(a+b*i,n)=pow(r,n)*cos(n*t)+i*pow(r,n)*sin(n*t)
     * sqrt(a+b*i)=sqrt(r)*cos(t/2)+i*sqrt(r)*sin(t/2)
     *
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "acos") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, acos(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, acos(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "asin") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, asin(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, asin(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "atan") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, atan(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, atan(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "atan2") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, atan2(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToPObject(object, atan2(Gua_ObjectToReal(argv[1]), Gua_ObjectToReal(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToPObject(object, atan2(Gua_ObjectToInteger(argv[1]), Gua_ObjectToReal(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, atan2(Gua_ObjectToReal(argv[1]), Gua_ObjectToInteger(argv[2])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ceil") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, ceil(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, ceil(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "cos") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, cos(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, cos(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToPObject(object, cos(a) * cosh(b), -sin(a) * sinh(b));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "cosh") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, cosh(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, cosh(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToPObject(object, cosh(a) * cos(b), sinh(a) * sin(b));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "deg") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) != OBJECT_TYPE_INTEGER) && (argv[1].type != OBJECT_TYPE_REAL)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, (Gua_ObjectToInteger(argv[1]) * 180.0 / MATH_PI));
        } else {
            Gua_RealToPObject(object, (Gua_ObjectToReal(argv[1]) * 180.0 / MATH_PI));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "dist") == 0) {
        if (!((argc == 5) || (argc == 7))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 7) {
            if (!((Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (!((Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 6 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }

        if (argc == 5) {
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[1]));
                Gua_RealToObject(argv[1], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[2]));
                Gua_RealToObject(argv[2], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[3]));
                Gua_RealToObject(argv[3], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[4]));
                Gua_RealToObject(argv[4], Gua_ObjectToReal(o));
            }

            /*
             * x1 = argv[1], y1 = argv[2], x2 = argv[3], y2 = argv[4]
             * d = sqrt((x2-x1)^2 + (y2-y1)^2)
             */
            Gua_RealToPObject(object, sqrt(pow(Gua_ObjectToReal(argv[3]) - Gua_ObjectToReal(argv[1]), 2) + pow(Gua_ObjectToReal(argv[4]) - Gua_ObjectToReal(argv[2]), 2)));
        } else {
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[1]));
                Gua_RealToObject(argv[1], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[2]));
                Gua_RealToObject(argv[2], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[3]));
                Gua_RealToObject(argv[3], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[4]));
                Gua_RealToObject(argv[4], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[5]));
                Gua_RealToObject(argv[5], Gua_ObjectToReal(o));
            }
            if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
                Gua_RealToObject(o, Gua_ObjectToInteger(argv[6]));
                Gua_RealToObject(argv[6], Gua_ObjectToReal(o));
            }

            /*
             * x1 = argv[1], y1 = argv[2], z1 = argv[3], x2 = argv[4], y2 = argv[5], z2 = argv[6]
             * d = sqrt((x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2)
             */
            Gua_RealToPObject(object, sqrt(pow(Gua_ObjectToReal(argv[4]) - Gua_ObjectToReal(argv[1]), 2) + pow(Gua_ObjectToReal(argv[5]) - Gua_ObjectToReal(argv[2]), 2) + pow(Gua_ObjectToReal(argv[6]) - Gua_ObjectToReal(argv[3]), 2)));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "exp") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, exp(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, exp(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToPObject(object, exp(a) * cos(b), sin(b));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fabs") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, labs(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, fabs(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "factorial") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectToInteger(argv[1]) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectToInteger(argv[1]) == 0) || (Gua_ObjectToInteger(argv[1]) == 1)) {
            Gua_IntegerToPObject(object, 1);
        } else if (Gua_ObjectToInteger(argv[1]) > 1) {
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(argv[1]));
            for (i = Gua_ObjectToInteger(argv[1]) - 1; i > 1; i--) {
                Gua_IntegerToPObject(object, (Gua_PObjectToInteger(object) * i));
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "floor") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, floor(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, floor(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fmax") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            if (Gua_ObjectToInteger(argv[1]) > Gua_ObjectToInteger(argv[2])) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(argv[1]));
            } else {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(argv[2]));
            }
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            if (Gua_ObjectToReal(argv[1]) > Gua_ObjectToReal(argv[2])) {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
            } else {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[2]));
            }
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToObject(o, Gua_ObjectToInteger(argv[1]));
            Gua_RealToObject(argv[1], Gua_ObjectToReal(o));
            
            if (Gua_ObjectToReal(argv[1]) > Gua_ObjectToReal(argv[2])) {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
            } else {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[2]));
            }
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToObject(o, Gua_ObjectToInteger(argv[2]));
            Gua_RealToObject(argv[2], Gua_ObjectToReal(o));
            
            if (Gua_ObjectToReal(argv[1]) > Gua_ObjectToReal(argv[2])) {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
            } else {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[2]));
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fmin") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            if (Gua_ObjectToInteger(argv[1]) < Gua_ObjectToInteger(argv[2])) {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(argv[1]));
            } else {
                Gua_IntegerToPObject(object, Gua_ObjectToInteger(argv[2]));
            }
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            if (Gua_ObjectToReal(argv[1]) < Gua_ObjectToReal(argv[2])) {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
            } else {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[2]));
            }
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToObject(o, Gua_ObjectToInteger(argv[1]));
            Gua_RealToObject(argv[1], Gua_ObjectToReal(o));
            
            if (Gua_ObjectToReal(argv[1]) < Gua_ObjectToReal(argv[2])) {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
            } else {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[2]));
            }
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToObject(o, Gua_ObjectToInteger(argv[2]));
            Gua_RealToObject(argv[2], Gua_ObjectToReal(o));
            
            if (Gua_ObjectToReal(argv[1]) < Gua_ObjectToReal(argv[2])) {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
            } else {
                Gua_RealToPObject(object, Gua_ObjectToReal(argv[2]));
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fmod") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, fmod(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToPObject(object, fmod(Gua_ObjectToReal(argv[1]), Gua_ObjectToReal(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToPObject(object, fmod(Gua_ObjectToInteger(argv[1]), Gua_ObjectToReal(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, fmod(Gua_ObjectToReal(argv[1]), Gua_ObjectToInteger(argv[2])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ldexp") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
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
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, ldexp(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, ldexp(Gua_ObjectToReal(argv[1]), Gua_ObjectToInteger(argv[2])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "log") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, log(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, log(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            r = sqrt(a * a + b * b);
            t = atan(b / a);
            Gua_ComplexToPObject(object, log(r), t);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "log10") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, log10(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, log10(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "pow") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, pow(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToPObject(object, pow(Gua_ObjectToReal(argv[1]), Gua_ObjectToReal(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            Gua_RealToPObject(object, pow(Gua_ObjectToInteger(argv[1]), Gua_ObjectToReal(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            Gua_RealToPObject(object, pow(Gua_ObjectToReal(argv[1]), Gua_ObjectToInteger(argv[2])));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            r = sqrt(a * a + b * b);
            t = atan(b / a);
            n = Gua_ObjectToInteger(argv[2]);
            Gua_ComplexToPObject(object, pow(r, n) * cos(n * t), pow(r, n) * sin(n * t));
        } else if ((Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            r = sqrt(a * a + b * b);
            t = atan(b / a);
            n = Gua_ObjectToReal(argv[2]);
            Gua_ComplexToPObject(object, pow(r, n) * cos(n * t), pow(r, n) * sin(n * t));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "rad") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((Gua_ObjectType(argv[1]) != OBJECT_TYPE_INTEGER) && (argv[1].type != OBJECT_TYPE_REAL)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, (Gua_ObjectToInteger(argv[1]) * MATH_PI / 180.0));
        } else {
            Gua_RealToPObject(object, (Gua_ObjectToReal(argv[1]) * MATH_PI / 180.0));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "random") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
#ifdef _WINDOWS_
        Gua_RealToPObject(object, (1.0 * (rand() / (RAND_MAX + 1.0))));
#else
        Gua_RealToPObject(object, (1.0 * (random() / (RAND_MAX + 1.0))));
#endif
    } else if (strcmp(Gua_ObjectToString(argv[0]), "round") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, round(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, round(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "roundl") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_IntegerToPObject(object, roundl(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_IntegerToPObject(object, roundl(Gua_ObjectToReal(argv[1])));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sin") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, sin(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, sin(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToPObject(object, sin(a) * cosh(b), cos(a) * sinh(b));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sinh") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, sinh(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, sinh(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToPObject(object, sinh(a) * cos(b), cosh(a) * sin(b));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqrt") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            if (Gua_ObjectToInteger(argv[1]) > 0) {
                Gua_RealToPObject(object, sqrt(Gua_ObjectToInteger(argv[1])));
            } else {
                Gua_ComplexToPObject(object, 0.0, sqrt(labs(Gua_ObjectToInteger(argv[1]))));
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            if (Gua_ObjectToReal(argv[1]) > 0) {
                Gua_RealToPObject(object, sqrt(Gua_ObjectToReal(argv[1])));
            } else {
                Gua_ComplexToPObject(object, 0.0, sqrt(fabs(Gua_ObjectToReal(argv[1]))));
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            r = sqrt(a * a + b * b);
            t = atan(b / a);
            Gua_ComplexToPObject(object, sqrt(r) * cos(t / 2), sqrt(r) * sin(t / 2));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "srandom") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
#ifdef _WINDOWS_
            srand(Gua_ObjectToInteger(argv[1]));
#else
            srandom(Gua_ObjectToInteger(argv[1]));
#endif            
            Gua_IntegerToPObject(object, Gua_ObjectToInteger(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
#ifdef _WINDOWS_
            srand(Gua_ObjectToReal(argv[1]));
#else
            srandom(Gua_ObjectToReal(argv[1]));
#endif            
            
            Gua_RealToPObject(object, Gua_ObjectToReal(argv[1]));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "tan") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, tan(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, tan(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToObject(sine, sin(a) * cosh(b), cos(a) * sinh(b));
            Gua_ComplexToObject(cosine, cos(a) * cosh(b), -sin(a) * sinh(b));
            a = Gua_ObjectToReal(sine);
            b = Gua_ObjectToImaginary(sine);
            c = Gua_ObjectToReal(cosine);
            d = Gua_ObjectToImaginary(cosine);
            Gua_ComplexToPObject(object, (a * c + b * d) / (c * c + d * d), (b * c - a * d) / (c * c + d * d));
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "tanh") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            Gua_RealToPObject(object, tanh(Gua_ObjectToInteger(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            Gua_RealToPObject(object, tanh(Gua_ObjectToReal(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            a = Gua_ObjectToReal(argv[1]);
            b = Gua_ObjectToImaginary(argv[1]);
            Gua_ComplexToObject(sine, sinh(a) * cos(b), cosh(a) * sin(b));
            Gua_ComplexToObject(cosine, cosh(a) * cos(b), sinh(a) * sin(b));
            a = Gua_ObjectToReal(sine);
            b = Gua_ObjectToImaginary(sine);
            c = Gua_ObjectToReal(cosine);
            d = Gua_ObjectToImaginary(cosine);
            Gua_ComplexToPObject(object, (a * c + b * d) / (c * c + d * d), (b * c - a * d) / (c * c + d * d));
        }
    } else {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "unknown function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Math_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the math functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the math functions.
 */
Gua_Status Math_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Math_MathFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "acos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "acos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "asin", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "asin");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "atan", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "atan");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "atan2", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "atan2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ceil", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ceil");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "cos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "cos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "cosh", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "cosh");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "deg", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "deg");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "dist", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "dist");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "exp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "exp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fabs", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fabs");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "factorial", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "factorial");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "floor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "floor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fmax", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fmax");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fmin", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fmin");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fmod", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fmod");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ldexp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ldexp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "log", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "log");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "log10", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "log10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "pow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "pow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "rad", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "rad");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "random", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "random");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "round", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "round");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "roundl", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "roundl");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sin", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sin");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sinh", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sinh");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqrt", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqrt");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "srandom", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "srandom");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "tan", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "tan");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "tanh", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "tanh");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    Gua_RealToObject(object, MATH_E);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "E", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "E");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_RealToObject(object, MATH_PI);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "PI", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "PI");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     MATH_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, MATH_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "MATH_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "MATH_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}
