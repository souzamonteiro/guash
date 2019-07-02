/*
 * interp.h
 *
 *     This library implements an expression interpreter.
 *
 * Copyright (c) 2017 Roberto Luiz Souza Monteiro, Hernane B. B. Pereira, Marcelo A. Moret.
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
 * RCS: @(#) $Id: interp.h,v 3.1 2017/05/27 09:42:00 monteiro Exp $
 *
 */

#include <stdio.h>
#include <stdbool.h>

#define TOKEN_TYPE_INTEGER           0
#define TOKEN_TYPE_REAL              1
#define TOKEN_TYPE_STRING            2
#define TOKEN_TYPE_PARENTHESIS       3
#define TOKEN_TYPE_BRACKET           4
#define TOKEN_TYPE_BRACE             5
#define TOKEN_TYPE_NOT               6
#define TOKEN_TYPE_BIT_NOT           7
#define TOKEN_TYPE_POWER             8
#define TOKEN_TYPE_MULT              9
#define TOKEN_TYPE_DIV              10
#define TOKEN_TYPE_MOD              11
#define TOKEN_TYPE_PLUS             12
#define TOKEN_TYPE_MINUS            13
#define TOKEN_TYPE_LEFT_SHIFT       14
#define TOKEN_TYPE_RIGHT_SHIFT      15
#define TOKEN_TYPE_LESS             16
#define TOKEN_TYPE_LE               17
#define TOKEN_TYPE_GREATER          18
#define TOKEN_TYPE_GE               19
#define TOKEN_TYPE_EQ               20
#define TOKEN_TYPE_NE               21
#define TOKEN_TYPE_BIT_AND          22
#define TOKEN_TYPE_BIT_XOR          23
#define TOKEN_TYPE_BIT_OR           24
#define TOKEN_TYPE_AND              25
#define TOKEN_TYPE_OR               26
#define TOKEN_TYPE_AND_OR           27
#define TOKEN_TYPE_FUNCTION         28
#define TOKEN_TYPE_VARIABLE         29
#define TOKEN_TYPE_MACRO            30
#define TOKEN_TYPE_INDIRECTION      31
#define TOKEN_TYPE_ARG_SEPARATOR    32
#define TOKEN_TYPE_SEPARATOR        33
#define TOKEN_TYPE_COMMENT          34
#define TOKEN_TYPE_ASSIGN           35
#define TOKEN_TYPE_IF               36
#define TOKEN_TYPE_ELSEIF           37
#define TOKEN_TYPE_ELSE             38
#define TOKEN_TYPE_WHILE            39
#define TOKEN_TYPE_DO               40
#define TOKEN_TYPE_FOR              41
#define TOKEN_TYPE_FOREACH          42
#define TOKEN_TYPE_DEFINE_FUNCTION  43
#define TOKEN_TYPE_TRY              44
#define TOKEN_TYPE_CATCH            45
#define TOKEN_TYPE_TEST             46
#define TOKEN_TYPE_SCRIPT           47
#define TOKEN_TYPE_END              48
#define TOKEN_TYPE_UNKNOWN          49

#define PARENTHESIS_OPEN   '('
#define PARENTHESIS_CLOSE  ')'
#define BRACKET_OPEN       '['
#define BRACKET_CLOSE      ']'
#define BRACE_OPEN         '{'
#define BRACE_CLOSE        '}'
#define SINGLE_QUOTE       '\''
#define DOUBLE_QUOTE       '"'
#define COMMA              ','
#define SEMI_COLON         ';'
#define HASH               '#'

#define OPERATORS             "!~*/%+-<>=&^|$@"
#define OPERATOR_NOT          '!'
#define OPERATOR_BIT_NOT      '~'
#define OPERATOR_MULT         '*'
#define OPERATOR_DIV          '/'
#define OPERATOR_MOD          '%'
#define OPERATOR_PLUS         '+'
#define OPERATOR_MINUS        '-'
#define OPERATOR_LESS         '<'
#define OPERATOR_GREATER      '>'
#define OPERATOR_EQ           '='
#define OPERATOR_BIT_AND      '&'
#define OPERATOR_BIT_XOR      '^'
#define OPERATOR_BIT_OR       '|'
#define OPERATOR_MACRO        '$'
#define OPERATOR_INDIRECTION  '@'

#define LEXEME_TRUE             "TRUE"
#define LEXEME_FALSE            "FALSE"
#define LEXEME_NULL             "NULL"
#define LEXEME_COMPLEX          "i"
#define LEXEME_IF               "if"
#define LEXEME_ELSEIF           "elseif"
#define LEXEME_ELSE             "else"
#define LEXEME_WHILE            "while"
#define LEXEME_DO               "do"
#define LEXEME_FOR              "for"
#define LEXEME_FOREACH          "foreach"
#define LEXEME_DEFINE_FUNCTION  "function"
#define LEXEME_TRY              "try"
#define LEXEME_CATCH            "catch"
#define LEXEME_TEST             "test"

