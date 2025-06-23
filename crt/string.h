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

#ifndef CRT_STRING_H
#define CRT_STRING_H

#include <stddef.h>

#define strchr(str, c) memchr(str, c, strlen(str))
#define strerror(...)  "[NOT IMPLEMENTED YET]"

int    strcmp(const char *s1, const char *s2);
int    strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);
char  *uitoa(unsigned long val, char *s, unsigned long base);
char  *itoa(long val, char *s, unsigned long base);
char  *strncpy(char *dst, char *src, size_t n);
char  *strcpy(char *dst, const char *src);
char  *strdup(char *src);
char  *strstr(const char *super, const char *sub);
char  *strcat(char *dest, const char *src);

void  memset(void *buff, int val, size_t buffsize);
int   memcmp(const void *buff1, const void *buff2, size_t buffsize);
void  memcpy(void *dst, const void *src, size_t buffsize);
void *memchr(const void *str, int c, size_t n);
void *memmove(void *dst, void *src, size_t n);

#endif
