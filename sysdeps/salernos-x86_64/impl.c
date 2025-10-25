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

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sysdeps/intf.h>

// In impl.S
extern void salernos_sysinfo(void *buf);
extern int
salernos_open(const char *path, unsigned long pathlen, unsigned long flags);
extern void *
salernos_mmap(unsigned long hint, unsigned long size, unsigned long flags);

// These must not be static, because others will use them
// It is non-standard
const int    SYS_STDOUT    = 0;
const int    SYS_STDIN     = 1;
const int    SYS_STDERR    = 2;
const char  *SYS_OS_NAME   = "SalernOS";
const size_t SYS_PAGE_SIZE = 4096;

static char **Env = NULL;

int tcsetattr(int fd, int opt, void *termios) {
    (void)opt;
    return sys_ioctl(fd, TCSETS, termios);
}

int tcgetattr(int fd, void *termios) {
    return sys_ioctl(fd, TCGETS, termios);
}

char *sys_getenv(const char *name) {
    if (NULL == Env) {
        return NULL;
    }

    char  *var      = Env[0];
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

// NOTE: this is because SalernOS 0.2.1-0.2.3 had a weird implementation of
// mmap. DO NOT apply this unless your OS also does weird stuff in there. My
// advice is to implement sys_mmap correctly.
void *sys_mmap(void         *addr,
               size_t        len,
               int           prot,
               int           flags,
               int           fildes,
               unsigned long off) {
    (void)addr;
    (void)prot;
    (void)fildes;
    (void)off;
    return salernos_mmap(0, len, flags);
}

// NOTE: same thing as sys_mmap
int sys_open(const char *path, int flags) {
    return salernos_open(path, strlen(path), (unsigned long)flags);
}

// This is pretty standard ELF auxv, so you could try to use it for your port as
// well
int salernos_trampoline(unsigned long *stackptr) {
#define STACK_POP() (*(stackptr++))
    int    argc = (int)STACK_POP();
    char **argv = (char **)stackptr;

    while (0 != STACK_POP());

    Env = (char **)stackptr;

    return crt_entry(argc, argv);
#undef STACK_POP
}
