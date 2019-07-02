/**
 * File:
 *     fs.c
 *
 * Package:
 *     Fs
 *
 * Description:
 *     This library implements the file system access functions.
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
 * RCS: @(#) $Id: fs.c,v 2.0 2017/05/26 21:16:00 monteiro Exp $
 * 
 */

#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WINDOWS_
    #include <direct.h> 
    #include <io.h>
    #define F_OK 0
    #define W_OK 2
    #define R_OK 4
    #define access(p,m) _access((p),(m))
#else
    #include <unistd.h>
#endif
#include "interp.h"
#include "fs.h"

#ifdef _WINDOWS_
    #include <windows.h>
    #define realpath(f,p) _fullpath((p),(f),_MAX_PATH)
#endif

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_Copy(Gua_String source, Gua_String target, Gua_String error)
 *
 * Description:
 *     Copy source file to target file.
 *
 * Arguments:
 *     source,    the source file name;
 *     target,    the target file name;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Copy source file to target file.
 */
Gua_Status Fs_Copy(Gua_String source, Gua_String target, Gua_String error)
{
    FILE *fs;
    FILE *ft;
    Gua_Char c;
    Gua_String errMessage;
    
    /* Open input and output files in binary mode for Windows compatibility. */
    if ((fs = fopen(source, "rb")) == NULL) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "could not open file", source);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if ((ft = fopen(target, "wb")) == NULL) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "could not open file", target);
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    while (!feof(fs)) {
        fread(&c, sizeof(Gua_Char), 1, fs);
        fwrite(&c, sizeof(Gua_Char), 1, ft);
    }
    
    fclose(fs);
    fclose(ft);
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_Delete(Gua_String file, Gua_String error)
 *
 * Description:
 *     Delete the specified file.
 *
 * Arguments:
 *     file,    file name;
 *     error,   a pointer to the error message.
 *
 * Results:
 *     Delete the specified file.
 */
