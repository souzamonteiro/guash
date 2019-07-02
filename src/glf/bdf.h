/*
 * bdf.h
 *
 *     This file implements a BDF, Glyph Bitmap Distribution Format,
 *     loader library.
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
 * RCS: @(#) $Id: bdf.h,v 1.0 2015/10/06 17:38:00 monteiro Exp $
 * 
 */

typedef struct {
    char *glyphName;
    int encoding[2];
    int swidth[2];
    int dwidth[2];
    int swidth1[2];
    int dwidth1[2];
    int vvector[2];
    int bbx[4];
    int bitmapLength;
    char *bitmap;
} Bdf_Glyph;

typedef struct {
    char *formatVersion;
    char *comment;
    int contentVersion;
    char *fontName;
    int fontSize[3];
    int fontBoundingBox[4];
    int metricsSet;
    int swidth[2];
    int dwidth[2];
    int swidth1[2];
    int dwidth1[2];
    int vvector[2];
    int chars;
    Bdf_Glyph *glyph;
} Bdf_Font;

typedef struct {
    int width;
    int height;
    char *image;
} Bdf_Bitmap;

typedef struct {
    int width;
    int height;
    char *image;
} Bdf_Graymap;

typedef struct {
    int width;
    int height;
    char *image;
} Bdf_Pixmap;

#define Bdf_ClearFont(f) { \
    (f).formatVersion = NULL; \
    (f).comment = NULL; \
    (f).contentVersion = 0; \
    (f).fontName = NULL; \
    (f).fontSize[0] = 0; \
    (f).fontSize[1] = 0; \
    (f).fontSize[2] = 0; \
    (f).fontBoundingBox[0] = 0; \
    (f).fontBoundingBox[1] = 0; \
    (f).fontBoundingBox[2] = 0; \
    (f).fontBoundingBox[3] = 0; \
    (f).metricsSet = 0; \
    (f).swidth[0] = 0; \
    (f).swidth[1] = 0; \
    (f).dwidth[0] = 0; \
    (f).dwidth[1] = 0; \
    (f).swidth1[0] = 0; \
    (f).swidth1[1] = 0; \
    (f).dwidth1[0] = 0; \
    (f).dwidth1[1] = 0; \
    (f).vvector[0] = 0; \
    (f).vvector[1] = 0; \
    (f).chars = 0; \
    (f).glyph = NULL; \
}

#define Bdf_NewGlyph(g,n,f) { \
    (g).glyphName = (char *)malloc(sizeof(char) * (strlen(n) + 1)); \
    memset((g).glyphName, '\0', sizeof(char) * (strlen(n) + 1)); \
    strncpy((g).glyphName, n, strlen(n)); \
    (g).encoding[0] = 0; \
    (g).encoding[1] = 0; \
    (g).swidth[0] = (f)->swidth[0]; \
    (g).swidth[1] = (f)->swidth[1]; \
    (g).dwidth[0] = (f)->dwidth[0]; \
    (g).dwidth[1] = (f)->dwidth[1]; \
    (g).swidth1[0] = (f)->swidth1[0]; \
    (g).swidth1[1] = (f)->swidth1[1]; \
    (g).dwidth1[0] = (f)->dwidth1[0]; \
    (g).dwidth1[1] = (f)->dwidth1[1]; \
    (g).vvector[0] = (f)->vvector[0]; \
    (g).vvector[1] = (f)->vvector[1]; \
    (g).bbx[0] = (f)->fontBoundingBox[0]; \
    (g).bbx[1] = (f)->fontBoundingBox[1]; \
    (g).bbx[2] = (f)->fontBoundingBox[2]; \
    (g).bbx[3] = (f)->fontBoundingBox[3]; \
    (g).bitmapLength = 0; \
    (g).bitmap = NULL; \
}

#define Bdf_CopyGlyph(t,s) { \
    (t)->glyphName = (char *)malloc(sizeof(char) * (strlen((s).glyphName) + 1)); \
    memset((t)->glyphName, '\0', sizeof(char) * (strlen((s).glyphName) + 1)); \
    strncpy((t)->glyphName, (s).glyphName, strlen((s).glyphName)); \
    (t)->encoding[0] = (s).encoding[0]; \
    (t)->encoding[1] = (s).encoding[1]; \
    (t)->swidth[0] = (s).swidth[0]; \
    (t)->swidth[1] = (s).swidth[1]; \
    (t)->dwidth[0] = (s).dwidth[0]; \
    (t)->dwidth[1] = (s).dwidth[1]; \
    (t)->swidth1[0] = (s).swidth1[0]; \
    (t)->swidth1[1] = (s).swidth1[1]; \
    (t)->dwidth1[0] = (s).dwidth1[0]; \
    (t)->dwidth1[1] = (s).dwidth1[1]; \
    (t)->vvector[0] = (s).vvector[0]; \
    (t)->vvector[1] = (s).vvector[1]; \
    (t)->bbx[0] = (s).bbx[0]; \
    (t)->bbx[1] = (s).bbx[1]; \
    (t)->bbx[2] = (s).bbx[2]; \
    (t)->bbx[3] = (s).bbx[3]; \
    (t)->bitmapLength = (s).bitmapLength; \
    (t)->bitmap = (char *)malloc(sizeof(char) * (s).bitmapLength); \
    memcpy((t)->bitmap, (s).bitmap, (s).bitmapLength); \
}

