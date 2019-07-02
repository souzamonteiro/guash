/*
 * File:
 *     regexp.c
 *
 * Package:
 *     Regexp
 *
 * Description:
 *     This library implements a regexp like library wrapper
 *     for the GuaraScript interpreter.
 *
 *     Based on public domain source code developed by Ozan S. Yigit.
 *
 * Regular Expressions:
 *
 *      [1]     char    matches itself, unless it is a special
 *                      character (metachar): . \ [ ] * + ^ $
 *
 *      [2]     .       matches any character.
 *
 *      [3]     \       matches the character following it, except
 *                      when followed by a left or right round bracket,
 *                      a digit 1 to 9 or a left or right angle bracket. 
 *                      (see [7], [8] and [9])
 *                      It is used as an escape character for all 
 *                      other meta-characters, and itself. When used
 *                      in a set ([4]), it is treated as an ordinary
 *                      character.
 *
 *      [4]     [set]   matches one of the characters in the set.
 *                      If the first character in the set is "^",
 *                      it matches a character NOT in the set, i.e. 
 *                      complements the set. A shorthand S-E is 
 *                      used to specify a set of characters S up to 
 *                      E, inclusive. The special characters "]" and 
 *                      "-" have no special meaning if they appear 
 *                      as the first chars in the set.
 *
 *                      Examples:        Match:
 *
 *                      [a-z]            any lowercase alpha
 *
 *                      [^]-]            any char except ] and -
 *
 *                      [^A-Z]           any char except uppercase alpha
 *
 *                      [a-zA-Z]         any alpha
 *
 *      [5]     *       any regular expression form [1] to [4], followed by
 *                      closure char (*) matches zero or more matches of
 *                      that form.
 *
 *      [6]     +       same as [5], except it matches one or more.
 *
 *      [7]             a regular expression in the form [1] to [10], enclosed
 *                      as \(form\) matches what form matches. The enclosure
 *                      creates a set of tags, used for [8] and for
 *                      pattern substution. The tagged forms are numbered
 *                      starting from 1.
 *
 *      [8]             a \ followed by a digit 1 to 9 matches whatever a
 *                      previously tagged regular expression ([7]) matched.
 *
 *      [9]     \<      a regular expression starting with a \< construct
 *              \>      and/or ending with a \> construct, restricts the
 *                      pattern matching to the beginning of a word, and/or
 *                      the end of a word. A word is defined to be a character
 *                      string beginning and/or ending with the characters
 *                      A-Z a-z 0-9 and _. It must also be preceded and/or
 *                      followed by any character outside those mentioned.
 *
 *      [10]            a composite regular expression xy where x and y
 *                      are in the form [1] to [10] matches the longest
 *                      match of x followed by a match for y.
 *
 *      [11]    ^       a regular expression starting with a ^ character
 *              $       and/or ending with a $ character, restricts the
 *                      pattern matching to the beginning of the line,
 *                      or the end of line. [anchors] Elsewhere in the
 *                      pattern, ^ and $ are treated as ordinary characters.
 *
 *
 * Acknowledgements:
 *     HCR's Hugh Redelmeier has been most helpful in various
 *     stages of development. He convinced me to include BOW
 *     and EOW constructs, originally invented by Rob Pike at
 *     the University of Toronto.
 *
 * References:
 *              Software tools              Kernighan & Plauger
 *              Software tools in Pascal    Kernighan & Plauger
 *              Grep [rsx-11 C dist]        David Conroy
 *              ed - text editor            Un*x Programmer's Manual
 *              Advanced editing on Un*x    B. W. Kernighan
 *              RegExp routines             Henry Spencer
 *
 * Notes:
 *
 *    This implementation uses a bit-set representation for character
 *    classes for speed and compactness. Each character is represented 
 *    by one bit in a 128-bit block. Thus, CCL always takes a 
 *    constant 16 bytes in the internal nfa, and re_exec does a single
 *    bit comparison to locate the character in the set.
 *
 * Examples:
 *
 *    pattern:    foo*.*
 *    compile:    CHR f CHR o CLO CHR o END CLO ANY END END
 *    matches:    fo foo fooo foobar fobar foxx ...
 *
 *    pattern:    fo[ob]a[rz]    
 *    compile:    CHR f CHR o CCL bitset CHR a CCL bitset END
 *    matches:    fobar fooar fobaz fooaz
 *
 *    pattern:    foo\\+
 *    compile:    CHR f CHR o CHR o CHR \ CLO CHR \ END END
 *    matches:    foo\ foo\\ foo\\\  ...
 *
 *    pattern:    \(foo\)[1-3]\1    (same as foo[1-3]foo)
 *    compile:    BOT 1 CHR f CHR o CHR o EOT 1 CCL bitset REF 1 END
 *    matches:    foo1foo foo2foo foo3foo
 *
 *    pattern:    \(fo.*\)-\1
 *    compile:    BOT 1 CHR f CHR o CLO ANY END EOT 1 CHR - REF 1 END
 *    matches:    foo-foo fo-fo fob-fob foobar-foobar ...
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
 * RCS: @(#) $Id: regexp.c,v 1.1 2015/10/24 16:43:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "regexp.h"

static CHAR bitarr[] = {1,2,4,8,16,32,64,128}; /* Pre-set bits. */

