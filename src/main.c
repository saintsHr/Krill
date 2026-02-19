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

#include "log.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool hasKrillExtension(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return false;
    return strcmp(ext, ".krill") == 0;
}

int main(int argc, char** argv) {
    // checks correct usage
    if (argc < 2) {
        logMsg(
            0,
            0,
            0,
            "Incorrect Usage",
            "Invalid usage (wrong arguments)",
            "try using 'krill -h'",
            "unknown",
            FATAL
        );
        return 1;
    }

    const char* filename = argv[1];

    // checks correct extension on input file
    if (!hasKrillExtension(filename)){
        logMsg(
            0,
            0,
            1,
            "Wrong file extension",
            "Source file provided has wrong extension (%s)",
            "Provided file must be a .krill file",
            "unknown",
            FATAL,

            filename
        );
        return 1;
    }

    // tryes to open input file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        logMsg(
            0,
            0,
            2,
            "Cannot open file",
            "Failed to open source file provided (%s)",
            "Make sure the file provided exists",
            "unknown",
            FATAL,

            filename
        );
        return 1;
    }

    // gets file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocates buffer
    char* buffer = malloc(fileSize + 1);
    if (!buffer) {
        logMsg(
            0,
            0,
            3,
            "Failed allocation",
            "Failed to allocate memory for file read",
            "Try to free some RAM and dont load files too big (2GB+)",
            "unknown",
            FATAL
        );
    }

    // reads the file & load to buffer
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    // flags
    bool flag_help = false;

    // parses arguments for flags
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            char flag = argv[i][1];
            switch(flag) {
                case 'h':
                    flag_help = true;
                    break;
                default:
                    logMsg(
                        0,
                        0,
                        4,
                        "Incorrect Usage",
                        "Unknown flag provided: -%c",
                        "try using 'krill -h'",
                        "unknown",
                        FATAL,

                        flag
                    );
                    return 1;
            }
        }
    }

    printf(buffer);

    free(buffer);
    return 0;
}