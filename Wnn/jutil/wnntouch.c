#ifndef lint
static char *rcs_id = "$Id: wnntouch.c,v 1.1.1.1 2000-01-16 05:07:46 ura Exp $";
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
  辞書のヘッダを書き換えて、辞書の inode と合わせるプログラム。
  */

#include <stdio.h>
#include "jslib.h"
#include "commonhd.h"

extern int input_file_header(), check_inode(), change_file_uniq();
static void usage();

char *com_name;
struct wnn_file_head fh;

static void
parse_options(argc, argv)
int argc;
char **argv;
{
    int c;
    extern int optind;
    extern char *optarg;

    while ((c = getopt(argc,argv,"")) != EOF) {
    }
    if (optind) {
	optind--;
	argc -= optind;
	argv += optind;
    }
    if(argc < 2){
	usage();
    }
}

static void
usage()
{
  fprintf(stderr , "Usage: %s Wnn_file_name* \n",com_name);
  exit(1);
}

void
main(argc, argv)
int argc;
char **argv;
{
    FILE *ifpter;
    int k;

    com_name = argv[0];
    parse_options(argc, argv);

    for(k = 1 ; k < argc ; k++){
	if((ifpter = fopen(argv[k] , "r")) == NULL){
	    fprintf(stderr , "Can't open the input file %s.\n" , argv[k]);
	    perror("");
	    exit(1);
	}
	if(input_file_header(ifpter, &fh) == -1){
	    fprintf(stderr, "%s %s: It's not a Wnn File.\n", com_name, argv[k]);
	    exit(1);
	}
	if(check_inode(ifpter, &fh) == -1){
	    if(change_file_uniq(&fh, argv[k]) == -1){
		fprintf(stderr, "%s %s: Can't change file_uniq.\n", com_name, argv[k]);
		perror("");
		exit(1);
	    }
	}
	fclose(ifpter);
    }
    exit(0);
}
