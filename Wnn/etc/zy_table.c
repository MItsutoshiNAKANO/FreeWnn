/*
 *  $Id: zy_table.c,v 1.4 2005-04-10 15:26:37 aonoto Exp $
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

/**  cWnn  Version 1.1   **/

#include  <ctype.h>
#include "commonhd.h"
#ifdef CHINESE
#include "cplib.h"

/* zy_shengmu_tbl: ShengMu table of Chinese ZhuYin  */
char *zy_shengmu_tbl[ZY_NUM_SHENGMU] = {

  "", "Å", "Ø", "Ô", "É",   /*      "", "B", "C",  "Ch", "D",   */
  "È", "Í", "Ï", "Ð", "Î", /*     "F", "G", "H",  "J",  "K", */
  "Ì", "Ç", "Ë", "Æ", "Ñ", /*     "L", "M", "N",  "P",  "Q",  */
  "Ö", "Ù", "Õ", "Ê", "è", /*     "R", "S", "Sh", "T",  "W",  */
  "Ò", "ç", "×", "Ó"        /*      "X", "Y", "Z",  "Zh"   */
};


/* zy_yunmu_tbl: YunMu table of Chinese ZhuYin  */
char *zy_yunmu_tbl[ZY_NUM_YUNMU * 5] = {
/* 3.20
        "",     "",     "",      "",      "", 
*/
  "À", "Á", "Â", "Ã", "Ä", /* a    */
  "ÚÀ", "ÚÁ", "ÚÂ", "ÚÃ", "ÚÄ",       /* a    */
  "ÞÀ", "ÞÁ", "ÞÂ", "ÞÃ", "ÞÄ",       /* ai   */
  "âÀ", "âÁ", "âÂ", "âÃ", "âÄ",       /* an   */
  "äÀ", "äÁ", "äÂ", "äÃ", "äÄ",       /* ang  */
  "àÀ", "àÁ", "àÂ", "àÃ", "àÄ",       /* ao   */
  "ÜÀ", "ÜÁ", "ÜÂ", "ÜÃ", "ÜÄ",       /* e    */
  "ßÀ", "ßÁ", "ßÂ", "ßÃ", "ßÄ",       /* ei   */
  "ãÀ", "ãÁ", "ãÂ", "ãÃ", "ãÄ",       /* en   */
  "åÀ", "åÁ", "åÂ", "åÃ", "åÄ",       /* eng  */
  "æÀ", "æÁ", "æÂ", "æÃ", "æÄ",       /* er   */
  "çÀ", "çÁ", "çÂ", "çÃ", "çÄ",       /* i    */
  "çÚÀ", "çÚÁ", "çÚÂ", "çÚÃ", "çÚÄ",     /* ia   */
  "çâÀ", "çâÁ", "çâÂ", "çâÃ", "çâÄ",     /* ian  */
  "çäÀ", "çäÁ", "çäÂ", "çäÃ", "çäÄ",     /* iang */
  "çàÀ", "çàÁ", "çàÂ", "çàÃ", "çàÄ",     /* iao  */
  "çÝÀ", "çÝÁ", "çÝÂ", "çÝÃ", "çÝÄ",     /* ie   */
  "çãÀ", "çãÁ", "çãÂ", "çãÃ", "çãÄ",     /* in   */
  "çåÀ", "çåÁ", "çåÂ", "çåÃ", "çåÄ",     /* ing  */
  "éåÀ", "éåÁ", "éåÂ", "éåÃ", "éåÄ",     /* iong */
  "çáÀ", "çáÁ", "çáÂ", "çáÃ", "çáÄ",     /* iu   */
  "ÇÀ", "ÇÁ", "ÇÂ", "ÇÃ", "ÇÄ",       /* m    */
  "ËÀ", "ËÁ", "ËÂ", "ËÃ", "ËÄ",       /* n    */
  "@À", "@Á", "@Â", "@Ã", "@Ä",    /* ng   */
  "ÛÀ", "ÛÁ", "ÛÂ", "ÛÃ", "ÛÄ",       /* o    */
  "èåÀ", "èåÁ", "èåÂ", "èåÃ", "èåÄ",     /* ong  */
  "áÀ", "áÁ", "áÂ", "áÃ", "áÄ",       /* ou   */
  "èÀ", "èÁ", "èÂ", "èÃ", "èÄ",       /* u    */
  "èÚÀ", "èÚÁ", "èÚÂ", "èÚÃ", "èÚÄ",     /* ua   */
  "èÞÀ", "èÞÁ", "èÞÂ", "èÞÃ", "èÞÄ",     /* uai  */
  "èâÀ", "èâÁ", "èâÂ", "èâÃ", "èâÄ",     /* uan  */
  "èäÀ", "èäÁ", "èäÂ", "èäÃ", "èäÄ",     /* uang */
  "éÝÀ", "éÝÁ", "éÝÂ", "éÝÃ", "éÝÄ",     /* ue   */
  "èßÀ", "èßÁ", "èßÂ", "èßÃ", "èßÄ",     /* ui   */
  "èãÀ", "èãÁ", "èãÂ", "èãÃ", "èãÄ",     /* un   */
  "èÛÀ", "èÛÁ", "èÛÂ", "èÛÃ", "èÛÄ",     /* uo   */
  "éÀ", "éÁ", "éÂ", "éÃ", "éÄ",       /* ¹   */
  "éÝÀ", "éÝÁ", "éÝÂ", "éÝÃ", "éÝÄ",     /* ¹e  */
  "0À", "1Á", "2Â", "3Ã", "4Ä",    /* undefinited YunMu */
  "éâÀ", "éâÁ", "éâÂ", "éâÃ", "éâÄ",     /* ¹an,        only ZhuYin */
  "éãÀ", "éãÁ", "éãÂ", "éãÃ", "éãÄ",     /* ¹n, only ZhuYin */
};


