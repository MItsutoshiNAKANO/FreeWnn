/*
 * $Id: server_env.c,v 1.2 2000-01-16 06:37:13 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright 1991, 1992 Massachusetts Institute of Technology
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

#include <stdio.h>
#include "commonhd.h"
#include "wnn_config.h"
#include "wnn_os.h"

typedef struct _server_env_struct {
    char *lang;
    char *env;
} server_env_struct;

static server_env_struct server_env[] = {
    {WNN_J_LANG, WNN_JSERVER_ENV},
    {WNN_C_LANG, WNN_CSERVER_ENV},
    {WNN_K_LANG, WNN_KSERVER_ENV},
    {WNN_T_LANG, WNN_TSERVER_ENV},
    {NULL, NULL}
};

char *
get_server_env(lang)
register char *lang;
{
    register server_env_struct *p;

    if (!lang || !*lang) return(NULL);

    for (p = server_env; p->lang; p++) {
	if (!strncmp(lang, p->lang, strlen(lang))) {
	    return(p->env);
	}
    }
    return(NULL);
}
