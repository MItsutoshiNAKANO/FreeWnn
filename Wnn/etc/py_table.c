/*
 * $Id: py_table.c,v 1.1.1.2 2000-01-16 05:10:49 ura Exp $
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
 *	'99/03/20	片山＠ＰＦＵ <kate@pfu.co.jp>
 *		Cei、Chua を有効な中国語音節に追加
 *
 * Last modified date: 20,Mar.1999
 *
 * Code:
 *
 */
/**  cWnn  Version 1.1	 **/

#include  <ctype.h>
#include "commonhd.h"
#ifdef CHINESE
#include "cplib.h"

/* pyshengmu_tbl[]: ShengMu table of Chinese PinYin */
char 	*py_shengmu_tbl[PY_NUM_SHENGMU] = {
	
	"",  "B", "C",  "Ch", "D",  
	"F", "G", "H",  "J",  "K", 
	"L", "M", "N",  "P",  "Q", 
	"R", "S", "Sh", "T",  "W",
	"X", "Y", "Z",  "Zh" 
	};
	
/* py_yunmu_tbl[]: YunMu table of Chinese ZhuYin */
char 	*py_yunmu_tbl[PY_NUM_YUNMU*5] = {

	"ﾀ",    "ﾀ",    "ﾀ",    "ﾀ",    "ﾀ",
	"aﾀ",    "｡ﾀ",    "｢ﾀ",    "｣ﾀ",    "､ﾀ",
	"aiﾀ",   "｡iﾀ",   "｢iﾀ",   "｣iﾀ",   "､iﾀ",
	"anﾀ",   "｡nﾀ",   "｢nﾀ",   "｣nﾀ",   "､nﾀ",
	"angﾀ",  "｡ngﾀ",  "｢ngﾀ",  "｣ngﾀ",  "､ngﾀ",
	"aoﾀ",   "｡oﾀ",   "｢oﾀ",   "｣oﾀ",   "､oﾀ",
	"eﾀ",    "･ﾀ",    "ｦﾀ",    "ｧﾀ",    "ｨﾀ",
	"eiﾀ",   "･iﾀ",   "ｦiﾀ",   "ｧiﾀ",   "ｨiﾀ",
	"enﾀ",   "･nﾀ",   "ｦnﾀ",   "ｧnﾀ",   "ｨnﾀ",
	"engﾀ",  "･ngﾀ",  "ｦngﾀ",  "ｧngﾀ",  "ｨngﾀ",
	"erﾀ",   "･rﾀ",   "ｦrﾀ",   "ｧrﾀ",   "ｨrﾀ",
	"iﾀ",    "ｩﾀ",    "ｪﾀ",    "ｫﾀ",    "ｬﾀ",
	"iaﾀ",   "i｡ﾀ",   "i｢ﾀ",   "i｣ﾀ",   "i､ﾀ",
	"ianﾀ",  "i｡nﾀ",  "i｢nﾀ",  "i｣nﾀ",  "i､nﾀ",
	"iangﾀ", "i｡ngﾀ", "i｢ngﾀ", "i｣ngﾀ", "i､ngﾀ",
	"iaoﾀ",  "i｡oﾀ",  "i｢oﾀ",  "i｣oﾀ",  "i､oﾀ",
	"ieﾀ",   "i･ﾀ",   "iｦﾀ",   "iｧﾀ",   "iｨﾀ",
	"inﾀ",   "ｩnﾀ",   "ｪnﾀ",   "ｫnﾀ",   "ｬnﾀ",
	"ingﾀ",  "ｩngﾀ",  "ｪngﾀ",  "ｫngﾀ",  "ｬngﾀ",
	"iongﾀ", "iｭngﾀ", "iｮngﾀ", "iｯngﾀ", "iｰngﾀ",
	"iuﾀ",   "iｱﾀ",   "iｲﾀ",   "iｳﾀ",   "iｴﾀ",
	"mﾀ",    "mﾀ",    "mﾀ",    "mﾀ",    "mﾀ",
	"nﾀ",    "nﾀ",    "ｽﾀ",    "ｾﾀ",    "ｿﾀ",
	"ngﾀ",   "ngﾀ",   "ngﾀ",   "ngﾀ",   "ngﾀ",
	"oﾀ",    "ｭﾀ",    "ｮﾀ",    "ｯﾀ",    "ｰﾀ",
	"ongﾀ",  "ｭngﾀ",  "ｮngﾀ",  "ｯngﾀ",  "ｰngﾀ",
	"ouﾀ",   "ｭuﾀ",   "ｮuﾀ",   "ｯuﾀ",   "ｰuﾀ",
	"uﾀ",    "ｱﾀ",    "ｲﾀ",    "ｳﾀ",    "ｴﾀ",
	"uaﾀ",   "u｡ﾀ",   "u｢ﾀ",   "u｣ﾀ",   "u､ﾀ",
	"uaiﾀ",  "u｡iﾀ",  "u｢iﾀ",  "u｣iﾀ",  "u､iﾀ",
	"uanﾀ",  "u｡nﾀ",  "u｢nﾀ",  "u｣nﾀ",  "u､nﾀ",
	"uangﾀ", "u｡ngﾀ", "u｢ngﾀ", "u｣ngﾀ", "u､ngﾀ",
	"ueﾀ",   "u･ﾀ",   "uｦﾀ",   "uｧﾀ",   "uｨﾀ",
	"uiﾀ",   "uｩﾀ",   "uｪﾀ",   "uｫﾀ",   "uｬﾀ",
	"unﾀ",   "ｱnﾀ",   "ｲnﾀ",   "ｳnﾀ",   "ｴnﾀ",
	"uoﾀ",   "uｭﾀ",   "uｮﾀ",   "uｯﾀ",   "uｰﾀ",
	"ｹﾀ",    "ｵﾀ",    "ｶﾀ",    "ｷﾀ",    "ｸﾀ",
	"ｹeﾀ",   "ｹ･ﾀ",   "ｹｦﾀ",   "ｹｧﾀ",   "ｹｨﾀ",
	"0ﾀ",   "1ﾀ",   "2ﾀ",   "3ﾀ",   "4ﾀ",  /* for undefinited YunMu  */
	};


/* pinyin_tbl:  size is NUM_SHENGMU*NUM_YUNMU, including empty ShengMu */
/*     and empty YunMu , and undefinited YunMu'-' */

int  pinyin_tbl[PY_NUM_SHENGMU*PY_NUM_YUNMU] = {
					    	
  0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,
  0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
  0,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
  0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1
	};
#endif /* CHINESE */
