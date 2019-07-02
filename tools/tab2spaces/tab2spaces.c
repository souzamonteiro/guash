/*
 * tab2spaces.c
 *
 *     This file converts any TAB character to FOUR SPACES in a given file.
 *
 * Copyright (C) 2005 Roberto Luiz Souza Monteiro
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * RCS: @(#) $Id: spaces.c,v 1.0 2007/01/11 18:01:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 65536

int main(int argc, char *argv[])
{
    FILE *src;
    FILE *dst;
    char c;
    
    if (argc != 2) {
        printf("Usage: tab2space filename\n");
    }

    src = fopen(argv[1], "r");

    if (src == NULL) {
        printf("Error: can not open file %s\n", argv[1]);
        exit(1);
    }

    dst = fopen("tab2spaces.tmp", "w");

    if (dst == NULL) {
        printf("Error: can not create file tab2spaces.tmp\n");
        fclose(src);
        exit(1);
    }

    while (!feof(src)) {
        fread(&c, sizeof(char), 1, src);
        if (c == '\t') {
            fwrite("    ", sizeof(char), 4, dst);
        } else {
            fwrite(&c, sizeof(char), 1, dst);
        }
    }
    
    fclose(src);
    fclose(dst);
    
    remove(argv[1]);
    rename("tab2spaces.tmp", argv[1]);
    
    return 0;
}






