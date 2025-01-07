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
// SOFTWARE.

#ifndef FRESH_SYSDEPS_INTF_H
#define FRESH_SYSDEPS_INTF_H

#include <stddef.h>

// DEFINED BY crt.c
int crt_entry(int argc, char *const argv[]);

extern const int SYS_STDOUT;
extern const int SYS_STDIN;
extern const int SYS_STDERR;

extern const char *SYS_OS_NAME;
extern const char *SYS_KERNEL_NAME;
extern const char *SYS_CPU_NAME;
extern const char *SYS_GPU_NAME;
extern const char *SYS_MEMORY;
extern const size_t SYS_USED_MEMORY;
extern const size_t SYS_TOTAL_MEMORY;
extern const size_t SYS_PAGE_SIZE;

char *sys_getenv(const char *name);
int sys_write(int fd, const void *s, size_t n);
int sys_read(int fd, void *buf, size_t n);
int sys_fork(void);
int sys_execve(const char *path, char *const argv[], char *const envp[]);
__attribute__((noreturn)) void sys_exit(int status);
int sys_waitpid(int pid, int *status, int flags);
int sys_open(const char *path, int flags);
int sys_ioctl(int fd, int op, void *buf);
void *sys_mmap(void *addr, size_t len, int prot, int flags, int fildes,
               unsigned long off);

#endif // !FRESH_SYSDEPS_INTF_H
