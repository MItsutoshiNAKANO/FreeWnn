/*
 * $Id: jlib.h,v 1.1.1.1 2000-01-16 05:07:45 ura Exp $
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
/*	Version 3.0
 */
/*
	Nihongo	Henkan	Library Header File

	08/Jul/1987	(wnn_errorno)
	24/Apr/1987
	09/Mar/1987	(update server_dead)
	25/Feb/1987	(update wchar_t)
	18/Feb/1987	(update Comment)
	28/Nov/1986
	23/Oct/1986
*/
#include <setjmp.h>

#ifndef w_char
#define w_char	unsigned short
#endif /* w_char */
#ifndef WCHAR_T
#define wchar_t	w_char
#define wchar	w_char
#define WCHAR_T
#endif /* WCHAR_T */

#define JLIB

#define	S_BUF_SIZ	1024	/* NEVER change this */
#define	R_BUF_SIZ	1024	/* NEVER change this */



struct	kouho_entry {
	int	s_ichi;		/*	かなバッファへのオフセット	*/
	int	jl;		/*	自立語の長さ			*/
	int	fl;		/*	付属語の長さ			*/
	int	pl;		/*	ディスプレイ時の長さ		*/
	int	jishono;	/*	自立語の辞書番号		*/
	int	serial;		/*	自立語の番号			*/
	w_char	*k_data;	/*	漢字バッファへのポインタ	*/
};
#define	KOUHO_ENT	struct	kouho_entry


struct	jikouho_entry {
	int	jl;		/*	自立語の長さ			*/
	int	jishono;	/*	自立語の辞書番号		*/
	int	serial;		/*	自立語の番号			*/
	w_char	*k_data;	/*	漢字バッファへのポインタ	*/
};
#define	JIKOUHO_ENT	struct	jikouho_entry


struct	bunjoho {
	w_char	*kana_buf;	/*	かなバッファ			*/
	int	kana_size;	/*	かなバッファの長さ		*/
	KOUHO_ENT *klist;	/*	候補リスト・バッファ		*/
	int	klist_size;	/*	候補リスト・バッファの長さ	*/
	w_char	*kanji_buf;	/*	漢字バッファ			*/
	int	kanji_buf_size;	/*	漢字バッファ の長さ		*/
};


struct	jikouhojoho{
	JIKOUHO_ENT *jlist;	/*	次候補バッファ			*/
	int	jlist_size;	/*	次候補バッファの長さ		*/
	w_char	*kanji_buf;	/*	漢字バッファ			*/
	int	kanji_buf_size;	/*	漢字バッファの長さ		*/
};
#define	JIKOUHOJOHO		struct	jikouhojoho


struct	jishojoho{
	w_char	*k_data;	/*	漢字バッファへのポインタ	*/
	int	bumpo;		/*	文法情報			*/
	int	hindo;		/*	頻度				*/
	int	jisho;		/*	辞書番号			*/
	int	serial;		/*	項目番号			*/
};
#define	JISHOJOHO	struct	jishojoho


struct dicinfo{
	int	dic_no;		/*	辞書番号			*/
	int	dic_size;	/*	単語数				*/
	int	ttl_hindo;	/*	頻度総数			*/
	int	dic_type;	/*	1:ユーザ,2:システム		*/
	int	udp;		/*	0:使用中でない,1:使用中		*/
	int	prio;		/*	辞書プライオリティ		*/
	int	rdonly;		/*	0:ライト可能,1:リードオンリー	*/
	char	*file_name;	/*	辞書ファイル名			*/
	char	*h_file_name;	/*	頻度ファイル名			*/
};
#define	DICINFO	struct	dicinfo

extern	jmp_buf	jd_server_dead_env;	/* サーバが死んだ時に飛んでいくenv */
extern	int	jd_server_dead_env_flg; /* jd_server_dead_envが有効か否か  */
extern	int	wnn_errorno;		/* Wnnのエラーはこの変数に報告される */

/* header file for dic_syurui */
#define NO_EXIST -1
#define NOT_A_JISHO 0
#define USER_DIC 1
#define SYSTEM_DIC 3

struct jwho {
    int sd;   /* socket discripter in jserver */
    char *user_name;   /* user name */
    char *host_name;   /* host name */
};

#define JWHO struct jwho

/*
  Local Variables:
  kanji-flag: t
  End:
*/
