/*
 * regexp.h
 *
 *     This library implements a regexp like library wrapper
 *     for the GuaraScript interpreter.
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
 * RCS: @(#) $Id: regexp.h,v 1.1 2015/10/24 16:43:00 monteiro Exp $
 * 
 */

#define OKP  1
#define NOP  0

#define CHR   1
#define ANY   2
#define CCL   3
#define BOL   4
#define EOL   5
#define BOT   6
#define EOT   7
#define BOW   8
#define EOW   9
#define REF  10
#define CLO  11

#define END  0

#define MAXCHR  128
#define CHRBIT  8
#define BITBLK  MAXCHR/CHRBIT
#define BLKIND  0170
#define BITIND  07

#define ASCIIB  0177

#ifdef NO_UCHAR
typedef char CHAR;
#else
typedef unsigned char CHAR;
#endif

#define MAXNFA  1024
#define MAXTAG  10

typedef struct {
    int tagstk[MAXTAG];  /* Sub-pattern tag stack. */
    CHAR nfa[MAXNFA];    /* Automaton. */
    int sta;             /* Status of last pattern. */
    CHAR bittab[BITBLK]; /* Bit table for CCL. */
    char *bol;
    char *bopat[MAXTAG];
    char *eopat[MAXTAG];
} Regexp_Regex;

#define REGEXP_VERSION "1.1"

Gua_String Regexp_Compile(Regexp_Regex *regex, Gua_String pattern);
Gua_Integer Regexp_Execute(Regexp_Regex *regex, Gua_String lp);
Gua_String Regexp_PatternMatch(Regexp_Regex *regex, Gua_String lp, Gua_String ap);
Gua_Integer Regexp_Replace(Regexp_Regex *regex, Gua_String src, Gua_String dst);
void Regex_Fail(Gua_String err, Gua_Char c);
Gua_Status Regexp_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Regexp_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