#define OBJECT_TYPE_INTEGER    0
#define OBJECT_TYPE_REAL       1
#define OBJECT_TYPE_COMPLEX    2
#define OBJECT_TYPE_STRING     3
#define OBJECT_TYPE_ARRAY      4
#define OBJECT_TYPE_MATRIX     5
#define OBJECT_TYPE_FILE       6
#define OBJECT_TYPE_HANDLE     7
#define OBJECT_TYPE_NAMESPACE  8
#define OBJECT_TYPE_UNKNOWN    9

#define FUNCTION_TYPE_C        0
#define FUNCTION_TYPE_SCRIPT   1
#define FUNCTION_TYPE_UNKNOWN  2

/* Error codes. */
#define GUA_OK                          0
#define GUA_CONTINUE                    1
#define GUA_BREAK                       2
#define GUA_RETURN                      3
#define GUA_EXIT                        4
#define GUA_ERROR                       5
#define GUA_ERROR_UNEXPECTED_TOKEN      6
#define GUA_ERROR_OUT_OF_RANGE          7
#define GUA_ERROR_UNDERFLOW             8
#define GUA_ERROR_OVERFLOW              9
#define GUA_ERROR_UNTERMINATED_STRING  10
#define GUA_ERROR_UNCLOSED_EXPRESSION  11
#define GUA_ERROR_ILLEGAL_OPERAND      12
#define GUA_ERROR_DIVISION_BY_ZERO     13
#define GUA_ERROR_ILLEGAL_ASSIGNMENT   14
#define GUA_ERROR_FUNCTION             15

/* Error strings. */
#define GUA_OK_MSG                         ""
#define GUA_CONTINUE_MSG                   ""
#define GUA_BREAK_MSG                      ""
#define GUA_RETURN_MSG                     ""
#define GUA_EXIT_MSG                       ""
#define GUA_ERROR_MSG                      "parse error"
#define GUA_ERROR_UNEXPECTED_TOKEN_MSG     "unexpected token"
#define GUA_ERROR_OUT_OF_RANGE_MSG         "out of range"
#define GUA_ERROR_UNDERFLOW_MSG            "underflow"
#define GUA_ERROR_OVERFLOW_MSG             "overflow"
#define GUA_ERROR_UNTERMINATED_STRING_MSG  "unterminated string"
#define GUA_ERROR_UNCLOSED_EXPRESSION_MSG  "unclosed delimiter"
#define GUA_ERROR_ILLEGAL_OPERAND_MSG      "illegal operand"
#define GUA_ERROR_DIVISION_BY_ZERO_MSG     "division by zero"
#define GUA_ERROR_ILLEGAL_ASSIGNMENT_MSG   "illegal assignment"
#define GUA_ERROR_FUNCTION_MSG             "parsing function"

#define SCOPE_LOCAL   0
#define SCOPE_STACK   1
#define SCOPE_GLOBAL  2

#define GUA_VERSION  "3.3"

#define END_OF_LINE     '\n'
#define EXPRESSION_END  '\0'

#define EPSILON  0.000000000000001
#define ZERO     0.0

#define MAX_ERROR_MSG_SIZE  1024

#define NAMESPACE_HASH_SIZE  32

typedef int Gua_Type;
typedef int Gua_Short;
typedef long Gua_Integer;
typedef double Gua_Real;
typedef double Gua_Imaginary;
typedef char Gua_Char;
typedef Gua_Char* Gua_PChar;
typedef Gua_PChar Gua_String;
typedef long Gua_Length;
typedef int Gua_Status;
typedef int Gua_Stored;

typedef struct {
    Gua_Type type;
    Gua_String start;
    Gua_Length length;
    Gua_Integer integer;
    Gua_Real real;
    Gua_Status status;
} Gua_Token;

typedef struct {
    Gua_String description;
    Gua_String symbol;
} Gua_TokenData;

typedef struct {
    Gua_Type type;
    Gua_Integer integer;
    Gua_Real real;
    Gua_Imaginary imaginary;
    Gua_String string;
    struct Gua_Element *array;
    struct Gua_Matrix *matrix;
    struct Gua_File *file;
    struct Gua_Handle *handle;
    struct Gua_Namespace *nspace;
    Gua_Length length;
    Gua_Stored stored;
} Gua_Object;

typedef struct {
    Gua_Object key;
    Gua_Object object;
    struct Gua_Element *previous;
    struct Gua_Element *next;
} Gua_Element;

typedef struct {
    Gua_Short dimc;
    Gua_Integer *dimv;
    struct Gua_Object *object;
} Gua_Matrix;

