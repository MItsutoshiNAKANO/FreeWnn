#ifndef lint
static char *rcs_id = "$Id: sort.c,v 1.1.1.1 2000-01-16 05:07:46 ura Exp $";
#endif /* lint */

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
sort program
*/

#include <stdio.h>
#include "commonhd.h"
#include "config.h"
#include "jslib.h"
#include "jh.h"
#include "jdata.h"

extern int init_heap();
extern void ujis_header(), read_ujis(), reverse_yomi(), sort(),
  output_ujis();
int reverse = 0;
struct JT jt;
struct wnn_file_head file_head;

void
main(argc, argv)
int argc;
char **argv;
{
    int c;
    char *cswidth_name;
    extern char *get_cswidth_name();
    extern void set_cswidth();
    extern int optind;
    extern char *optarg;

    if (cswidth_name = get_cswidth_name(WNN_DEFAULT_LANG))
	set_cswidth(create_cswidth(cswidth_name));
    while ((c = getopt(argc,argv,"r")) != EOF) {
	switch(c){
	case 'r':
	    reverse = 1;
	    break;
	}
    }
    init_heap(MAX_ENTRIES * HEAP_PER_LINE,
	      MAX_ENTRIES * YOMI_PER_LINE,
	      MAX_ENTRIES, MAX_ENTRIES, stdin);
    
#ifdef CHINESE
    { int which_dict;
    ujis_header(&which_dict);
    }
#else
    ujis_header();
#endif
    read_ujis(NORMAL, 0, 0);
#ifdef CHINESE
    sort();
#else
    if(reverse)
	reverse_yomi();
    sort();
    if(reverse)
	reverse_yomi();
#endif
    output_ujis(stdout, 0, 1);
}
