#ifndef lint
static char *rcs_id = "$Id: oldatonewa.c,v 1.1.1.1 2000-01-16 05:07:46 ura Exp $";
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
#include <stdio.h>
#include "commonhd.h"
#include "wnn_os.h"
#include "wnn_string.h"

static char *ohinsi[] = {
    "カ行五段",
    "カ行(行く)",
    "ガ行五段",
    "サ行五段",
    "タ行五段",
    "ナ行五段",
    "バ行五段",
    "マ行五段",
    "ラ行五段",
    "ワ行五段",
    "一段&名詞",
    "一段",
    "サ行(する)&名詞",
    "サ行(する)",
    "ザ行(ずる)",
    "来(こ)",
    "形容詞",
    "形容動詞",
    "名詞",
    "連体詞",
    "副詞",
    "接続詞,感動詞",
    "来(き)",
    "来(く)",
    "接頭語",
    "接尾語",
    "助数詞",
    "数詞",
    "ラ行(下さい)",
    "固有名詞",
    "形容動詞(たる)",
    "単漢字"
  };

#define LINE_SIZE 1024

extern int motoni2();
static int get_line1();
static char *get_string();

void
main()
{
  char buffer[LINE_SIZE];

  fgets(buffer, LINE_SIZE, stdin);
  printf("\\total %s",buffer);
  for(;fgets(buffer, LINE_SIZE, stdin) != NULL;){
    get_line1(buffer);
  }
  exit (0);
}

static int
get_line1(buffer)
char *buffer;
{
  char *get_string();
  char *c = buffer;
  char tmp[LINE_SIZE];
  char kanji[LINE_SIZE];
  char yomi[LINE_SIZE];
  /*
  char comment[LINE_SIZE];
  */
  w_char wyomi[LINE_SIZE];
  w_char wyomi1[LINE_SIZE];
  int hinsi;
  int hindo;
  int k;
  int	len;

  if((c = get_string(yomi, c)) == NULL ) return(1);
  wnn_Sstrcpy(wyomi,yomi);
  wnn_Sreverse(wyomi1,wyomi);
  wnn_sStrcpy(yomi,wyomi1);
  if((c = get_string(kanji, c)) == NULL ) return(-1);
  if((c = get_string(tmp, c)) == NULL ) return(-1);
  sscanf(tmp,"%x",&hinsi);
  if((c = get_string(tmp,c)) == NULL ) return(-1);
  sscanf(tmp,"%d",&hindo);
  hindo = motoni2(hindo);
  for(k = 0 ; k < 32 ; k++){
    if(hinsi & (1 << k)){
/*
      printf("%s\t\t%s\t\t%s\t\t%d\n",yomi,kanji,ohinsi[k],hindo);
*/
      printf("%s\t", yomi);
      len = strlen(yomi);
	if(len < 8)	printf("\t");
	if(len < 16)	printf("\t");
      printf("%s\t", kanji);
      len = strlen(kanji);
	if(len < 8)	printf("\t");
	if(len < 16)	printf("\t");
      printf("%s\t", ohinsi[k]);
      len = strlen(ohinsi[k]);
	if(len < 8)	printf("\t");
      printf("%d\n", hindo);
    }
  }
  return(0);
}

static char *
get_string(str,buf)
char *str;
char *buf;
{
  char *c = buf;
  /*
  int k;
  */
  for(;*c == '\t' || *c == ' ';c++);
  if(*c == '\0' || *c == '\n'){
    return(NULL);
  }
  for(;*c != '\t' && *c != ' ' && *c != '\n' && *c != '\0';c++){
    *str++ = *c;
  }
  *str = 0;
  return(c);
}

