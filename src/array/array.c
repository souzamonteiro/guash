/**
 * File:
 *     array.c
 *
 * Package:
 *     Array
 *
 * Description:
 *     This library implements the array functions for an expression interpreter.
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
 * RCS: @(#) $Id: array.c,v 1.7 2015/10/08 21:51:00 monteiro Exp $
 * 
 */

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "array.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Array_Intersection(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
 *
 * Description:
 *     Get the intersection between two associative arrays.
 *
 * Arguments:
 *     a,        an associative array;
 *     b,        an associative array;
 *     c,        an associative array (c = a intersection b);
 *     error,    a pointer to the error message.
 *
 * Results:
 *     The function returns an associative array c = a intersection b.
 */
Gua_Status Array_Intersection(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error)
{
    Gua_Element *e1;
    Gua_Element *e2;
    Gua_Object o1;
    Gua_Object o2;
    Gua_Integer newKey;
    Gua_Object newObject;
    Gua_Element *previous;
    Gua_Element *newElement;
    Gua_Short isEqual;
    Gua_String errMessage;
    
    previous = NULL;
    newElement = NULL;
    newKey = 0;
    
    if (Gua_PObjectType(a) != OBJECT_TYPE_ARRAY) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "illegal argument 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    if (Gua_PObjectType(b) != OBJECT_TYPE_ARRAY) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "illegal argument 2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    e1 = (Gua_Element *)Gua_PObjectToArray(a);
    e2 = (Gua_Element *)Gua_PObjectToArray(b);
    
    if ((e1 != NULL) && (e2 != NULL)) {
        Gua_FreeObject(c);
        
        while (true) {
            o1 = e1->object;
            
            while (true) {
                o2 = e2->object;
                isEqual = false;
                
                if (Gua_ObjectType(o1) == Gua_ObjectType(o2)) {
                    Gua_ClearObject(newObject);
                    
                    if (Gua_ObjectType(o1) == OBJECT_TYPE_INTEGER) {
                        if (Gua_ObjectToInteger(o1) == Gua_ObjectToInteger(o2)) {
                            Gua_IntegerToObject(newObject, Gua_ObjectToInteger(o1));
                            isEqual = true;
                        }
                    }
                    if (Gua_ObjectType(o1) == OBJECT_TYPE_REAL) {
                        if (Gua_ObjectToReal(o1) == Gua_ObjectToReal(o2)) {
                            Gua_RealToObject(newObject, Gua_ObjectToReal(o1));
                            isEqual = true;
                        }
                    }
                    if (Gua_ObjectType(o1) == OBJECT_TYPE_COMPLEX) {
                        if ((Gua_ObjectToReal(o1) == Gua_ObjectToReal(o2)) && (Gua_ObjectToImaginary(o1) == Gua_ObjectToImaginary(o2))) {
                            Gua_ComplexToObject(newObject, Gua_ObjectToReal(o1), Gua_ObjectToImaginary(o1));
                            isEqual = true;
                        }
                    }
                    if (Gua_ObjectType(o1) == OBJECT_TYPE_STRING) {
                        if (Gua_ObjectLength(o1) == Gua_ObjectLength(o2)) {
                            if (Gua_ObjectLength(o1) == Gua_ObjectLength(o2)) {
                                if (memcmp(Gua_ObjectToString(o1), Gua_ObjectToString(o2), Gua_ObjectLength(o1)) == 0) {
                                    Gua_ByteArrayToObject(newObject, Gua_ObjectToString(o1), Gua_ObjectLength(o1));
                                    isEqual = true;
                                }
                            }
                        }
                    }
                    
                    if (isEqual) {
                        /* Create a new element. */
                        newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                        /* The element key. */
                        Gua_IntegerToObject(newElement->key, newKey);
                        /* The element object. */
                        Gua_LinkObjects(newElement->object, newObject);
                        
                        if (previous) {
                            /* Set the target array chain. */
                            newElement->previous = (struct Gua_Element *)previous;
                            newElement->next = NULL;
                            previous->next = (struct Gua_Element *)newElement;
                        } else {
                            newElement->previous = NULL;
                            newElement->next = NULL;
                            
                            /* Link the first element. */
                            Gua_ArrayToPObject(c, (struct Gua_Element *)newElement, 1);
                        }
                        
                        previous = newElement;
                        
                        newKey++;
                    }
                }
                
                if (e2->next) {
                    e2 = (Gua_Element *)e2->next;
                } else {            
                    e2 = (Gua_Element *)Gua_PObjectToArray(b);
                    break;
                }
            }
            
            if (e1->next) {
                e1 = (Gua_Element *)e1->next;
            } else {
                break;
            }
        }
        
        /* Update the array length entry. */
        Gua_SetPObjectLength(c, newKey);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Array_Sort(Gua_Object *target, Gua_Object *source, Gua_Integer order, Gua_String error)
 *
 * Description:
 *     Sort an associative array.
 *
 * Arguments:
 *     target,    the target associative array;
 *     source,    the source associative array;
 *     order,     the sort order;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The function sorts an associative array.
 */
Gua_Status Array_Sort(Gua_Object *target, Gua_Object *source, Gua_Integer order, Gua_String error)
{
    Gua_Element *e1;
    Gua_Element *e2;
    Gua_Object o1;
    Gua_Object o2;
    Gua_Object object;
    Gua_Element *previous;
    Gua_Element *p;
    Gua_Element *n;
    Gua_Integer newKey;
    Gua_String errMessage;
    
    previous = NULL;
    newKey = 0;
    
    if (Gua_PObjectType(source) != OBJECT_TYPE_ARRAY) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "illegal argument 1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (Gua_PObjectToArray(source) != NULL) {
        Gua_ClearObject(object);
        
        Gua_CopyArray(&object, source, false);
        
        Gua_FreeObject(target);
        
        while (true) {
            e1 = (Gua_Element *)Gua_ObjectToArray(object);
            e2 = e1;
            o1 = e1->object;
            
            while (true) {
                o2 = e2->object;
                
                if (order == ARRAY_ASCENDING_ORDER) {
                    if ((Gua_ObjectType(o1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2) == OBJECT_TYPE_INTEGER)) {
                        if (Gua_ObjectToInteger(o1) > Gua_ObjectToInteger(o2)) {
                            e1 = e2;
                            o1 = e1->object;
                        }
                    }
                    if ((Gua_ObjectType(o1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2) == OBJECT_TYPE_REAL)) {
                        if (Gua_ObjectToReal(o1) > Gua_ObjectToReal(o2)) {
                            e1 = e2;
                            o1 = e1->object;
                        }
                    }
                    if ((Gua_ObjectType(o1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(o2) == OBJECT_TYPE_STRING)) {
                        if (memcmp(Gua_ObjectToString(o1), Gua_ObjectToString(o2), Gua_ObjectLength(o1)) > 0) {
                            e1 = e2;
                            o1 = e1->object;
                        }
                    }
                } else {
                    if ((Gua_ObjectType(o1) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(o2) == OBJECT_TYPE_INTEGER)) {
                        if (Gua_ObjectToInteger(o1) < Gua_ObjectToInteger(o2)) {
                            e1 = e2;
                            o1 = e1->object;
                        }
                    }
                    if ((Gua_ObjectType(o1) == OBJECT_TYPE_REAL) && (Gua_ObjectType(o2) == OBJECT_TYPE_REAL)) {
                        if (Gua_ObjectToReal(o1) < Gua_ObjectToReal(o2)) {
                            e1 = e2;
                            o1 = e1->object;
                        }
                    }
                    if ((Gua_ObjectType(o1) == OBJECT_TYPE_STRING) && (Gua_ObjectType(o2) == OBJECT_TYPE_STRING)) {
                        if (memcmp(Gua_ObjectToString(o1), Gua_ObjectToString(o2), Gua_ObjectLength(o1)) < 0) {
                            e1 = e2;
                            o1 = e1->object;
                        }
                    }
                }
                
                if (e2->next) {
                    e2 = (Gua_Element *)e2->next;
                } else {
                    break;
                }
            }
            
            /* Remove the selected element from the source array. */
            p = (Gua_Element *)e1->previous;
            n = (Gua_Element *)e1->next;
            if (p) {
                p->next = (struct Gua_Element *)n;
            }
            if (n) {
                n->previous = (struct Gua_Element *)p;
            }
            if (!p) {
                if (!n) {
                    object.array = NULL;
                } else {
                    object.array = (struct Gua_Element *)n;
                }
            }
            
            /* Update the array length entry. */
            Gua_SetObjectLength(object, Gua_ObjectLength(object) - 1);
            
            /* Update the element key. */
            Gua_FreeObject(&(e1->key));
            Gua_IntegerToObject(e1->key, newKey);
            
            /* Insert the selected element in the end of the target array. */
            if (previous) {
                /* Set the target array chain. */
                e1->previous = (struct Gua_Element *)previous;
                e1->next = NULL;
                previous->next = (struct Gua_Element *)e1;
                
                /* Update the array length entry. */
                Gua_SetPObjectLength(target, newKey);
            } else {
                e1->previous = NULL;
                e1->next = NULL;
                
                /* Link the first element. */
                Gua_ArrayToPObject(target, (struct Gua_Element *)e1, 1);
            }
            
            previous = e1;
            
            newKey++;
            
            if (!object.array) {
                break;
            }
        }
        
        /* Free the temporary array object. */
        Gua_FreeObject(&object);
        
        return GUA_OK;
    }
    
    return GUA_ERROR;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Array_ArrayFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Array functions wrapper.
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
Gua_Status Array_ArrayFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Element *element;
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
     *     intersection(array1, array2)
     *
     * Description:
     *     Returns a new array containing the intersection of two supplied arrays.
     *
     * Examples:
     *     a={1,2,3,4}
     *     b={3,4,5,6}
     *     c=intersection(a,b) # Return {3,4}.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "intersection") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_ARRAY) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_ARRAY) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Array_Intersection(&argv[1], &argv[2], object, error) != GUA_OK) {
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     search(array, valor)
     *
     * Description:
     *     Returns the key to the first instance the given value.
     *
     * Examples:
     *     a={4,1,3,2}
     *     c=search(a,3) # Return 2.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "search") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_ARRAY) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_FILE))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        element = (Gua_Element *)Gua_ObjectToArray(argv[1]);
        
        if (element) {
            while (true) {
                if ((Gua_ObjectType(element->object) == OBJECT_TYPE_INTEGER) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER)) {
                    if (Gua_ObjectToInteger(element->object) == Gua_ObjectToInteger(argv[2])) {
                        if (Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) {
                            Gua_LinkToPObject(object, element->key);
                        } else {
                            Gua_StringToPObject(object, Gua_ObjectToString(element->key));
                        }
                        return GUA_OK;
                    }
                } else if ((Gua_ObjectType(element->object) == OBJECT_TYPE_REAL) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL)) {
                    if (Gua_ObjectToReal(element->object) == Gua_ObjectToReal(argv[2])) {
                        if (Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) {
                            Gua_LinkToPObject(object, element->key);
                        } else {
                            Gua_StringToPObject(object, Gua_ObjectToString(element->key));
                        }
                        return GUA_OK;
                    }
                } else if ((Gua_ObjectType(element->object) == OBJECT_TYPE_COMPLEX) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_COMPLEX)) {
                    if ((Gua_ObjectToReal(element->object) == Gua_ObjectToReal(argv[2])) && (Gua_ObjectToImaginary(element->object) == Gua_ObjectToImaginary(argv[2]))) {
                        if (Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) {
                            Gua_LinkToPObject(object, element->key);
                        } else {
                            Gua_StringToPObject(object, Gua_ObjectToString(element->key));
                        }
                        return GUA_OK;
                    }
                } else if ((Gua_ObjectType(element->object) == OBJECT_TYPE_STRING) && (Gua_ObjectType(argv[2]) == OBJECT_TYPE_STRING)) {
                    if (Gua_ObjectLength(element->object) == Gua_ObjectLength(argv[2])) {
                        if (memcmp(Gua_ObjectToString(element->object), Gua_ObjectToString(argv[2]), Gua_ObjectLength(element->object)) == 0) {
                            if (Gua_ObjectType(element->key) == OBJECT_TYPE_INTEGER) {
                                Gua_LinkToPObject(object, element->key);
                            } else {
                                Gua_StringToPObject(object, Gua_ObjectToString(element->key));
                            }
                            return GUA_OK;
                        }
                    }
                }
                
                if (element->next) {
                    element = (Gua_Element *)element->next;
                } else {
                    break;
                }
            }
        }
     /**
     * Group:
     *     Scripting
     *
     * Function:
     *     sort(array)
     *
     * Description:
     *     Returns a new array containing the elements of the specified array ordered.
     *
     * Examples:
     *     a={2,4,1,3}
     *     c=sort(a) # Return {1,2,3,4}.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sort") == 0) {
        if ((argc < 2) || (argc > 3)) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_ARRAY) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc == 3) {
            if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        if (argc == 2) {
            if (Array_Sort(object, &argv[1], ARRAY_ASCENDING_ORDER, error) != GUA_OK) {
                return GUA_ERROR;
            }
        } else {
            if (Array_Sort(object, &argv[1], Gua_ObjectToInteger(argv[2]), error) != GUA_OK) {
                return GUA_ERROR;
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
 *     Gua_Status Array_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the array functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the array functions.
 */
Gua_Status Array_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, Array_ArrayFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "intersection", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "intersection");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "search", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "search");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sort", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sort");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     ARRAY_ASCENDING_ORDER
     *
     * Description:
     *     Sort array elements in a ascending order.
     */
    Gua_IntegerToObject(object, ARRAY_ASCENDING_ORDER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "ARRAY_ASCENDING_ORDER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "ARRAY_ASCENDING_ORDER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     ARRAY_DESCENDING_ORDER
     *
     * Description:
     *     Sort array elements in a descending order.
     */
    Gua_IntegerToObject(object, ARRAY_DESCENDING_ORDER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "ARRAY_DESCENDING_ORDER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "ARRAY_DESCENDING_ORDER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     ARRAY_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, ARRAY_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "ARRAY_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "ARRAY_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