Gua_Status Fs_Delete(Gua_String file, Gua_String error)
{
    Gua_String errMessage;
    
    if (remove(file) != 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can not remove file", file);
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
 *     Gua_Integer Fs_Exists(Gua_String file)
 *
 * Description:
 *     Check if the specified file exists.
 *
 * Arguments:
 *     file,    file name.
 *
 * Results:
 *     Return true if the specified file exists.
 */
Gua_Integer Fs_Exists(Gua_String file)
{
    Gua_Short fileMode;
    
    if (strlen(file) > 0) {
        fileMode = F_OK;
        
        if (access(file, fileMode) == -1) {
            return 0;
        }
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Fs_Extension(Gua_String file, Gua_String *extension)
 *
 * Description:
 *     Get the file extenxion.
 *
 * Arguments:
 *     file,         the full file name;
 *     extension,    the file extension.
 *
 * Results:
 *     Return the file extenxion.
 */
void Fs_Extension(Gua_String file, Gua_String *extension)
{
    Gua_Short i;
    Gua_Short j;
    Gua_Short k;
    Gua_Short patternFound;
    Gua_String newString;
    
    if (strlen(file) > 0) {
        /* Find the extension separator. */
        patternFound = 0;
        for (i = strlen(file) - 1; i > 0; i--) {
            if ((file[i] == '\\') || (file[i] == '/')) {
                break;
            }
            if (file[i] == '.') {
                patternFound = 1;
                break;
            }
        }
        
        if (patternFound) {
            /* Skip the extension separator. */
            if (file[i] == '.') {
                i++;
            }
            
            /* Get the file name. */
            newString = (Gua_String)malloc(sizeof(Gua_Char) * strlen(file) + 1);
            k = 0;
            for (j = i; j < strlen(file); j++) {
                newString[k] = file[j];
                k++;
            }
            newString[k] = '\0';
        } else {
            newString = (Gua_String)malloc(sizeof(Gua_Char));
            newString[0] = '\0';
        }
    } else {
        newString = (Gua_String)malloc(sizeof(Gua_Char));
        newString[0] = '\0';
    }
    
    *extension = newString;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Fs_FullPath(Gua_String file, Gua_String *path)
 *
 * Description:
 *     Get the file extenxion.
 *
 * Arguments:
 *     file,    the full file name;
 *     path,    the file full path.
 *
 * Results:
 *     Return the file full path.
 */
void Fs_FullPath(Gua_String file, Gua_String *path)
{
    Gua_Short i;
    Gua_String realPath;
    Gua_String unixPath;
    
    realPath = realpath(file, NULL);
    
    /* Convert Windows path to Unix path. */
    if (realPath != NULL) {
        unixPath = (Gua_String)malloc(sizeof(Gua_Char) * strlen(realPath) + 1);
        for (i = 0; i < strlen(realPath); i++) {
            unixPath[i] = realPath[i] == '\\' ? '/' : realPath[i];
        }
        unixPath[i] = '\0';
        
        Gua_Free(realPath);
        
        *path = unixPath;
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Fs_IsDirectory(Gua_String file)
 *
 * Description:
 *     Check if the specified file is a directory.
 *
 * Arguments:
 *     file,    file name.
 *
 * Results:
 *     Return true if the specified file is a directory.
 */
Gua_Integer Fs_IsDirectory(Gua_String file)
{
    struct stat fileStat;
    
    if (strlen(file) > 0) {
        if (stat(file, &fileStat) == -1) {
            return 0;
        }
        
        if (S_ISDIR(fileStat.st_mode)) {
            return 1;
        }
    }
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Fs_IsExecutable(Gua_String file)
 *
 * Description:
 *     Check if the specified file is executable.
 *
 * Arguments:
 *     file,    file name.
 *
 * Results:
 *     Return true if the specified file is executable.
 */
Gua_Integer Fs_IsExecutable(Gua_String file)
{
#ifndef _WINDOWS_
    Gua_Short fileMode;
#endif
    
    if (strlen(file) > 0) {
#ifdef _WINDOWS_
        if ((strstr(file, ".bat") != NULL) || (strstr(file, ".com") != NULL) || (strstr(file, ".exe") != NULL)) {
            return 1;
        }
#else
        fileMode = X_OK;
        
        if (access(file, fileMode) != -1) {
            return 1;
        }
#endif
    }
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Fs_IsReadable(Gua_String file)
 *
 * Description:
 *     Check if the specified file is readable.
 *
 * Arguments:
 *     file,    file name.
 *
 * Results:
 *     Return true if the specified file is readable.
 */
Gua_Integer Fs_IsReadable(Gua_String file)
{
    Gua_Short fileMode;
    
    if (strlen(file) > 0) {
        fileMode = R_OK;
        
        if (access(file, fileMode) == -1) {
            return 0;
        }
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Fs_IsWritable(Gua_String file)
 *
 * Description:
 *     Check if the specified file is writable.
 *
 * Arguments:
 *     file,    file name.
 *
 * Results:
 *     Return true if the specified file is writable.
 */
Gua_Integer Fs_IsWritable(Gua_String file)
{
    Gua_Short fileMode;
    
    if (strlen(file) > 0) {
        fileMode = W_OK;
        
        if (access(file, fileMode) == -1) {
            return 0;
        }
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Fs_Link(Gua_String source, Gua_String target)
 *
 * Description:
 *     Create a link to the source file.
 *
 * Arguments:
 *     source,    the source file name;
 *     target,    the target file name.
 *
 * Results:
 *     Create a link to the source file.
 */
Gua_Integer Fs_Link(Gua_String source, Gua_String target)
{
    if ((strlen(source) > 0) && (strlen(target) > 0)) {
#ifdef _WINDOWS_
        return 0;
#else
        if (symlink(source, target) == -1) {
            return 0;
        }
#endif
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_List(Gua_String directory, Gua_Object *list, Gua_String error)
 *
 * Description:
 *     Get the directory file list.
 *
 * Arguments:
 *     directory,    the directory name;
 *     list,         directory file list;
 *     error,        a pointer to the error message.
 *
 * Results:
 *     Return the directory file list.
 */
Gua_Status Fs_List(Gua_String directory, Gua_Object *list, Gua_String error)
{
    Gua_Short i;
    DIR *dirPointer;
    struct dirent *dirEntry;
    Gua_Element *previous;
    Gua_Element *newElement;
    Gua_String errMessage;
    
    if (strlen(directory) > 0) {
        if ((dirPointer = opendir(directory)) == NULL) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "could not open directory", directory);
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        i = 0;
        
        while ((dirEntry = readdir(dirPointer)) != NULL) {
            if (i == 0) {
                /* Create the first element. */
                newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                /* The element key. */
                Gua_IntegerToObject(newElement->key, i);
                /* The element object. */
                Gua_StringToObject(newElement->object, dirEntry->d_name);
                
                /* Set the target array chain. */
                newElement->previous = NULL;
                newElement->next = NULL;
                
                /* Link the first element. */
                Gua_ArrayToPObject(list, (struct Gua_Element *)newElement, 1);
                
                previous = (Gua_Element *)Gua_PObjectToArray(list);
            } else {
                /* Create a new element. */
                newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                /* The element key. */
                Gua_IntegerToObject(newElement->key, i);
                /* The element object. */
                Gua_StringToObject(newElement->object, dirEntry->d_name);
                
                /* Set the target array chain. */
                newElement->previous = (struct Gua_Element *)previous;
                newElement->next = NULL;
                
                previous->next = (struct Gua_Element *)newElement;
                previous = newElement;
            }
            
            i++;
        }
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(list, i);
        
        closedir(dirPointer);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Fs_MkDir(Gua_String directory)
 *
 * Description:
 *     Create the specified directory.
 *
 * Arguments:
 *     directory,    directory name.
 *
 * Results:
 *     Create the specified directory.
 */
Gua_Integer Fs_MkDir(Gua_String directory)
{
    if (strlen(directory) > 0) {
#ifdef _WINDOWS_
        if (_mkdir(directory) == -1) {
            return 0;
        }
#else
        if (mkdir(directory, 0777) == -1) {
            return 0;
        }
#endif
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Fs_Name(Gua_String file, Gua_String *name)
 *
 * Description:
 *     Get the file name without path.
 *
 * Arguments:
 *     file,     the file name;
 *     name,     the file name without path.
 *
 * Results:
 *     Get the file name without path.
 */
void Fs_Name(Gua_String file, Gua_String *name)
{
    Gua_Short i;
    Gua_Short j;
    Gua_Short k;
    Gua_String newString;
    
    if (strlen(file) > 0) {
        /* Find the directory separator. */
        for (i = strlen(file) - 1; i > 0; i--) {
            if ((file[i] == '\\') || (file[i] == '/')) {
                break;
            }
        }
        
        /* Skip the directory separator. */
        if ((file[i] == '\\') || (file[i] == '/')) {
            i++;
        }
        
        /* Get the file name. */
        newString = (Gua_String)malloc(sizeof(Gua_Char) * strlen(file) + 1);
        k = 0;
        for (j = i; j < strlen(file); j++) {
            newString[k] = file[j];
            k++;
        }
        newString[k] = '\0';
    } else {
        newString = (Gua_String)malloc(sizeof(Gua_Char));
        newString[0] = '\0';
    }
    
    *name = newString;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Fs_Path(Gua_String file, Gua_String *path)
 *
 * Description:
 *     Get the file path.
 *
 * Arguments:
 *     file,     the file name;
 *     path,     the file path.
 *
 * Results:
 *     Get the file path.
 */
void Fs_Path(Gua_String file, Gua_String *path)
{
    Gua_Short i;
    Gua_Short j;
    Gua_String newString;
    
    if (strlen(file) > 0) {
        /* Find the directory separator. */
        for (i = strlen(file) - 1; i > 0; i--) {
            if (((file[i] == '\\') || (file[i] == '/')) && (i != (strlen(file) - 1))) {
                break;
            }
        }
        
        /* Get the file path. */
        newString = (Gua_String)malloc(sizeof(Gua_Char) * strlen(file) + 1);
        for (j = 0; j < i; j++) {
            newString[j] = file[j];
        }
        newString[j] = '\0';
    } else {
        newString = (Gua_String)malloc(sizeof(Gua_Char));
        newString[0] = '\0';
    }
    
    *path = newString;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_Rename(Gua_String source, Gua_String target, Gua_String error)
 *
 * Description:
 *     Rename a file.
 *
 * Arguments:
 *     source,    the original file name;
 *     target,    the new file name;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Rename the specified file.
 */
Gua_Status Fs_Rename(Gua_String source, Gua_String target, Gua_String error)
{
    Gua_String errMessage;
    
    if ((strlen(source) > 0) && (strlen(target) > 0)) {
        if (rename(source, target) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "can not rename file", source);
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
 *     Gua_Integer Fs_Size(Gua_String file)
 *
 * Description:
 *     Get the file size.
 *
 * Arguments:
 *     file,    the file name.
 *
 * Results:
 *     Return the file size.
 */
Gua_Integer Fs_Size(Gua_String file)
{
    struct stat fileStat;
    
    if (strlen(file) > 0) {
        if (stat(file, &fileStat) == -1) {
            return -1;
        }
        
        return (Gua_Integer)(fileStat.st_size);
    }
    
    return -1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Real Fs_Time(Gua_String file)
 *
 * Description:
 *     Get the modification file time.
 *
 * Arguments:
 *     file,    the file name.
 *
 * Results:
 *     Return the modification file time.
 */
Gua_Real Fs_Time(Gua_String file)
{
    struct stat fileStat;
    
    if (strlen(file) > 0) {
        if (stat(file, &fileStat) == -1) {
            return -1;
        }
        
        return (Gua_Real)(fileStat.st_mtime);
    }
    
    return -1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Fs_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String newString;
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
     *     fsCopy(source, target)
     *
     * Description:
     *     Copy (binary) source file to target file.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "fsCopy") == 0) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        return Fs_Copy(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2]), error);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsDelete(file)
     *
     * Description:
     *     Delete the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsDelete") == 0) {
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
        
        return Fs_Delete(Gua_ObjectToString(argv[1]), error);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsExists(file)
     *
     * Description:
     *     Return TRUE if the specified file exists.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsExists") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_Exists(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsExtension(file)
     *
     * Description:
     *     Return the file name extension.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsExtension") == 0) {
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
        
        Fs_Extension(Gua_ObjectToString(argv[1]), &newString);
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsFullPath(file)
     *
     * Description:
     *     Return the full path for the specified file name.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsFullPath") == 0) {
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
        
        Fs_FullPath(Gua_ObjectToString(argv[1]), &newString);
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsDirectory(file)
     *
     * Description:
     *     Return TRUE if the specified file is a directory.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsDirectory") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_IsDirectory(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsExecutable(file)
     *
     * Description:
     *     Return TRUE if the specified file is a program.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsExecutable") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_IsExecutable(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsReadable(file)
     *
     * Description:
     *     Return TRUE if the specified file is readable.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsReadable") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_IsReadable(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsWritable(file)
     *
     * Description:
     *     Return TRUE if the specified file is writable.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsWritable") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_IsWritable(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsLink(source, target)
     *
     * Description:
     *     Create a symbolic link from source to target.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsLink") == 0) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, Fs_Link(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsList(path)
     *
     * Description:
     *     Return the list of files in the specified directory.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsList") == 0) {
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
        
        return Fs_List(Gua_ObjectToString(argv[1]), object, error);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsMkDir(path)
     *
     * Description:
     *     Create the specified directory.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsMkDir") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_MkDir(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsName(path)
     *
     * Description:
     *     Return the file name without the file path.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsName") == 0) {
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
        
        Fs_Name(Gua_ObjectToString(argv[1]), &newString);
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsPath(path)
     *
     * Description:
     *     Return the file path without the file name.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsPath") == 0) {
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
        
        Fs_Path(Gua_ObjectToString(argv[1]), &newString);
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsRename(file)
     *
     * Description:
     *     Delete the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsRename") == 0) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        return Fs_Rename(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2]), error);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsSize(file)
     *
     * Description:
     *     Return the file size.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsSize") == 0) {
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
        
        Gua_IntegerToPObject(object, Fs_Size(Gua_ObjectToString(argv[1])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsTime(file)
     *
     * Description:
     *     Return the modification time of the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsTime") == 0) {
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
        
        Gua_RealToPObject(object, Fs_Time(Gua_ObjectToString(argv[1])));
    }

    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Fs_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsCopy", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsCopy");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsDelete", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsDelete");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsExists", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsExists");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsExtension", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsExtension");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsFullPath", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsFullPath");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsDirectory", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsDirectory");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsExecutable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsExecutable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsReadable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsReadable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsWritable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsWritable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsLink", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsLink");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsList", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsList");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsMkDir", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsMkDir");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsName", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsName");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsPath", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsPath");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsRename", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsRename");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsTime", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsTime");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     FS_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, FS_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "FS_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "FS_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

