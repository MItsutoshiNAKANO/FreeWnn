/*
 * $Id: pwd.c,v 1.1.1.1 2000-01-16 05:07:45 ura Exp $
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
/*	Version 4.0
 */
/*
  eval(crypt(PWD, "HA")) routine.
*/
#ifndef JS
#include <stdio.h>
#include "commonhd.h"
#include "jslib.h"
#include "wnn_os.h"
#endif

extern char *crypt();

#ifdef JS
static
#endif
void
new_pwd(src, encd)
char *src, *encd;
{
    int i, x,c;
    char xx[2];
    char *cr;

    if(encd == NULL)encd = src;
    if(strcmp(src, "") == 0){ bzero(encd, WNN_PASSWD_LEN);return;}
    x = time(NULL);
    xx[0] = x & 0x3f;
    xx[1] = (x & 0x3f00) >> 8;
    for (i = 0; i < 2; i++) {
	c = xx[i] + '.';
	if (c > '9')
	    c += 7;
	if (c > 'Z')
	    c += 6;
	xx[i] = c;
    }
    cr = crypt(src, xx);
    bzero(encd, WNN_PASSWD_LEN);
    strncpy(encd, cr, WNN_PASSWD_LEN);
}

#ifdef JS
static
#endif
int
check_pwd(src, encd)
char *src, *encd;
{
    if(strcmp(encd, "") == 0) return(1);  /* No passwd */
    return(!strncmp(encd, crypt(src, encd),WNN_PASSWD_LEN));
}
