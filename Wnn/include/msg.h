/*
 * $Id: msg.h,v 1.1.1.1 2000-01-16 05:07:45 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright 1990, 1992 OMRON Corporation and Massachusetts Institute of Technology
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

struct msg_bd {
    int msg_id;
    char *msg;
};

struct msg_cat {
    char lang[32];
    char name[64];
    char nlspath[64];
    int msg_cnt;
    struct msg_cat *nextp;
    struct msg_bd *msg_bd;
    /*int encoding;*/
};

#define	DEF_MSG "Message not found.\n"
/*
#define	DEF_LANG "C"
*/
#define	DEF_LANG "ja_JP"

extern struct msg_cat *msg_open();
extern char *msg_get();
extern void msg_close();
