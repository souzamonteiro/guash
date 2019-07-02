/**
 * File:
 *     glft.c
 *
 * Package:
 *     Glft
 *
 * Description:
 *     This file implements a BDF, Glyph Bitmap Distribution Format,
 *     OpenGL render library.
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
 * RCS: @(#) $Id: glft.c,v 1.0 2015/10/04 00:11:00 monteiro Exp $
 * 
 */
 
#include <stddef.h>
#include <GL/gl.h>
#include "glft.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     int Glft_DrawText(float x, float y, char *file, char *text)
 *
 * Description:
 *     Draw a text string in an OpenGL surface.
 *
 * Arguments:
 *     x,       top left x coordinate;
 *     y,       top left y coordinate;
 *     file,    font file;
 *     text,    text string.
 *
 * Results:
 *     The function draw a text string in an OpenGL surface.
 */
int Glft_DrawText(float x, float y, char *file, char *text)
{
    Bdf_Font font;
    Bdf_Bitmap bitmap;
    Bdf_Bitmap flipped;
    
    Bdf_ClearFont(font);
    
    /* Load font. */
    if (!Bdf_LoadFont(&font, file)) {
        return 0;
    }
    
    /* Draw text on a bitmap surface. */
    if (!Bdf_DrawText(&font, (char *)text, &bitmap)) {
        Bdf_FreeFont(&font);
        
        return 0;
    }
    
    /* Flip the bitmap. */
    Bdf_FlipBitmap(&flipped, &bitmap);
    
    /* Draw bitmap on display. */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glRasterPos2f(x, y);
    
    glBitmap(Bdf_BitmapWidth(flipped), Bdf_BitmapHeight(flipped), 0.0, 0.0, (float)(Bdf_BitmapWidth(flipped)), 0.0, (const GLubyte *)(Bdf_BitmapImage(flipped)));
    
    /* Free resources. */
    Bdf_FreeBitmap(&bitmap);
    Bdf_FreeBitmap(&flipped);
    Bdf_FreeFont(&font);
    
    return 1;
}

