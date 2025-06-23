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

#ifndef CRT_TERMIOS_H
#define CRT_TERMIOS_H

#include <sys/termios.h>

struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

// THE FOLLOWING CODE IS TAKEN FROM THE GNU C LIBRARY
/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/*
 *	POSIX Standard: 7.1-2 General Terminal Interface	<termios.h>
 */

#ifdef __USE_MISC
/* Compare a character C to a value VAL from the `c_cc' array in a
   `struct termios'.  If VAL is _POSIX_VDISABLE, no character can match it.  */
#define CCEQ(val, c) ((c) == (val) && (val) != _POSIX_VDISABLE)
#endif

/* Return the output baud rate stored in *TERMIOS_P.  */
extern speed_t cfgetospeed(const struct termios *__termios_p);

/* Return the input baud rate stored in *TERMIOS_P.  */
extern speed_t cfgetispeed(const struct termios *__termios_p);

/* Set the output baud rate stored in *TERMIOS_P to SPEED.  */
extern int cfsetospeed(struct termios *__termios_p, speed_t __speed);

/* Set the input baud rate stored in *TERMIOS_P to SPEED.  */
extern int cfsetispeed(struct termios *__termios_p, speed_t __speed);

#ifdef __USE_MISC
/* Set both the input and output baud rates in *TERMIOS_OP to SPEED.  */
extern int cfsetspeed(struct termios *__termios_p, speed_t __speed);
#endif

/* Put the state of FD into *TERMIOS_P.  */
extern int tcgetattr(int __fd, struct termios *__termios_p);

/* Set the state of FD to *TERMIOS_P.
   Values for OPTIONAL_ACTIONS (TCSA*) are in <bits/termios.h>.  */
extern int
tcsetattr(int __fd, int __optional_actions, const struct termios *__termios_p);

#ifdef __USE_MISC
/* Set *TERMIOS_P to indicate raw mode.  */
extern void cfmakeraw(struct termios *__termios_p);
#endif

/* Send zero bits on FD.  */
extern int tcsendbreak(int __fd, int __duration);

/* Wait for pending output to be written on FD.

   This function is a cancellation point and therefore not marked with
   .  */
extern int tcdrain(int __fd);

/* Flush pending data on FD.
   Values for QUEUE_SELECTOR (TC{I,O,IO}FLUSH) are in <bits/termios.h>.  */
extern int tcflush(int __fd, int __queue_selector);

/* Suspend or restart transmission on FD.
   Values for ACTION (TC[IO]{OFF,ON}) are in <bits/termios.h>.  */
extern int tcflow(int __fd, int __action);

#if defined __USE_XOPEN_EXTENDED || defined __USE_XOPEN2K8
/* Get process group ID for session leader for controlling terminal FD.  */
extern __pid_t tcgetsid(int __fd);
#endif

#endif
