/**
 * File:
 *     file.c
 *
 * Package:
 *     File
 *
 * Description:
 *     This library implements a virtual file system for an expression interpreter.
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
 * RCS: @(#) $Id: file.c,v 1.3 2013/09/24 19:28:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "file.h"

#define BUFFER_SIZE 65536

/**
 * Group:
 *     C
 *
 * Function:
 *     void File_Clearerr(void *h)
 *
 * Description:
 *     File functions(clearerr) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
void File_Clearerr(void *h)
{
    clearerr((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Close(void *h)
 *
 * Description:
 *     File functions(fclose) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Close(void *h)
{
    Gua_Short res;
    
    res = fclose((FILE *)Gua_GetFileHandle((Gua_File *)h));
    
    Gua_SetFileHandle((Gua_File *)h, NULL);
    
    return res;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Eof(void *h)
 *
 * Description:
 *     File functions(feof) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Eof(void *h)
{
    return feof((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Error(void *h)
 *
 * Description:
 *     File functions(ferror) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Error(void *h)
{
    return ferror((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Fileno(void *h)
 *
 * Description:
 *     File functions(fileno) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Fileno(void *h)
{
    return fileno((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Flush(void *h)
 *
 * Description:
 *     File functions(fflush) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Flush(void *h)
{
    return fflush((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String File_Gets(Gua_String s, Gua_Short l, void *h)
 *
 * Description:
 *     File functions(fgets) wrapper.
 *
 * Arguments:
 *     s,    pointer to a string buffer;
 *     l,    length;
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_String File_Gets(Gua_String s, Gua_Short l, void *h)
{
    return fgets(s, l, (FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_File *File_Open(Gua_String p, Gua_String m)
 *
 * Description:
 *     File functions(fopen) wrapper.
 *
 * Arguments:
 *     p,    file path;
 *     m,    access mode.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_File *File_Open(Gua_String p, Gua_String m)
{
    Gua_File *h;
    FILE *fp;
    
    fp = fopen(p, m);
    
    Gua_NewFile(h, fp);
    
    Gua_SetFileClearerr(h, File_Clearerr);
    Gua_SetFileClose(h, File_Close);
    Gua_SetFileEof(h, File_Eof);
    Gua_SetFileError(h, File_Error);
    Gua_SetFileFileno(h, File_Fileno);
    Gua_SetFileFlush(h, File_Flush);
    Gua_SetFileGets(h, File_Gets);
    Gua_SetFilePuts(h, File_Puts);
    Gua_SetFileRead(h, File_Read);
    Gua_SetFileRewind(h, File_Rewind);
    Gua_SetFileSeek(h, File_Seek);
    Gua_SetFileTell(h, File_Tell);
    Gua_SetFileWrite(h, File_Write);
    
    return h;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Puts(Gua_String s, void *h)
 *
 * Description:
 *     File functions(fputs) wrapper.
 *
 * Arguments:
 *     s,    pointer to a string buffer;
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Puts(Gua_String s, void *h)
{
    return fputs(s, (FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer File_Read(Gua_String s, Gua_Integer l, Gua_Integer n, void *h)
 *
 * Description:
 *     File functions(fread) wrapper.
 *
 * Arguments:
 *     s,    pointer to a string buffer;
 *     l,    length;
 *     n,    number of units of l length;
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Integer File_Read(Gua_String s, Gua_Integer l, Gua_Integer n, void *h)
{
    return fread(s, l, n, (FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void File_Rewind(void *h)
 *
 * Description:
 *     File functions(rewind) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
void File_Rewind(void *h)
{
    return rewind((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Short File_Seek(void *h, Gua_Integer o, Gua_Short w)
 *
 * Description:
 *     File functions(fseek) wrapper.
 *
 * Arguments:
 *     h,    file handle;
 *     o,    offset;
 *     w,    whence.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Short File_Seek(void *h, Gua_Integer o, Gua_Short w)
{
    return fseek((FILE *)Gua_GetFileHandle((Gua_File *)h), o, w);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer File_Tell(void *h)
 *
 * Description:
 *     File functions(ftell) wrapper.
 *
 * Arguments:
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Integer File_Tell(void *h)
{
    return ftell((FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer File_Write(Gua_String, Gua_Integer, Gua_Integer, void *h)
 *
 * Description:
 *     File functions(fwrite) wrapper.
 *
 * Arguments:
 *     s,    pointer to a string buffer;
 *     l,    length;
 *     n,    number of units of l length;
 *     h,    file handle.
 *
 * Results:
 *     Do the specified file operation.
 */
