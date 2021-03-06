/*
 *  $Id: wnntouch.c,v 1.9 2013-09-02 11:01:39 itisango Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002, 2004
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

/*
 * 辞書のヘッダを書き換えて、辞書の inode と合わせるプログラム。
 */

#ifndef lint
static char *rcs_id = "$Id: wnntouch.c,v 1.9 2013-09-02 11:01:39 itisango Exp $";
#endif /* lint */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#if STDC_HEADERS
#  include <stdlib.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#include "jslib.h"
#include "commonhd.h"
#include "getopt.h"	/* GNU getopt in the stock */

#include "etc.h"

extern int input_file_header (), check_inode (), change_file_uniq ();
static void usage (void);

char *com_name;
struct wnn_file_head fh;

static void
parse_options (int argc, char** argv)
{
  int c;

  while ((c = getopt (argc, argv, "")) != EOF)
    {
    }
  if (optind)
    {
      optind--;
      argc -= optind;
      argv += optind;
    }
  if (argc < 2)
    {
      usage ();
    }
}

static void
usage (void)
{
  fprintf (stderr, "Usage: %s Wnn_file_name* \n", com_name);
  exit (1);
}

int
main (int argc, char** argv)
{
  FILE *ifpter;
  int k;

  com_name = argv[0];
  parse_options (argc, argv);

  for (k = 1; k < argc; k++)
    {
      if ((ifpter = fopen (argv[k], "r")) == NULL)
        {
          fprintf (stderr, "Can't open the input file %s.\n", argv[k]);
          perror ("");
          exit (1);
        }
      if (input_file_header (ifpter, &fh) == -1)
        {
          fprintf (stderr, "%s %s: It's not a Wnn File.\n", com_name, argv[k]);
          exit (1);
        }
      if (check_inode (ifpter, &fh) == -1)
        {
          if (change_file_uniq (&fh, argv[k]) == -1)
            {
              fprintf (stderr, "%s %s: Can't change file_uniq.\n", com_name, argv[k]);
              perror ("");
              exit (1);
            }
        }
      fclose (ifpter);
    }
  exit (0);
}
