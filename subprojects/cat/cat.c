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

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sysdeps/intf.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("cat: must specify path to a file\n");
    return -1;
  }

  char *path = argv[1];
  char buf[2048];
  size_t len = 0;

  int fd = sys_open(path, 0);

  while (0 != (len = sys_read(fd, buf, 2047))) {
    buf[len] = 0;
    sys_write(SYS_STDOUT, buf, len);
  }

  return 0;
}