Gua_Integer File_Write(Gua_String s, Gua_Integer l, Gua_Integer n, void *h)
{
    return fwrite(s, l, n, (FILE *)Gua_GetFileHandle((Gua_File *)h));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status File_FileFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     File functions wrapper.
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
 *     Do file operations.
 */
Gua_Status File_FileFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_File *h;
    Gua_String buffer;
    Gua_Integer n;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
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
     *     clearerr(file_pointer)
     *
     * Description:
     *     Clears the internal error control variable of file handling.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "clearerr") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        File_Clearerr(Gua_ObjectToFile(argv[1]));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fclose(file_pointer)
     *
     * Description:
     *     Closes the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fclose") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (File_Close(Gua_ObjectToFile(argv[1])) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not close file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     feof(file_pointer)
     *
     * Description:
     *     Returns TRUE if the end of file has been reached.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "feof") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, File_Eof(Gua_ObjectToFile(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     ferror(file_pointer)
     *
     * Description:
     *     Returns the error code occurred accessing the file
     *     indicated by the specified file pointer.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ferror") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, File_Error(Gua_ObjectToFile(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fflush(file_pointer)
     *
     * Description:
     *     Flushes the buffer of the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fflush") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (File_Flush(Gua_ObjectToFile(argv[1])) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not flush the file buffer");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fgets(n, file_pointer)
     *
     * Description:
     *     Reads n characters of the file, or to find a character newline.
     *     The newline character is dependent on the underlying operating system: 
     *     UNIX, LF; Windows, CR + LF; MacOS, CR.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fgets") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
        
        if (File_Gets(buffer, BUFFER_SIZE, Gua_ObjectToFile(argv[1])) != NULL) {
            Gua_StringToPObject(object, buffer);
        }
        
        Gua_Free(buffer);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fileno(file_pointer)
     *
     * Description:
     *     Returns the file indicator for the specified file pointer.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fileno") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, File_Fileno(Gua_ObjectToFile(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fopen(file, mode)
     *
     * Description:
     *     Opens the specified file, with the access mode determined, and returns
     *     a file pointer to it. The access mode may be determined by any
     *     combination of reading mode ("r"), write ("r") or attaching ("a").
     *     Also update modes are supported for reading ("r+"), writing ("w+")
     *     or attachment the ("a+"). To open the file in binary mode
     *     should use the suffix "b", thereby reading modes ("rb"),
     *     written ("wb") or attachment ("ab") are also supported.
     *
     * Examples:
     *     h=fopen("test.txt","w") # Open the file in write mode.
     *     fputs("Hello World!",h) # Write to the file.
     *     h=fclose(h)             # Close the file, and destroy the variable.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fopen") == 0) {
        if ((argc < 2) || (argc > 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc == 2) {
            h = File_Open(Gua_ObjectToString(argv[1]), "r");
            
            if (Gua_GetFileHandle(h) == NULL) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "could not open file", Gua_ObjectToString(argv[1]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        } else {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            h = File_Open(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2]));
            
            if (Gua_GetFileHandle(h) == NULL) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "could not open file", Gua_ObjectToString(argv[1]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }

        Gua_FileToPObject(object, (struct Gua_File *)h);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fputs(string, file_pointer)
     *
     * Description:
     *     Writes the specified string in the file and includes a
     *     newline character to the end of it.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fputs") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (File_Puts(Gua_ObjectToString(argv[1]), Gua_ObjectToFile(argv[2])) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not write to file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fread(n, file_pointer)
     *
     * Description:
     *     Reads n characters from the file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fread") == 0) {
        if (argc != 3) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (char *)Gua_Alloc(sizeof(char) * (Gua_ObjectToInteger(argv[1]) + 1));
        memset(buffer, '\0', Gua_ObjectToInteger(argv[1]) + 1);
        
        n = File_Read(buffer, sizeof(char), Gua_ObjectToInteger(argv[1]), Gua_ObjectToFile(argv[2]));
        
        Gua_ByteArrayToPObject(object, buffer, n);
        
        Gua_Free(buffer);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fseek(file_pointer, position, reference)
     *
     * Description:
     *     Positions the read pointer and write the file in specified position
     *     on the indicated reference. The Reference can be:
     *     SEEK_SET, for the beginning of the file; SEEK_CUR, for the current
     *     position in the file and SEEK_END, for the end of the file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fseek") == 0) {
        if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
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
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (File_Seek(Gua_ObjectToFile(argv[1]), Gua_ObjectToInteger(argv[2]), Gua_ObjectToInteger(argv[3])) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %ld\n", "could not set file position to ", Gua_ObjectToInteger(argv[2]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     ftell(file_pointer)
     *
     * Description:
     *     Returns the current position in the file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "ftell") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, File_Tell(Gua_ObjectToFile(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fwrite(string, file_pointer)
     *
     * Description:
     *     Writes the specified string in the file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fwrite") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (File_Write(Gua_ObjectToString(argv[1]), sizeof(char), Gua_ObjectLength(argv[1]), Gua_ObjectToFile(argv[2])) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not write to file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     getchar()
     *
     * Description:
     *     Reads the next character of stdin.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "getchar") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (char *)Gua_Alloc(sizeof(char) * 2);
        
        if (!(buffer[0] = getchar())) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not read from file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(buffer);
            
            return GUA_ERROR;
        }
        buffer[1] = '\0';
        
        Gua_StringToPObject(object, buffer);
        
        Gua_Free(buffer);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     gets()
     *
     * Description:
     *     Reads a string from the console to find a newline character.
     *     The newline character is dependent on the underlying operating system:
     *     UNIX, LF; Windows, CR + LF, MacOS, CR.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "gets") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        buffer = (char *)Gua_Alloc(sizeof(char) * BUFFER_SIZE);
        
        if (!fgets(buffer, BUFFER_SIZE, stdin)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not read from file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            Gua_Free(buffer);
            
            return GUA_ERROR;
        }
        
        Gua_StringToPObject(object, buffer);

        Gua_Free(buffer);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     putchar(character)
     *
     * Description:
     *     Write a character to stdout.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "putchar") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectLength(argv[1]) != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not put more than one character at time");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (putchar(Gua_ObjectToString(argv[1])[0]) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not write to file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     puts(string)
     *
     * Description:
     *     Write the specified string in the console and includes a
     *     newline character to the end of it.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "puts") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (puts(Gua_ObjectToString(argv[1])) < 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not write to file");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     rewind(file_pointer)
     *
     * Description:
     *      Returns the read / write pointer to the beginning of the file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "rewind") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_FILE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        File_Rewind(Gua_ObjectToFile(argv[1]));
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
 *     Gua_Status File_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the system functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the system functions.
 */
