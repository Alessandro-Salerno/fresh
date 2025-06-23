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
        printf("grep: must specify a pattern to recognize\n");
        return -1;
    }

    char  *pattern     = argv[1];
    size_t pattern_len = strlen(pattern);

    char   buf[2048];
    size_t len        = 0;
    size_t line_start = 0;
    bool   print_rem  = false;

    while (0 != (len = sys_read(SYS_STDIN, buf, 2047))) {
        buf[len] = 0;

        for (size_t i = 0; i < len; i++) {
            if (0 == strncmp(&buf[i], pattern, pattern_len)) {
                char c = buf[i];
                buf[i] = 0;
                printf("%s", &buf[line_start]);
                buf[i]               = c;
                c                    = buf[i + pattern_len];
                buf[i + pattern_len] = 0;
                printf("\033[0;34m%s\033[0;0m", &buf[i]);
                buf[i + pattern_len] = c;
                line_start           = i + pattern_len;
                i += pattern_len - 1;
                print_rem = true;
                continue;
            }

            if ('\n' == buf[i]) {
                if (print_rem) {
                    if (line_start < i) {
                        buf[i] = 0;
                        printf("%s", &buf[line_start]);
                        buf[i] = '\n';
                    }

                    printf("\n");
                }

                line_start = 0;
                print_rem  = false;
            }
        }
    }

    return 0;
}
