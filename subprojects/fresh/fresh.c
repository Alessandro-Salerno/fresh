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
#include <fresh/utils.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sysdeps/intf.h>
#include <termios.h>
#include <unistd.h>

int put(int c) {
    if (1 == sys_write(SYS_STDOUT, &c, 1)) {
        return c;
    }

    return EOF;
}

int main(int argc, char *const argv[]) {
    (void)argc;
    (void)argv;

    printf("Welcome to fresh, the freestanding shell!\n");
    printf("Copyright (C) 2024 - 2025 Alessandro Salerno\n\n");
    printf("Running on: %s\n\n", SYS_OS_NAME);

    while (1) {
        char buf[2048];
        printf("fresh-0.0.2 # ");
        fresh_gets(buf, 2048);

        char *argbuf[8 * 128] = {0};
        char *envbuf[8 * 128] = {0};

        switch (fresh_parse(buf,
                            strlen(buf),
                            (unsigned long *)argbuf,
                            128,
                            (unsigned long *)envbuf,
                            128)) {
            case -1:
                printf("fresh: invalid syntax\n");
                break;

            case -2:
                printf("fresh: out of memory\n");
                break;

            case 0: {
                int pid = sys_fork();
                if (pid < 0) {
                    printf("fresh: unable to fork\n");
                    break;
                }

                if (0 == pid) {
                    if (0 != sys_execve(argbuf[0], argbuf, envbuf)) {
                        printf("fresh: %s: command not found\n", argbuf[0]);
                        sys_exit(-1);
                    }
                    break;
                }

                int status;
                sys_waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
