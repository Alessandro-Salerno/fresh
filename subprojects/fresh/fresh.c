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

#include <fresh/custom_crt.h>
#include <fresh/parse.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sysdeps/intf.h>

struct strtup {
  const char *s1;
  const char *s2;
};

static const char *SHELL_LOGO = "              ooooooooooo\n"
                                "          ooooooooooooooooooo\n"
                                "       oooooooo   oooo   oooooooo\n"
                                "     ooooooo      oooo      ooooooo\n"
                                "    oooooo        oooo        oooooo\n"
                                "   oooooo         oooo         oooooo\n"
                                "   ooooo          oooo          ooooo\n"
                                "  oooooo         oooooo         oooooo\n"
                                "  oooooo        oooooooo        oooooo\n"
                                "   ooooo      oooooooooooo      ooooo\n"
                                "   oooooo   oooo  oooo  oooo   oooooo\n"
                                "    oooooooooo    oooo    oooooooooo\n"
                                "     oooooooo     oooo     oooooooo\n"
                                "       oooooooo   oooo   oooooooo\n"
                                "          oooooooooooooooooooo\n"
                                "              oooooooooooo\n";

int put(int c) {
  char buf[2];
  buf[0] = c;
  buf[1] = 0;

  if (1 == sys_write(SYS_STDOUT, buf, 1)) {
    return c;
  }

  return EOF;
}

static void print_logo(void) {
  printf("\n\n");
  size_t logo_len = strlen(SHELL_LOGO);
  size_t line = 0;
  size_t line_i = 0;

  unsigned long colors[] = {196, 214, 190, 34, 45, 21, 129, 129};
  size_t color_idx = 0;

  // hacky thing for testing
  struct strtup data[] = {
      {NULL, NULL},
      {NULL, NULL},
      {NULL, NULL},
      {NULL, NULL},
      {"            \033[0;34mfresh 0.0.1", ""},
      {"           -----------", ""},
      {"           \033[0;34mOS\033[0;0m:      ", SYS_OS_NAME},
      {"          \033[0;34mKernel\033[0;0m:  ", SYS_KERNEL_NAME},
      {"          \033[0;34mCPU\033[0;0m:     ", SYS_CPU_NAME},
      {"           \033[0;34mGPU:\033[0;0m:    ", SYS_GPU_NAME},
      {"           \033[0;34mMemory:\033[0;0m: ", SYS_MEMORY}};

  for (size_t i = 0; i < logo_len; i++) {
    if (0 == line_i) {
      unsigned long color = colors[color_idx];
      printf("\033[38;5;%um", color);
    }

    if ('\n' != SHELL_LOGO[i]) {
      put(SHELL_LOGO[i]);
      line_i++;
    }

    if ('\n' == SHELL_LOGO[i]) {
      if (line < 11 && NULL != data[line].s1) {
        printf("\033[0;0m%s", data[line].s1);

        if (NULL != data[line].s2) {
          printf("%s", data[line].s2);
        } else {
          printf("N/A");
        }
      }

      line++;
      if (0 == line % 2) {
        color_idx++;
      }
      line_i = 0;
      printf("\033[0;0m\n");
    }
  }

  printf("\n\n");
}

int main(int argc, char *const argv[]) {
  (void)argc;
  (void)argv;
  print_logo();
  printf("Welcome to fresh, the freestanding shell!\n");
  printf("Copyright (C) 2024 Alessandro Salerno\n\n");

  printf("Credits to Bob Allison for the logo! Taken from "
         "https://ascii.co.uk/art/peace\n");
  printf("> fresh stands for freedom (do you get the pun?)\n\n");

  while (1) {
    char buf[2048];
    printf("fresh-0.0.1 # ");
    frt_gets(buf, 2048);

    char *argbuf[8 * 128] = {0};
    char *envbuf[8 * 128] = {0};

    switch (fresh_parse(buf, strlen(buf), (unsigned long *)argbuf, 128,
                        (unsigned long *)envbuf, 128)) {
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
