/*
 *  $Id: strings.c,v 1.4.2.1 2001-07-08 06:39:08 iwao Exp $
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

/*
        long strings library. Copyed from GMW programs!!
*/
#include <stdio.h>
#include "commonhd.h"
#include "wnn_string.h"

w_char *
wnn_Strcat (s1, s2)
     register w_char *s1;
     register w_char *s2;
{
  register w_char *d;

  for (d = s1; *d++ != 0;);
  for (--d; (*d++ = *s2++) != 0;);
  return s1;
}

w_char *
wnn_Strncat (s1, s2, n)
     register w_char *s1;
     register w_char *s2;
     register int n;
{
  register w_char *d;

  for (d = s1; *d++ != 0;);
  for (--d; n > 0 && (*d++ = *s2++) != 0; n--);
  if (n <= 0)
    {
      *d = 0;
      return d;
    }
  else
    {
      return --d;
    }
}

int
wnn_Strcmp (s1, s2)
     register w_char *s1;
     register w_char *s2;
{
  for (; *s1 != 0 && *s1 == *s2; s1++, s2++);
  if (*s1 > *s2)
    return 1;
  if (*s1 == *s2)
    return 0;
  return (-1);
}

/* s1 is substr of s2?  then return 1*/
int
wnn_Substr (s1, s2)
     register w_char *s1;
     register w_char *s2;
{
  for (; *s1 != 0 && *s1 == *s2; s1++, s2++);
  return (int) (!*s1);
}



int
wnn_Strncmp (s1, s2, n)
     register w_char *s1;
     register w_char *s2;
     register int n;
{
  if (n == 0)
    return (0);
  for (; n > 0 && *s1++ == *s2++; n--);
  return (int) (*--s1 - *--s2);
}

w_char *
wnn_Strcpy (s1, s2)
     register w_char *s1;
     register w_char *s2;
{
  register w_char *d;

  for (d = s1; (*d++ = *s2++) != 0;);
  return s1;
}

w_char *
wnn_Strncpy (s1, s2, n)
     register w_char *s1;
     register w_char *s2;
     register int n;
{
  if (s2 > s1)
    {
      for (; n-- > 0;)
        {
          *s1++ = *s2++;
        }
    }
  else if (s2 < s1)
    {
      s1 += n - 1;
      s2 += n - 1;
      for (; n-- > 0;)
        {
          *s1-- = *s2--;
        }
    }
  return s1;
}

int
wnn_Strlen (s)
     register w_char *s;
{
  register int n;

  for (n = 0; *s++ != 0; n++);
  return n;
}

void
wnn_Sreverse (d, s)
     w_char *d, *s;
{
  w_char *s0;

  s0 = s;
  for (; *s++;);
  s--;
  for (; --s >= s0;)
    {
      *d++ = *s;
    }
  *d = 0;
}