static CHAR chrtyp[MAXCHR] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 0, 0, 0, 0, 0
};

#define inascii(x) (0177&(x))
#define iswordc(x) chrtyp[inascii(x)]
#define isinset(x,y) ((x)[((y) & BLKIND) >> 3] & bitarr[(y) & BITIND])

#define ANYSKIP  2 /* [CLO] ANY END. */
#define CHRSKIP  3 /* [CLO] CHR chr END. */
#define CCLSKIP 18 /* [CLO] CCL 16bytes END. */

#define badpat(x) (*regex->nfa = END, x)
#define store(x) *mp++ = x

#define EXPRESSION_SIZE  4096

Gua_Namespace *globalNameSpace;

static void chset(Regexp_Regex *regex, CHAR c)
{
    regex->bittab[(CHAR) ((c) & BLKIND) >> 3] |= bitarr[(c) & BITIND];
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Regexp_Compile(Regexp_Regex *regex, Gua_String pattern)
 *
 * Description:
 *     Compile a regular expression to a NFA.
 *
 * Arguments:
 *     regex,      regular handle;
 *     pattern,    regular expression.
 *
 * Results:
 *     The function compiles a regular expression to be
 *     used by Regexp_Execute. The function returns a pointer to
 *     an error message is it fails to compile the expression or
 *     0 otherwise.
 */
Gua_String Regexp_Compile(Regexp_Regex *regex, Gua_String pattern)
{
    char *p;      /* Pattern pointer. */
    CHAR *mp=regex->nfa; /* NFA pointer. */
    CHAR *lp;     /* Saved pointer. */
    CHAR *sp=regex->nfa; /* Another one. */
    
    int tagi = 0; /* Tag regex->stack index. */
    int tagc = 1; /* Actual tag count. */
    
    int n;
    CHAR mask; /* XOR mask -CCL/NCL. */
    int c1, c2;
    
    if (!pattern || !*pattern) {
        if (regex->sta) {
            return 0;
        } else {
            return badpat("no previous regular expression");
        }
    }
    
    regex->sta = NOP;

    for (p = pattern; *p; p++) {
        lp = mp;
        switch(*p) {
            /* Match any char. */
            case '.':
                store(ANY);
                break;
            /* Match beginning. */
            case '^':
                if (p == pattern) {
                    store(BOL);
                } else {
                    store(CHR);
                    store(*p);
                }
                break;
            /* Match end of line. */
            case '$':
                if (!*(p+1)) {
                    store(EOL);
                } else {
                    store(CHR);
                    store(*p);
                }
                break;
            /* Match char class.*/
            case '[':
                store(CCL);

                if (*++p == '^') {
                    mask = 0377;    
                    p++;
                } else {
                    mask = 0;
                }
                /* Real dash. */
                if (*p == '-') {
                    chset(regex, *p++);
                }
                /* Real bracket. */
                if (*p == ']') {
                    chset(regex, *p++);
                }
                while (*p && *p != ']') {
                    if (*p == '-' && *(p+1) && *(p+1) != ']') {
                        p++;
                        c1 = *(p-2) + 1;
                        c2 = *p++;
                        while (c1 <= c2) {
                            chset(regex, (CHAR)c1++);
                        }
                    } else {
                        chset(regex, *p++);
                    }
                }
                if (!*p) {
                    return badpat("missing ]");
                }
                for (n = 0; n < BITBLK; regex->bittab[n++] = (char) 0) {
                    store(mask ^ regex->bittab[n]);
                }
                break;
            /* Match 0 or more. */
            case '*':
            /* Match 1 or more. */
            case '+':
                if (p == pattern) {
                    return badpat("empty closure");
                }
                /* Previous opcode. */
                lp = sp;
                /* Equivalence. */
                if (*lp == CLO) {
                    break;
                }
                
                switch(*lp) {
                    case BOL:
                    case BOT:
                    case EOT:
                    case BOW:
                    case EOW:
                    case REF:
                        return badpat("illegal closure");
                    default:
                        break;
                }

                if (*p == '+') {
                    for (sp = mp; lp < sp; lp++) {
                        store(*lp);
                    }
                }
                store(END);
                store(END);
                sp = mp;
                while (--mp > lp) {
                    *mp = mp[-1];
                }
                store(CLO);
                mp = sp;
                break;
            /* Tags, back references. */
            case '\\':
                switch(*++p) {
                    case '(':
                        if (tagc < MAXTAG) {
                            regex->tagstk[++tagi] = tagc;
                            store(BOT);
                            store(tagc++);
                        } else {
                            return badpat("too many \\(\\) pairs");
                        }
                        break;
                    case ')':
                        if (*sp == BOT) {
                            return badpat("null pattern inside \\(\\)");
                        }
                        if (tagi > 0) {
                            store(EOT);
                            store(regex->tagstk[tagi--]);
                        } else {
                            return badpat("unmatched \\)");
                        }
                        break;
                    case '<':
                        store(BOW);
                        break;
                    case '>':
                        if (*sp == BOW) {
                            return badpat("null pattern inside \\<\\>");
                        }
                        store(EOW);
                        break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        n = *p-'0';
                        if (tagi > 0 && regex->tagstk[tagi] == n) {
                            return badpat("cyclical reference");
                        }
                        if (tagc > n) {
                            store(REF);
                            store(n);
                        } else {
                            return badpat("undetermined reference");
                        }
                        break;
                    default:
                        store(CHR);
                        store(*p);
                }
                break;
            /* An ordinary char. */
            default :
                store(CHR);
                store(*p);
                break;
        }
        sp = lp;
    }
    
    if (tagi > 0) {
        return badpat("unmatched \\(");
    }
    
    store(END);
    regex->sta = OKP;
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Regexp_Compile(Regexp_Regex *regex, Gua_String pattern)
 *
 * Description:
 *     Execute a regular expression on a given string.
 *
 *     Execute NFA to find a match.
 *
 *     Special cases: (regex->nfa[0])    
 *         BOL
 *             Match only once, starting from the beginning.
 *         CHR
 *             First locate the character without
 *             calling pmatch, and if found, call Regexp_PatternMatch
 *             for the remaining string.
 *         END
 *             Regexp_Compile failed, poor luser did not
 *             check for it. Fail fast.
 *
 *     If a match is found, regex->bopat[0] and regex->eopat[0] are set
 *     to the beginning and the end of the matched fragment,
 *     respectively.
 *
 * Arguments:
 *     regex,    regular handle;
 *     lp,       an string.
 *
 * Results:
 *     The function execute a previously compiled regular expression
 *     on a given string. It returns false if fail or true otherwise.
 */
Gua_Integer Regexp_Execute(Regexp_Regex *regex, Gua_String lp)
{
    CHAR c;
    char *ep = 0;
    CHAR *ap = regex->nfa;
    
    regex->bol = lp;
    
    regex->bopat[0] = 0;
    regex->bopat[1] = 0;
    regex->bopat[2] = 0;
    regex->bopat[3] = 0;
    regex->bopat[4] = 0;
    regex->bopat[5] = 0;
    regex->bopat[6] = 0;
    regex->bopat[7] = 0;
    regex->bopat[8] = 0;
    regex->bopat[9] = 0;
    
    switch(*ap) {
        /* Anchored: match from BOL only. */
        case BOL:
            ep = Regexp_PatternMatch(regex, lp, (Gua_String)ap);
            break;
        /* Ordinary char: locate it fast. */
        case CHR:
            c = *(ap+1);
            while (*lp && *lp != c) {
                lp++;
            }
            /* If EOS, fail, else fall through. */
            if (!*lp) {
                return 0;
            }
        /* Regular matching all the way. */
        default:
            do {
                if ((ep = Regexp_PatternMatch(regex, lp, (Gua_String)ap))) {
                    break;
                }
                lp++;
            } while (*lp);
            break;
        /* Munged automaton. Fail always. */
        case END:
            return 0;
    }
    if (!ep) {
        return 0;
    }
    regex->bopat[0] = lp;
    regex->eopat[0] = ep;
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Regexp_PatternMatch(Regexp_Regex *regex, Gua_String lp, Gua_String ap)
 *
 * Description:
 *     Obtains each occurrence of pattern in the string.
 *
 *     This code is partly snarfed from an early grep written by
 *     David Conroy. The backref and tag stuff, and various other
 *     innovations are by oz.
 *
 *     Special case optimizations: (regex->nfa[n], regex->nfa[n+1])
 *         CLO ANY
 *             We KNOW .* will match everything upto the
 *             end of line. Thus, directly go to the end of
 *             line, without recursive pmatch calls. As in
 *             the other closure cases, the remaining pattern
 *             must be matched by moving backwards on the
 *             string recursively, to find a match for xy
 *             (x is ".*" and y is the remaining pattern)
 *             where the match satisfies the LONGEST match for
 *             x followed by a match for y.
 *         CLO CHR
 *             We can again scan the string forward for the
 *             single char and at the point of failure, we
 *             execute the remaining regex->nfa recursively, same as
 *             above.
 *
 *       At the end of a successful match, regex->bopat[n] and regex->eopat[n]
 *       are set to the beginning and end of subpatterns matched
 *       by tagged expressions (n = 1 to 9).    
 *
 * Arguments:
 *     regex,    regular handle;
 *     lp,       an string;
 *     ap,       compiled regular expression.
 *
 * Results:
 *     The function obtains each occurrence of pattern in the string.
 */
Gua_String Regexp_PatternMatch(Regexp_Regex *regex, Gua_String lp, Gua_String ap)
{
    int op, c, n;
    char *e;   /* Extra pointer for CLO. */
    char *bp;  /* Beginning of sub-pattern. */
    char *ep;  /* Ending of sub-pattern. */
    char *are; /* To save the line ptr. */
    
    while ((op = *ap++) != END) {
        switch(op) {
            case CHR:
                if (*lp++ != *ap++) {
                    return 0;
                }
                break;
            case ANY:
                if (!*lp++) {
                    return 0;
                }
                break;
            case CCL:
                c = *lp++;
                if (!isinset(ap, c)) {
                    return 0;
                }
                ap += BITBLK;
                break;
            case BOL:
                if (lp != regex->bol) {
                    return 0;
                }
                break;
            case EOL:
                if (*lp) {
                    return 0;
                }
                break;
            case BOT:
                regex->bopat[(int)(*ap++)] = lp;
                break;
            case EOT:
                regex->eopat[(int)(*ap++)] = lp;
                break;
             case BOW:
                if ((lp!=regex->bol && iswordc(lp[-1])) || !iswordc(*lp)) {
                    return 0;
                }
                break;
            case EOW:
                if (lp==regex->bol || !iswordc(lp[-1]) || iswordc(*lp)) {
                    return 0;
                }
                break;
            case REF:
                n = *ap++;
                bp = regex->bopat[n];
                ep = regex->eopat[n];
                while (bp < ep) {
                    if (*bp++ != *lp++)
                        return 0;
                    }
                break;
            case CLO:
                are = lp;
                switch(*ap) {
                    case ANY:
                        while (*lp) {
                            lp++;
                        }
                        n = ANYSKIP;
                        break;
                    case CHR:
                        c = *(ap+1);
                        while (*lp && c == *lp) {
                            lp++;
                        }
                        n = CHRSKIP;
                        break;
                    case CCL:
                        while ((c = *lp) && isinset(ap+1,c)) {
                            lp++;
                        }
                        n = CCLSKIP;
                        break;
                    default:
                        Regex_Fail("bad NFA", *ap);
                        return 0;
                }

                ap += n;

                while (lp >= are) {
                    if ((e = Regexp_PatternMatch(regex, lp, ap))) {
                        return e;
                    }
                    --lp;
                }
                return 0;
            default:
                Regex_Fail("bad NFA", op);
                return 0;
        }
    }
    return lp;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Integer Regexp_Replace(Regexp_Regex *regex, Gua_String src, Gua_String dst)
 *
 * Description:
 *     Perform a sub-pattern substitution on a given tagged string.
 *
 * Details:
 *      Substitute the matched portions of the src in dst.
 *
 *      &         substitute the entire matched pattern.
 *
 *      \digit    substitute a sub-pattern, with the given tag number.
 *                Tags are numbered from 1 to 9. If the particular
 *                tagged sub-pattern does not exist, null is substituted.
 *
 * Arguments:
 *     regex,    regular handle;
 *     src,      string containing tags indicating what sub-pattern should be
 *               copied and where to copy it;
 *     dst,      buffer to copy the new string.
 *
 * Results:
 *     The function performs a sub-pattern substitution on a given tagged string.
 */
Gua_Integer Regexp_Replace(Regexp_Regex *regex, Gua_String src, Gua_String dst)
{
    char c;
    int  pin;
    char *bp;
    char *ep;
    
    if (!*src || !regex->bopat[0]) {
        return 0;
    }
    
    while ((c = *src++)) {
        switch(c) {
            case '&':
                pin = 0;
                break;
            case '\\':
                c = *src++;
                if (c >= '0' && c <= '9') {
                    pin = c - '0';
                    break;
                }
            default:
                *dst++ = c;
                continue;
        }

        if ((bp = regex->bopat[pin]) && (ep = regex->eopat[pin])) {
            while (*bp && bp < ep) {
                *dst++ = *bp++;
            }
            if (bp < ep) {
                return 0;
            }
        }
    }
    *dst = (char) 0;
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Regex_Fail(Gua_String err, Gua_Char c)
 *
 * Description:
 *     Error handler for re_exec.
 *
 * Arguments:
 *     err,    error message;
 *     c,      codigo.
 *
 * Results:
 *     Assign the error message to REGEXP_ERROR.
 */
void Regex_Fail(Gua_String err, Gua_Char c)
{
    Gua_String expression;
    Gua_Object value;
    
    Gua_ClearObject(value);
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "%s [opcode %o]\n", err, c);
    
    Gua_LinkStringToObject(value, expression);
    Gua_SetStoredObject(value);
    
    Gua_SetVariable(globalNameSpace, "REGEXP_ERROR", &value, SCOPE_GLOBAL);
    
    Gua_Free(expression);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Regexp_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Regexp_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Regexp_Regex regex;
    Gua_String dst;
    Gua_String err;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    memset(&regex, 0, sizeof(regex));
    
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
     *     regexp(pattern, string)
     *
     * Description:
     *     Return true if the string contents matches the specified
     *     regular expression pattern.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "regexp") == 0) {
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
        
        if ((err = Regexp_Compile(&regex, Gua_ObjectToString(argv[1]))) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s in expression %-.20s...\n", err, Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_IntegerToPObject(object, Regexp_Execute(&regex, Gua_ObjectToString(argv[2])));
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     regsub(pattern, source, target)
     *
     * Description:
     *     Searches the pattern on source and replace each sub-pattern on
     *     the specified places of target.
     *
     * Details:
     *      Substitute the matched portions of the source in target.
     *
     *      &         substitute the entire matched pattern.
     *
     *      \digit    substitute a sub-pattern, with the given tag number.
     *                Tags are numbered from 1 to 9. If the particular
     *                tagged sub-pattern does not exist, null is substituted.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "regsub") == 0) {
        if (argc != 4) {
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
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if ((err = Regexp_Compile(&regex, Gua_ObjectToString(argv[1]))) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s in expression %-.20s...\n", err, Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        } else {
            if (Regexp_Execute(&regex, Gua_ObjectToString(argv[2])) == 0) {
                Gua_StringToPObject(object, "");
            } else {
                dst = (Gua_String) Gua_Alloc(10 * sizeof(char) * Gua_ObjectLength(argv[2]) + sizeof(char) * Gua_ObjectLength(argv[3]) + 1);
                memset(dst, '\0', 10 * sizeof(char) * Gua_ObjectLength(argv[2]) + sizeof(char) * Gua_ObjectLength(argv[3]) + 1);
                if (Regexp_Replace(&regex, Gua_ObjectToString(argv[3]), dst) == 0) {
                    Gua_StringToPObject(object, "");
                } else {
                    Gua_StringToPObject(object, dst);
                }
                Gua_Free(dst);
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
 *     Gua_Status Regexp_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Regexp_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    globalNameSpace = nspace;
    
    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Regexp_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "regexp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "regexp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Regexp_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "regsub", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "regsub");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
       
    /* Define each extension constant... */
    Gua_LinkStringToObject(object, "");
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "REGEXP_ERROR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "REGEXP_ERROR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     REGEXP_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, REGEXP_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "REGEXP_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "REGEXP_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

