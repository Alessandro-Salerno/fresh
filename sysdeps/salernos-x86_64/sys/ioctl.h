#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H

#include <sysdeps/intf.h>

#define ioctl(...) sys_ioctl(__VA_ARGS__)

// THE FOLLOWING CODE IS TAKEN FROM THE GNU C LIBRARY
/* Copyright (C) 1996-2024 Free Software Foundation, Inc.
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

/* Use the definitions from the kernel header files.  */
#define TCGETS       0x5401
#define TCSETS       0x5402
#define TCSETSW      0x5403
#define TCSETSF      0x5404
#define TCGETA       0x5405
#define TCSETA       0x5406
#define TCSETAW      0x5407
#define TCSETAF      0x5408
#define TCSBRK       0x5409
#define TCXONC       0x540A
#define TCFLSH       0x540B
#define TIOCEXCL     0x540C
#define TIOCNXCL     0x540D
#define TIOCSCTTY    0x540E
#define TIOCGPGRP    0x540F
#define TIOCSPGRP    0x5410
#define TIOCOUTQ     0x5411
#define TIOCSTI      0x5412
#define TIOCGWINSZ   0x5413
#define TIOCSWINSZ   0x5414
#define TIOCMGET     0x5415
#define TIOCMBIS     0x5416
#define TIOCMBIC     0x5417
#define TIOCMSET     0x5418
#define TIOCGSOFTCAR 0x5419
#define TIOCSSOFTCAR 0x541A
#define FIONREAD     0x541B
#define TIOCINQ      FIONREAD
#define TIOCLINUX    0x541C
#define TIOCCONS     0x541D
#define TIOCGSERIAL  0x541E
#define TIOCSSERIAL  0x541F
#define TIOCPKT      0x5420
#define FIONBIO      0x5421
#define TIOCNOTTY    0x5422
#define TIOCSETD     0x5423
#define TIOCGETD     0x5424
#define TCSBRKP      0x5425 /* Needed for POSIX tcsendbreak() */
#define TIOCSBRK     0x5427 /* BSD compatibility */
#define TIOCCBRK     0x5428 /* BSD compatibility */
#define TIOCGSID     0x5429 /* Return the session ID of FD */
#define TCGETS2      _IOR('T', 0x2A, struct termios2)
#define TCSETS2      _IOW('T', 0x2B, struct termios2)
#define TCSETSW2     _IOW('T', 0x2C, struct termios2)
#define TCSETSF2     _IOW('T', 0x2D, struct termios2)
#define TIOCGRS485   0x542E
#ifndef TIOCSRS485
#define TIOCSRS485 0x542F
#endif
#define TIOCGPTN \
    _IOR('T', 0x30, unsigned int)       /* Get Pty Number (of pty-mux device) */
#define TIOCSPTLCK _IOW('T', 0x31, int) /* Lock/unlock Pty */
#define TIOCGDEV                                                             \
    _IOR('T', 0x32, unsigned int) /* Get primary device node of /dev/console \
                                   */
#define TCGETX       0x5432       /* SYS5 TCGETX compatibility */
#define TCSETX       0x5433
#define TCSETXF      0x5434
#define TCSETXW      0x5435
#define TIOCSIG      _IOW('T', 0x36, int) /* pty: generate signal */
#define TIOCVHANGUP  0x5437
#define TIOCGPKT     _IOR('T', 0x38, int) /* Get packet mode state */
#define TIOCGPTLCK   _IOR('T', 0x39, int) /* Get Pty lock state */
#define TIOCGEXCL    _IOR('T', 0x40, int) /* Get exclusive mode state */
#define TIOCGPTPEER  _IO('T', 0x41)       /* Safely open the slave */
#define TIOCGISO7816 _IOR('T', 0x42, struct serial_iso7816)
#define TIOCSISO7816 _IOWR('T', 0x43, struct serial_iso7816)

#define FIONCLEX        0x5450
#define FIOCLEX         0x5451
#define FIOASYNC        0x5452
#define TIOCSERCONFIG   0x5453
#define TIOCSERGWILD    0x5454
#define TIOCSERSWILD    0x5455
#define TIOCGLCKTRMIOS  0x5456
#define TIOCSLCKTRMIOS  0x5457
#define TIOCSERGSTRUCT  0x5458 /* For debugging only */
#define TIOCSERGETLSR   0x5459 /* Get line status register */
#define TIOCSERGETMULTI 0x545A /* Get multiport config  */
#define TIOCSERSETMULTI 0x545B /* Set multiport config */

#define TIOCMIWAIT  0x545C /* wait for a change on serial input line(s) */
#define TIOCGICOUNT 0x545D /* read serial port __inline__ interrupt counts */

/*
 * Some arches already define FIOQSIZE due to a historical
 * conflict with a Hayes modem-specific ioctl value.
 */
#ifndef FIOQSIZE
#define FIOQSIZE 0x5460
#endif

