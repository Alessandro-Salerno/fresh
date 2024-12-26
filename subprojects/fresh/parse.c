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

#include <fresh/parse.h>
#include <stddef.h>
#include <stdio.h>

int find_next_space(char *s, size_t i, size_t len) {
  for (; i < len; i++) {
    if (' ' == s[i]) {
      return i;
    }
  }

  return -1;
}

int fresh_parse(char *s, size_t len, unsigned long *argbuf, size_t argbufsz,
                unsigned long *envbuf, size_t envbufsz) {
  size_t start_idx = 0;
  size_t argbuuf_next = 0;
  size_t envbuf_next = 0;

  for (size_t i = 0; i < len; i++) {
    if ('=' == s[i]) {
      int next_space = find_next_space(s, i, len);

      if (-1 == next_space) {
        return -1;
      }

      s[next_space] = 0;

      if (envbuf_next == envbufsz) {
        return -2;
      }

      envbuf[envbuf_next] = (unsigned long)&s[start_idx];
      envbuf_next++;
      start_idx = next_space + 1;
      i = next_space;
      continue;
    }

    if (' ' == s[i]) {
      if (argbuuf_next >= argbufsz) {
        return -2;
      }

      argbuf[argbuuf_next] = (unsigned long)&s[start_idx];
      argbuuf_next++;
      s[i] = 0;
      start_idx = i + 1;
      continue;
    }
  }

  s[len] = 0;

  if (argbuuf_next + 1 >= argbufsz) {
    return -2;
  }

  if (envbuf_next >= envbufsz) {
    return -2;
  }

  argbuf[argbuuf_next] = (unsigned long)&s[start_idx];
  argbuuf_next++;

  argbuf[argbuuf_next] = (unsigned long)NULL;
  envbuf[envbuf_next] = (unsigned long)NULL;
  return 0;
}
