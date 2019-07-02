/**
 * File:
 *     gl.c
 *
 * Package:
 *     Gl
 *
 * Description:
 *     This library implements an OpenGL ES wrapper for the GuaraScript interpreter.
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
 * RCS: @(#) $Id: gl.c,v 1.2 2014/08/05 14:34:00 monteiro Exp $
 * 
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MAC_OS_X_
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif
#include "interp.h"
#include "gl.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gl_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Gl_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    GLint arg1i;
    GLint arg2i;
    GLint arg3i;
    GLint arg4i;
    GLint arg5i;
    GLint arg6i;
    GLint arg7i;
    GLint arg8i;
    GLdouble arg1f;
    GLdouble arg2f;
    GLdouble arg3f;
    GLdouble arg4f;
    GLdouble arg5f;
    GLdouble arg6f;
    GLdouble arg1d;
    GLbyte *arg1bv;
    GLbyte *arg4bv;
#ifndef _WINDOWS_
    GLbyte *arg8bv;
#endif
    GLbyte *arg9bv;
    GLuint *arg1iv;
    GLuint *arg2iv;
    GLfloat *arg1fv;
    GLfloat *arg2fv;
    GLfloat *arg3fv;
    GLfloat *arg4fv;
    GLdouble *arg2dv;
    const GLubyte *resbv;
    Gua_Handle *h;
    Gua_Length i;
    Gua_Length l;
    Gua_Matrix *m;
    Gua_Object *o;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    arg1i = 0;
    arg2i = 0;
    arg3i = 0;
    arg4i = 0;
    arg5i = 0;
    arg6i = 0;
    arg7i = 0;
    arg8i = 0;
    
    arg1f = 0.0;
    arg2f = 0.0;
    arg3f = 0.0;
    arg4f = 0.0;
    arg5f = 0.0;
    arg6f = 0.0;
    
    arg1d = 0.0;
    
    i = 0;
    l = 0;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "glAlphaFunc") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        
        glAlphaFunc(arg1i, arg2f);
#ifndef _WINDOWS_
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glActiveTexture") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glActiveTexture(arg1i);
#endif
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glArrayOfByte") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
	    if (!(Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        l = Gua_ObjectLength(argv[1]);
        
        arg1bv = (GLbyte *)malloc(sizeof(GLbyte) * l + 1);
        
        for (i = 0; i < l; i++) {
            arg1bv[i] = argv[1].string[i];
        }
        arg1bv[l] = '\0';
        
        Gua_NewHandle(h, "glArrayOfByte", arg1bv);
        
        Gua_HandleToPObject(object, (struct Gua_Handle *)h);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glArrayOfInt") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
	    if (!(Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        m = (Gua_Matrix *)Gua_ObjectToMatrix(argv[1]);
        o = (Gua_Object *)m->object;
        
        l = Gua_ObjectLength(argv[1]);
        
        arg1iv = (GLuint *)malloc(sizeof(GLuint) * l);
        
        for (i = 0; i < l; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                arg1iv[i] = Gua_ObjectToInteger(o[i]);
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                arg1iv[i] = roundl(Gua_ObjectToReal(o[i]));
            } else {
                arg1iv[i] = 0.0;
            }
        }
        
        Gua_NewHandle(h, "glArrayOfInt", arg1iv);
        
        Gua_HandleToPObject(object, (struct Gua_Handle *)h);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glArrayOfFloat") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
	    if (!(Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        m = (Gua_Matrix *)Gua_ObjectToMatrix(argv[1]);
        o = (Gua_Object *)m->object;
        
        l = Gua_ObjectLength(argv[1]);
        
        arg1fv = (GLfloat *)malloc(sizeof(GLfloat) * l);
        
        for (i = 0; i < l; i++) {
            if (Gua_ObjectType(o[i]) == OBJECT_TYPE_INTEGER) {
                arg1fv[i] = Gua_ObjectToInteger(o[i]);
            } else if (Gua_ObjectType(o[i]) == OBJECT_TYPE_REAL) {
                arg1fv[i] = Gua_ObjectToReal(o[i]);
            } else {
                arg1fv[i] = 0.0;
            }
        }
        
        Gua_NewHandle(h, "glArrayOfFloat", arg1fv);
        
        Gua_HandleToPObject(object, (struct Gua_Handle *)h);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glBegin") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glBegin(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glBindBuffer") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glBindTexture") == 0) {
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

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glBindTexture(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glBlendFunc") == 0) {
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

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glBlendFunc(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glBufferData") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glBufferSubData") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glCallList") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glCallList(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glClear") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glClear(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glClearColor") == 0) {
        if (argc != 5) {
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

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4f = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4f = Gua_ObjectToReal(argv[4]);            
        }
        
        glClearColor(arg1f, arg2f, arg3f, arg4f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glClearDepth") == 0) {
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
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        
        glClearDepth(arg1f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glClearStencil") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glClearStencil(arg1i);
#ifndef _WINDOWS_
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glClientActiveTexture") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glClientActiveTexture(arg1i);
#endif
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glClipPlane") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glColor") == 0) {
        if (argc != 5) {
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

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4f = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4f = Gua_ObjectToReal(argv[4]);            
        }
        
        glColor4f(arg1f, arg2f, arg3f, arg4f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glColorMask") == 0) {
        if (argc != 5) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        
        glColorMask(arg1i, arg2i, arg3i, arg4i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glColorMaterial") == 0) {
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

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glColorMaterial(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glColorPointer") == 0) {
        if (argc != 5) {
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
        if (!(Gua_ObjectType(argv[4]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        /*
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        */
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[4]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg4fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glColorPointer(arg1i, GL_FLOAT, arg3i, arg4fv);
#ifndef _WINDOWS_
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glCompressedTexImage2D") == 0) {
        if (argc != 9) {
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
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[8]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6i = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6i = roundl(Gua_ObjectToReal(argv[6]));            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7i = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7i = roundl(Gua_ObjectToReal(argv[7]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[8]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg8bv = (GLbyte *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glCompressedTexImage2D(arg1i, arg2i, arg3i, arg4i, arg5i, arg6i, arg7i, arg8bv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glCompressedTexSubImage2D") == 0) {
        if (argc != 10) {
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
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[9]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6i = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6i = roundl(Gua_ObjectToReal(argv[6]));            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7i = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7i = roundl(Gua_ObjectToReal(argv[7]));            
        }
        if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) {
            arg8i = Gua_ObjectToInteger(argv[8]);
        } else if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL) {
            arg8i = roundl(Gua_ObjectToReal(argv[8]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[9]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg9bv = (GLbyte *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glCompressedTexSubImage2D(arg1i, arg2i, arg3i, arg4i, arg5i, arg6i, arg7i, arg8i, arg9bv);
#endif
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glCopyTexImage2D") == 0) {
        if (argc != 9) {
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
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6i = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6i = roundl(Gua_ObjectToReal(argv[6]));            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7i = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7i = roundl(Gua_ObjectToReal(argv[7]));            
        }
        if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) {
            arg8i = Gua_ObjectToInteger(argv[8]);
        } else if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL) {
            arg8i = roundl(Gua_ObjectToReal(argv[8]));            
        }
        
        glCopyTexImage2D(arg1i, arg2i, arg3i, arg4i, arg5i, arg6i, arg7i, arg8i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glCopyTexSubImage2D") == 0) {
        if (argc != 9) {
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
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6i = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6i = roundl(Gua_ObjectToReal(argv[6]));            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7i = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7i = roundl(Gua_ObjectToReal(argv[7]));            
        }
        if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) {
            arg8i = Gua_ObjectToInteger(argv[8]);
        } else if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL) {
            arg8i = roundl(Gua_ObjectToReal(argv[8]));            
        }
        
        glCopyTexSubImage2D(arg1i, arg2i, arg3i, arg4i, arg5i, arg6i, arg7i, arg8i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glCullFace") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glCullFace(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDeleteBuffers") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDeleteLists") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glDeleteLists(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDeleteTextures") == 0) {
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
        if (!(Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[2]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfInt") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg2iv = (GLuint *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glDeleteTextures(arg1i, arg2iv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDepthFunc") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glDepthFunc(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDepthMask") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glDepthMask(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDepthRange") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        
        glDepthRange(arg1f, arg2f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDisable") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glDisable(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDisableClientState") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glDisableClientState(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDrawArrays") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glDrawArrays(arg1i, arg2i, arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glDrawElements") == 0) {
        if (argc != 5) {
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
        if (!(Gua_ObjectType(argv[4]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[4]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg4bv = (GLbyte *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glDrawElements(arg1i, arg2i, arg3i, arg4bv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glEnable") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glEnable(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glEnableClientState") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glEnableClientState(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glEnd") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glEnd();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glEndList") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glEndList();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glFinish") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glFinish();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glFlush") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glFlush();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glFog") == 0) {
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
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[2]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg2fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            glFogf(arg1i, arg2f);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_MATRIX) {
            glFogfv(arg1i, arg2fv);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glFree") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (!((strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") == 0) || (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfInt") == 0) || (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") == 0))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") == 0) {
            arg1bv = (GLbyte *)Gua_GetHandlePointer((Gua_Handle *)h);
            
            Gua_Free(arg1bv);
        } else if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfInt") == 0) {
            arg1iv = (GLuint *)Gua_GetHandlePointer((Gua_Handle *)h);
            
            Gua_Free(arg1iv);
        } else if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") == 0) {
            arg1fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
            
            Gua_Free(arg1fv);
        }
        
        Gua_SetHandlePointer((Gua_Handle *)h, NULL);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glFrontFace") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glFrontFace(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glFrustum") == 0) {
        if (argc != 7) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4f = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4f = Gua_ObjectToReal(argv[4]);            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5f = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5f = Gua_ObjectToReal(argv[5]);            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6f = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6f = Gua_ObjectToReal(argv[6]);            
        }
        
        glFrustum(arg1f, arg2f, arg3f, arg4f, arg5f, arg6f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGenBuffers") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGenLists") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        Gua_IntegerToPObject(object, glGenLists(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGenTextures") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        if (arg1i == 0) {
            return GUA_OK;
        }
        
        l = arg1i;
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg2iv = (GLuint *)malloc(sizeof(GLuint) * l);
        
        glGenTextures(arg1i, arg2iv);
        
        for (i = 0; i < l; i++) {
            Gua_IntegerToObject(o[i], arg2iv[i]);
        }
        
        Gua_Free(arg2iv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGet") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
#ifndef _WINDOWS_
        if (arg1i == GL_ACTIVE_TEXTURE) {
            l = 1;
        } else if (arg1i == GL_ALIASED_POINT_SIZE_RANGE) {
            l = 2;
        } else if (arg1i == GL_ALIASED_LINE_WIDTH_RANGE) {
            l = 2;
        } else if (arg1i == GL_ALPHA_BITS) {
#else
        if (arg1i == GL_ALPHA_BITS) {
#endif
            l = 1;
        } else if (arg1i == GL_ALPHA_TEST) {
            l = 1;
        } else if (arg1i == GL_ALPHA_TEST_FUNC) {
            l = 1;
        } else if (arg1i == GL_ALPHA_TEST_REF) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_ARRAY_BUFFER_BINDING) {
            l = 1;
#endif
        } else if (arg1i == GL_BLEND) {
            l = 1;
        } else if (arg1i == GL_BLEND_DST) {
            l = 1;
        } else if (arg1i == GL_BLEND_SRC) {
            l = 1;
        } else if (arg1i == GL_BLUE_BITS) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_CLIENT_ACTIVE_TEXTURE) {
            l = 1;
#endif
        } else if (arg1i == GL_CLIP_PLANE0) {
            l = 1;
        } else if (arg1i == GL_CLIP_PLANE1) {
            l = 1;
        } else if (arg1i == GL_CLIP_PLANE2) {
            l = 1;
        } else if (arg1i == GL_CLIP_PLANE3) {
            l = 1;
        } else if (arg1i == GL_CLIP_PLANE4) {
            l = 1;
        } else if (arg1i == GL_CLIP_PLANE5) {
            l = 1;
        } else if (arg1i == GL_COLOR_ARRAY) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_COLOR_ARRAY_BUFFER_BINDING) {
            l = 1;
#endif
        } else if (arg1i == GL_COLOR_ARRAY_SIZE) {
            l = 1;
        } else if (arg1i == GL_COLOR_ARRAY_STRIDE) {
            l = 1;
        } else if (arg1i == GL_COLOR_ARRAY_TYPE) {
            l = 1;
        } else if (arg1i == GL_COLOR_CLEAR_VALUE) {
            l = 4;
        } else if (arg1i == GL_COLOR_LOGIC_OP) {
            l = 1;
        } else if (arg1i == GL_COLOR_MATERIAL) {
            l = 1;
        } else if (arg1i == GL_COLOR_WRITEMASK) {
            l = 4;
#ifndef _WINDOWS_
        } else if (arg1i == GL_COMPRESSED_TEXTURE_FORMATS) {
            glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &arg2i);
            l = arg2i;