typedef struct {
    void *file;
    void (*clearerr)(void *);
    Gua_Short (*close)(void *);
    Gua_Short (*eof)(void *);
    Gua_Short (*error)(void *);
    Gua_Short (*fileno)(void *);
    Gua_Short (*flush)(void *);
    Gua_String (*gets)(Gua_String, Gua_Short, void *);
    Gua_Short (*puts)(Gua_String, void *);
    Gua_Integer (*read)(Gua_String, Gua_Integer, Gua_Integer, void *);
    void (*rewind)(void *);
    Gua_Short (*seek)(void *, Gua_Integer, Gua_Short);
    Gua_Integer (*tell)(void *);
    Gua_Integer (*write)(Gua_String, Gua_Integer, Gua_Integer, void *);
} Gua_File;

typedef struct {
    Gua_String type;
    void *pointer;
} Gua_Handle;

typedef struct {
    Gua_String name;
    Gua_Object object;
    struct Gua_Variable *previous;
    struct Gua_Variable *next;
} Gua_Variable;

typedef struct {
    Gua_String name;
    Gua_Object object;
} Gua_Argument;

typedef struct {
    Gua_Type type;
    Gua_String name;
    Gua_Short argc;
    struct Gua_Argument *argv;
    Gua_String script;
    Gua_Short (*pointer)(void *, Gua_Short, Gua_Object *, Gua_Object *, Gua_String);
    struct Gua_Function *previous;
    struct Gua_Function *next;
} Gua_Function;

typedef struct {
    Gua_Variable *variable[NAMESPACE_HASH_SIZE];
    Gua_Function *function[NAMESPACE_HASH_SIZE];
    struct Gua_Namespace *previous;
    struct Gua_Namespace *next;
} Gua_Namespace;

typedef struct timeval Gua_Time;

/* 
 * The following macros and functions should be used only by applications that implement
 * a parser.
 */

/* Lexer macros and functions. */
#define Gua_ClearToken(t) { \
    (t).type = TOKEN_TYPE_UNKNOWN; \
    (t).start = NULL; \
    (t).length = 0; \
    (t).integer = 0; \
    (t).real = 0.0; \
    (t).status = GUA_ERROR_UNEXPECTED_TOKEN; \
}
#define Gua_ClearPToken(t) { \
    (t)->type = TOKEN_TYPE_UNKNOWN; \
    (t)->start = NULL; \
    (t)->length = 0; \
    (t)->integer = 0; \
    (t)->real = 0.0; \
    (t)->status = GUA_ERROR_UNEXPECTED_TOKEN; \
}

#define Gua_IsSeparator(c) ((c == END_OF_LINE) || (c == SEMI_COLON))
#define Gua_IsSpace(c) ((c == ' ') || (c == '\r') || (c == '\t') || (c == '\f') || ((c) == '\v'))
#define Gua_IsOperator(c) (strchr(OPERATORS, c) != NULL)
#define Gua_IsNumber(c) (isdigit(c) || (c == '.'))
#define Gua_IsIdentifier(c) (isalpha(c) || (c == '_'))

