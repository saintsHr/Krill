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

#pragma once

#include <stdio.h>
#include <stdarg.h>

#define RESET_COLOR   "\033[0m"
#define INFO_COLOR    "\033[1;36m"
#define WARNING_COLOR "\033[1;35m"
#define ERROR_COLOR   "\033[1;91m"
#define FATAL_COLOR   "\033[1;31m"

typedef enum {
    INFO,
    WARNING,
    ERROR,
    FATAL
} Severity;

static inline void logMsg(
    int lin,
    int col,
    int id,
    const char* msgFmt,
    const char* descFmt,
    const char* hint,
    const char* file,
    Severity sev,
    ...
) {
    const char* sevNames[]  = {"INFO", "WARNING", "ERROR", "FATAL"};
    const char* sevColors[] = {INFO_COLOR, WARNING_COLOR, ERROR_COLOR, FATAL_COLOR};
    const char* prefixes[]  = {"I-", "W-", "E-", "F-"};

    char msg[512];
    char desc[512];

    va_list args;
    va_start(args, sev);

    vsnprintf(msg, sizeof(msg), msgFmt, args);

    va_list args2;
    va_copy(args2, args);
    vsnprintf(desc, sizeof(desc), descFmt, args2);

    va_end(args2);
    va_end(args);

    printf("\n");
    printf(
        "[%s%s%s] [%s%04d%s] %s:%d:%d\n",
        sevColors[sev], sevNames[sev], RESET_COLOR,
        prefixes[sev], id, RESET_COLOR,
        file, lin, col
    );

    printf("  Message:     %s\n", msg);
    printf("  Description: %s\n", desc);
    printf("  Hint:        %s\n", hint);
    printf("\n");
}
