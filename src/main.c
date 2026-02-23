#include "log.h"
#include "preprocessor.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool hasKrillExtension(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return false;
    return strcmp(ext, ".krill") == 0;
}

char* compile(const char* src, const char* filename) {
    // --- preprocessor ---
    char* preprocessed = preprocessor(src, filename);

    return preprocessed;
}

int main(int argc, char** argv) {
    // --- verify correct usage ---
    if (argc < 2) {
        logMsg(0, 0, 0,
            "Incorrect Usage",
            "Invalid usage (wrong arguments)",
            "try using 'krill -h'",
            "unknown",
            FATAL
        );
        return 1;
    }

    const char* filename = argv[1];

    // --- verify extension ---
    if (!hasKrillExtension(filename)) {
        logMsg(0, 0, 1,
            "Wrong file extension",
            "Source file provided has wrong extension (%s)",
            "Provided file must be a .krill file",
            "unknown",
            FATAL,
            filename
        );
        return 1;
    }

    // --- opens input file ---
    FILE* file = fopen(filename, "r");
    if (!file) {
        logMsg(0, 0, 2,
            "Cannot open file",
            "Failed to open source file provided (%s)",
            "Make sure the file provided exists",
            "unknown",
            FATAL,
            filename
        );
        return 1;
    }

    // --- reads input ---
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(fileSize + 1);
    if (!buffer) {
        logMsg(0, 0, 3,
            "Failed allocation",
            "Failed to allocate memory for file read",
            "Try to free some RAM and don't load files too big (2GB+)",
            "unknown",
            FATAL
        );
        fclose(file);
        return 1;
    }

    size_t readBytes = fread(buffer, 1, fileSize, file);
    fclose(file);

    if (readBytes != fileSize) {
        logMsg(0, 0, 4,
            "Failed to read file",
            "Could not read the entire file",
            "Check file integrity",
            filename,
            FATAL
        );
        free(buffer);
        return 1;
    }
    buffer[fileSize] = '\0';

    // --- flags & arguments ---
    bool flag_help = false;
    char* output_filename = "output.asm";

    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            char flag = argv[i][1];
            switch(flag) {
                case 'h':
                    flag_help = true;
                    break;
                case 'o':
                    if (i + 1 < argc) {
                        output_filename = argv[i + 1];
                        i++;
                    }
                    break;
                default:
                    logMsg(0, 0, 5,
                        "Incorrect Usage",
                        "Unknown flag provided: -%c",
                        "try using 'krill -h'",
                        "unknown",
                        FATAL,
                        flag
                    );
                    free(buffer);
                    return 1;
            }
        }
    }

    if (flag_help) {
        printf("Usage: krill <source.krill> [-o output.asm]\n");
        free(buffer);
        return 0;
    }

    // --- compilation ---
    char* compiled = compile(buffer, filename);
    free(buffer);

    // --- writes output ---
    FILE* outputFile = fopen(output_filename, "w");
    if (!outputFile) {
        logMsg(0, 0, 7,
            "Cannot open file",
            "Failed to open output file (%s)",
            "Ensure that you have disk space and writing permission",
            "unknown",
            FATAL,
            output_filename
        );
        free(compiled);
        return 1;
    }

    size_t bufferSize = strlen(compiled);
    size_t written = fwrite(compiled, 1, bufferSize, outputFile);
    fclose(outputFile);

    if (written != bufferSize) {
        logMsg(0, 0, 8,
            "Cannot write file",
            "Failed write output to file",
            "Make sure you have enough RAM and try again",
            "unknown",
            FATAL
        );
        free(compiled);
        return 1;
    }

    free(compiled);
    return 0;
}