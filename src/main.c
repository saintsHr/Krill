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
#include "preprocessor.h"

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
        exit(1);
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
        exit(1);
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
        exit(1);
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
        free(buffer);
        fclose(file);
        exit(1);
    }

    // reads the file & load to buffer
    size_t readBytes = fread(buffer, 1, fileSize, file);
    if (readBytes != fileSize) {
        logMsg(
            0,
            0,
            4,
            "Failed to read file",
            "Could not read the entire file",
            "Check file integrity",
            filename,
            FATAL
        );
        free(buffer);
        fclose(file);
        exit(1);
    }
    buffer[fileSize] = '\0';
    fclose(file);

    // flags
    bool  flag_help = false;
    char* output_filename = "output.asm";

    // parses arguments for flags
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            char flag = argv[i][1];
            switch(flag) {
                case 'h':
                    flag_help = true;
                    break;
                
                case 'o':
                    output_filename = argv[i + 1];
                    i++;
                    break;

                default:
                    logMsg(
                        0,
                        0,
                        5,
                        "Incorrect Usage",
                        "Unknown flag provided: -%c",
                        "try using 'krill -h'",
                        "unknown",
                        FATAL,

                        flag
                    );
                    exit(1);
            }
        }
    }
    
    size_t bufferSize = strlen(buffer);

    // preprocessor
    preprocessor(buffer, fileSize, filename);
    bufferSize = strlen(buffer);

    // tryes to open output file
    FILE* outputFile = fopen(output_filename, "w");
    if (!outputFile) {
        logMsg(
            0,
            0,
            7,
            "Cannot open file",
            "Failed to open output file (%s)",
            "Ensure that you have disk space and writing permission",
            "unknown",
            FATAL,

            output_filename
        );
        exit(1);
    }

    // tryes to write the buffer to the output file
    size_t written = fwrite(buffer, 1, bufferSize, outputFile);
    if (written != bufferSize) {
        logMsg(
            0,
            0,
            8,
            "Cannot write file",
            "Failed write output to file",
            "Make sure you have enough RAM and try again",
            "unknown",
            FATAL
        );
        fclose(outputFile);
        exit(1);
    }

    fclose(outputFile);
    free(buffer);
    return 0;
}