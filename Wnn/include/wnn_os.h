/*
 * $Id: wnn_os.h,v 1.1.1.1 2000-01-16 05:07:45 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 *
 * Author: OMRON SOFTWARE Co., Ltd. <freewnn@rd.kyoto.omronsoft.co.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Commentary:
 *
 * Change log:
 *
 * Last modified date: 8,Feb.1999
 *
 * Code:
 *
 */

#ifndef _WNN_OS_
#define _WNN_OS_

/* OS dependent */

#include <signal.h>

#ifdef SYSVR2
#include <fcntl.h>
#include <string.h>
#define index	strchr
#ifndef re_signal
# define re_signal(x, y) signal((x), (y))
#endif
#else
# define re_signal(x, y)
#endif

#if defined(SYSVR2) || defined(UX386)
#include <sys/param.h>
#define getdtablesize() (NOFILE)        /* sys/param.h must be included */
#ifndef SIGCHLD
# define SIGCHLD SIGCLD
#endif
#endif

#include <sys/types.h>
#include <sys/file.h>

#ifdef BSD42
#include <strings.h>
#endif

#ifdef TERMINFO
#include <curses.h>
#include <term.h>
#endif


/*
  if your system has wait3() system call define HAVE_WAIT3.
  wait3() doesn't have to be fully supported.
  uum uses only NULL for the 3rd parameter rusage.
 */

#if !defined(UX386) && !defined(SVR4) && !defined(hpux) && !defined(AIXV3)
#define HAVE_WAIT3
#endif

#if defined(luna) && !defined(SIGNALRETURNSINT)
#define SIGNALRETURNSINT
#endif

#ifndef SIGNALRETURNSINT
typedef void intfntype;
#define SIGNAL_RETURN	return
#else
typedef int intfntype;
#define SIGNAL_RETURN	return(0)
#endif
typedef intfntype (*intfnptr)();

#endif	/* _WNN_OS_ */
