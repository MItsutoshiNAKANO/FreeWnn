/*
 * $Id: do_hindo_s.c,v 1.1.1.1 2000-01-16 05:07:45 ura Exp $
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
#include "commonhd.h"
#include "jslib.h"
#include "kaiseki.h"
#include "jdata.h"
#include "de_header.h"
    
static int hindo_set();

void
js_hindo_set()
{
    int env_id,dic,entry,ima,hindo,x;
    int err = 0;
    if((env_id=envhandle()) == -1) err = -1;
    dic=get4_cur();
    entry=get4_cur();
    ima=get4_cur();
    hindo=get4_cur();
    if(err == -1){ error_ret(); return;}
    if(dic == -1){
	giji_hindoup(entry);
	put4_cur(0);
	putc_purge();
	return;
    }
    if(find_dic_in_env(env_id,dic) == -1){ /* no such dic */
	wnn_errorno=WNN_DICT_NOT_USED; error_ret();
	return;
    }
    if(dic_table[dic].hindo_rw){
	wnn_errorno = WNN_RDONLY_HINDO;error_ret();
	return;
    }
    switch(ima){
    case WNN_HINDO_NOP:
    case WNN_IMA_ON:
    case WNN_IMA_OFF:
	break;
    default:
	error_ret();
	return;
    }
    if (entry < 0) {
	error_ret();
	return;
    }
    x=hindo_set(dic,entry,ima,hindo);
    if(x== -1){ error_ret();}
    else{ put4_cur(x); }
    putc_purge();
    return;
}

static int
hindo_set(dic_no,entry,imaop,hinop)
int dic_no;
int entry;
int imaop;
int hinop;
{
#ifdef SRAND48
    double drand48();
#define RAND()  drand48()
#else
    double drand();
#define RAND()  ((double)rand() / (double)((1<<31) - 1))
#endif
    UCHAR *hst;
    int ima, hindo;

    if(dic_table[dic_no].hindo == -1){
	hst = ((struct JT *)(files[dic_table[dic_no].body].area))->hindo;
	((struct JT *)(files[dic_table[dic_no].body].area))->hdirty = 1;
	((struct JT *)(files[dic_table[dic_no].body].area))->dirty = 1;
    }else{
	hst = ((struct HJT *)(files[dic_table[dic_no].hindo].area))->hindo;
	((struct HJT *)(files[dic_table[dic_no].hindo].area))->hdirty = 1;
    }
    ima = hst[entry] & 0x80;
    hindo = hst[entry] & 0x7f;
    switch(imaop){
    case WNN_HINDO_NOP:
	break;
    case WNN_IMA_ON:
	ima = 0x80;
	break;
    case WNN_IMA_OFF:
	ima = 0x0;
	break;
    }
    switch(hinop){
    case WNN_HINDO_NOP:
	break;
    case WNN_HINDO_INC:
      if((hindo&0x7e) != 0x7e && ( RAND() < (double)1 / ((hindo >> 2) + 1)))
	  hindo++;
	break;
    case WNN_HINDO_DECL:
      if(hindo > 0 && hindo <= 126 &&
	 ( RAND() < (double)1 / ((hindo >> 2) + 1)))
	  hindo--;
	break;
    case WNN_ENTRY_NO_USE:
	hindo = 0x7f;
	break;
    default:
	hindo = asshuku(hinop);
    }
    hst[entry] = hindo | ima;
    return(0);
}

    