Gua_Status File_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_File *h;
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, File_FileFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "clearerr", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "clearerr");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fclose", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fclose");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "feof", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "feof");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ferror", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ferror");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fflush", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fflush");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fgets", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fgets");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fileno", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fileno");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fopen", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fopen");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fputs", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fputs");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fread", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fread");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fseek", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fseek");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "ftell", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "ftell");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fwrite", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fwrite");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "getchar", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "getchar");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "gets", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "gets");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "putchar", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "putchar");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "puts", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "puts");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "rewind", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "rewind");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     stdin
     *
     * Description:
     *     Standard input.
     */
    Gua_NewFile(h, stderr);
    Gua_FileToObject(object, (struct Gua_File *)h);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "stderr", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "stderr");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     stdout
     *
     * Description:
     *     Standard output.
     */
    Gua_NewFile(h, stdin);
    Gua_FileToObject(object, (struct Gua_File *)h);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "stdin", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "stdin");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     stderr
     *
     * Description:
     *     Standard error output.
     */
    Gua_NewFile(h, stdout);
    Gua_FileToObject(object, (struct Gua_File *)h);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "stdout", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "stdout");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     EOF
     *
     * Description:
     *     End of file.
     */
    Gua_IntegerToObject(object, -1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "EOF", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "EOF");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     SEEK_END
     *
     * Description:
     *     Reference to the end of the file.
     */
    Gua_IntegerToObject(object, SEEK_END);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SEEK_END", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SEEK_END");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     SEEK_CUR
     *
     * Description:
     *     Reference to the current position in the file.
     */
    Gua_IntegerToObject(object, SEEK_CUR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SEEK_CUR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SEEK_CUR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     SEEK_SET
     *
     * Description:
     *     Reference to the beginning of the file.
     */
    Gua_IntegerToObject(object, SEEK_SET);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SEEK_SET", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SEEK_SET");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     FILE_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, FILE_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "FILE_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "FILE_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

