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
    for (const char *p = start; p < pos; p++) {
        if (*p == '\n') line++;
    }
    return line;
}

int getColumn(const char *start, const char *pos) {
    const char *last_newline = NULL;
    for (const char *p = start; p < pos; p++) {
        if (*p == '\n') last_newline = p;
    }
    if (last_newline)
        return (int)(pos - last_newline);
    else
        return (int)(pos - start + 1);
}

void deleteLine(char *buffer, long line) {
    char *start = buffer;
    char *line_start = buffer;
    char *line_end;
    int current_line = 1;

    while (*start && current_line < line) {
        if (*start == '\n') current_line++;
        start++;
    }
    line_start = start;

    if (!*line_start) return;

    line_end = line_start;
    while (*line_end && *line_end != '\n') line_end++;
    if (*line_end == '\n') line_end++;

    memmove(line_start, line_end, strlen(line_end) + 1);
}

void preprocessor(char* src, long size, const char* filename) {
    char *read_ptr = src;
    char *write_ptr = src;

    while (*read_ptr) {
        if (read_ptr[0] == '/' && read_ptr[1] == '/') {
            while (*read_ptr && *read_ptr != '\n') read_ptr++;
            if (*read_ptr == '\n') read_ptr++;
            continue;
        }

        const char *line_start = read_ptr;
        const char *line_end = line_start;
        while (*line_end && *line_end != '\n') line_end++;

        bool empty = true;
        for (const char *p = line_start; p < line_end; p++) {
            if (*p != ' ' && *p != '\t') {
                empty = false;
                break;
            }
        }
        if (empty) {
            read_ptr = (*line_end) ? line_end + 1 : line_end;
            continue;
        }

        for (const char *p = line_start; p < line_end; p++) {
            if (!isAscii(*p)) {
                logMsg(
                    getLine(src, p),
                    getColumn(src, p),
                    6,
                    "Invalid Character",
                    "Non-ASCII character found",
                    "Use only ASCII characters in source file",
                    filename,
                    FATAL
                );
                exit(1);
            }
        }

        while (read_ptr <= line_end) {
            *write_ptr++ = *read_ptr++;
        }
    }

    *write_ptr = '\0';
}