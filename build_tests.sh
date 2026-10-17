#!/bin/bash

Compiler="clang"

CompileFlags=" \
    -g \
    -O0 \
    -ffreestanding \
    -fpie \
    -fno-stack-protector \
    -std=c11 \
    -nostdlib \
    -mavx \
    -Wall -Wextra -Wpedantic -Werror \
    -Wno-unused-parameter \
    -Wno-unused-variable \
    -Wno-unused-but-set-variable \
    -Wno-unused-function"

LinkFlags=" \
    -fuse-ld=lld \
    -Wl,-nostdlib \
    -Wl,--entry,LinuxEntry"

for SourceFile in test*.c; do
    OutputFile=${SourceFile%.*}
    $Compiler $CompileFlags -o $OutputFile $SourceFile $LinkFlags
done

