/**
 * File:
 *     guash.c
 *
 * Package:
 *     Guash
 *
 * Description:
 *     This file implements a shell to the GuaraScript programming language.
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
 * RCS: @(#) $Id: guash.c,v 2.1 2015/10/22 09:43:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "array.h"
#include "complex.h"
#include "file.h"
#include "fs.h"
#ifdef _MPI_
    #include "mpi.h"
#endif
#ifdef _OPENGL_
    #include "gl.h"
#ifndef _GLWM_
    #include "glu.h"
#endif
#endif
#ifdef _GLUT_
    #include "glut.h"
#endif
#ifdef _GLWM_
    #include "glw.h"
#endif
#ifdef _OPENGL_
    #include "glf.h"
#endif
#ifdef _OPENGL_
    #include "glo.h"
#endif
#include "match.h"
#include "math.h"
#include "matrix.h"
#include "numeric.h"
#include "printf.h"
#include "regexp.h"
#include "sqlite.h"
#include "string.h"
#include "system.h"
#include "time.h"
#ifndef _WINDOWS_
    #include "tui.h"
#endif
#include "utf8.h"

#define SHELL_VERSION "2.1"

#define ERROR_SIZE 65536
#define GUA_SIZE  65536

int main(int argc, char *argv[], char **env)
{
    FILE *fp;
    Gua_String script;
    Gua_Length length;
    Gua_Namespace *nspace;
    Gua_String p;
    Gua_Object object;
    Gua_Object objectAsString;
    Gua_Status status;
    Gua_String error;
    Gua_String expr;
    Gua_Integer exitCode;
    
    exitCode = 0;
    
    nspace = (Gua_Namespace *)Gua_Alloc(sizeof(Gua_Namespace));
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    expr = (Gua_String)Gua_Alloc(sizeof(char) * GUA_SIZE);
    
    Gua_ClearNamespace(nspace);
    
    *error = '\0';
    
    status = Gua_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Array_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Complex_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = File_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Fs_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#ifdef _OPENGL_
    status = Gl_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#ifndef _GLWM_
    status = Glu_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
#endif
#ifdef _GLUT_
    status = Glut_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
#ifdef _GLWM_
    status = Glw_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
#ifdef _OPENGL_
    status = Glf_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
#ifdef _OPENGL_
    status = Glo_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
    status = Match_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Math_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Matrix_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#ifdef _MPI_
    status = Mpi_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
    status = Numeric_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Printf_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Regexp_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Sqlite_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = String_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = System_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    status = Time_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#ifndef _WINDOWS_
    status = Tui_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
#endif
    status = Utf8_Init(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    
    /* Setup the shell version variable. */
    Gua_LinkStringToObject(object, SHELL_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SHELL_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        printf("\nError: can't set variable SHELL_VERSION\n");
    }

    /* Setup the "last result" variable. */
    Gua_LinkStringToObject(object, " ");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "ans", &object, SCOPE_GLOBAL) != GUA_OK) {
        printf("\nError: can't set variable ans\n");
    }
    
    /* Cleanup the the expression return object. */
    Gua_ClearObject(object);
    
    /* Run a script. */
    if (argc > 1) {
        fp = fopen(argv[1], "r");
        
        if (fp == NULL) {
            printf("\nError: can not open file %s\n", argv[1]);
            Gua_FreeNamespace(nspace);
            Gua_Free(error);
            Gua_Free(expr);
            exit(1);
        }
        
        fseek(fp, 0, SEEK_END);
        length = ftell(fp) + 1;
        fseek(fp, 0, SEEK_SET);
        
        script = (Gua_String)Gua_Alloc(sizeof(char) * length + 1);
        memset(script, '\0', length + 1);
        
        if (length > 0) {
            fread(script, sizeof(char), length, fp);
            p = script;
            p = Gua_Evaluate(nspace, p, &object, &status, error);
        }
        
        fclose(fp);
        
        Gua_Free(script);
        
        if (!((status == GUA_OK) || (status == GUA_EXIT))) {
            printf("\nError: %s\n", error);
        }
    /* Enter the interactive mode. */
    } else {
        /* Show the copyright notice. */
        printf("\nguash(The GuaraScript Shell), version %s\n", SHELL_VERSION);
        printf("Copyright (C) 2005, 2012 Roberto Luiz Souza Monteiro.\n\n");
        printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
        printf("This is Gua_Free software, and you are welcome to\n");
        printf("redistribute it under certain conditions.\n");
        printf("See file COPYING for details.\n\n");
        printf("Expression interpreter, version %s\n\n", GUA_VERSION);
        printf("Built in libraries:\n");
        printf("Array library, version %s\n", ARRAY_VERSION);
        printf("Complex library, version %s\n", COMPLEX_VERSION);
        printf("File library, version %s\n", FILE_VERSION);
        printf("File system access library, version %s\n", FS_VERSION);
#ifdef _OPENGL_
        printf("OpenGL library, version %s\n", GUA_GL_VERSION);
#ifndef _GLWM_
        printf("GLU library, version %s\n", GUA_GLU_VERSION);
#endif
#endif
#ifdef _GLUT_
        printf("GLUT library, version %s\n", GUA_GLUT_VERSION);
#endif
#ifdef _GLWM_
        printf("GLWM library, version %s\n", GUA_GLWM_VERSION);
#endif
#ifdef _OPENGL_
        printf("GLF library, version %s\n", GUA_GLF_VERSION);
#endif
#ifdef _OPENGL_
        printf("GLO library, version %s\n", GUA_GLO_VERSION);
#endif
#ifdef _MPI_
        printf("MPI library, version %s\n", GUA_MPI_VERSION);
#endif
        printf("Match library, version %s\n", MATCH_VERSION);
        printf("Math library, version %s\n", MATH_VERSION);
        printf("Matrix library, version %s\n", MATRIX_VERSION);
        printf("Numeric library, version %s\n", NUMERIC_VERSION);
        printf("Printf library, version %s\n", PRINTF_VERSION);
        printf("Regexp library, version %s\n", REGEXP_VERSION);
        printf("SQLite library, version %s\n", GUA_SQLITE_VERSION);
        printf("String library, version %s\n", STRING_VERSION);
        printf("System library, version %s\n", SYSTEM_VERSION);
        printf("Time library, version %s\n", TIME_VERSION);
#ifndef _WINDOWS_
        printf("TUI library, version %s\n", TUI_VERSION);
#endif
        printf("UTF-8 library, version %s\n", UTF8_VERSION);
        
        while (true) {
            *error = '\0';
            *expr = '\0';
            
            printf(": ");
            
            if (!fgets(expr, GUA_SIZE - 1, stdin)) {
                break;
            }
            
            p = expr;
            
            p = Gua_Evaluate(nspace, p, &object, &status, error);
            
            if (status != GUA_OK) {
                if (status == GUA_RETURN) {
                    if (!Gua_IsObjectStored(object)) {
                        Gua_FreeObject(&object);
                    }
                }
                if (status == GUA_EXIT) {
                    break;
                }
                
                printf("\nError: %s", error);
            } else {
                if (Gua_ObjectType(object) == OBJECT_TYPE_INTEGER) {
                    printf("%ld\n", Gua_ObjectToInteger(object));
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_REAL) {
                    printf("%g\n", Gua_ObjectToReal(object));
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_STRING) {
                    printf("%s\n", Gua_ObjectToString(object));
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_COMPLEX) {
                    printf("%g%+g*i\n", Gua_ObjectToReal(object), Gua_ObjectToImaginary(object));
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_ARRAY) {
                    Gua_ClearObject(objectAsString);
                    Gua_ArrayToString(&object, &objectAsString);
                    printf("%s\n", Gua_ObjectToString(objectAsString));
                    Gua_FreeObject(&objectAsString);
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_MATRIX) {
                    Gua_ClearObject(objectAsString);
                    Gua_MatrixToString(&object, &objectAsString);
                    printf("%s\n", Gua_ObjectToString(objectAsString));
                    Gua_FreeObject(&objectAsString);
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_FILE) {
                    printf("File%lu\n", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(object)));
                } else if (Gua_ObjectType(object) == OBJECT_TYPE_HANDLE) {
                    printf("%s%lu\n", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(object)), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(object)));
                } else {
                    printf("\r");
                }
                if (Gua_SetVariable((Gua_Namespace *)nspace, "ans", &object, SCOPE_GLOBAL) != GUA_OK) {
                    printf("\nError: can't set variable ans\n");
                }
            }
        }
    }
    
    if (Gua_ObjectType(object) == OBJECT_TYPE_INTEGER) {
        exitCode = Gua_ObjectToInteger(object);
    }
    
    status = System_Finish(nspace, argc, argv, env, error);
    if (status != GUA_OK) {
        printf("\nError: %s\n", error);
    }
    
    Gua_FreeNamespace(nspace);
    Gua_Free(error);
    Gua_Free(expr);
    
    if (status == GUA_EXIT) {
        if (exitCode != 0) {
            exit(exitCode);
        }
    }
    
    return 0;
}