Gua_String Gua_ScanNumber(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanOperator(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanIdentifier(Gua_Namespace *nspace, Gua_String start, Gua_Token *token);
Gua_String Gua_ScanDoubleQuotes(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanString(Gua_String target, Gua_String source, Gua_Integer n);
Gua_String Gua_ScanArgSeparator(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanSeparator(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanComment(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanParenthesis(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanBracket(Gua_String start, Gua_Token *token);
Gua_String Gua_ScanBrace(Gua_String start, Gua_Token *token);
Gua_String Gua_NextToken(Gua_Namespace *nspace, Gua_String start, Gua_Token *token);

/* Parser macros and functions. */
Gua_Status Gua_BuiltInFunction(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Gua_CountArguments(Gua_String start);
Gua_Status Gua_ParseArguments(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Short first, Gua_Status *status, Gua_String error);
Gua_Status Gua_CountCommands(Gua_String start);
Gua_Status Gua_ParseCommands(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error);
Gua_Status Gua_ParseFormalArguments(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error);
Gua_Status Gua_CountMatrixElements(Gua_String start);
Gua_Status Gua_ParseMatrixElements(Gua_Namespace *nspace, Gua_String start, Gua_Short argc, Gua_Object *argv, Gua_Status *status, Gua_String error);
void Gua_FreeArguments(Gua_Short argc, Gua_Object *argv);
Gua_String Gua_ParseIf(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseWhile(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseDo(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseFor(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseForeach(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseTry(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseTest(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseFunction(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_Status Gua_EvalFunction(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_String Gua_ParseAssign(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseAssignVariable(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseAssignFunction(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseAssignUnknown(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseAssignMacro(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseAssignIndirection(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseLogicOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseLogicAnd(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseBitOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseBitXOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseBitAnd(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseLogicAndOr(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseEqual(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseLess(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseLeftShift(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParsePlus(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseMult(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParsePower(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseUnaryPlus(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseNot(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_ParseObject(Gua_Namespace *nspace, Gua_String start, Gua_Token *token, Gua_Object *object, Gua_Status *status, Gua_String error);

/* The following macros and functions were designed for general purpose use. */

/*
 * Support for, platform independent, memory management.
 */
#define Gua_Alloc(s) malloc(s)
#define Gua_Realloc(p,s) realloc(p,s)
#define Gua_Free(p) free(p)

/*
 * Support for manipulation of objects,
 * independent of the version of the interpreter.
 */
#define Gua_ClearObject(o) { \
    (o).type = OBJECT_TYPE_UNKNOWN; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_ClearPObject(o) { \
    (o)->type = OBJECT_TYPE_UNKNOWN; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}
#define Gua_LinkObjects(t,s) { \
    (t).type = (s).type; \
    (t).integer = (s).integer; \
    (t).real = (s).real; \
    (t).imaginary = (s).imaginary; \
    (t).string = (s).string; \
    (t).array = (s).array; \
    (t).matrix = (s).matrix; \
    (t).file = (s).file; \
    (t).handle = (s).handle; \
    (t).nspace = (s).nspace; \
    (t).length = (s).length; \
    (t).stored = (s).stored; \
}
#define Gua_LinkPObjects(t,s) { \
    (t)->type = (s)->type; \
    (t)->integer = (s)->integer; \
    (t)->real = (s)->real; \
    (t)->imaginary = (s)->imaginary; \
    (t)->string = (s)->string; \
    (t)->array = (s)->array; \
    (t)->matrix = (s)->matrix; \
    (t)->file = (s)->file; \
    (t)->handle = (s)->handle; \
    (t)->nspace = (s)->nspace; \
    (t)->length = (s)->length; \
    (t)->stored = (s)->stored; \
}
#define Gua_LinkFromPObject(t,s) { \
    (t).type = (s)->type; \
    (t).integer = (s)->integer; \
    (t).real = (s)->real; \
    (t).imaginary = (s)->imaginary; \
    (t).string = (s)->string; \
    (t).array = (s)->array; \
    (t).matrix = (s)->matrix; \
    (t).file = (s)->file; \
    (t).handle = (s)->handle; \
    (t).nspace = (s)->nspace; \
    (t).length = (s)->length; \
    (t).stored = (s)->stored; \
}
#define Gua_LinkToPObject(t,s) { \
    (t)->type = (s).type; \
    (t)->integer = (s).integer; \
    (t)->real = (s).real; \
    (t)->imaginary = (s).imaginary; \
    (t)->string = (s).string; \
    (t)->array = (s).array; \
    (t)->matrix = (s).matrix; \
    (t)->file = (s).file; \
    (t)->handle = (s).handle; \
    (t)->nspace = (s).nspace; \
    (t)->length = (s).length; \
    (t)->stored = (s).stored; \
}
#define Gua_IntegerToObject(o,i) { \
    (o).type = OBJECT_TYPE_INTEGER; \
    (o).integer = i; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_IntegerToPObject(o,i) { \
    (o)->type = OBJECT_TYPE_INTEGER; \
    (o)->integer = i; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}
#define Gua_RealToObject(o,r) { \
    (o).type = OBJECT_TYPE_REAL; \
    (o).integer = 0; \
    (o).real = r; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_RealToPObject(o,r) { \
    (o)->type = OBJECT_TYPE_REAL; \
    (o)->integer = 0; \
    (o)->real = r; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}
#define Gua_ComplexToObject(o,r,i) { \
    (o).type = OBJECT_TYPE_COMPLEX; \
    (o).integer = 0; \
    (o).real = r; \
    (o).imaginary = i; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_ComplexToPObject(o,r,i) { \
    (o)->type = OBJECT_TYPE_COMPLEX; \
    (o)->integer = 0; \
    (o)->real = r; \
    (o)->imaginary = i; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}
#define Gua_StringToObject(o,s) { \
    (o).type = OBJECT_TYPE_STRING; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = (Gua_String)malloc(sizeof(char) * (strlen(s) + 1)); \
    memset((o).string, '\0', sizeof(char) * (strlen(s) + 1)); \
    strncpy((o).string, s, strlen(s)); \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = strlen(s); \
    (o).stored = false; \
}
#define Gua_StringToPObject(o,s) { \
    (o)->type = OBJECT_TYPE_STRING; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = (Gua_String)malloc(sizeof(char) * (strlen(s) + 1)); \
    memset((o)->string, '\0', sizeof(char) * (strlen(s) + 1)); \
    strncpy((o)->string, s, strlen(s)); \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = strlen(s); \
    (o)->stored = false; \
}
#define Gua_ByteArrayToObject(o,s,n) { \
    (o).type = OBJECT_TYPE_STRING; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = (Gua_String)malloc(sizeof(char) * (n + 1)); \
    memset((o).string, '\0', sizeof(char) * (n + 1)); \
    memcpy((o).string, s, n); \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = n; \
    (o).stored = false; \
}
#define Gua_ByteArrayToPObject(o,s,n) { \
    (o)->type = OBJECT_TYPE_STRING; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = (Gua_String)malloc(sizeof(char) * (n + 1)); \
    memset((o)->string, '\0', sizeof(char) * (n + 1)); \
    memcpy((o)->string, s, n); \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = n; \
    (o)->stored = false; \
}
#define Gua_LinkStringToObject(o,s) { \
    (o).type = OBJECT_TYPE_STRING; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = s; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = strlen(s); \
    (o).stored = false; \
}
#define Gua_LinkStringToPObject(o,s) { \
    (o)->type = OBJECT_TYPE_STRING; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = s; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = strlen(s); \
    (o)->stored = false; \
}
#define Gua_LinkByteArrayToObject(o,s,n) { \
    (o).type = OBJECT_TYPE_STRING; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = s; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = n; \
    (o).stored = false; \
}
#define Gua_LinkByteArrayToPObject(o,s,n) { \
    (o)->type = OBJECT_TYPE_STRING; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = s; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = n; \
    (o)->stored = false; \
}
#define Gua_ArrayToObject(o,a,n) { \
    (o).type = OBJECT_TYPE_ARRAY; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = a; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = n; \
    (o).stored = false; \
}
#define Gua_ArrayToPObject(o,a,n) { \
    (o)->type = OBJECT_TYPE_ARRAY; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = a; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = n; \
    (o)->stored = false; \
}
#define Gua_MatrixToObject(o,m,n) { \
    (o).type = OBJECT_TYPE_MATRIX; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = m; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = n; \
    (o).stored = false; \
}
#define Gua_MatrixToPObject(o,m,n) { \
    (o)->type = OBJECT_TYPE_MATRIX; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = m; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = n; \
    (o)->stored = false; \
}
#define Gua_FileToObject(o,h) { \
    (o).type = OBJECT_TYPE_FILE; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = h; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_FileToPObject(o,h) { \
    (o)->type = OBJECT_TYPE_FILE; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = h; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}
#define Gua_HandleToObject(o,h) { \
    (o).type = OBJECT_TYPE_HANDLE; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = h; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_HandleToPObject(o,h) { \
    (o)->type = OBJECT_TYPE_HANDLE; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = h; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}
#define Gua_NamespaceToObject(o,s) { \
    (o).type = OBJECT_TYPE_NAMESPACE; \
    (o).integer = 0; \
    (o).real = 0.0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = s; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_NamespaceToPObject(o,s) { \
    (o)->type = OBJECT_TYPE_NAMESPACE; \
    (o)->integer = 0; \
    (o)->real = 0.0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = s; \
    (o)->length = 0; \
    (o)->stored = false; \
}

/*
 * Support to convert the internal representation of a value.
 */
#define Gua_ConvertIntegerObjectToReal(o) { \
    (o).type = OBJECT_TYPE_REAL; \
    (o).real = (o).integer; \
    (o).integer = 0; \
    (o).imaginary = 0.0; \
    (o).string = NULL; \
    (o).array = NULL; \
    (o).matrix = NULL; \
    (o).file = NULL; \
    (o).handle = NULL; \
    (o).nspace = NULL; \
    (o).length = 0; \
    (o).stored = false; \
}
#define Gua_ConvertIntegerPObjectToReal(o) { \
    (o)->type = OBJECT_TYPE_REAL; \
    (o)->real = (o)->integer; \
    (o)->integer = 0; \
    (o)->imaginary = 0.0; \
    (o)->string = NULL; \
    (o)->array = NULL; \
    (o)->matrix = NULL; \
    (o)->file = NULL; \
    (o)->handle = NULL; \
    (o)->nspace = NULL; \
    (o)->length = 0; \
    (o)->stored = false; \
}

#define Gua_SetObjectType(o,t) {(o).type = t;}
#define Gua_SetPObjectType(o,t) {(o)->type = t;}

#define Gua_SetObjectLength(o,l) {(o).length = l;}
#define Gua_SetPObjectLength(o,l) {(o)->length = l;}

#define Gua_SetStoredObject(o) {(o).stored = true;}
#define Gua_SetNotStoredObject(o) {(o).stored = false;}

#define Gua_SetStoredPObject(o) {(o)->stored = true;}
#define Gua_SetNotStoredPObject(o) {(o)->stored = false;}

#define Gua_SetObjectStoredState(o,s) {(o).stored = s;}
#define Gua_SetPObjectStoredState(o,s) {(o)->stored = s;}

#define Gua_ObjectType(o) (o).type
#define Gua_ObjectToInteger(o) (o).integer
#define Gua_ObjectToReal(o) (o).real
#define Gua_ObjectToImaginary(o) (o).imaginary
#define Gua_ObjectToString(o) (o).string
#define Gua_ObjectToArray(o) (o).array
#define Gua_ObjectToMatrix(o) (o).matrix
#define Gua_ObjectToFile(o) (o).file
#define Gua_ObjectToHandle(o) (o).handle
#define Gua_ObjectToNamespace(o) (o).nspace
#define Gua_ObjectLength(o) (o).length
#define Gua_IsObjectStored(o) (o).stored

#define Gua_PObjectType(o) (o)->type
#define Gua_PObjectToInteger(o) (o)->integer
#define Gua_PObjectToReal(o) (o)->real
#define Gua_PObjectToImaginary(o) (o)->imaginary
#define Gua_PObjectToString(o) (o)->string
#define Gua_PObjectToArray(o) (o)->array
#define Gua_PObjectToMatrix(o) (o)->matrix
#define Gua_PObjectToFile(o) (o)->file
#define Gua_PObjectToHandle(o) (o)->handle
#define Gua_PObjectToNamespace(o) (o)->nspace
#define Gua_PObjectLength(o) (o)->length
#define Gua_IsPObjectStored(o) (o)->stored

/* Generic file handles support. */
#define Gua_NewFile(h,p) { \
    h = (Gua_File *)malloc(sizeof(Gua_File)); \
    Gua_ClearFile(h); \
    (h)->file = p; \
}
#define Gua_ClearFile(h) { \
    (h)->file = NULL; \
    (h)->clearerr = NULL; \
    (h)->close = NULL; \
    (h)->eof = NULL; \
    (h)->error = NULL; \
    (h)->fileno = NULL; \
    (h)->flush = NULL; \
    (h)->gets = NULL; \
    (h)->puts = NULL; \
    (h)->read = NULL; \
    (h)->rewind = NULL; \
    (h)->seek = NULL; \
    (h)->tell = NULL; \
    (h)->write = NULL; \
}

#define Gua_GetFileHandle(h) (h)->file
#define Gua_GetFileClearerr(h) (h)->clearerr
#define Gua_GetFileClose(h) (h)->close
#define Gua_GetFileEof(h) (h)->eof
#define Gua_GetFileError(h) (h)->error
#define Gua_GetFileFileno(h) (h)->fileno
#define Gua_GetFileFlush(h) (h)->flush
#define Gua_GetFileGets(h) (h)->gets
#define Gua_GetFilePuts(h) (h)->puts
#define Gua_GetFileRead(h) (h)->read
#define Gua_GetFileRewind(h) (h)->rewind
#define Gua_GetFileSeek(h) (h)->seek
#define Gua_GetFileTell(h) (h)->tell
#define Gua_GetFileWrite(h) (h)->write

#define Gua_SetFileHandle(h,p) {(h)->file = p;}
#define Gua_SetFileClearerr(h,f) {(h)->clearerr = f;}
#define Gua_SetFileClose(h,f) {(h)->close = f;}
#define Gua_SetFileEof(h,f) {(h)->eof = f;}
#define Gua_SetFileError(h,f) {(h)->error = f;}
#define Gua_SetFileFileno(h,f) {(h)->fileno = f;}
#define Gua_SetFileFlush(h,f) {(h)->flush = f;}
#define Gua_SetFileGets(h,f) {(h)->gets = f;}
#define Gua_SetFilePuts(h,f) {(h)->puts = f;}
#define Gua_SetFileRead(h,f) {(h)->read = f;}
#define Gua_SetFileRewind(h,f) {(h)->rewind = f;}
#define Gua_SetFileSeek(h,f) {(h)->seek = f;}
#define Gua_SetFileTell(h,f) {(h)->tell = f;}
#define Gua_SetFileWrite(h,f) {(h)->write = f;}

#define Gua_FileClearerr(h) (h)->clearerr(h)
#define Gua_FileClose(h) (h)->close(h)
#define Gua_FileEof(h) (h)->eof(h)
#define Gua_FileError(h) (h)->error(h)
#define Gua_FileFileno(h) (h)->fileno(h)
#define Gua_FileFlush(h) (h)->flush(h)
#define Gua_FileGets(s,l,h) (h)->gets(s,l,h)
#define Gua_FilePuts(s,h) (h)->puts(s,h)
#define Gua_FileRead(s,l,n,h) (h)->read(s,l,n,h)
#define Gua_FileRewind(h) (h)->rewind(h)
#define Gua_FileSeek(h,o,w) (h)->seek(h,o,w)
#define Gua_FileTell(h) (h)->tell(h)
#define Gua_FileWrite(s,l,n,h) (h)->write(s,l,n,h)

#define Gua_LinkFilesFunctions(t,s) { \
    (t)->clearerr = (s)->clearerr; \
    (t)->close = (s)->close; \
    (t)->eof = (s)->eof; \
    (t)->error = (s)->error; \
    (t)->fileno = (s)->fileno; \
    (t)->flush = (s)->flush; \
    (t)->gets = (s)->gets; \
    (t)->puts = (s)->puts; \
    (t)->read = (s)->read; \
    (t)->rewind = (s)->rewind; \
    (t)->seek = (s)->seek; \
    (t)->tell = (s)->tell; \
    (t)->write = (s)->write; \
}
#define Gua_LinkFiles(t,s) { \
    (t)->file = (s)->file; \
    Gua_LinkFilesFunctions(t,s); \
}

/* Generic handles support. */
#define Gua_NewHandle(h,t,p) { \
    h = (Gua_Handle *)malloc(sizeof(Gua_Handle)); \
    Gua_ClearHandle(h); \
    (h)->type = (Gua_String)malloc(sizeof(char) * (strlen(t) + 1)); \
    memset((h)->type, '\0', sizeof(char) * (strlen(t) + 1)); \
    strncpy((h)->type, t, strlen(t)); \
    (h)->pointer = p; \
}
#define Gua_ClearHandle(h) { \
    (h)->type = NULL; \
    (h)->pointer = NULL; \
}

#define Gua_LinkHandles(t,s) { \
    (t)->type = (s)->type; \
    (t)->pointer = (s)->pointer; \
}

#define Gua_GetHandleType(h) (h)->type
#define Gua_GetHandlePointer(h) (h)->pointer

#define Gua_SetHandleType(h,t) {(h)->type = t;}
#define Gua_SetHandlePointer(h,p) {(h)->pointer = p;}

/*
 * Support for manipulation of variables,
 * independent of the version of the interpreter.
 */
#define Gua_NewVariable(v,n) { \
    v = (Gua_Variable *)malloc(sizeof(Gua_Variable)); \
    (v)->name = (Gua_String)malloc(sizeof(char) * (strlen(n) + 1)); \
    memset((v)->name, '\0', sizeof(char) * (strlen(n) + 1)); \
    strncpy((v)->name, n, strlen(n)); \
}

/*
 * Support for manipulation of functions,
 * independent of the version of the interpreter.
 */
#define Gua_NewFunction(f,n) { \
    f = (Gua_Function *)malloc(sizeof(Gua_Function)); \
    (f)->name = (Gua_String)malloc(sizeof(char) * (strlen(n) + 1)); \
    memset((f)->name, '\0', sizeof(char) * (strlen(n) + 1)); \
    strncpy((f)->name, n, strlen(n)); \
}
#define Gua_ClearFunction(f) { \
    (f).type = FUNCTION_TYPE_C; \
    (f).name = NULL; \
    (f).pointer = NULL; \
    (f).argc = 0; \
    (f).argv = NULL; \
    (f).script = NULL; \
    (f).previous = NULL; \
    (f).next = NULL; \
}
#define Gua_ClearPFunction(f) { \
    (f)->type = FUNCTION_TYPE_C; \
    (f)->name = NULL; \
    (f)->pointer = NULL; \
    (f)->argc = 0; \
    (f)->argv = NULL; \
    (f)->script = NULL; \
    (f)->previous = NULL; \
    (f)->next = NULL; \
}
#define Gua_LinkFunctions(t,s) { \
    (t).type = (s).type; \
    (t).name = (s).name; \
    (t).pointer = (s).pointer; \
    (t).argc = (s).argc; \
    (t).argv = (s).argv; \
    (t).script = (s).script; \
    (t).previous = (s).previous; \
    (t).next = (s).next; \
}
#define Gua_LinkPFunctions(t,s) { \
    (t)->type = (s)->type; \
    (t)->name = (s)->name; \
    (t)->pointer = (s)->pointer; \
    (t)->argc = (s)->argc; \
    (t)->argv = (s)->argv; \
    (t)->script = (s)->script; \
    (t)->previous = (s)->previous; \
    (t)->next = (s)->next; \
}
#define Gua_LinkFromPFunction(t,s) { \
    (t).type = (s)->type; \
    (t).name = (s)->name; \
    (t).pointer = (s)->pointer; \
    (t).argc = (s)->argc; \
    (t).argv = (s)->argv; \
    (t).script = (s)->script; \
    (t).previous = (s)->previous; \
    (t).next = (s)->next; \
}
#define Gua_LinkToPFunction(t,s) { \
    (t)->type = (s).type; \
    (t)->name = (s).name; \
    (t)->pointer = (s).pointer; \
    (t)->argc = (s).argc; \
    (t)->argv = (s).argv; \
    (t)->script = (s).script; \
    (t)->previous = (s).previous; \
    (t)->next = (s).next; \
}
#define Gua_LinkCFunctionToFunction(f,c) { \
    (f).type = FUNCTION_TYPE_C; \
    (f).name = NULL; \
    (f).pointer = (c); \
    (f).argc = 0; \
    (f).argv = NULL; \
    (f).script = NULL; \
    (f).previous = NULL; \
    (f).next = NULL; \
}
#define Gua_LinkCFunctionToPFunction(f,c) { \
    (f)->type = FUNCTION_TYPE_C; \
    (f)->name = NULL; \
    (f)->pointer = (c); \
    (f)->argc = 0; \
    (f)->argv = NULL; \
    (f)->script = NULL; \
    (f)->previous = NULL; \
    (f)->next = NULL; \
}
#define Gua_SetPFunctionFromPFunction(t,s) { \
    (t)->type = (s)->type; \
    (t)->pointer = (s)->pointer; \
    (t)->argc = (s)->argc; \
    (t)->argv = (s)->argv; \
    (t)->script = (s)->script; \
}
#define Gua_ClearArguments(argc,argv) { \
    { \
        int i; \
        for (i = 0; i < argc; i++) { \
            Gua_ClearObject(argv[i]); \
        } \
    } \
}

