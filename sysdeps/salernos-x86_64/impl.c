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

#include <stddef.h>
#include <stdint.h>
#include <sysdeps/intf.h>

extern int main(int argc, char *const argv[]);

const int SYS_STDOUT = 0;
const int SYS_STDIN = 1;
const int SYS_STDERR = 2;

const char *SYS_OS_NAME = "SalernOS";
const char *SYS_OS_VERSION = "0.0.1";
const char *SYS_KERNEL_NAME = "SalernOS Kernel";
const char *SYS_KERNEL_VERSION = "0.2.1";

int salernos_trampoline(uintptr_t *stackptr) {
#define STACK_POP() (*(++stackptr))
  int argc = (int)STACK_POP();
  char **argv = (void *)STACK_POP();

  while (1) {
    char **p = (void *)STACK_POP();
    if (NULL == p) {
      break;
    }
    argv = p;
  }

  return main(argc, argv);
}
