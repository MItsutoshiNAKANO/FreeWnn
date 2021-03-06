/*
 * $Id: curses.c,v 1.2 2001-06-14 18:16:11 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
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
/*
 * X11R5 Input Method Test Program
 * curses.c v 1.0   Tue Mar  5 18:44:49 JST 1991
 */

/*
 *      Author: Takashi Inoue    OMRON Corporation
 *                               takashi@ari.ncl.omron.co.jp
 */

#include <stdio.h>

#include "exvalue.h"
#include "func.h"

void
initmwin ()                     /* INITialize Main WINdow */
{
  initscr ();
  cbreak ();
  noecho ();
  prdisp = newwin (PRDISP_GEOM);
  modisp = newwin (MODISP_GEOM);
  stdisp = newwin (STDISP_GEOM);
  cls (prdisp);
  cls (modisp);
  cls (stdisp);
}

void
prtitle ()                      /* PRint TITLE */
{
  cls (prdisp);
  prtorm (TITLE_YX, "X11R5 Input Method Test Program");
  prtorm (COPYR_YX, "Copyright 1991 by OMRON Corporation");
}

/* *INDENT-OFF* */
u_short
psmenu ()                       /* Print and Select main MENU */
/* *INDENT-ON* */

{
  int num = 0;                  /* menu NUMber for selection */

  cls (prdisp);
  prtorm (MTITLE_YX, "Selection Mode Menu");
  prtorm (MON_YX, "1. on-the-spot mode");
  prtorm (MOVER_YX, "2. over-the-spot mode");
  prtorm (MOFF_YX, "3. off-the-spot mode");
  prtorm (MROOT_YX, "4. root-window mode");
  prtorm (EXIT_YX, "5. exit");
  prmode ("X11R5 Input Method Test Program : <<Menu Selection mode>>");
  prstatus ("Input number to select test mode.");
  while (1)
    {
      num = getchar ();
      switch (num)
        {
        case '1':
          return (ONSP);
          break;
        case '2':
          return (OVERSP);
          break;
        case '3':
          return (OFFSP);
          break;
        case '4':
          return (ROOTW);
          break;
        case '5':
          return (ENDAT);
          break;
        default:
          break;
        }
    }
}

void
prprint (str)                   /* PRint strings on PRINT part */
     char *str;
{
  wprintw (prdisp, str);
  wrefresh (prdisp);
}

void
prerrfunc ()
{
  cls (prdisp);
  cls (stdisp);
  prprint ("This Function cannot use in this mode.\n\n");
  prprint ("Please Move next test.\n\n");
}

void
prerrld (logfile)               /* PRe-ERRor for Log file Directory */
     char logfile[];
{
  wprintw (prdisp, "Cannot open log file \"%s\".\n\n", logfile);
  wprintw (prdisp, "check environment valuable \"XIMLOGDIR\".\n\n");
  wprintw (prdisp, "This program will shutdown soon.\n\nBye.");
  wrefresh (prdisp);
  freexres ();
  freemwin ();
}

void
prveres (num)                   /* PRint VERify RESult */
     int num;
{
  wprintw (prdisp, "Verify valuables have %d error%s.\n\n", num, ((num == 0 || num == 1) ? "" : "s"));
  wrefresh (prdisp);
}

void
prverlog (num)                  /* PRint VERify result for LOG file */
     int num;
{
  fprintf (icfp, "\nVerify valuables have %d error%s.\n\n", num, ((num == 0 || num == 1) ? "" : "s"));
}

void
prtorm (y, x, str)              /* PRint strings of Title OR Menu on print part */
     int x, y;
     char *str;
{
  wmove (prdisp, y, x);
  waddstr (prdisp, str);
  wrefresh (prdisp);
}

int
outchar (c)
     char c;
{
  putchar (c);
  return (0);
}



void
prmode (str)                    /* PRint strings on MODE part */
     char *str;
{
#ifndef SYSV
  char strbuf[MAX_BUF];
  char *bufptr;
  char *nor, *rev;

  char *tgetstr ();
#endif

#ifdef SYSV
  wattrset (modisp, A_REVERSE);
#else
  bufptr = strbuf;
  rev = tgetstr ("mr", &bufptr);
  tputs (rev, 1, outchar);
#endif
  cls (modisp);
  wmove (modisp, MODISP_YX);
  waddstr (modisp, str);
  wrefresh (modisp);
#ifdef SYSV
  wattrset (modisp, 0);
#else
  nor = tgetstr ("me", &bufptr);
  tputs (nor, 1, outchar);
#endif
}

void
prstatus (str)                  /* PRint strings on STATUS part */
     char *str;
{
  if (strcmp (str, "done."))
    {
      cls (stdisp);
      wmove (stdisp, STDISP_YX);
    }
  else
    {
/*      sleep(SLEEP_TIME); */
    }
  wprintw (stdisp, str);
  wrefresh (stdisp);
}

void
prpok ()
{
  wmove (prdisp, 18, 0);
  prprint ("Input character on keyboard ");
  prprint ("or click your mouse on menu selection window.\n\n");
}

void
prpng ()
{
  wmove (prdisp, 18, 0);
  prprint ("I will retry this test from \"create IC\".\n\n");
}

void
cls (win)                       /* CLear Screen on main window */
     WINDOW *win;
{
  wclear (win);
  wrefresh (win);
}

void
freemwin ()                     /* FREE Main WINdow */
{
  cls (prdisp);
  cls (modisp);
  cls (stdisp);
  wrefresh (prdisp);
  delwin (prdisp);
  delwin (modisp);
  delwin (stdisp);
  endwin ();
  printf ("\n\nThis test program will shutdown soon.\n\n");
  printf ("Bye.\n\n");
  exit (NOERR);
}

void
prcaret (position, direction, style)
     int position;
     char *direction;
     char *style;
{
  wprintw (prdisp, "    position  : %-25d\n", position);
  wprintw (prdisp, "    direction : %-25s\n", direction);
  wprintw (prdisp, "    style     : %-25s\n\n", style);
  wrefresh (prdisp);
}