/* zhuyin_tbl:  size is ZY_NUM_SHENGMU*ZY_NUM_YUNMU, including empty ShengMu */
/*              empty YunMu and undefinited ShengMu '-' */

int zhuyin_tbl[ZY_NUM_SHENGMU * ZY_NUM_YUNMU] = {

/* empty ShengMu */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X9586, 0X0000, 0X9592, 0X9599,
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X959B, 0X95A0, 0X0000, 0X959E,
  0X95A2,

/* SHengMu B */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X8000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu C */
  0X828B, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X0280, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu Ch */
  0X838B, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X8000,
  0X0000, 0X0380, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu D */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X0000, 0X8000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu F */
  0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu G */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu H */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X8000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu J */
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X08A4, 0X0000, 0X0000,
  0X08A7, 0X0000, 0X08A5, 0X0000, 0X08A8, 0X0000, 0X889B, 0X88A0, 0X8000, 0X889E,
  0X88A2,

/* ShengMu K */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu L */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000,
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X0000,

/* ShengMu M */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X8000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu N */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X0000,

/* ShengMu P */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X8000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu Q */
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0EA4, 0X0000, 0X0000,
  0X0EA7, 0X0000, 0X0EA5, 0X0000, 0X0EA8, 0X0000, 0X8E9B, 0X8EA0, 0X8000, 0X8E9E,
  0X8EA2,

/* ShengMu R */
  0X8F8B, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X8000,
  0X0000, 0X0F80, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu S */
  0X908B, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000, 0X8000,
  0X0000, 0X1080, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu Sh */
  0X918B, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X1180, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu T */
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X8000,
  0X0000, 0X8000, 0X0000, 0X8000, 0X0000, 0X8000, 0X8000, 0X0000, 0X8000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu W */
  0X939B, 0X8000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000, 0X0000, 0X1380, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu X */
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X14A4, 0X0000, 0X0000,
  0X14A7, 0X0000, 0X14A5, 0X0000, 0X14A8, 0X0000, 0X949B, 0X94A0, 0X8000, 0X949E,
  0X94A2,

/* ShengMu Y */
  0X958B, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0090, 0X0000, 0X9591, 0X9592,
  0X0000, 0X1580, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X1588, 0X1589, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X0093, 0X8000, 0X00A4, 0X0000, 0X0000,
  0X00A7, 0X0000, 0X00A5, 0X0000, 0X00A8, 0X0000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu Z */
  0X968B, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X1680, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000,
  0X8000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

/* ShengMu Zh  */
  0X978B, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X0000, 0X1780, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
  0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X8000, 0X8000, 0X8000, 0X8000, 0X8000,
  0X8000, 0X8000, 0X0000, 0X8000, 0X8000, 0X8000, 0X0000, 0X0000, 0X8000, 0X0000,
  0X0000,

};
#endif /* CHINESE */