#define Bdf_LynkGlyphs(t,s) { \
    (t)->glyphName = (s)->glyphName; \
    (t)->encoding[0] = (s)->encoding[0]; \
    (t)->encoding[1] = (s)->encoding[1]; \
    (t)->swidth[0] = (s)->swidth[0]; \
    (t)->swidth[1] = (s)->swidth[1]; \
    (t)->dwidth[0] = (s)->dwidth[0]; \
    (t)->dwidth[1] = (s)->dwidth[1]; \
    (t)->swidth1[0] = (s)->swidth1[0]; \
    (t)->swidth1[1] = (s)->swidth1[1]; \
    (t)->dwidth1[0] = (s)->dwidth1[0]; \
    (t)->dwidth1[1] = (s)->dwidth1[1]; \
    (t)->vvector[0] = (s)->vvector[0]; \
    (t)->vvector[1] = (s)->vvector[1]; \
    (t)->bbx[0] = (s)->bbx[0]; \
    (t)->bbx[1] = (s)->bbx[1]; \
    (t)->bbx[2] = (s)->bbx[2]; \
    (t)->bbx[3] = (s)->bbx[3]; \
    (t)->bitmapLength = (s)->bitmapLength; \
    (t)->bitmap = (s)->bitmap; \
}

#define Bdf_GlyphName(g) (g).glyphName
#define Bdf_GlyphEncoding(g) (g).encoding
#define Bdf_GlyphSWidth(g) (g).swidth
#define Bdf_GlyphDWidth(g) (g).dwidth
#define Bdf_GlyphSWidth1(g) (g).swidth1
#define Bdf_GlyphDWidth1(g) (g).dwidth1
#define Bdf_GlyphVVector(g) (g).vvector
#define Bdf_GlyphBBx(g) (g).bbx
#define Bdf_GlyphBitmapLength(g) (g).bitmapLength
#define Bdf_GlyphBitmap(g) (g).bitmap
#define Bdf_GlyphWidth(g) (g).bbx[0]
#define Bdf_GlyphHeight(g) (g).bbx[1]

#define Bdf_FontFormatVersion(f) (f).formatVersion
#define Bdf_FontComment(f) (f).comment
#define Bdf_FontContentVersion(f) (f).contentVersion
#define Bdf_FontName(f) (f).fontName
#define Bdf_FontFontSize(f) (f).fontSize
#define Bdf_FontMetricsSet(f) (f).metricsSet
#define Bdf_FontSWidth(f) (f).swidth
#define Bdf_FontDWidth(f) (f).dwidth
#define Bdf_FontSWidth1(f) (f).swidth1
#define Bdf_FontDWidth1(f) (f).dwidth1
#define Bdf_FontVVector(f) (f).vvector
#define Bdf_FontChars(f) (f).chars
#define Bdf_FontGlyph(f) (f).glyph

#define Bdf_BitmapWidth(b) (b).width
#define Bdf_BitmapHeight(b) (b).height
#define Bdf_BitmapImage(b) (b).image

#define Bdf_GraymapWidth(g) (g).width
#define Bdf_GraymapHeight(g) (g).height
#define Bdf_GraymapImage(g) (g).image

/*
 * Function prototypes.
 */
int Bdf_GetTokens(char *expression, char *separator, int *tokc, char ***tokv);
void Bdf_FreeTokens(int *tokc, char ***tokv);
void Bdf_FreeGlyph(Bdf_Glyph *glyph);
void Bdf_FreeFont(Bdf_Font *font);
void Bdf_FreeBitmap(Bdf_Bitmap *bitmap);
void Bdf_FreeGraymap(Bdf_Graymap *graymap);
void Bdf_FreePixmap(Bdf_Pixmap *pixmap);
unsigned char Bdf_Hex2Bin(unsigned char c);
char* Bdf_ScanHexadecimal(char *buffer, unsigned char *c);
int Bdf_ParseFont(Bdf_Font *font, char *buffer);
int Bdf_LoadFont(Bdf_Font *font, char *file);
int Bdf_SaveFont(Bdf_Font *font, char *file);
int Bdf_ExportGlyph(Bdf_Glyph *glyph, char *file);
int Bdf_GetGlyph(Bdf_Font *font, int encoding, Bdf_Glyph *glyph);
int Bdf_DrawText(Bdf_Font *font, char *text, Bdf_Bitmap *bitmap);
int Bdf_ExportBitmap(Bdf_Bitmap *bitmap, char *file);
int Bdf_ExportGraymap(Bdf_Graymap *graymap, char *file);
int Bdf_ExportPixmap(Bdf_Pixmap *pixmap, char *file);
void Bdf_Bitmap2Graymap(Bdf_Graymap *graymap, Bdf_Bitmap *bitmap);
void Bdf_Bitmap2Pixmap(Bdf_Pixmap *pixmap, Bdf_Bitmap *bitmap, int fr, int fg, int fb, int br, int bg, int bb);
void Bdf_FlipBitmap(Bdf_Bitmap *target, Bdf_Bitmap *source);