#endif
        } else if (arg1i == GL_CULL_FACE) {
            l = 1;
        } else if (arg1i == GL_CULL_FACE_MODE) {
            l = 1;
        } else if (arg1i == GL_CURRENT_COLOR) {
            l = 4;
        } else if (arg1i == GL_CURRENT_NORMAL) {
            l = 3;
        } else if (arg1i == GL_CURRENT_TEXTURE_COORDS) {
            l = 4;
        } else if (arg1i == GL_DEPTH_BITS) {
            l = 1;
        } else if (arg1i == GL_DEPTH_CLEAR_VALUE) {
            l = 1;
        } else if (arg1i == GL_DEPTH_FUNC) {
            l = 1;
        } else if (arg1i == GL_DEPTH_RANGE) {
            l = 2;
        } else if (arg1i == GL_DEPTH_TEST) {
            l = 1;
        } else if (arg1i == GL_DEPTH_WRITEMASK) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_ELEMENT_ARRAY_BUFFER_BINDING) {
            l = 1;
#endif
        } else if (arg1i == GL_FOG) {
            l = 1;
        } else if (arg1i == GL_FOG_COLOR) {
            l = 4;
        } else if (arg1i == GL_FOG_DENSITY) {
            l = 1;
        } else if (arg1i == GL_FOG_END) {
            l = 1;
        } else if (arg1i == GL_FOG_HINT) {
            l = 1;
        } else if (arg1i == GL_FOG_MODE) {
            l = 1;
        } else if (arg1i == GL_FOG_START) {
            l = 1;
        } else if (arg1i == GL_FRONT_FACE) {
            l = 1;
        } else if (arg1i == GL_GREEN_BITS) {
            l = 1;
        } else if (arg1i == GL_LIGHT_MODEL_AMBIENT) {
            l = 4;
        } else if (arg1i == GL_LIGHT_MODEL_TWO_SIDE) {
            l = 1;
        } else if (arg1i == GL_LIGHT0) {
            l = 1;
        } else if (arg1i == GL_LIGHT1) {
            l = 1;
        } else if (arg1i == GL_LIGHT2) {
            l = 1;
        } else if (arg1i == GL_LIGHT3) {
            l = 1;
        } else if (arg1i == GL_LIGHT4) {
            l = 1;
        } else if (arg1i == GL_LIGHT5) {
            l = 1;
        } else if (arg1i == GL_LIGHT6) {
            l = 1;
        } else if (arg1i == GL_LIGHT7) {
            l = 1;
        } else if (arg1i == GL_LIGHTING) {
            l = 1;
        } else if (arg1i == GL_LINE_SMOOTH) {
            l = 1;
        } else if (arg1i == GL_LINE_SMOOTH_HINT) {
            l = 1;
        } else if (arg1i == GL_LINE_WIDTH) {
            l = 1;
        } else if (arg1i == GL_LOGIC_OP_MODE) {
            l = 1;
        } else if (arg1i == GL_MATRIX_MODE) {
            l = 1;
        } else if (arg1i == GL_MAX_CLIP_PLANES) {
            l = 1;
        } else if (arg1i == GL_MAX_LIGHTS) {
            l = 1;
        } else if (arg1i == GL_MAX_MODELVIEW_STACK_DEPTH) {
            l = 1;
        } else if (arg1i == GL_MAX_PROJECTION_STACK_DEPTH) {
            l = 1;
        } else if (arg1i == GL_MAX_TEXTURE_SIZE) {
            l = 1;
        } else if (arg1i == GL_MAX_TEXTURE_STACK_DEPTH) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_MAX_TEXTURE_UNITS) {
            l = 1;
#endif
        } else if (arg1i == GL_MAX_VIEWPORT_DIMS) {
            l = 2;
        } else if (arg1i == GL_MODELVIEW_MATRIX) {
            l = 16;
        } else if (arg1i == GL_MODELVIEW_STACK_DEPTH) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_MULTISAMPLE) {
            l = 1;
#endif
        } else if (arg1i == GL_NORMAL_ARRAY) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_NORMAL_ARRAY_BUFFER_BINDING) {
            l = 1;
#endif
        } else if (arg1i == GL_NORMAL_ARRAY_STRIDE) {
            l = 1;
        } else if (arg1i == GL_NORMAL_ARRAY_TYPE) {
            l = 1;
        } else if (arg1i == GL_NORMALIZE) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_NUM_COMPRESSED_TEXTURE_FORMATS) {
            l = 1;
#endif
        } else if (arg1i == GL_PACK_ALIGNMENT) {
            l = 1;
        } else if (arg1i == GL_PERSPECTIVE_CORRECTION_HINT) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_POINT_DISTANCE_ATTENUATION) {
            l = 3;
        } else if (arg1i == GL_POINT_FADE_THRESHOLD_SIZE) {
            l = 1;
#endif
        } else if (arg1i == GL_POINT_SIZE) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_POINT_SIZE_MAX) {
            l = 1;
        } else if (arg1i == GL_POINT_SIZE_MIN) {
            l = 1;
#endif
        } else if (arg1i == GL_POINT_SMOOTH) {
            l = 1;
        } else if (arg1i == GL_POINT_SMOOTH_HINT) {
            l = 1;
        } else if (arg1i == GL_POLYGON_OFFSET_FACTOR) {
            l = 1;
        } else if (arg1i == GL_POLYGON_OFFSET_FILL) {
            l = 1;
        } else if (arg1i == GL_POLYGON_OFFSET_UNITS) {
            l = 1;
        } else if (arg1i == GL_PROJECTION_MATRIX) {
            l = 16;
        } else if (arg1i == GL_PROJECTION_STACK_DEPTH) {
            l = 1;
        } else if (arg1i == GL_RED_BITS) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_RESCALE_NORMAL) {
            l = 1;
        } else if (arg1i == GL_SAMPLE_ALPHA_TO_COVERAGE) {
            l = 1;
        } else if (arg1i == GL_SAMPLE_ALPHA_TO_ONE) {
            l = 1;
        } else if (arg1i == GL_SAMPLE_BUFFERS) {
            l = 1;
        } else if (arg1i == GL_SAMPLE_COVERAGE) {
            l = 1;
        } else if (arg1i == GL_SAMPLE_COVERAGE_INVERT) {
            l = 1;
        } else if (arg1i == GL_SAMPLE_COVERAGE_VALUE) {
            l = 1;
        } else if (arg1i == GL_SAMPLES) {
            l = 1;
#endif
        } else if (arg1i == GL_SCISSOR_BOX) {
            l = 4;
        } else if (arg1i == GL_SCISSOR_TEST) {
            l = 1;
        } else if (arg1i == GL_SHADE_MODEL) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_SMOOTH_LINE_WIDTH_RANGE) {
            l = 2;
        } else if (arg1i == GL_SMOOTH_POINT_SIZE_RANGE) {
            l = 2;
#endif
        } else if (arg1i == GL_STENCIL_BITS) {
            l = 1;
        } else if (arg1i == GL_STENCIL_CLEAR_VALUE) {
            l = 1;
        } else if (arg1i == GL_STENCIL_FAIL) {
            l = 1;
        } else if (arg1i == GL_STENCIL_FUNC) {
            l = 1;
        } else if (arg1i == GL_STENCIL_PASS_DEPTH_FAIL) {
            l = 1;
        } else if (arg1i == GL_STENCIL_PASS_DEPTH_PASS) {
            l = 1;
        } else if (arg1i == GL_STENCIL_REF) {
            l = 1;
        } else if (arg1i == GL_STENCIL_TEST) {
            l = 1;
        } else if (arg1i == GL_STENCIL_VALUE_MASK) {
            l = 1;
        } else if (arg1i == GL_STENCIL_WRITEMASK) {
            l = 1;
        } else if (arg1i == GL_SUBPIXEL_BITS) {
            l = 1;
        } else if (arg1i == GL_TEXTURE_2D) {
            l = 1;
        } else if (arg1i == GL_TEXTURE_BINDING_2D) {
            l = 1;
        } else if (arg1i == GL_TEXTURE_COORD_ARRAY) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING) {
            l = 1;
#endif
        } else if (arg1i == GL_TEXTURE_COORD_ARRAY_SIZE) {
            l = 1;
        } else if (arg1i == GL_TEXTURE_COORD_ARRAY_STRIDE) {
            l = 1;
        } else if (arg1i == GL_TEXTURE_COORD_ARRAY_TYPE) {
            l = 1;
        } else if (arg1i == GL_TEXTURE_MATRIX) {
            l = 16;
        } else if (arg1i == GL_TEXTURE_STACK_DEPTH) {
            l = 1;
        } else if (arg1i == GL_UNPACK_ALIGNMENT) {
            l = 1;
        } else if (arg1i == GL_VIEWPORT) {
            l = 4;
        } else if (arg1i == GL_VERTEX_ARRAY) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg1i == GL_VERTEX_ARRAY_BUFFER_BINDING) {
            l = 1;
#endif
        } else if (arg1i == GL_VERTEX_ARRAY_SIZE) {
            l = 1;
        } else if (arg1i == GL_VERTEX_ARRAY_STRIDE) {
            l = 1;
        } else if (arg1i == GL_VERTEX_ARRAY_TYPE) {
            l = 1;
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg2fv = (GLfloat *)malloc(sizeof(GLfloat) * l);
        
        glGetFloatv(arg1i, arg2fv);
        
        for (i = 0; i < l; i++) {
            Gua_IntegerToObject(o[i], arg2fv[i]);
        }
        
        Gua_Free(arg2fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetBufferParameteriv") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetClipPlane") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        l = 4;
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg2dv = (GLdouble *)malloc(sizeof(GLdouble) * l);
        
        glGetClipPlane(arg1i, arg2dv);
        
        for (i = 0; i < l; i++) {
            Gua_RealToObject(o[i], arg2dv[i]);
        }
        
        Gua_Free(arg2dv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetError") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, glGetError());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetLight") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        if (arg2i == GL_AMBIENT) {
            l = 4;
        } else if (arg2i == GL_DIFFUSE) {
            l = 4;
        } else if (arg2i == GL_SPECULAR) {
            l = 4;
        } else if (arg2i == GL_POSITION) {
            l = 4;
        } else if (arg2i == GL_SPOT_DIRECTION) {
            l = 3;
        } else if (arg2i == GL_SPOT_EXPONENT) {
            l = 1;
        } else if (arg2i == GL_SPOT_CUTOFF) {
            l = 1;
        } else if (arg2i == GL_CONSTANT_ATTENUATION) {
            l = 1;
        } else if (arg2i == GL_LINEAR_ATTENUATION) {
            l = 1;
        } else if (arg2i == GL_QUADRATIC_ATTENUATION) {
            l = 1;
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg3fv = (GLfloat *)malloc(sizeof(GLfloat) * l);
        
        glGetLightfv(arg1i, arg2i, arg3fv);
        
        for (i = 0; i < l; i++) {
            Gua_IntegerToObject(o[i], arg3fv[i]);
        }
        
        Gua_Free(arg3fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetMaterial") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        if (arg2i == GL_AMBIENT) {
            l = 4;
        } else if (arg2i == GL_DIFFUSE) {
            l = 4;
        } else if (arg2i == GL_SPECULAR) {
            l = 4;
        } else if (arg2i == GL_EMISSION) {
            l = 4;
        } else if (arg2i == GL_SHININESS) {
            l = 1;
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg3fv = (GLfloat *)malloc(sizeof(GLfloat) * l);
        
        glGetMaterialfv(arg1i, arg2i, arg3fv);
        
        for (i = 0; i < l; i++) {
            Gua_IntegerToObject(o[i], arg3fv[i]);
        }
        
        Gua_Free(arg3fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetPointerv") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetString") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        resbv = glGetString(arg1i);
        
        Gua_StringToPObject(object, (const char *)resbv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetTexEnv") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        if (arg2i == GL_TEXTURE_ENV_MODE) {
            l = 1;
        } else if (arg2i == GL_TEXTURE_ENV_COLOR) {
            l = 4;
#ifndef _WINDOWS_
        } else if (arg2i == GL_COMBINE_RGB) {
            l = 1;
        } else if (arg2i == GL_COMBINE_ALPHA) {
            l = 1;
        } else if (arg2i == GL_SRC0_RGB) {
            l = 1;
        } else if (arg2i == GL_SRC1_RGB) {
            l = 1;
        } else if (arg2i == GL_SRC2_RGB) {
            l = 1;
        } else if (arg2i == GL_SRC0_ALPHA) {
            l = 1;
        } else if (arg2i == GL_SRC1_ALPHA) {
            l = 1;
        } else if (arg2i == GL_SRC2_ALPHA) {
            l = 1;
        } else if (arg2i == GL_OPERAND0_RGB) {
            l = 1;
        } else if (arg2i == GL_OPERAND1_RGB) {
            l = 1;
        } else if (arg2i == GL_OPERAND2_RGB) {
            l = 1;
        } else if (arg2i == GL_OPERAND0_ALPHA) {
            l = 1;
        } else if (arg2i == GL_OPERAND1_ALPHA) {
            l = 1;
        } else if (arg2i == GL_OPERAND2_ALPHA) {
            l = 1;
        } else if (arg2i == GL_RGB_SCALE) {
            l = 1;
#endif
        } else if (arg2i == GL_ALPHA_SCALE) {
            l = 1;
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg3fv = (GLfloat *)malloc(sizeof(GLfloat) * l);
        
        glGetTexEnvfv(arg1i, arg2i, arg3fv);
        
        for (i = 0; i < l; i++) {
            Gua_IntegerToObject(o[i], arg3fv[i]);
        }
        
        Gua_Free(arg3fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glGetTexParameter") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        if (arg2i == GL_TEXTURE_MIN_FILTER) {
            l = 1;
        } else if (arg2i == GL_TEXTURE_MAG_FILTER) {
            l = 1;
        } else if (arg2i == GL_TEXTURE_WRAP_S) {
            l = 1;
        } else if (arg2i == GL_TEXTURE_WRAP_T) {
            l = 1;
#ifndef _WINDOWS_
        } else if (arg2i == GL_GENERATE_MIPMAP) {
            l = 1;
#endif
        } else {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_MatrixToPObject(object, (struct Gua_Matrix *)Gua_Alloc(sizeof(Gua_Matrix)), l);
        
        m = (Gua_Matrix *)Gua_PObjectToMatrix(object);
        
        m->dimc = 1;
        m->dimv = Gua_Alloc(m->dimc * sizeof(Gua_Integer));
        m->dimv[0] = l;
        
        m->object = (struct Gua_Object *)Gua_Alloc(l * sizeof(Gua_Object));
        o = (Gua_Object *)m->object;
        
        arg3fv = (GLfloat *)malloc(sizeof(GLfloat) * l);
        
        glGetTexParameterfv(arg1i, arg2i, arg3fv);
        
        for (i = 0; i < l; i++) {
            Gua_IntegerToObject(o[i], arg3fv[i]);
        }
        
        Gua_Free(arg3fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glHint") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));           
        }
        
        glHint(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glIsBuffer") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glIsEnabled") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        Gua_IntegerToPObject(object, glIsEnabled(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glIsList") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        Gua_IntegerToPObject(object, glIsList(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glIsTexture") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        Gua_IntegerToPObject(object, glIsTexture(arg1i));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glLight") == 0) {
        if (argc != 4) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[3]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg3fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        if ((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL)) {
            glLightf(arg1i, arg2i, arg3f);
        } else {
            glLightfv(arg1i, arg2i, arg3fv);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glLightModel") == 0) {
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
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_HANDLE) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[2]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg2fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        if ((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
            glLightModelf(arg1i, arg2f);
        } else {
            glLightModelfv(arg1i, arg2fv);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glLineWidth") == 0) {
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
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        
        glLineWidth(arg1f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glListBase") == 0) {
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
            arg1d = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1d = Gua_ObjectToReal(argv[1]);            
        }
        
        glListBase(arg1d);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glLoadIdentity") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glLoadIdentity();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glLoadMatrix") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
	    if (!(Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glLoadMatrixf(arg1fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glLogicOp") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glLogicOp(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glMaterial") == 0) {
        if (argc != 4) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[3]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg3fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        if ((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL)) {
            glMaterialf(arg1i, arg2i, arg3f);
        } else {
            glMaterialfv(arg1i, arg2i, arg3fv);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glMatrixMode") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glMatrixMode(arg1i);
#ifndef _WINDOWS_
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glMultiTexCoord") == 0) {
        if (argc != 6) {
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
        if (!((Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4f = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4f = Gua_ObjectToReal(argv[4]);            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5f = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5f = Gua_ObjectToReal(argv[5]);            
        }
        
        glMultiTexCoord4f(arg1i, arg2f, arg3f, arg4f, arg5f);
#endif
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glMultMatrix") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
	    if (!(Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glMultMatrixf(arg1fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glNewList") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glNewList(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glNormal") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        
        glNormal3f(arg1f, arg2f, arg3f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glNormalPointer") == 0) {
        if (argc != 4) {
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
        if (!(Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        /*
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        */
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[3]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg3fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glNormalPointer(GL_FLOAT, arg2i, arg3fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glOrtho") == 0) {
        if (argc != 7) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4f = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4f = Gua_ObjectToReal(argv[4]);            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5f = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5f = Gua_ObjectToReal(argv[5]);            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6f = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6f = Gua_ObjectToReal(argv[6]);            
        }
        
        glOrtho(arg1f, arg2f, arg3f, arg4f, arg5f, arg6f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glPixelStore") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glPixelStorei(arg1i, arg2i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glPointParameter") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glPointSize") == 0) {
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
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        
        glPointSize(arg1f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glPolygonOffset") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        
        glPolygonOffset(arg1f, arg2f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glPopMatrix") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glPopMatrix();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glPushMatrix") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glPushMatrix();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glRasterPos") == 0) {
        if (!((argc >= 2) && (argc <= 5))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc >= 2) {
		    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
                arg1f = Gua_ObjectToInteger(argv[1]);
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
                arg1f = Gua_ObjectToReal(argv[1]);            
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
                h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
                
                if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
                
                arg1fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
            }
        }
        if (argc >= 3) {
		    if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
                arg2f = Gua_ObjectToInteger(argv[2]);
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
                arg2f = Gua_ObjectToReal(argv[2]);            
            }
        }
        if (argc >= 4) {
		    if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                arg3f = Gua_ObjectToInteger(argv[3]);
            } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
                arg3f = Gua_ObjectToReal(argv[3]);            
            }
        }
        if (argc == 5) {
            if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
                arg4f = Gua_ObjectToInteger(argv[4]);
            } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
                arg4f = Gua_ObjectToReal(argv[4]);            
            }
        }
        if (argc == 2) {
            if (Gua_ObjectLength(argv[1]) == 2) {
                glRasterPos2fv(arg1fv);
            } else if (Gua_ObjectLength(argv[1]) == 3) {
                glRasterPos3fv(arg1fv);
            } else if (Gua_ObjectLength(argv[1]) == 4) {
                glRasterPos4fv(arg1fv);
            }
        } else if (argc == 3) {
            glRasterPos2f(arg1f, arg2f);
        } else if (argc == 4) {
            glRasterPos3f(arg1f, arg2f, arg3f);
        } else if (argc == 5) {
            glRasterPos4f(arg1f, arg2f, arg3f, arg4f);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glReadPixels") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glRotate") == 0) {
        if (argc != 5) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4f = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4f = Gua_ObjectToReal(argv[4]);            
        }
        
        glRotatef(arg1f, arg2f, arg3f, arg4f);
#ifndef _WINDOWS_
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glSampleCoverage") == 0) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        
        glSampleCoverage(arg1f, arg2i);
#endif
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glScale") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        
        glScalef(arg1f, arg2f, arg3f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glScissor") == 0) {
        if (argc != 5) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        
        glScissor(arg1i, arg2i, arg3i, arg4i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glShadeModel") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glShadeModel(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glStencilFunc") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glStencilFunc(arg1i, arg2i, arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glStencilMask") == 0) {
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
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        
        glStencilMask(arg1i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glStencilOp") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        glStencilOp(arg1i, arg2i, arg3i);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexCoord") == 0) {
        if (!((argc >= 2) && (argc <= 5))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (argc >= 2) {
		    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
                arg1f = Gua_ObjectToInteger(argv[1]);
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
                arg1f = Gua_ObjectToReal(argv[1]);            
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
                h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
                
                if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
                
                arg1fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
            }
        }
        if (argc >= 3) {
		    if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
                arg2f = Gua_ObjectToInteger(argv[2]);
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
                arg2f = Gua_ObjectToReal(argv[2]);            
            }
        }
        if (argc >= 4) {
		    if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                arg3f = Gua_ObjectToInteger(argv[3]);
            } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
                arg3f = Gua_ObjectToReal(argv[3]);            
            }
        }
        if (argc == 5) {
            if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
                arg4f = Gua_ObjectToInteger(argv[4]);
            } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
                arg4f = Gua_ObjectToReal(argv[4]);            
            }
        }
        if (argc == 2) {
            if (Gua_ObjectLength(argv[1]) == 2) {
                glTexCoord2fv(arg1fv);
            } else if (Gua_ObjectLength(argv[1]) == 3) {
                glTexCoord3fv(arg1fv);
            } else if (Gua_ObjectLength(argv[1]) == 4) {
                glTexCoord4fv(arg1fv);
            }
        } else if (argc == 3) {
            glTexCoord2f(arg1f, arg2f);
        } else if (argc == 4) {
            glTexCoord3f(arg1f, arg2f, arg3f);
        } else if (argc == 5) {
            glTexCoord4f(arg1f, arg2f, arg3f, arg4f);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexCoordPointer") == 0) {
        if (argc != 5) {
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
        if (!(Gua_ObjectType(argv[4]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        /*
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        */
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[4]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg4fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glTexCoordPointer(arg1i, GL_FLOAT, arg3i, arg4fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexEnv") == 0) {
        if (argc != 4) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        
        glTexEnvf(arg1i, arg2i, arg3f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexGen") == 0) {
        if (argc != 4) {
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
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[3]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg3fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            glTexGenf(arg1i, arg2i, arg3f);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_HANDLE) {
            glTexGenfv(arg1i, arg2i, arg3fv);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexImage2D") == 0) {
        if (argc != 10) {
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
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[9]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6i = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6i = roundl(Gua_ObjectToReal(argv[6]));            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7i = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7i = roundl(Gua_ObjectToReal(argv[7]));            
        }
        if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) {
            arg8i = Gua_ObjectToInteger(argv[8]);
        } else if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL) {
            arg8i = roundl(Gua_ObjectToReal(argv[8]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[9]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg9bv = (GLbyte *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glTexImage2D(arg1i, arg2i, arg3i, arg4i, arg5i, arg6i, arg7i, arg8i, arg9bv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexParameter") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        
        glTexParameterf(arg1i, arg2i, arg3f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTexSubImage2D") == 0) {
        if (argc != 10) {
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
        if (!((Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 7 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 8 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[9]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_INTEGER) {
            arg5i = Gua_ObjectToInteger(argv[5]);
        } else if (Gua_ObjectType(argv[5]) == OBJECT_TYPE_REAL) {
            arg5i = roundl(Gua_ObjectToReal(argv[5]));            
        }
        if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_INTEGER) {
            arg6i = Gua_ObjectToInteger(argv[6]);
        } else if (Gua_ObjectType(argv[6]) == OBJECT_TYPE_REAL) {
            arg6i = roundl(Gua_ObjectToReal(argv[6]));            
        }
        if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_INTEGER) {
            arg7i = Gua_ObjectToInteger(argv[7]);
        } else if (Gua_ObjectType(argv[7]) == OBJECT_TYPE_REAL) {
            arg7i = roundl(Gua_ObjectToReal(argv[7]));            
        }
        if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_INTEGER) {
            arg8i = Gua_ObjectToInteger(argv[8]);
        } else if (Gua_ObjectType(argv[8]) == OBJECT_TYPE_REAL) {
            arg8i = roundl(Gua_ObjectToReal(argv[8]));            
        }
                
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[9]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfByte") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 9 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg9bv = (GLbyte *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glTexSubImage2D(arg1i, arg2i, arg3i, arg4i, arg5i, arg6i, arg7i, arg8i, arg9bv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glTranslate") == 0) {
        if (argc != 4) {
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
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1f = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1f = Gua_ObjectToReal(argv[1]);            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2f = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2f = Gua_ObjectToReal(argv[2]);            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        
        glTranslatef(arg1f, arg2f, arg3f);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glVertex") == 0) {
        if (!((argc >= 2) && (argc <= 5))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }

        if (argc >= 2) {
		    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
                arg1f = Gua_ObjectToInteger(argv[1]);
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
                arg1f = Gua_ObjectToReal(argv[1]);            
            } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
                h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
                
                if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
                    errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                    sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                    strcat(error, errMessage);
                    Gua_Free(errMessage);
                    
                    return GUA_ERROR;
                }
                
                arg1fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
            }
        }
        if (argc >= 3) {
		    if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
                arg2f = Gua_ObjectToInteger(argv[2]);
            } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
                arg2f = Gua_ObjectToReal(argv[2]);            
            }
        }
        if (argc >= 4) {
		    if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
                arg3f = Gua_ObjectToInteger(argv[3]);
            } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
                arg3f = Gua_ObjectToReal(argv[3]);            
            }
        }
        if (argc == 5) {
            if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
                arg4f = Gua_ObjectToInteger(argv[4]);
            } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
                arg4f = Gua_ObjectToReal(argv[4]);            
            }
        }
        if (argc == 2) {
            if (Gua_ObjectLength(argv[1]) == 2) {
                glVertex2fv(arg1fv);
            } else if (Gua_ObjectLength(argv[1]) == 3) {
                glVertex3fv(arg1fv);
            } else if (Gua_ObjectLength(argv[1]) == 4) {
                glVertex4fv(arg1fv);
            }
        } else if (argc == 3) {
            glVertex2f(arg1f, arg2f);
        } else if (argc == 4) {
            glVertex3f(arg1f, arg2f, arg3f);
        } else if (argc == 5) {
            glVertex4f(arg1f, arg2f, arg3f, arg4f);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glVertexPointer") == 0) {
        if (argc != 5) {
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
        if (!(Gua_ObjectType(argv[4]) == OBJECT_TYPE_HANDLE)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        /*
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        */
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[4]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "glArrayOfFloat") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg4fv = (GLfloat *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        glVertexPointer(arg1i, GL_FLOAT, arg3i, arg4fv);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glViewport") == 0) {
        if (argc != 5) {
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

        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            arg1i = Gua_ObjectToInteger(argv[1]);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            arg1i = roundl(Gua_ObjectToReal(argv[1]));            
        }
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3i = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3i = roundl(Gua_ObjectToReal(argv[3]));            
        }
        if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) {
            arg4i = Gua_ObjectToInteger(argv[4]);
        } else if (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL) {
            arg4i = roundl(Gua_ObjectToReal(argv[4]));            
        }
        
        glViewport(arg1i, arg2i, arg3i, arg4i);
    }

    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Gl_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Gl_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glActiveTexture", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glActiveTexture");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glAlphaFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glAlphaFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glArrayOfByte", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glArrayOfByte");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glArrayOfInt", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glArrayOfInt");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glArrayOfFloat", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glArrayOfFloat");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glBegin", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glBegin");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glBindBuffer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glBindBuffer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glBindTexture", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glBindTexture");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glBlendFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glBlendFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glBufferData", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glBufferData");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glBufferSubData", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glBufferSubData");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glCallList", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glCallList");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glClear", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glClear");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glClearColor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glClearColor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glClearDepth", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glClearDepth");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glClearStencil", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glClearStencil");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glClientActiveTexture", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glClientActiveTexture");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glClipPlane", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glClipPlane");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glColor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glColor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glColorMask", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glColorMask");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glColorMaterial", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glColorMaterial");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glColorPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glColorPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glCompressedTexImage2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glCompressedTexImage2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glCompressedTexSubImage2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glCompressedTexSubImage2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glCopyTexImage2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glCopyTexImage2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glCopyTexSubImage2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glCopyTexSubImage2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glCullFace", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glCullFace");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDeleteBuffers", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDeleteBuffers");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDeleteLists", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDeleteLists");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDeleteTextures", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDeleteTextures");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDepthFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDepthFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDepthMask", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDepthMask");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDepthRange", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDepthRange");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDisable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDisable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDisableClientState", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDisableClientState");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDrawArrays", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDrawArrays");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glDrawElements", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glDrawElements");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glEnable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glEnable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glEnableClientState", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glEnableClientState");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glEnd", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glEnd");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glEndList", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glEndList");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glFinish", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glFinish");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glFlush", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glFlush");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glFog", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glFog");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glFree", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glFree");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glFrontFace", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glFrontFace");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glFrustum", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glFrustum");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGenBuffers", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGenBuffers");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGenLists", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGenLists");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGenTextures", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGenTextures");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGet", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGet");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetBufferParameteriv", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetBufferParameteriv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetClipPlane", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetClipPlane");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetError", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetError");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetLight", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetLightfv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetMaterial", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetMaterialfv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetPointerv", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetPointerv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetTexEnv", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetTexEnvfv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glGetTexParameter", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glGetTexParameterfv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glHint", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glHint");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glIsBuffer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glIsBuffer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glIsEnabled", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glIsEnabled");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glIsList", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glIsList");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glIsTexture", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glIsTexture");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glLight", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glLight");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glLightModel", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glLightModel");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glLineWidth", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glLineWidth");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glListBase", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glListBase");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glLoadIdentity", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glLoadIdentity");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glLoadMatrix", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glLoadMatrix");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glLogicOp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glLogicOp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glMaterial", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glMaterial");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glMatrixMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glMatrixMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glMultiTexCoord", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glMultiTexCoord");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glMultMatrix", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glMultMatrix");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glNewList", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glNewList");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glNormal", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glNormal");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glNormalPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glNormalPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glOrtho", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glOrtho");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glPixelStore", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glPixelStore");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glPointParameter", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glPointParameter");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glPointSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glPointSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glPolygonOffset", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glPolygonOffset");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glPopMatrix", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glPopMatrix");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glPushMatrix", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glPushMatrix");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glRasterPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glRasterPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glReadPixels", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glReadPixels");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glRotate", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glRotate");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glSampleCoverage", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glSampleCoverage");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glScale", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glScale");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glScissor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glScissor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glShadeModel", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glShadeModel");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glStencilFunc", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glStencilFunc");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glStencilMask", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glStencilMask");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glStencilOp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glStencilOp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexCoord", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexCoord");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexCoordPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexCoordPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexEnv", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexEnv");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexGen", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexGen");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexImage2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexImage2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexParameter", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexParameter");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTexSubImage2D", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTexSubImage2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glTranslate", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glTranslate");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glVertex", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glVertex");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glVertexPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glVertexPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Gl_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glViewport", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glViewport");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }

    /* Define each extension constant... */
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_ACTIVE_TEXTURE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ACTIVE_TEXTURE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ACTIVE_TEXTURE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_ADD);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ADD", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ADD");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_ADD_SIGNED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ADD_SIGNED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ADD_SIGNED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALIASED_LINE_WIDTH_RANGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALIASED_LINE_WIDTH_RANGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALIASED_LINE_WIDTH_RANGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALIASED_POINT_SIZE_RANGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALIASED_POINT_SIZE_RANGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALIASED_POINT_SIZE_RANGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALPHA_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALPHA_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALPHA_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALPHA_SCALE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALPHA_SCALE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALPHA_SCALE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALPHA_TEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALPHA_TEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALPHA_TEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALPHA_TEST_FUNC);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALPHA_TEST_FUNC", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALPHA_TEST_FUNC");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALPHA_TEST_REF);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALPHA_TEST_REF", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALPHA_TEST_REF");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ALWAYS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ALWAYS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ALWAYS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_AMBIENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_AMBIENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_AMBIENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_AMBIENT_AND_DIFFUSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_AMBIENT_AND_DIFFUSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_AMBIENT_AND_DIFFUSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_AND);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_AND", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_AND");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_AND_INVERTED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_AND_INVERTED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_AND_INVERTED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_AND_REVERSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_AND_REVERSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_AND_REVERSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_ARRAY_BUFFER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ARRAY_BUFFER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ARRAY_BUFFER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ARRAY_BUFFER_BINDING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ARRAY_BUFFER_BINDING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ARRAY_BUFFER_BINDING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_BACK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BACK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BACK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_BLEND);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BLEND", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BLEND");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_BLEND_DST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BLEND_DST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BLEND_DST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_BLEND_SRC);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BLEND_SRC", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BLEND_SRC");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_BLUE_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BLUE_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BLUE_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_BUFFER_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BUFFER_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BUFFER_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_BUFFER_USAGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BUFFER_USAGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BUFFER_USAGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_BYTE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_BYTE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_BYTE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CCW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CCW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CCW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_CLAMP_TO_EDGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLAMP_TO_EDGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLAMP_TO_EDGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_CLEAR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLEAR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLEAR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_CLIENT_ACTIVE_TEXTURE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIENT_ACTIVE_TEXTURE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIENT_ACTIVE_TEXTURE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_CLIP_PLANE0);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIP_PLANE0", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIP_PLANE0");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CLIP_PLANE1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIP_PLANE1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIP_PLANE1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CLIP_PLANE2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIP_PLANE2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIP_PLANE2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CLIP_PLANE3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIP_PLANE3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIP_PLANE3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CLIP_PLANE4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIP_PLANE4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIP_PLANE4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CLIP_PLANE5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CLIP_PLANE5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CLIP_PLANE5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_ARRAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_ARRAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_ARRAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_COLOR_ARRAY_BUFFER_BINDING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_ARRAY_BUFFER_BINDING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_ARRAY_BUFFER_BINDING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_COLOR_ARRAY_POINTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_ARRAY_POINTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_ARRAY_POINTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_ARRAY_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_ARRAY_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_ARRAY_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_ARRAY_STRIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_ARRAY_STRIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_ARRAY_STRIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_ARRAY_TYPE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_ARRAY_TYPE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_ARRAY_TYPE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_BUFFER_BIT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_BUFFER_BIT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_BUFFER_BIT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_CLEAR_VALUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_CLEAR_VALUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_CLEAR_VALUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_LOGIC_OP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_LOGIC_OP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_LOGIC_OP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_MATERIAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_MATERIAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_MATERIAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COLOR_WRITEMASK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COLOR_WRITEMASK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COLOR_WRITEMASK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_COMBINE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COMBINE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COMBINE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COMBINE_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COMBINE_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COMBINE_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COMBINE_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COMBINE_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COMBINE_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COMPRESSED_TEXTURE_FORMATS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COMPRESSED_TEXTURE_FORMATS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COMPRESSED_TEXTURE_FORMATS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CONSTANT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CONSTANT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CONSTANT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_CONSTANT_ATTENUATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CONSTANT_ATTENUATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CONSTANT_ATTENUATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COPY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COPY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COPY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_COPY_INVERTED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_COPY_INVERTED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_COPY_INVERTED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CULL_FACE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CULL_FACE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CULL_FACE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CULL_FACE_MODE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CULL_FACE_MODE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CULL_FACE_MODE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CURRENT_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CURRENT_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CURRENT_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CURRENT_NORMAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CURRENT_NORMAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CURRENT_NORMAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CURRENT_TEXTURE_COORDS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CURRENT_TEXTURE_COORDS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CURRENT_TEXTURE_COORDS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_CW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_CW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_CW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DECAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DECAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DECAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DECR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DECR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DECR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_BUFFER_BIT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_BUFFER_BIT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_BUFFER_BIT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_CLEAR_VALUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_CLEAR_VALUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_CLEAR_VALUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_FUNC);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_FUNC", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_FUNC");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_RANGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_RANGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_RANGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_TEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_TEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_TEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DEPTH_WRITEMASK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DEPTH_WRITEMASK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DEPTH_WRITEMASK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DIFFUSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DIFFUSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DIFFUSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DITHER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DITHER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DITHER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DONT_CARE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DONT_CARE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DONT_CARE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_DOT3_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DOT3_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DOT3_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DOT3_RGBA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DOT3_RGBA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DOT3_RGBA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_DST_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DST_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DST_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_DST_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DST_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DST_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_DYNAMIC_DRAW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_DYNAMIC_DRAW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_DYNAMIC_DRAW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ELEMENT_ARRAY_BUFFER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ELEMENT_ARRAY_BUFFER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ELEMENT_ARRAY_BUFFER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ELEMENT_ARRAY_BUFFER_BINDING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ELEMENT_ARRAY_BUFFER_BINDING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ELEMENT_ARRAY_BUFFER_BINDING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_EMISSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_EMISSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_EMISSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_EQUAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_EQUAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_EQUAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_EQUIV);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_EQUIV", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_EQUIV");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_EXP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_EXP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_EXP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_EXP2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_EXP2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_EXP2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_EXTENSIONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_EXTENSIONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_EXTENSIONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FALSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FALSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FALSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FASTEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FASTEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FASTEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifdef _LINUX_
    Gua_IntegerToObject(object, GL_FIXED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FIXED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FIXED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_FLAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FLAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FLAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FLOAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FLOAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FLOAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG_DENSITY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG_DENSITY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG_DENSITY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG_END);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG_END", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG_END");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG_HINT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG_HINT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG_HINT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG_MODE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG_MODE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG_MODE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FOG_START);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FOG_START", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FOG_START");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FRONT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FRONT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FRONT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FRONT_AND_BACK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FRONT_AND_BACK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FRONT_AND_BACK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_FRONT_FACE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_FRONT_FACE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_FRONT_FACE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_GENERATE_MIPMAP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_GENERATE_MIPMAP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_GENERATE_MIPMAP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_GENERATE_MIPMAP_HINT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_GENERATE_MIPMAP_HINT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_GENERATE_MIPMAP_HINT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_GEQUAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_GEQUAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_GEQUAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_GREATER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_GREATER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_GREATER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_GREEN_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_GREEN_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_GREEN_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_INCR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_INCR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_INCR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_INTERPOLATE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_INTERPOLATE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_INTERPOLATE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_INVALID_ENUM);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_INVALID_ENUM", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_INVALID_ENUM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_INVALID_OPERATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_INVALID_OPERATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_INVALID_OPERATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_INVALID_VALUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_INVALID_VALUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_INVALID_VALUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_INVERT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_INVERT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_INVERT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_KEEP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_KEEP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_KEEP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LEQUAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LEQUAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LEQUAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LESS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LESS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LESS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT_MODEL_AMBIENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT_MODEL_AMBIENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT_MODEL_AMBIENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT_MODEL_TWO_SIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT_MODEL_TWO_SIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT_MODEL_TWO_SIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT0);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT0", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT0");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHT7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHT7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHT7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LIGHTING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LIGHTING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LIGHTING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINE_LOOP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINE_LOOP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINE_LOOP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINE_SMOOTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINE_SMOOTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINE_SMOOTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINE_SMOOTH_HINT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINE_SMOOTH_HINT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINE_SMOOTH_HINT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINE_STRIP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINE_STRIP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINE_STRIP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINE_WIDTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINE_WIDTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINE_WIDTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINEAR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINEAR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINEAR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINEAR_ATTENUATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINEAR_ATTENUATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINEAR_ATTENUATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINEAR_MIPMAP_LINEAR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINEAR_MIPMAP_LINEAR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINEAR_MIPMAP_LINEAR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINEAR_MIPMAP_NEAREST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINEAR_MIPMAP_NEAREST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINEAR_MIPMAP_NEAREST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LINES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LINES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LINES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LOGIC_OP_MODE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LOGIC_OP_MODE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LOGIC_OP_MODE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LUMINANCE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LUMINANCE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LUMINANCE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_LUMINANCE_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_LUMINANCE_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_LUMINANCE_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MATRIX_MODE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MATRIX_MODE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MATRIX_MODE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MAX_CLIP_PLANES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_CLIP_PLANES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_CLIP_PLANES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MAX_LIGHTS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_LIGHTS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_LIGHTS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MAX_MODELVIEW_STACK_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_MODELVIEW_STACK_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_MODELVIEW_STACK_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MAX_PROJECTION_STACK_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_PROJECTION_STACK_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_PROJECTION_STACK_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MAX_TEXTURE_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_TEXTURE_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_TEXTURE_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MAX_TEXTURE_STACK_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_TEXTURE_STACK_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_TEXTURE_STACK_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_MAX_TEXTURE_UNITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_TEXTURE_UNITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_TEXTURE_UNITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_MAX_VIEWPORT_DIMS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MAX_VIEWPORT_DIMS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MAX_VIEWPORT_DIMS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MODELVIEW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MODELVIEW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MODELVIEW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MODELVIEW_MATRIX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MODELVIEW_MATRIX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MODELVIEW_MATRIX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MODELVIEW_STACK_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MODELVIEW_STACK_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MODELVIEW_STACK_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_MODULATE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MODULATE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MODULATE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_MULTISAMPLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_MULTISAMPLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_MULTISAMPLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_NAND);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NAND", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NAND");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NEAREST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NEAREST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NEAREST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NEAREST_MIPMAP_LINEAR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NEAREST_MIPMAP_LINEAR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NEAREST_MIPMAP_LINEAR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NEAREST_MIPMAP_NEAREST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NEAREST_MIPMAP_NEAREST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NEAREST_MIPMAP_NEAREST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NEVER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NEVER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NEVER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NICEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NICEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NICEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NO_ERROR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NO_ERROR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NO_ERROR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NOOP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NOOP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NOOP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NORMAL_ARRAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NORMAL_ARRAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NORMAL_ARRAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_NORMAL_ARRAY_BUFFER_BINDING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NORMAL_ARRAY_BUFFER_BINDING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NORMAL_ARRAY_BUFFER_BINDING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_NORMAL_ARRAY_POINTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NORMAL_ARRAY_POINTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NORMAL_ARRAY_POINTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NORMAL_ARRAY_STRIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NORMAL_ARRAY_STRIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NORMAL_ARRAY_STRIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NORMAL_ARRAY_TYPE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NORMAL_ARRAY_TYPE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NORMAL_ARRAY_TYPE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NORMALIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NORMALIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NORMALIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_NOTEQUAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NOTEQUAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NOTEQUAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_NUM_COMPRESSED_TEXTURE_FORMATS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_NUM_COMPRESSED_TEXTURE_FORMATS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_NUM_COMPRESSED_TEXTURE_FORMATS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_ONE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ONE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ONE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ONE_MINUS_DST_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ONE_MINUS_DST_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ONE_MINUS_DST_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ONE_MINUS_DST_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ONE_MINUS_DST_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ONE_MINUS_DST_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ONE_MINUS_SRC_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ONE_MINUS_SRC_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ONE_MINUS_SRC_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ONE_MINUS_SRC_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ONE_MINUS_SRC_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ONE_MINUS_SRC_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_OPERAND0_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OPERAND0_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OPERAND0_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OPERAND0_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OPERAND0_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OPERAND0_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OPERAND1_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OPERAND1_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OPERAND1_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OPERAND1_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OPERAND1_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OPERAND1_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OPERAND2_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OPERAND2_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OPERAND2_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OPERAND2_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OPERAND2_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OPERAND2_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_OR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OR_INVERTED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OR_INVERTED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OR_INVERTED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OR_REVERSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OR_REVERSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OR_REVERSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_OUT_OF_MEMORY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_OUT_OF_MEMORY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_OUT_OF_MEMORY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_PACK_ALIGNMENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PACK_ALIGNMENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PACK_ALIGNMENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_PACK_ALIGNMENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PACK_ALIGNMENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PACK_ALIGNMENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_PERSPECTIVE_CORRECTION_HINT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PERSPECTIVE_CORRECTION_HINT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PERSPECTIVE_CORRECTION_HINT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_POINT_DISTANCE_ATTENUATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_DISTANCE_ATTENUATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_DISTANCE_ATTENUATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POINT_FADE_THRESHOLD_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_FADE_THRESHOLD_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_FADE_THRESHOLD_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_POINT_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_POINT_SIZE_MAX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_SIZE_MAX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_SIZE_MAX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POINT_SIZE_MIN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_SIZE_MIN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_SIZE_MIN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_POINT_SMOOTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_SMOOTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_SMOOTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POINT_SMOOTH_HINT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINT_SMOOTH_HINT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINT_SMOOTH_HINT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POINTS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POINTS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POINTS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POLYGON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POLYGON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POLYGON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POLYGON_OFFSET_FACTOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POLYGON_OFFSET_FACTOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POLYGON_OFFSET_FACTOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POLYGON_OFFSET_FILL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POLYGON_OFFSET_FILL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POLYGON_OFFSET_FILL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POLYGON_OFFSET_UNITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POLYGON_OFFSET_UNITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POLYGON_OFFSET_UNITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_POSITION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_POSITION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_POSITION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_PREVIOUS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PREVIOUS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PREVIOUS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_PRIMARY_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PRIMARY_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PRIMARY_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_PROJECTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PROJECTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PROJECTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_PROJECTION_MATRIX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PROJECTION_MATRIX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PROJECTION_MATRIX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_PROJECTION_STACK_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_PROJECTION_STACK_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_PROJECTION_STACK_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_QUADRATIC_ATTENUATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_QUADRATIC_ATTENUATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_QUADRATIC_ATTENUATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_QUADS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_QUADS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_QUADS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_QUAD_STRIP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_QUAD_STRIP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_QUAD_STRIP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_RED_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_RED_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_RED_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_RENDERER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_RENDERER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_RENDERER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_REPEAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_REPEAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_REPEAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_REPLACE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_REPLACE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_REPLACE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_RESCALE_NORMAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_RESCALE_NORMAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_RESCALE_NORMAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_RGB_SCALE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_RGB_SCALE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_RGB_SCALE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_RGBA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_RGBA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_RGBA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_SAMPLE_ALPHA_TO_COVERAGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLE_ALPHA_TO_COVERAGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLE_ALPHA_TO_COVERAGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SAMPLE_ALPHA_TO_ONE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLE_ALPHA_TO_ONE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLE_ALPHA_TO_ONE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SAMPLE_BUFFERS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLE_BUFFERS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLE_BUFFERS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SAMPLE_COVERAGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLE_COVERAGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLE_COVERAGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SAMPLE_COVERAGE_INVERT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLE_COVERAGE_INVERT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLE_COVERAGE_INVERT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SAMPLE_COVERAGE_VALUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLE_COVERAGE_VALUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLE_COVERAGE_VALUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SAMPLES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SAMPLES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SAMPLES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_SCISSOR_BOX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SCISSOR_BOX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SCISSOR_BOX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SCISSOR_TEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SCISSOR_TEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SCISSOR_TEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SCISSOR_TEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SCISSOR_TEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SCISSOR_TEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SET);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SET", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SET");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SHADE_MODEL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SHADE_MODEL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SHADE_MODEL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SHININESS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SHININESS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SHININESS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SHORT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SHORT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SHORT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SMOOTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SMOOTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SMOOTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_SMOOTH_LINE_WIDTH_RANGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SMOOTH_LINE_WIDTH_RANGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SMOOTH_LINE_WIDTH_RANGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SMOOTH_POINT_SIZE_RANGE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SMOOTH_POINT_SIZE_RANGE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SMOOTH_POINT_SIZE_RANGE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_SPECULAR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SPECULAR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SPECULAR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SPOT_CUTOFF);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SPOT_CUTOFF", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SPOT_CUTOFF");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SPOT_DIRECTION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SPOT_DIRECTION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SPOT_DIRECTION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SPOT_EXPONENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SPOT_EXPONENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SPOT_EXPONENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC_ALPHA_SATURATE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC_ALPHA_SATURATE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC_ALPHA_SATURATE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_SRC0_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC0_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC0_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC0_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC0_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC0_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC1_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC1_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC1_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC1_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC1_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC1_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC2_ALPHA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC2_ALPHA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC2_ALPHA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SRC2_RGB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SRC2_RGB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SRC2_RGB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_STACK_OVERFLOW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STACK_OVERFLOW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STACK_OVERFLOW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STACK_UNDERFLOW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STACK_UNDERFLOW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STACK_UNDERFLOW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_STATIC_DRAW);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STATIC_DRAW", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STATIC_DRAW");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_STENCIL_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_BUFFER_BIT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_BUFFER_BIT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_BUFFER_BIT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_CLEAR_VALUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_CLEAR_VALUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_CLEAR_VALUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_FAIL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_FAIL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_FAIL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_FUNC);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_FUNC", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_FUNC");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_PASS_DEPTH_FAIL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_PASS_DEPTH_FAIL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_PASS_DEPTH_FAIL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_PASS_DEPTH_PASS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_PASS_DEPTH_PASS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_PASS_DEPTH_PASS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_REF);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_REF", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_REF");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_TEST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_TEST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_TEST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_VALUE_MASK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_VALUE_MASK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_VALUE_MASK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_STENCIL_WRITEMASK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_STENCIL_WRITEMASK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_STENCIL_WRITEMASK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_SUBPIXEL_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SUBPIXEL_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SUBPIXEL_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_SUBTRACT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_SUBTRACT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_SUBTRACT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_TEXTURE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_2D);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_2D", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_BINDING_2D);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_BINDING_2D", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_BINDING_2D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_COORD_ARRAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_COORD_ARRAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_COORD_ARRAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_TEXTURE_COORD_ARRAY_POINTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_COORD_ARRAY_POINTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_COORD_ARRAY_POINTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_COORD_ARRAY_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_COORD_ARRAY_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_COORD_ARRAY_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_COORD_ARRAY_STRIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_COORD_ARRAY_STRIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_COORD_ARRAY_STRIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_COORD_ARRAY_TYPE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_COORD_ARRAY_TYPE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_COORD_ARRAY_TYPE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_ENV);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_ENV", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_ENV");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_ENV_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_ENV_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_ENV_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_ENV_MODE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_ENV_MODE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_ENV_MODE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_MAG_FILTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_MAG_FILTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_MAG_FILTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_MATRIX);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_MATRIX", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_MATRIX");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_MIN_FILTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_MIN_FILTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_MIN_FILTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_STACK_DEPTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_STACK_DEPTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_STACK_DEPTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_WRAP_S);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_WRAP_S", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_WRAP_S");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE_WRAP_T);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE_WRAP_T", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE_WRAP_T");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_TEXTURE0);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE0", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE0");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE10);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE10", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE11);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE11", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE11");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE12);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE12", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE12");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE13);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE13", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE13");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE14);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE14", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE14");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE15);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE15", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE15");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE16);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE16", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE16");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE17);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE17", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE17");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE18);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE18", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE18");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE19);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE19", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE19");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE20);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE20", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE20");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE21);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE21", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE21");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE22);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE22", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE22");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE23);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE23", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE23");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE24);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE24", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE24");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE25);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE25", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE25");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE26);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE26", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE26");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE27);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE27", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE27");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE28);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE28", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE28");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE29);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE29", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE29");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE30);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE30", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE30");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE31);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE31", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE31");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TEXTURE9);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TEXTURE9", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TEXTURE9");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_TRIANGLE_FAN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TRIANGLE_FAN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TRIANGLE_FAN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TRIANGLE_STRIP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TRIANGLE_STRIP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TRIANGLE_STRIP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TRIANGLES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TRIANGLES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TRIANGLES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_TRUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_TRUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_TRUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_UNPACK_ALIGNMENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_UNPACK_ALIGNMENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_UNPACK_ALIGNMENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_UNSIGNED_BYTE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_UNSIGNED_BYTE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_UNSIGNED_BYTE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_UNSIGNED_SHORT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_UNSIGNED_SHORT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_UNSIGNED_SHORT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_UNSIGNED_SHORT_4_4_4_4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_UNSIGNED_SHORT_4_4_4_4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_UNSIGNED_SHORT_4_4_4_4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_UNSIGNED_SHORT_5_5_5_1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_UNSIGNED_SHORT_5_5_5_1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_UNSIGNED_SHORT_5_5_5_1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_UNSIGNED_SHORT_5_6_5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_UNSIGNED_SHORT_5_6_5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_UNSIGNED_SHORT_5_6_5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_VENDOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VENDOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VENDOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_VERTEX_ARRAY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERTEX_ARRAY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERTEX_ARRAY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#ifndef _WINDOWS_
    Gua_IntegerToObject(object, GL_VERTEX_ARRAY_BUFFER_BINDING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERTEX_ARRAY_BUFFER_BINDING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERTEX_ARRAY_BUFFER_BINDING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
#endif
    Gua_IntegerToObject(object, GL_VERTEX_ARRAY_POINTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERTEX_ARRAY_POINTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERTEX_ARRAY_POINTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_VERTEX_ARRAY_SIZE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERTEX_ARRAY_SIZE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERTEX_ARRAY_SIZE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_VERTEX_ARRAY_STRIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERTEX_ARRAY_STRIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERTEX_ARRAY_STRIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_VERTEX_ARRAY_TYPE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VERTEX_ARRAY_TYPE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VERTEX_ARRAY_TYPE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_VIEWPORT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_VIEWPORT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_VIEWPORT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_XOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_XOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_XOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GL_ZERO);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GL_ZERO", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GL_ZERO");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_GL_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_GL_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_GL_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_GL_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

