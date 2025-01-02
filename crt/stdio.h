// MIT License
//
// Copyright (c) 2024 Alessandro Salerno
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

#ifndef CRT_STDIO_H
#define CRT_STDIO_H

#include <stdarg.h>
#include <sysdeps/intf.h>

#define STDIN_FILENO SYS_STDIN
#define STDOUT_FILENO SYS_STDOUT
#define STDERR_FILENO SYS_STDERR

#define read(...) sys_read(__VA_ARGS__)
#define write(...) sys_write(__VA_ARGS__)

#define EOF (-1)

int vprintf(const char *fmt, va_list args);
int printf(const char *fmt, ...);
char *gets(char *s);
void perror(const char *s);

#endif
