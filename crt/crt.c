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

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysdeps/intf.h>

extern int main(int argc, char *const argv[]);
static void (*Exitfn)() = NULL;

void memset(void *buff, int val, size_t buffsize) {
  // Use compiler optiimzations for better performance
  for (size_t i = 0; i < buffsize; i++)
    *(char *)((unsigned long)(buff) + i) = val;
}

int memcmp(const void *buff1, const void *buff2, size_t buffsize) {
  for (size_t i = 0; i < buffsize; i++) {
    char buff1val = *(char *)(buff1 + i);
    char buff2val = *(char *)(buff2 + i);

    if (buff1val != buff2val) {
      return -1;
    }
  }

  return 0;
}

void memcpy(void *dst, const void *src, size_t buffsize) {
  unsigned long src_off = (unsigned long)src;
  unsigned long dst_off = (unsigned long)dst;

  for (size_t i = 0; i < buffsize; i++) {
    char val = *(char *)(src_off + i);
    *(char *)(dst_off + i) = val;
  }
}

void *memchr(const void *str, int c, size_t n) {
  const char *s = str;
  for (size_t i = 0; i < n; i++) {
    if (c == s[i]) {
      return (void *)&s[i];
    }
  }

  return NULL;
}

void *memmove(void *dst, void *src, size_t n) {
  char *pdest = (char *)dst;
  const char *psrc = (const char *)src;

  if (src > dst) {
    for (size_t i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if (src < dst) {
    for (size_t i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dst;
}

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

int strncmp(const char *s1, const char *s2, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (s1[i] != s2[i]) {
      return -1;
    }
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

char *strncpy(char *dst, char *src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i]; i++) {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return dst;
}

char *strcpy(char *dst, const char *src) {
  char *orig = dst;

  while (0 != *src) {
    *dst = *src;
    src++;
    dst++;
  }

  *dst = 0;
  return orig;
}

char *strdup(char *src) {
  size_t l = strlen(src);
  char *buf = malloc(l + 1);
  if (NULL == buf) {
    return NULL;
  }

  strcpy(buf, src);
  return buf;
}

char *strstr(const char *super, const char *sub) {
  size_t sublen = strlen(sub);
  size_t superlen = strlen(super);

  for (size_t i = 0; i < superlen; i++) {
    if (0 == strncmp(&super[i], sub, sublen)) {
      return (char *)&super[i];
    }
  }

  return NULL;
}

char *strcat(char *dest, const char *src) {
  size_t i, j;
  for (i = 0; 0 != dest[i]; i++)
    ;
  for (j = 0; 0 != src[j]; j++)
    dest[i + j] = src[j];
  dest[i + j] = 0;
  return dest;
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

char *getenv(const char *name) { return sys_getenv(name); }

char *gets(char *s) {
  int len = sys_read(SYS_STDIN, s, 2048);
  s[len] = 0;
  return s;
}

void perror(const char *s) { printf("ERROR: %s\n", s); }

int atexit(void (*fn)()) {
  Exitfn = fn;
  return 0;
}

int crt_entry(int argc, char *const argv[]) {
  int ret = main(argc, argv);

  if (NULL != Exitfn) {
    Exitfn();
  }

  return ret;
}

// void *malloc(size_t size) {
//   static char malloc_buf[1024UL * 1024UL * 128];
//   static size_t malloc_off;
//
//   if (malloc_off + sizeof(size_t) + size < sizeof(malloc_buf)) {
//     size_t *sizebuf = (size_t *)&malloc_buf[malloc_off];
//     *sizebuf = size;
//     malloc_off += sizeof(size_t);
//     void *ret = &malloc_buf[malloc_off];
//     malloc_off += size;
//     return ret;
//   }
//
//   printf("ERROR: out of memory. aborting\n");
//   exit(ENOMEM);
//   return NULL;
// }

void free(void *p) {
  (void)p;
  // TODO: implement this properly
}
//
// void *realloc(void *p, size_t size) {
//   void *nbuf = malloc(size);
//
//   if (NULL == nbuf) {
//     return NULL;
//   }
//
//   if (NULL != p) {
//     size_t *sizebuf = (size_t *)((char *)p - sizeof(size_t));
//     size_t old_size = *sizebuf;
//     memcpy(nbuf, p, old_size);
//     free(p);
//   }
//
//   return nbuf;
// }