/* Used for packet mode */
#define TIOCPKT_DATA       0
#define TIOCPKT_FLUSHREAD  1
#define TIOCPKT_FLUSHWRITE 2
#define TIOCPKT_STOP       4
#define TIOCPKT_START      8
#define TIOCPKT_NOSTOP     16
#define TIOCPKT_DOSTOP     32
#define TIOCPKT_IOCTL      64

#define TIOCSER_TEMT 0x01 /* Transmitter physically empty */

/* Routing table calls.  */
#define SIOCADDRT 0x890B /* add routing table entry	*/
#define SIOCDELRT 0x890C /* delete routing table entry	*/
#define SIOCRTMSG 0x890D /* call to routing system	*/

/* Socket configuration controls. */
#define SIOCGIFNAME        0x8910 /* get iface name		*/
#define SIOCSIFLINK        0x8911 /* set iface channel		*/
#define SIOCGIFCONF        0x8912 /* get iface list		*/
#define SIOCGIFFLAGS       0x8913 /* get flags			*/
#define SIOCSIFFLAGS       0x8914 /* set flags			*/
#define SIOCGIFADDR        0x8915 /* get PA address		*/
#define SIOCSIFADDR        0x8916 /* set PA address		*/
#define SIOCGIFDSTADDR     0x8917 /* get remote PA address	*/
#define SIOCSIFDSTADDR     0x8918 /* set remote PA address	*/
#define SIOCGIFBRDADDR     0x8919 /* get broadcast PA address	*/
#define SIOCSIFBRDADDR     0x891a /* set broadcast PA address	*/
#define SIOCGIFNETMASK     0x891b /* get network PA mask		*/
#define SIOCSIFNETMASK     0x891c /* set network PA mask		*/
#define SIOCGIFMETRIC      0x891d /* get metric			*/
#define SIOCSIFMETRIC      0x891e /* set metric			*/
#define SIOCGIFMEM         0x891f /* get memory address (BSD)	*/
#define SIOCSIFMEM         0x8920 /* set memory address (BSD)	*/
#define SIOCGIFMTU         0x8921 /* get MTU size			*/
#define SIOCSIFMTU         0x8922 /* set MTU size			*/
#define SIOCSIFNAME        0x8923 /* set interface name		*/
#define SIOCSIFHWADDR      0x8924 /* set hardware address 	*/
#define SIOCGIFENCAP       0x8925 /* get/set encapsulations       */
#define SIOCSIFENCAP       0x8926
#define SIOCGIFHWADDR      0x8927 /* Get hardware address		*/
#define SIOCGIFSLAVE       0x8929 /* Driver slaving support	*/
#define SIOCSIFSLAVE       0x8930
#define SIOCADDMULTI       0x8931 /* Multicast address lists	*/
#define SIOCDELMULTI       0x8932
#define SIOCGIFINDEX       0x8933       /* name -> if_index mapping	*/
#define SIOGIFINDEX        SIOCGIFINDEX /* misprint compatibility :-)	*/
#define SIOCSIFPFLAGS      0x8934       /* set/get extended flags set	*/
#define SIOCGIFPFLAGS      0x8935
#define SIOCDIFADDR        0x8936 /* delete PA address		*/
#define SIOCSIFHWBROADCAST 0x8937 /* set hardware broadcast addr	*/
#define SIOCGIFCOUNT       0x8938 /* get number of devices */

#define SIOCGIFBR 0x8940 /* Bridging support		*/
#define SIOCSIFBR 0x8941 /* Set bridging options 	*/

#define SIOCGIFTXQLEN 0x8942 /* Get the tx queue length	*/
#define SIOCSIFTXQLEN 0x8943 /* Set the tx queue length 	*/

/* ARP cache control calls. */
/*  0x8950 - 0x8952  * obsolete calls, don't re-use */
#define SIOCDARP 0x8953 /* delete ARP table entry	*/
#define SIOCGARP 0x8954 /* get ARP table entry		*/
#define SIOCSARP 0x8955 /* set ARP table entry		*/

/* RARP cache control calls. */
#define SIOCDRARP 0x8960 /* delete RARP table entry	*/
#define SIOCGRARP 0x8961 /* get RARP table entry		*/
#define SIOCSRARP 0x8962 /* set RARP table entry		*/

/* Driver configuration calls */

#define SIOCGIFMAP 0x8970 /* Get device parameters	*/
#define SIOCSIFMAP 0x8971 /* Set device parameters	*/

/* DLCI configuration calls */

#define SIOCADDDLCI 0x8980 /* Create new DLCI device	*/
#define SIOCDELDLCI 0x8981 /* Delete DLCI device		*/

/* Device private ioctl calls.  */

/* These 16 ioctls are available to devices via the do_ioctl() device
   vector.  Each device should include this file and redefine these
   names as their own. Because these are device dependent it is a good
   idea _NOT_ to issue them to random objects and hope.  */

#define SIOCDEVPRIVATE 0x89F0 /* to 89FF */

/*
 *	These 16 ioctl calls are protocol private
 */

#define SIOCPROTOPRIVATE 0x89E0 /* to 89EF */

#endif
