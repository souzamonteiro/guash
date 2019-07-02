/**
 * File:
 *     bdf.c
 *
 * Package:
 *     Bdf
 *
 * Description:
 *     This file implements a BDF, Glyph Bitmap Distribution Format,
 *     loader library.
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
 * RCS: @(#) $Id: bdf.c,v 1.0 2015/10/06 17:38:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bdf.h"

#define roundup(x) (long int)(ceil(x))

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_GetTokens(char *expression, char *separator, int *tokc, char ***tokv)
 *
 * Description:
 *     Splits an expression in its parts.
 *
 * Arguments:
 *     expression,    the expression to split;
 *     separator,     a list of characters to match when breaking the string;
 *     tokc,          the number of parts returned;
 *     tokv,          an array of strings containing each expression part.
 *
 * Results:
 *     The function splits an expression in its parts.
 */
int Bdf_GetTokens(char *expression, char *separator, int *tokc, char ***tokv)
{
    char *p;
    char *start;
    char *end;
    int length;
    char **args;
    int argsc;
    char *arg;
    int i;
    int j;
    
    /* Get the expression start. */
    p = expression;
    
    /* Count the number of parts in expression. */
    argsc = 1;
    
    while (*p) {
        if (strchr(separator, *p) != NULL) {
            argsc++;
        }
        /* Skip double quotes. */
        if (*p == '\"') {
            p++;
            while ((*p != '\"') && (*p != '\0')) {
                p++;
            }
            if (*p == '\"') {
                p++;
            }
        }
        if (*p != '\0') {
            p++;
        }
    }
    
    /* Allocate space for each expression part. */
    args = (char **) malloc(argsc * sizeof(char **));
    memset(args, 0, argsc * sizeof(char **));
    
    /* Get each expression part. */
    p = expression;
    
    start = p;
    end = p;
    
    i = 0;
    
    while (*p) {
        if (strchr(separator, *p) != NULL) {
            end = p - 1;
            
            length = end - start + 1;
            
            /* Allocate space for this expression part. */
            args[i] = (char *) malloc(length * sizeof(char) + 1);
            
            /* Copy this expression part to the allocated array. */
            arg = args[i];
            
            for (j = 0; j < length; j++) {
                arg[j] = *start;
                start++;
            }
            
            arg[j] = '\0';
            
            /* Restart search. */
            p++;
            i++;
            
            start = p;
            end = p;
        }
        /* Skip double quotes. */
        if (*p == '\"') {
            p++;
            while ((*p != '\"') && (*p != '\0')) {
                p++;
            }
            if (*p == '\"') {
                p++;
            }
        }
        if (*p != '\0') {
            p++;
        }
    }
    
    /* Get the last expression part. */
    end = p - 1;
    
    length = end - start + 1;
    
    /* Allocate space for this expression part. */
    args[i] = (char *) malloc(length * sizeof(char) + 1);
    
    /* Copy this expression part to the allocated array. */
    arg = args[i];
    
    for (j = 0; j < length; j++) {
        arg[j] = *start;
        start++;
    }
    
    arg[j] = '\0';
    
    /* Returns the expression parts array. */
    *tokv = args;
    *tokc = argsc;
    
    return(*tokc);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FreeTokens(int *tokc, char ***tokv)
 *
 * Description:
 *     Free an array of tokens.
 *
 * Arguments:
 *     tokc,    the number of tokens to free.
 *     tokv,    an array of strings containing each token to free.
 *
 * Results:
 *     The function frees an array of tokens.
 */
void Bdf_FreeTokens(int *tokc, char ***tokv)
{
    char **p;
    int i;
    
    p = *tokv;
    
    for (i = 0; i < *tokc; i++) {
        if (p[i]) {
            free(p[i]);
        }
    }
    
    free(*tokv);
    
    *tokc = 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FreeGlyph(Bdf_Glyph *glyph)
 *
 * Description:
 *     Free a BDF font glyph.
 *
 * Arguments:
 *     glyph,    a glyph structure to free.
 *
 * Results:
 *     The function frees a BDF font glyph.
 */
void Bdf_FreeGlyph(Bdf_Glyph *glyph)
{
    free((char *)(glyph->glyphName));
    free((Bdf_Glyph *)(glyph->bitmap));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FreeFont(Bdf_Font *font)
 *
 * Description:
 *     Free a BDF font.
 *
 * Arguments:
 *     font,    a font structure to free.
 *
 * Results:
 *     The function frees a BDF font.
 */
void Bdf_FreeFont(Bdf_Font *font)
{
    long i;
    
    for (i = 0; i < font->chars; i++) {
        free((char *)(font->glyph[i].glyphName));
        free((Bdf_Glyph *)(font->glyph[i].bitmap));
    }
    
    free(font->formatVersion);
    free(font->comment);
    free(font->fontName);
    
    free(font->glyph);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FreeBitmap(Bdf_Bitmap *bitmap)
 *
 * Description:
 *     Free a bitmap image.
 *
 * Arguments:
 *     bitmap,    a bitmap structure to free.
 *
 * Results:
 *     The function frees a bitmap image.
 */
void Bdf_FreeBitmap(Bdf_Bitmap *bitmap)
{
    free((char *)(bitmap->image));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FreeGraymap(Bdf_Graymap *graymap)
 *
 * Description:
 *     Free a graymap image.
 *
 * Arguments:
 *     graymap,    a graymap structure to free.
 *
 * Results:
 *     The function frees a graymap image.
 */
void Bdf_FreeGraymap(Bdf_Graymap *graymap)
{
    free((char *)(graymap->image));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FreePixmap(Bdf_Pixmap *pixmap)
 *
 * Description:
 *     Free a pixmap image.
 *
 * Arguments:
 *     pixmap,    a pixmap structure to free.
 *
 * Results:
 *     The function frees a pixmap image.
 */
void Bdf_FreePixmap(Bdf_Pixmap *pixmap)
{
    free((char *)(pixmap->image));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     unsigned char Bdf_Hex2Bin(unsigned char c)
 *
 * Description:
 *     Convert an hexadecimal digit to a binary number.
 *
 * Arguments:
 *     c,    an hexadecimal digit.
 *
 * Results:
 *     The function converts an hexadecimal digit to a binary number.
 */
unsigned char Bdf_Hex2Bin(unsigned char c)
{
    int b;
    
    if (c == '0') {
        b = 0;
    } else if (c == '1') {
        b = 1;
    } else if (c == '2') {
        b = 2;
    } else if (c == '3') {
        b = 3;
    } else if (c == '4') {
        b = 4;
    } else if (c == '5') {
        b = 5;
    } else if (c == '6') {
        b = 6;
    } else if (c == '7') {
        b = 7;
    } else if (c == '8') {
        b = 8;
    } else if (c == '9') {
        b = 9;
    } else if (toupper(c) == 'A') {
        b = 10;
    } else if (toupper(c) == 'B') {
        b = 11;
    } else if (toupper(c) == 'C') {
        b = 12;
    } else if (toupper(c) == 'D') {
        b = 13;
    } else if (toupper(c) == 'E') {
        b = 14;
    } else if (toupper(c) == 'F') {
        b = 15;
    }
    
    return b;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     char* Bdf_ScanHexadecimal(char *buffer, unsigned char *c)
 *
 * Description:
 *     Scan one byte, hexadecimal number, from a string.
 *
 * Arguments:
 *     c,    an hexadecimal digit.
 *
 * Results:
 *     The function returns an hexadecimal number, from a strings.
 */
char* Bdf_ScanHexadecimal(char *buffer, unsigned char *c)
{
    char *p;
    unsigned char h;
    unsigned char l;
    unsigned char b;
    
    h = 0;
    l = 0;
    
    p = buffer;
    
    if (*p) {
        h = Bdf_Hex2Bin(*p);
    }
    p++;
    if (*p) {
        l = Bdf_Hex2Bin(*p);
    }
    p++;
    
    b = (h << 4) | l;
    
    *c = b;
    
    if (*p) {
        return p;
    }
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_ParseFont(Bdf_Font *font, char *buffer)
 *
 * Description:
 *     Parses a BDF font file.
 *
 * Arguments:
 *     font,      a font structure to fill with font data;
 *     buffer,    a buffer containing the BDF file contants.
 *
 * Results:
 *     The function parses a BDF font file.
 */
int Bdf_ParseFont(Bdf_Font *font, char *buffer)
{
    char *p;
    char *q;
    char *r;
    char *line;
    int n;
    int tokc;
    char **tokv;
    int parsingGlyph;
    int parsingBitmap;
    int i;
    int j;
    unsigned char c;
    
    p = buffer;
    
    parsingGlyph = 0;
    parsingBitmap = 0;
    
    while (*p) {
        q = p;
        
        while ((*q != '\n') && (*q != '\0')) {
            q++;
        }
        
        n = q - p;
        
        if (q != p) {
            line = (char *)malloc(sizeof(char) * n + 1);
            memset(line, '\0', n + 1);
            strncpy(line, p, n);
            
            if (Bdf_GetTokens(line, " ", &tokc, &tokv) > 0) {
                if (strcmp(tokv[0], "STARTFONT") == 0) {
                    parsingGlyph = 0;
                    parsingBitmap = 0;
                    
                    font->formatVersion = (char *)malloc(sizeof(char) * strlen(tokv[1]) + 1);
                    memset(font->formatVersion, '\0', strlen(tokv[1]) + 1);
                    strncpy(font->formatVersion, tokv[1], strlen(tokv[1]));
                } else if (strcmp(tokv[0], "COMMENT") == 0) {
                } else if (strcmp(tokv[0], "CONTENTVERSION") == 0) {
                    font->contentVersion = atoi(tokv[1]);
                } else if (strcmp(tokv[0], "FONT") == 0) {
                    font->fontName = (char *)malloc(sizeof(char) * strlen(tokv[1]) + 1);
                    memset(font->fontName, '\0', strlen(tokv[1]) + 1);
                    strncpy(font->fontName, tokv[1], strlen(tokv[1]));
                } else if (strcmp(tokv[0], "SIZE") == 0) {
                    font->fontSize[0] = atoi(tokv[1]);
                    font->fontSize[1] = atoi(tokv[2]);
                    font->fontSize[2] = atoi(tokv[3]);
                } else if (strcmp(tokv[0], "FONTBOUNDINGBOX") == 0) {
                    font->fontBoundingBox[0] = atoi(tokv[1]);
                    font->fontBoundingBox[1] = atoi(tokv[2]);
                    font->fontBoundingBox[2] = atoi(tokv[3]);
                    font->fontBoundingBox[3] = atoi(tokv[4]);
                } else if (strcmp(tokv[0], "METRICSSET") == 0) {
                    font->metricsSet = atoi(tokv[1]);
                } else if (strcmp(tokv[0], "SWIDTH") == 0) {
                    if (parsingGlyph) {
                        font->glyph[i].swidth[0] = atoi(tokv[1]);
                        font->glyph[i].swidth[1] = atoi(tokv[2]);
                    } else {
                        font->swidth[0] = atoi(tokv[1]);
                        font->swidth[1] = atoi(tokv[2]);
                    }
                } else if (strcmp(tokv[0], "DWIDTH") == 0) {
                    if (parsingGlyph) {
                        font->glyph[i].dwidth[0] = atoi(tokv[1]);
                        font->glyph[i].dwidth[1] = atoi(tokv[2]);
                    } else {
                        font->dwidth[0] = atoi(tokv[1]);
                        font->dwidth[1] = atoi(tokv[2]);
                    }
                } else if (strcmp(tokv[0], "SWIDTH1") == 0) {
                    if (parsingGlyph) {
                        font->glyph[i].swidth1[0] = atoi(tokv[1]);
                        font->glyph[i].swidth1[1] = atoi(tokv[2]);
                    } else {
                        font->swidth1[0] = atoi(tokv[1]);
                        font->swidth1[1] = atoi(tokv[2]);
                    }
                } else if (strcmp(tokv[0], "DWIDTH1") == 0) {
                    if (parsingGlyph) {
                        font->glyph[i].dwidth1[0] = atoi(tokv[1]);
                        font->glyph[i].dwidth1[1] = atoi(tokv[2]);
                    } else {
                        font->dwidth1[0] = atoi(tokv[1]);
                        font->dwidth1[1] = atoi(tokv[2]);
                    }
                } else if (strcmp(tokv[0], "VVECTOR") == 0) {
                    if (parsingGlyph) {
                        font->glyph[i].vvector[0] = atoi(tokv[1]);
                        font->glyph[i].vvector[1] = atoi(tokv[2]);
                    } else {
                        font->vvector[0] = atoi(tokv[1]);
                        font->vvector[1] = atoi(tokv[2]);
                    }
                } else if (strcmp(tokv[0], "STARTPROPERTIES") == 0) {
                } else if (strcmp(tokv[0], "ENDPROPERTIES") == 0) {
                } else if (strcmp(tokv[0], "CHARS") == 0) {
                    font->chars = atoi(tokv[1]);
                    font->glyph = (Bdf_Glyph *)malloc(sizeof(Bdf_Glyph) * font->chars);
                    i = -1;
                } else if (strcmp(tokv[0], "STARTCHAR") == 0) {
                    parsingGlyph = 1;
                    i++;
                    Bdf_NewGlyph(font->glyph[i], tokv[1], font);
                } else if (strcmp(tokv[0], "ENCODING") == 0) {
                    if (tokc == 2) {
                        font->glyph[i].encoding[0] = atoi(tokv[1]);
                    }
                    if (tokc == 3) {
                        font->glyph[i].encoding[1] = atoi(tokv[2]);
                    }
                } else if (strcmp(tokv[0], "BBX") == 0) {
                    font->glyph[i].bbx[0] = atoi(tokv[1]);
                    font->glyph[i].bbx[1] = atoi(tokv[2]);
                    font->glyph[i].bbx[2] = atoi(tokv[3]);
                    font->glyph[i].bbx[3] = atoi(tokv[4]);
                    font->glyph[i].bitmapLength = font->glyph[i].bbx[1] * roundup(font->glyph[i].bbx[0] / 8.0);
                } else if (strcmp(tokv[0], "BITMAP") == 0) {
                    parsingBitmap = 1;
                    font->glyph[i].bitmap = (char *)malloc(sizeof(char) * font->glyph[i].bitmapLength);
                    j = 0;
                } else if (strcmp(tokv[0], "ENDCHAR") == 0) {
                    parsingGlyph = 0;
                    parsingBitmap = 0;
                } else if (strcmp(tokv[0], "ENDFONT") == 0) {
                } else {
                    if (parsingBitmap) {
                        if (j < font->glyph[i].bitmapLength) {
                            r = (char *)(tokv[0]);
                            while((r = Bdf_ScanHexadecimal(r, &c))) {
                                font->glyph[i].bitmap[j] = c;
                                j++;
                            }
                            font->glyph[i].bitmap[j] = c;
                            j++;
                        }
                    }
                }
                
                Bdf_FreeTokens(&tokc, &tokv);
            }
            
            free(line);
        }
        p = p + n + 1;
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_LoadFont(Bdf_Font *font, char *file)
 *
 * Description:
 *     Load a BDF font.
 *
 * Arguments:
 *     font,    a font structure to fill with font data;
 *     file,    BDF file name.
 *
 * Results:
 *     The function loads a BDF font.
 */
int Bdf_LoadFont(Bdf_Font *font, char *file)
{
    FILE *fp;
    long length;
    char *buffer;
    int ret;
    
    fp = fopen(file, "r");
    
    if (fp == NULL) {
        return 0;
    } else {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp) + 1;
        fseek(fp, 0, SEEK_SET);
        
        buffer = (char *)malloc(sizeof(char) * length + 1);
        memset(buffer, '\0', length + 1);
        
        fread(buffer, sizeof(char), length, fp);
        
        fclose(fp);
        
        ret = Bdf_ParseFont(font, buffer);
        
        free(buffer);
        
        return ret;
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_SaveFont(Bdf_Font *font, char *file)
 *
 * Description:
 *     Save a BDF font.
 *
 * Arguments:
 *     font,    a font structure containing font data;
 *     file,    BDF file name.
 *
 * Results:
 *     The function saves a BDF font.
 */
int Bdf_SaveFont(Bdf_Font *font, char *file)
{
    FILE *fp;
    char buffer[256];
    int i;
    int j;
    int k;
    
    fp = fopen(file, "w");
    
    if (fp == NULL) {
        return 0;
    } else {
        sprintf(buffer, "STARTFONT %s\n", font->formatVersion);
        fputs(buffer, fp);
        if (font->comment) {
            sprintf(buffer, "COMMENT %s\n", font->comment);
        }
        fputs(buffer, fp);
        sprintf(buffer, "CONTENTVERSION %d\n", font->contentVersion);
        fputs(buffer, fp);
        sprintf(buffer, "FONT %s\n", font->fontName);
        fputs(buffer, fp);
        sprintf(buffer, "SIZE %d %d %d\n", font->fontSize[0], font->fontSize[1], font->fontSize[2]);
        fputs(buffer, fp);
        sprintf(buffer, "FONTBOUNDINGBOX %d %d %d %d\n", font->fontBoundingBox[0], font->fontBoundingBox[1], font->fontBoundingBox[2], font->fontBoundingBox[3]);
        fputs(buffer, fp);
        sprintf(buffer, "METRICSSET %d\n", font->metricsSet);
        fputs(buffer, fp);
        sprintf(buffer, "SWIDTH %d %d\n", font->swidth[0], font->swidth[1]);
        fputs(buffer, fp);
        sprintf(buffer, "DWIDTH %d %d\n", font->dwidth[0], font->dwidth[1]);
        fputs(buffer, fp);
        sprintf(buffer, "SWIDTH1 %d %d\n", font->swidth1[0], font->swidth1[1]);
        fputs(buffer, fp);
        sprintf(buffer, "DWIDTH1 %d %d\n", font->dwidth1[0], font->dwidth1[1]);
        fputs(buffer, fp);
        sprintf(buffer, "VVECTOR %d %d\n", font->vvector[0], font->vvector[1]);
        fputs(buffer, fp);
        sprintf(buffer, "CHARS %d\n", font->chars);
        fputs(buffer, fp);
        
        for (i = 0; i < font->chars; i++) {
            sprintf(buffer, "STARTCHAR %s\n", font->glyph[i].glyphName);
            fputs(buffer, fp);
            if (font->glyph[i].encoding[0] == -1) {
                sprintf(buffer, "ENCODING %d %d\n", font->glyph[i].encoding[0], font->glyph[i].encoding[1]);
                fputs(buffer, fp);
            } else {
                sprintf(buffer, "ENCODING %d\n", font->glyph[i].encoding[0]);
                fputs(buffer, fp);
            }
            sprintf(buffer, "SWIDTH %d %d\n", font->glyph[i].swidth[0], font->glyph[i].swidth[1]);
            fputs(buffer, fp);
            sprintf(buffer, "DWIDTH %d %d\n", font->glyph[i].dwidth[0], font->glyph[i].dwidth[1]);
            fputs(buffer, fp);
            sprintf(buffer, "SWIDTH1 %d %d\n", font->glyph[i].swidth1[0], font->glyph[i].swidth1[1]);
            fputs(buffer, fp);
            sprintf(buffer, "DWIDTH1 %d %d\n", font->glyph[i].dwidth1[0], font->glyph[i].dwidth1[1]);
            fputs(buffer, fp);
            sprintf(buffer, "VVECTOR %d %d\n", font->glyph[i].vvector[0], font->glyph[i].vvector[1]);
            fputs(buffer, fp);
            sprintf(buffer, "BBX %d %d %d %d\n", font->glyph[i].bbx[0], font->glyph[i].bbx[1], font->glyph[i].bbx[2], font->glyph[i].bbx[3]);
            fputs(buffer, fp);
            
            strcpy(buffer, "BITMAP\n");
            fputs(buffer, fp);
            
            k = 0;
            for (j = 0; j < font->glyph[i].bitmapLength; j++) {
                sprintf(buffer, "%02X", (font->glyph[i].bitmap[j]) & 0xff);
                fputs(buffer, fp);
                k++;
                if (k == roundup(font->glyph[i].bbx[0] / 8.0)) {
                    fputs("\n", fp);
                    k = 0;
                }
            }
            
            strcpy(buffer, "ENDCHAR\n");
            fputs(buffer, fp);
        }
        
        strcpy(buffer, "ENDFONT\n");
        fputs(buffer, fp);
        
        fclose(fp);
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_ExportGlyph(Bdf_Glyph *glyph, char *file)
 *
 * Description:
 *     Exports a glyph bitmap as PBM file.
 *
 * Arguments:
 *     glyph,    the glyph structure;
 *     file,     the file name.
 *
 * Results:
 *     The function exports a glyph bitmap as PBM file.
 */
int Bdf_ExportGlyph(Bdf_Glyph *glyph, char *file)
{
    FILE *fp;
    int n;
    int status;
    int width;
    int height;
    
    width = 8 * roundup(glyph->bbx[0] / 8.0);
    height = glyph->bbx[1];
    
    status = 1;
    
    /*
     * Creates the PBM image file.
     */
    fp = fopen(file, "w");
    
    if (fp != NULL) {
        /* Writes the PBM file header. */
        fputs("P4\n", fp);
        fprintf(fp, "%d %d\n", width, height);
        
        /* Writes the raw image data. */
        n = fwrite(glyph->bitmap, sizeof(char), glyph->bitmapLength, fp);
        
        if (n != glyph->bitmapLength) {
            status = 0;
        }
        
        fclose(fp);
    } else {
        status = 0;
    }
    
    return status;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_GetGlyph(Bdf_Font *font, int encoding, Bdf_Glyph *glyph)
 *
 * Description:
 *     Search a glyph encoding and return the corresponding glyph.
 *
 * Arguments:
 *     font,        the font structure;
 *     encoding,    the encoding to search;
 *     glyph,       the glyph structure.
 *
 * Results:
 *     The function searches a glyph encoding and return the corresponding glyph.
 */
int Bdf_GetGlyph(Bdf_Font *font, int encoding, Bdf_Glyph *glyph)
{
    long i;
    
    for (i = 0; i < font->chars; i++) {
        if (font->glyph[i].encoding[0] != -1) {
            if (font->glyph[i].encoding[0] == encoding) {
                Bdf_CopyGlyph(glyph, font->glyph[i]);
                
                return 1;
            }
        } else {
            if (font->glyph[i].encoding[1] == encoding) {
                Bdf_CopyGlyph(glyph, font->glyph[i]);
                
                return 1;
            }
        }
    }
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_DrawText(Bdf_Font *font, char *text, Bdf_Bitmap *bitmap)
 *
 * Description:
 *     Draw a text string in a bitmap image.
 *
 * Arguments:
 *     font,      the font structure;
 *     text,      text string;
 *     bitmap,    the bitmap structure.
 *
 * Results:
 *     The function draw a text string in a bitmap image.
 */
int Bdf_DrawText(Bdf_Font *font, char *text, Bdf_Bitmap *bitmap)
{
    Bdf_Glyph glyph;
    int w;
    int h;
    int i;
    int j;
    int k;
    int l;
    int s;
    int r;
    int c;
    
    l = strlen(text);
    
    w = 8 * roundup(font->fontBoundingBox[0] / 8.0);
    h = font->fontBoundingBox[1] - (font->fontBoundingBox[3]);
    s = roundup(font->fontBoundingBox[0] / 8.0) * h * l;
    
    bitmap->width = w * l;
    bitmap->height = h;
    
    bitmap->image = (char *)malloc(sizeof(char) * s);
    memset(bitmap->image, 0, s);
    
    c = 0;
    for (i = 0; i < l; i++) {
        if (!Bdf_GetGlyph(font, (unsigned char)(text[i]), &glyph)) {
            Bdf_GetGlyph(font, 32, &glyph);
        }
        
        r = 0;
        k = 0;
        for (j = 0; j < glyph.bitmapLength; j++) {
            bitmap->image[r * l + c + k] = glyph.bitmap[j];
            k++;
            if (k == roundup(glyph.bbx[0] / 8.0)) {
                k = 0;
                r += roundup(glyph.bbx[0] / 8.0);
            }
        }
        
        c += roundup(glyph.bbx[0] / 8.0);
        
        Bdf_FreeGlyph(&glyph);
    }
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_ExportBitmap(Bdf_Bitmap *bitmap, char *file)
 *
 * Description:
 *     Exports a bitmap image as PBM file.
 *
 * Arguments:
 *     bitmap,    the bitmap structure;
 *     file,      the file name.
 *
 * Results:
 *     The function exports a bitmap image as PBM file.
 */
int Bdf_ExportBitmap(Bdf_Bitmap *bitmap, char *file)
{
    FILE *fp;
    int n;
    int status;
    
    status = 1;
    
    /*
     * Creates the PBM image file.
     */
    fp = fopen(file, "w");
    
    if (fp != NULL) {
        /* Writes the PBM file header. */
        fputs("P4\n", fp);
        fprintf(fp, "%d %d\n", bitmap->width, bitmap->height);
        
        /* Writes the raw image data. */
        n = fwrite(bitmap->image, sizeof(char), bitmap->height * roundup(bitmap->width / 8.0), fp);
        
        if (n != bitmap->height * roundup(bitmap->width / 8.0)) {
            status = 0;
        }
        
        fclose(fp);
    } else {
        status = 0;
    }
    
    return status;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_ExportGraymap(Bdf_Graymap *graymap, char *file)
 *
 * Description:
 *     Exports a graymap image as PGM file.
 *
 * Arguments:
 *     graymap,    the graymap structure;
 *     file,       the file name.
 *
 * Results:
 *     The function exports a graymap image as PGM file.
 */
int Bdf_ExportGraymap(Bdf_Graymap *graymap, char *file)
{
    FILE *fp;
    int n;
    int status;
    
    status = 1;
    
    /*
     * Creates the PGM image file.
     */
    fp = fopen(file, "w");
    
    if (fp != NULL) {
        /* Writes the PBM file header. */
        fputs("P5\n", fp);
        fprintf(fp, "%d %d\n", graymap->width, graymap->height);
        fputs("255\n", fp);
        
        /* Writes the raw image data. */
        n = fwrite(graymap->image, sizeof(char), graymap->width * graymap->height, fp);
        
        if (n != graymap->width * graymap->height) {
            status = 0;
        }
        
        fclose(fp);
    } else {
        status = 0;
    }
    
    return status;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Bdf_ExportPixmap(Bdf_Pixmap *pixmap, char *file)
 *
 * Description:
 *     Exports a pixmap image as PPM file.
 *
 * Arguments:
 *     pixmap,    the pixmap structure;
 *     file,      the file name.
 *
 * Results:
 *     The function exports a pixmap image as PPM file.
 */
int Bdf_ExportPixmap(Bdf_Pixmap *pixmap, char *file)
{
    FILE *fp;
    int n;
    int status;
    
    status = 1;
    
    /*
     * Creates the PPM image file.
     */
    fp = fopen(file, "w");
    
    if (fp != NULL) {
        /* Writes the PBM file header. */
        fputs("P6\n", fp);
        fprintf(fp, "%d %d\n", pixmap->width, pixmap->height);
        fputs("255\n", fp);
        
        /* Writes the raw image data. */
        n = fwrite(pixmap->image, sizeof(char), pixmap->width * pixmap->height * 3, fp);
        
        if (n != pixmap->width * pixmap->height * 3) {
            status = 0;
        }
        
        fclose(fp);
    } else {
        status = 0;
    }
    
    return status;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_Bitmap2Graymap(Bdf_Graymap *graymap, Bdf_Bitmap *bitmap)
 *
 * Description:
 *     Convert a bitmap image to a graymap image.
 *
 * Arguments:
 *     graymap,    the graymap structure;
 *     bitmap,     the bitmap structure.
 *
 * Results:
 *     The function converts a bitmap image to a graymap image.
 */
void Bdf_Bitmap2Graymap(Bdf_Graymap *graymap, Bdf_Bitmap *bitmap)
{
    int w;
    int h;
    int i;
    int j;
    int k;
    int l;
    
    graymap->width = bitmap->width;
    graymap->height = bitmap->height;
    
    graymap->image = (char *)malloc(sizeof(char) * graymap->width * graymap->height);
    memset(graymap->image, 0, graymap->width * graymap->height);
    
    w = graymap->width;
    h = graymap->height;
    
    l = (w / 8.0) * h;
    
    i = 0;
    for (j = 0; j < l; j++) {
        for (k = 7; k >= 0; k--) {
            graymap->image[i] = ((bitmap->image[j] >> k) & 1) ? 255 : 0;
            i++;
        }
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_Bitmap2Pixmap(Bdf_Pixmap *pixmap, Bdf_Bitmap *bitmap, int fr, int fg, int fb, int br, int bg, int bb)
 *
 * Description:
 *     Convert a bitmap image to a pixmap image.
 *
 * Arguments:
 *     pixmap,    the pixmap structure;
 *     bitmap,    the bitmap structure;
 *     fr,        fore color (R component);
 *     fg,        fore color (G component);
 *     fb,        fore color (B component);
 *     br,        background color (R component);
 *     bg,        background color (G component);
 *     bb,        background color (B component).
 *
 * Results:
 *     The function converts a bitmap image to a pixmap image.
 */
void Bdf_Bitmap2Pixmap(Bdf_Pixmap *pixmap, Bdf_Bitmap *bitmap, int fr, int fg, int fb, int br, int bg, int bb)
{
    int w;
    int h;
    int i;
    int j;
    int k;
    int l;
    
    pixmap->width = bitmap->width;
    pixmap->height = bitmap->height;
    
    pixmap->image = (char *)malloc(sizeof(char) * pixmap->width * pixmap->height * 3);
    memset(pixmap->image, 0, pixmap->width * pixmap->height * 3);
    
    w = pixmap->width;
    h = pixmap->height;
    
    l = (w / 8.0) * h;
    
    i = 0;
    for (j = 0; j < l; j++) {
        for (k = 7; k >= 0; k--) {
            pixmap->image[i] = ((bitmap->image[j] >> k) & 1) ? fr : br;
            pixmap->image[i + 1] = ((bitmap->image[j] >> k) & 1) ? fg : bg;
            pixmap->image[i + 2] = ((bitmap->image[j] >> k) & 1) ? fb : bb;
            i += 3;
        }
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Bdf_FlipBitmap(Bdf_Bitmap *target, Bdf_Bitmap *source)
 *
 * Description:
 *     Flip a vertically bitmap image.
 *
 * Arguments:
 *     target,    the original bitmap;
 *     source,    the flipped bitmap.
 *
 * Results:
 *     The function flips vertically a bitmap image.
 */
void Bdf_FlipBitmap(Bdf_Bitmap *target, Bdf_Bitmap *source)
{
    int w;
    int h;
    int i;
    int j;
    int k;
    
    target->width = source->width;
    target->height = source->height;
    
    w = (target->width / 8.0);
    h = target->height;
    
    target->image = (char *)malloc(sizeof(char) * target->width * target->height);
    memset(target->image, 0, target->width * target->height);
    
    for (j = 0; j < w; j++) {
        k = h - 1;
        for (i = 0; i < h; i++) {
            target->image[i * w + j] = source->image[k * w + j];
            k--;
        }
    }
}

