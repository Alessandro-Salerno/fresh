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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysdeps/intf.h>

int strcmp(const char *s1, const char *s2) {
  while (0 != *s1 || 0 != *s2) {
    if (*s1 != *s2) {
      return -1;
    }

    s1++;
    s2++;
  }

  return 0;
}

size_t strlen(const char *s) {
  size_t n = 0;
  for (; 0 != *s; s++) {
    n++;
  }
  return n;
}

char *uitoa(unsigned long val, char *s, unsigned long base) {
  if (10 == base) {
    unsigned long size_test = val;
    long size = 0;
    long idx = 0;

    while (size_test >= 10) {
      size_test /= 10;
      size++;
    }

    while (idx < size) {
      long rem = val % 10;
      val /= 10;
      s[size - idx] = rem + '0';
      idx++;
    }

    long rem = val % 10;
    val /= 10;
    s[size - idx] = rem + '0';
    s[size + 1] = 0;

    return s;
  } else if (16 == base) {
    unsigned long size_test = val;
    long size = 0;
    long idx = 0;

    while (size_test >= 16) {
      size_test /= 16;
      size++;
    }

    unsigned long diff = 16 - size;

    for (unsigned long i = 0; i < diff; i++) {
      s[i] = '0';
    }

    while (idx < size) {
      long rem = val % 16;
      val /= 16;
      if (rem < 10) {
        s[15 - idx] = rem + '0';
      } else {
        s[15 - idx] = rem - 10 + 'a';
      }
      idx++;
    }

    long rem = val % 16;
    val /= 16;
    if (rem < 10) {
      s[15 - idx] = rem + '0';
    } else {
      s[15 - idx] = rem - 10 + 'a';
    }
    s[16] = 0;

    return s;
  }

  s[0] = 0;
  return s;
}

char *itoa(long val, char *s, unsigned long base) {
  if (10 == base) {
    unsigned long size_test = 10;
    long size = 0;
    long idx = 0;
    long is_negative = 0;

    is_negative = val < 0;
    s[0] = (val < 0) ? '-' : '+';

    if (val < 0) {
      val *= -1;
    }

    while (val / size_test > 0) {
      size_test *= 10;
      size++;
    }

    while (idx < size) {
      long rem = val % 10;
      val /= 10;
      s[is_negative + size - idx] = rem + '0';
      idx++;
    }

    long rem = val % 10;
    val /= 10;
    s[is_negative + size - idx] = rem + '0';
    s[is_negative + size + 1] = 0;

    return s;
  }

  s[0] = 0;
  return s;
}

int vprintf(const char *fmt, va_list args) {
  int ret = 0;

  for (char *ptr = (char *)(fmt); 0 != *ptr; ptr++) {
    char buf[24];

    switch (*ptr) {
    case '%': {
      switch (*(++ptr)) {
      case 'u':
        (void)uitoa(va_arg(args, unsigned long), buf, 10);
        ret += sys_write(SYS_STDOUT, buf, strlen(buf));
        break;

      case 'p':
      case 'x':
        ret += sys_write(SYS_STDOUT, "0x", 2);
        (void)uitoa(va_arg(args, unsigned long), buf, 16);
        ret += sys_write(SYS_STDOUT, buf, strlen(buf));
        break;

      case 'd':
      case 'i':
        (void)itoa(va_arg(args, long), buf, 10);
        ret += sys_write(SYS_STDOUT, buf, strlen(buf));
        break;

      case 'c':
        buf[0] = (char)va_arg(args, int);
        buf[1] = 0;
        ret += sys_write(SYS_STDOUT, buf, 1);
        break;

      case 's': {
        char *s = va_arg(args, char *);
        ret += sys_write(SYS_STDOUT, s, strlen(s));
        break;
      }
      }

      break;
    }

    default:
      buf[0] = *ptr;
      buf[1] = 0;
      ret += sys_write(SYS_STDOUT, buf, 1);
      break;
    }
  }

  return ret;
}

int printf(const char *fmt, ...) {
  va_list _arguments;
  va_start(_arguments, fmt);
  int ret = vprintf(fmt, _arguments);
  va_end(_arguments);
  return ret;
}
