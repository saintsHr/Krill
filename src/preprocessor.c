/*

MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "preprocessor.h"
#include "log.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool isAscii(char c) {
    unsigned char uc = (unsigned char)c;
    return uc <= 127;
}

int getLine(const char *start, const char *pos) {
    int line = 1;
    const char *p = start;
    while (p < pos) {
        if (*p == '\n') {
            line++;
        }
        p++;
    }
    return line;
}

int getColumn(const char *start, const char *pos) {
    int col = 1;
    const char *p = start;
    const char *last_newline = start - 1;

    while (p < pos) {
        if (*p == '\n') {
            last_newline = p;
        }
        p++;
    }
    col = (int)(pos - last_newline);
    return col;
}

void deleteLine(char *buffer, long line) {
    char *start = buffer;
    char *line_start = buffer;
    char *line_end;
    int current_line = 1;

    while (*start && current_line < line) {
        if (*start == '\n') {
            current_line++;
        }
        start++;
    }
    line_start = start;

    if (!*line_start) return;

    line_end = line_start;
    while (*line_end && *line_end != '\n') {
        line_end++;
    }
    if (*line_end == '\n') line_end++;

    memmove(line_start, line_end, strlen(line_end) + 1);
}

void preprocessor(char* src, long size, const char* filename){
    for (long i = 0; src[i] != '\0'; i++){
        // checks if theres any non-ascii character
        if (!isAscii(src[i])) {
            logMsg(
                getLine(src, src + i),
                getColumn(src, src + i),
                6,
                "Invalid Character",
                "Non-ASCII character found",
                "Use only ASCII characters in source file",
                filename,
                FATAL
            );
            exit(1);
        }

        // checks for line comments & remove them
        if (src[i] == '/' && src[i + 1] != '\0' && src[i + 1] == '/') {
            int line = getLine(src, src + i);
            deleteLine(src, line);
            i--;
        } 

        // checks for empty lines & remove them
        
    }

    return;
}