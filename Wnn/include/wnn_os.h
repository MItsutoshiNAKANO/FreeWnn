/*
 *  $Id: wnn_os.h,v 1.5 2001-09-16 11:46:54 hiroo Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000
 *
 * Maintainer:  FreeWnn Project   <freewnn@tomo.gr.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef WNN_OS_H
#define WNN_OS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* OS dependent */

#include <signal.h>

#ifdef SYSVR2
#include <fcntl.h>
#include <string.h>
#define index   strchr
#ifndef re_signal
# define re_signal(x, y) signal((x), (y))
#endif
#else
# define re_signal(x, y)
#endif

#if defined(HAVE_SYS_PARAM_H)
#include <sys/param.h>
#define getdtablesize() (NOFILE)        /* sys/param.h must be included */
#ifndef SIGCHLD
#define SIGCHLD SIGCLD
#endif
#endif /* HAVE_SYS_PARAM_H */

#include <sys/types.h>
#include <sys/file.h>

#ifdef BSD42
#include <strings.h>
#endif

#ifdef TERMINFO
#include <curses.h>
#include <term.h>
#endif

#if defined(luna) && !defined(SIGNALRETURNSINT)
#define SIGNALRETURNSINT
#endif

#ifndef SIGNALRETURNSINT
typedef void intfntype;
#define SIGNAL_RETURN   return
#else
typedef int intfntype;
#define SIGNAL_RETURN   return(0)
#endif
typedef intfntype (*intfnptr) ();

/* ISO C and K&R compatibility */
#if !defined(P_)
# if (defined(__STDC__) && (__STDC__)) || defined(_cplusplus)
#  define P_(p) p
# else
#  define P_(p) ()
# endif
#endif


#endif  /* WNN_OS_H */
