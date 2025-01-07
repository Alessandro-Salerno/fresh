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

.global _start
_start:
  movq %rsp, %rdi
  call salernos_trampoline
  movq %rax, %rdi
  movq $0x07, %rax
  int $0x80

.global sys_write
sys_write:
  movq $0x01, %rax
  int $0x80
  ret

.global sys_read
sys_read:
  movq $0x02, %rax
  int $0x80
  ret

.global sys_fork
sys_fork:
  movq $0x04, %rax
  int $0x80
  ret

.global sys_execve
sys_execve:
  movq $0x03, %rax
  int $0x80
  movq %rdx, %rax
  ret

.global salernos_sysinfo
salernos_sysinfo:
  movq $0x05, %rax
  int $0x80
  ret

.global sys_exit
sys_exit:
  movq $0x07, %rax
  int $0x80
  ret

.global sys_waitpid
sys_waitpid:
  movq $0x06, %rax
  int $0x80
  ret

.global salernos_open
salernos_open:
  movq $0x09, %rax
  int $0x80
  ret

.global sys_ioctl
sys_ioctl:
  mov $0x08, %rax
  int $0x80
  ret

.global salernos_mmap
salernos_mmap:
  mov $0x0A, %rax
  int $0x80
  ret
