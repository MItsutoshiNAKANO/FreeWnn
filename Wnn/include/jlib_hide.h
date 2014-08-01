/*
 * $Id: jlib_hide.h,v 1.2 2014-08-01 22:04:26 itisango Exp $
 */
/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2001, 2002, 2003, 2013
 *
 * Maintainer:  FreeWnn Project   <freewnn@tomo.gr.jp>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef FRWNN_JLIB_HIDE_H
#define FRWNN_JLIB_HIDE_H 1

#include "jslib.h"

/* defined in jlib/js.c  */
#if DEBUG
extern void xerror FRWNN_PARAMS((char *));
extern void dmp FRWNN_PARAMS((char *, int));
#endif	/* DEBUG  */


#ifdef  nodef
extern void js_set_lang FRWNN_PARAMS((struct wnn_env *, register char *));
#endif	/* nodef  */

extern char *_wnn_get_machine_of_serv_defs FRWNN_PARAMS((const char *));

#endif	/* FRWNN_JLIB_HIDE_H  */