#define Gua_ArgName(a) (a).name
#define Gua_ArgObject(a) (a).object

void Gua_ClearNamespace(Gua_Namespace *nspace);
void Gua_FreeNamespace(Gua_Namespace *nspace);
Gua_Short Gua_NamespaceSlot(Gua_String name);
void Gua_FreeVariableList(Gua_Variable *variable);
void Gua_FreeVariable(Gua_Variable *variable);
Gua_Status Gua_UnsetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Short scope);
Gua_Status Gua_SearchVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope);
Gua_Status Gua_GetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope);
Gua_Status Gua_SetVariable(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope);
Gua_Status Gua_UpdateObject(Gua_Namespace *nspace, Gua_String name, Gua_Object *object, Gua_Short scope);
void Gua_FreeFunctionList(Gua_Function *function);
void Gua_FreeFunction(Gua_Function *function);
Gua_Status Gua_UnsetFunction(Gua_Namespace *nspace, Gua_String name);
Gua_Status Gua_SearchFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function);
Gua_Status Gua_GetFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function);
Gua_Status Gua_SetFunction(Gua_Namespace *nspace, Gua_String name, Gua_Function *function);
void Gua_FreeArray(Gua_Element *array);
void Gua_FreeMatrix(Gua_Matrix *matrix);
void Gua_FreeHandle(Gua_Handle *handle);
void Gua_FreeObject(Gua_Object *object);
Gua_Status Gua_UnsetArrayElement(Gua_Object *array, Gua_Object *key);
Gua_Status Gua_GetArrayElement(Gua_Object *array, Gua_Object *key, Gua_Object *object);
Gua_Status Gua_SetArrayElement(Gua_Object *array, Gua_Object *key, Gua_Object *object, Gua_Stored stored);
void Gua_SetStoredArray(Gua_Object *array, Gua_Stored stored);
Gua_Status Gua_CopyArray(Gua_Object *target, Gua_Object *source, Gua_Stored stored);
Gua_Status Gua_GetArrayKeys(Gua_Object *keys, Gua_Object *array);
Gua_Status Gua_ArrayToString(Gua_Object *array, Gua_Object *object);
Gua_Status Gua_ArgsToString(Gua_Short argc, Gua_Object *argv, Gua_Object *object);
Gua_Status Gua_IsArrayEqual(Gua_Object *a, Gua_Object *b);
void Gua_SetStoredMatrix(Gua_Object *matrix, Gua_Stored stored);
Gua_Status Gua_CopyMatrix(Gua_Object *target, Gua_Object *source, Gua_Stored stored);
Gua_Status Gua_GetMatrixDim(Gua_Object *array, Gua_Object *matrix);
Gua_Status Gua_MatrixToString(Gua_Object *matrix, Gua_Object *object);
Gua_Status Gua_IsMatrixEqual(Gua_Object *a, Gua_Object *b);
Gua_Status Gua_IsMatrixApproximatelyEqual(Gua_Object *a, Gua_Object *b, Gua_Object *c);
Gua_Status Gua_AddMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Gua_SubMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Gua_NegMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Status Gua_MulMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Gua_IdentMatrix(Gua_Object *a, Gua_Integer n, Gua_String error);
Gua_Status Gua_InvMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Status Gua_PowMatrix(Gua_Object *a, Gua_Integer n, Gua_Object *b, Gua_String error);
Gua_Status Gua_AndMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Gua_OrMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Gua_AndOrMatrix(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Gua_CopyFile(Gua_Object *target, Gua_Object *source, Gua_Stored stored);
Gua_Status Gua_CopyHandle(Gua_Object *target, Gua_Object *source, Gua_Stored stored);
Gua_Status Gua_ElapsedTime(Gua_Time *result, Gua_Time *tv1, Gua_Time *tv2);
Gua_String Gua_Expression(Gua_Namespace *nspace, Gua_String start, Gua_Object *object, Gua_Status *status, Gua_String error);
Gua_String Gua_Evaluate(Gua_Namespace *nspace, Gua_String start, Gua_Object *object, Gua_Status *status, Gua_String error);
void Gua_ParseVarAssignment(Gua_String expression, Gua_String variable, Gua_String value);
void Gua_KeyValuePairsToArray(Gua_Short n, Gua_String *key, Gua_String *value, Gua_Object *object);
void Gua_ArgvToArray(int argc, char **argv, Gua_Object *object);
Gua_Status Gua_Init(Gua_Namespace *nspace, int argc, char *argv[], char **env, Gua_String error);

