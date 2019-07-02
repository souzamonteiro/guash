/**
 * File:
 *     strptime.c
 *
 * Package:
 *     Strptime
 *
 * Description:
 *     This file implements a BSD like strptime function.
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
 * RCS: @(#) $Id: strptime.c,v 1.0 2015/10/11 12:35:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strptime.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     char *Strptime_Strptime(char *s, char *format, struct tm *tm);
 *
 * Description:
 *     A BSD like strptime function.
 *
 * Arguments:
 *     s,         string containing the time;
 *     format,    the format of the time in the string s;
 *     tm,        the tm structure that will contain the converted time.
 *
 * Results:
 *     The function convert a string representation of time to a time tm structure.
 */
char *Strptime_Strptime(char *s, char *format, struct tm *tm)
{
    /* Variables t and m represents, respectively,
       the time and the mask used to represent it. */
    char *t;
    char *m;
    char buffer[256];
    int i;
    int century;
    int period;
    int error;
    
    t = s;
    m = format;
    
    tm->tm_sec = 0;    /* Seconds (0-60). */
    tm->tm_min = 0;    /* Minutes (0-59). */
    tm->tm_hour = 0;   /* Hours (0-23). */
    tm->tm_mday = 1;   /* Day of the month (1-31). */
    tm->tm_mon = 0;    /* Month (0-11). */
    tm->tm_year = 0;   /* Year - 1900. */
    tm->tm_wday = 0;   /* Day of the week (0-6, Sunday = 0). */
    tm->tm_yday = 0;   /* Day in the year (0-365, 1 Jan = 0). */
    tm->tm_isdst = 0;  /* Daylight saving time (-1/0/1). */
    
    century = 0;
    period = 0;
    
    error = 0;
    
    while (*m) {
        memset(buffer, 0, sizeof(buffer));
        
        if (*m == '%') {
            m++;
            switch (*m) {
                /* Not supported. */
                case 'a':
                    error = 1;
                    break;
                /* Not supported. */
                case 'A':
                    error = 1;
                    break;
                /* Not supported. */
                case 'b':
                    error = 1;
                    break;
                /* Not supported. */
                case 'B':
                    error = 1;
                    break;
                /* Not supported. */
                case 'h':
                    error = 1;
                    break;
                /* Not supported. */
                case 'c':
                    error = 1;
                    break;
                /* The century number (0-99). */
                case 'C':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        century = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* The day of month (1-31). */
                case 'd':
                case 'e':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_mday = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* Not supported. */
                case 'D':
                    error = 1;
                    break;
                /* The hour (0-23). */
                case 'H':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_hour = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* Not supported. */
                case 'I':
                    error = 1;
                    break;
                /* The day number in the year (1-366). */
                case 'j':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_yday = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* The month number (1-12). */
                case 'm':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_mon = atoi(buffer) - 1;
                    } else {
                        error = 1;
                    }
                    break;
                /* The minute (0-59). */
                case 'M':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_min = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* Arbitrary whitespace. */
                case 'n':
                    while (*t) {
                        if (isspace(*t)) {
                            t++;
                        } else {
                            break;
                        }
                    }
                    break;
                /* The day period (AM or PM). */
                case 'p':
                    i = 0;
                    while (*t) {
                        if (isalpha(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strncmp("am", buffer, 2) == 0) {
                        period = 0;
                    } else if (strncmp("pm", buffer, 2) == 0) {
                        period = 1;
                    } else {
                        error = 1;
                    }
                    if (tm->tm_hour > 0) {
                        tm->tm_hour = tm->tm_hour + 12 * period;
                    }
                    break;
                /* Not supported. */
                case 'r':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_hour = atoi(buffer) + 12 * period;
                    } else {
                        error = 1;
                    }
                    break;
                /* Not supported. */
                case 'R':
                    error = 1;
                    break;
                /* The minute (0-59). */
                case 'S':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_sec = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* Arbitrary whitespace. */
                case 't':
                    while (*t) {
                        if (isspace(*t)) {
                            t++;
                        } else {
                            break;
                        }
                    }
                    break;
                /* Not supported. */
                case 'T':
                    error = 1;
                    break;
                /* Not supported. */
                case 'U':
                    error = 1;
                    break;
                /* The day of week (0-6). */
                case 'w':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_wday = atoi(buffer);
                    } else {
                        error = 1;
                    }
                    break;
                /* Not supported. */
                case 'W':
                    error = 1;
                    break;
                /* Not supported. */
                case 'x':
                    error = 1;
                    break;
                /* Not supported. */
                case 'X':
                    error = 1;
                    break;
                /* The year within century (0-99). */
                case 'y':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_year = atoi(buffer);
                        if (century == 0) {
                            if ((tm->tm_year >= 0) && (tm->tm_year <= 68)) {
                                tm->tm_year = tm->tm_year + 100;
                            } else if ((tm->tm_year >= 69) && (tm->tm_year <= 99)) {
                                tm->tm_year = tm->tm_year;
                            }
                        } else {
                            tm->tm_year = tm->tm_year + century * 100;
                        }
                    } else {
                        error = 1;
                    }
                    break;
                /* The year - 1900. */
                case 'Y':
                    i = 0;
                    while (*t) {
                        if (isdigit(*t)) {
                            buffer[i] = *t;
                            i++;
                            t++;
                        } else {
                            break;
                        }
                    }
                    buffer[i] = '\0';
                    if (strlen(buffer) > 0) {
                        tm->tm_year = atoi(buffer) - 1900;
                    } else {
                        error = 1;
                    }
                    break;
                default:
                    if (*t == *m) {
                        t++;
                        m++;
                    } else {
                        error = 1;
                    }
                    break;
            }
            m++;
        } else {
            if (*t == *m) {
                t++;
                m++;
            } else {
                error = 1;
            } 
        }
        if (error) {
            break;
        }
    }
    
    return t;
}

