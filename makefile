# MIT License

# Copyright (c) 2026 Henrique Rodrigues Santos

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# ====================================================
# Source and object files
# ====================================================
SRC := $(shell find src -name "*.c")
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

# ====================================================
# Compiler settings
# ====================================================
CC := gcc
DEPFLAGS := -MMD -MP
CFLAGS := -Iinclude -Wall -Wextra $(DEPFLAGS)
LDFLAGS :=

# ====================================================
# Target executable
# ====================================================
TARGET := krill

# ====================================================
# Phony targets
# ====================================================
.PHONY: all clean

# ====================================================
# Default target: build the executable
# ====================================================
all: bin/$(TARGET)

bin/$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# ====================================================
# Compile source files into object files
# ====================================================
build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ====================================================
# Include dependency files
# ====================================================
-include $(OBJ:.o=.d)

# ====================================================
# Clean build artifacts
# ====================================================
clean:
	rm -rf build bin
