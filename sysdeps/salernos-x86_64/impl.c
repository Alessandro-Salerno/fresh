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

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sysdeps/intf.h>

char *itoa(long val, char *s, unsigned long base);

struct salernos_sysinfo {
  char cpu[64];
  char gpu[64];
  char kernel[64];
  uintmax_t used_mem;
  uintmax_t sys_mem;
};

extern int main(int argc, char *const argv[]);
extern void salernos_sysinfo(void *buf);

const int SYS_STDOUT = 0;
const int SYS_STDIN = 1;
const int SYS_STDERR = 2;

const char *SYS_OS_NAME = "SalernOS";
const char *SYS_KERNEL_NAME = NULL;
const char *SYS_CPU_NAME = NULL;
const char *SYS_GPU_NAME = NULL;
const char *SYS_MEMORY = NULL;

static char **Env = NULL;
static struct salernos_sysinfo SysInfo;

char *sys_getenv(const char *name) {
  if (NULL == Env) {
    return NULL;
  }

  char *var = Env[0];
  size_t name_len = strlen(name);

  for (size_t i = 1; NULL != var; i++) {
    if (0 == strncmp(var, name, name_len)) {
      if ('=' == var[name_len]) {
        return &var[name_len + 1];
      }
    }

    var = Env[i];
  }

  return NULL;
}

int salernos_trampoline(uintptr_t *stackptr) {
#define STACK_POP() (*(stackptr++))
  int argc = (int)STACK_POP();
  char **argv = (char **)stackptr;

  while (0 != STACK_POP())
    ;

  Env = (char **)stackptr;

  salernos_sysinfo(&SysInfo);
  SYS_KERNEL_NAME = SysInfo.kernel;

  char memory[64];
  itoa(SysInfo.used_mem / (1024UL * 1024UL), memory, 10);
  size_t p1_len = strlen(memory);
  memory[p1_len] = ' ';
  memory[p1_len + 1] = 'M';
  memory[p1_len + 2] = 'i';
  memory[p1_len + 3] = 'B';
  memory[p1_len + 4] = ' ';
  memory[p1_len + 5] = '/';
  memory[p1_len + 6] = ' ';
  itoa(SysInfo.sys_mem / (1024UL * 1024UL), &memory[p1_len + 7], 10);
  size_t p2_len = strlen(memory);
  memory[p2_len] = ' ';
  memory[p2_len + 1] = 'M';
  memory[p2_len + 2] = 'i';
  memory[p2_len + 3] = 'B';
  memory[p2_len + 4] = 0;
  SYS_MEMORY = memory;

  return main(argc, argv);
#undef STACK_POP
}
