/*
 *  $Id: wnn_os.h,v 1.12 2002-06-22 13:24:31 hiroo Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002
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
#  include <config.h>
#endif

/* system headers needed for system dependent configuration */
#include <signal.h>
#if STDC_HEADERS
#  include <stdlib.h>
#  include <limits.h>
#endif /* STDC_HEADERS */

#include <sys/types.h>
#include <sys/file.h>
#if HAVE_SYS_PARAM_H
#  include <sys/param.h>
#endif

#ifdef TERMINFO
#include <curses.h>
#include <term.h>
#endif

/* strchr vs. index, etc. */
#if (HAVE_MEMSET) && !(HAVE_BZERO)
#  define bzero(adr,n)  memset((adr),0,(n))
#endif
#if !(HAVE_STRCHR) && (HAVE_INDEX)
#  define strchr  index
#endif
#if !(HAVE_STRRCHR) && (HAVE_RINDEX)
#  define strrchr rindex
#endif

/* SIGNAL */

#ifdef SYSVR2
#ifndef re_signal
# define re_signal(x, y) signal((x), (y))
#endif
#else
# define re_signal(x, y)
#endif

#if !defined (SIGCHLD) && defined (SIGCLD)
#  define SIGCHLD SIGCLD
#endif

typedef RETSIGTYPE (*intfnptr) ();

/* Temporally place the number of filedescripters hack here. */
#if HAVE_GETDTABLESIZE
#  define WNN_NFD getdtablesize()
#elif defined (OPEN_MAX)
#  define WNN_NFD OPEN_MAX
#elif defined (NOFILE)
#  define WNN_NFD NOFILE
#endif /* HAVE_GETDTABLESIZE */

/* pseudo ramdom number */
#if !defined (RAND_MAX)
#  if defined (INT_MAX)
#    define RAND_MAX INT_MAX
#  else
#    define RAND_MAX 0x7fffffff
#  endif /* INT_MAX */
#endif /* RAND_MAX*/

#if HAVE_DRAND48
#  define DRAND()  drand48 ()
#  define SDRAND(x)  srand48 (x)
#elif HAVE_RAMDOM
#  define DRAND()  ((double) random() / (double) RAND_MAX)
#  define SDRAND(x)  srandom (x)
#else
#  define DRAND()  ((double) rand() / (double) RAND_MAX)
#  define SDRAND(x)  srand (x)
#endif /* HAVE_DRAND48 */

#if HAVE_RANDOM
#  define RAND()  random ()
#  define SRAND(x)  srandom (x)
#else
#  define RAND()  rand ()
#  define SRAND(x)  srand (x)
#endif /* HAVE_RANDOM */

/* function prototypes (temporal use. need reconstruction) */
unsigned int create_cswidth (char *s);	/* xutoj.c */
int euksc_to_ksc (unsigned char *ksc,
		  unsigned char *euksc,
		  int eusiz);		/* xutoj.c */
int get_short (short *sp, FILE *ifpter);	/* bdic.c */

#endif  /* WNN_OS_H */
