/*
 * $Id: termio.c,v 1.1.1.1 2000-01-16 05:07:51 ura Exp $
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
#include "sdefine.h"
#include "sheader.h"

#ifdef TERMINFO
#include <stdio.h>
#include <curses.h>
#include <term.h>
#ifdef putchar
#undef putchar
#endif
extern int putchar();

extern char Term_Name[];
extern char    *Term_UnderScoreStart;
extern char    *Term_UnderScoreEnd;
extern char	*Term_ClrScreen;
extern char 	*Term_ClrEofLine;
extern char 	*Term_ThrowCursor;
extern char 	*Term_StandOutStart;
extern char 	*Term_StandOutEnd;
extern char 	*Term_BoldOutStart;
extern char 	*Term_BoldOutEnd;
static int	bold_mode_fun = 0;

int
openTermData()
{
    char *cp, *getenv(), *get_kbd_env();
    int status;
    int k;
    char lcode[10];
    char termchar[20];

	/* for convert_key --- added by Nide 10/3 */
    if (NULL == (cp = get_kbd_env()) ||
        0 != convert_getterm(cp, (0 != verbose_option))){
	fprintf(stderr, "Cannot get keyboard information.\n");
	return(-1); 
    }

    if ((cp = getenv("TERM")) == NULL){
        fprintf(stderr, "Cannot get terminal name.");
        return(-1);
    }
    strcpy(Term_Name, cp);

    if ((strlen(Term_Name) > 2) && (strcmp(Term_Name + (strlen(Term_Name) - 2),"-j") == 0)) {
	fprintf(stderr, MSG_GET(4));
	/*
	fprintf(stderr,"Uum:ｕｕｍからｕｕｍはおこせません。\n");
	*/
	return(-1);
    }
    setupterm(0,1,&status);
    reset_shell_mode();
    if (status != 1){
        return(-1);
    }
#if defined(uniosu)
    if(jterm < 2) {	/* kanji terminal */
        fprintf(stderr, "Not kanji terminal. Goodbye !\n");
        return(-1);
    }
#endif /* defined(uniosu) */
    if(save_cursor == (char *)NULL || *save_cursor == NULL ||
       restore_cursor == (char *)NULL || *restore_cursor == NULL ||
       change_scroll_region == (char *)NULL || *change_scroll_region == NULL) {
         fprintf(stderr, "Your terminal is not strong enough. Goodbye !\n");
         return(-1);
    }
    termchar[0] = 0;
    strcat(termchar,cp);
    strcat(termchar,"-j");
    setenv("TERM", termchar);

    sprintf(lcode,"%d", lines - conv_lines);
    setenv("LINES", lcode);

    if(cursor_normal && cursor_invisible){
	cursor_invisible_fun = 1;
    }else{
	cursor_invisible_fun = 0;
    }
    if (keypad_xmit && *keypad_xmit && keypad_local && *keypad_local) {
	keypad_fun = 1;
    } else {
	keypad_fun = 0;
    }
    Term_UnderScoreEnd = exit_underline_mode;
    Term_UnderScoreStart = enter_underline_mode;
    Term_StandOutEnd = exit_standout_mode;
    Term_StandOutStart = enter_standout_mode;
    if (enter_bold_mode && exit_attribute_mode)
	bold_mode_fun = 1;
    else
	bold_mode_fun = 0;

    Term_BoldOutStart = enter_bold_mode;
    Term_BoldOutEnd = exit_attribute_mode;
    return(0);
}


void
closeTermData()
{
    resetterm();
    reset_shell_mode();
}

void
set_keypad_on()
{
    tputs(keypad_xmit, 1, putchar);
}

void
set_keypad_off()
{
    tputs(keypad_local, 1, putchar);
}

void
set_scroll_region(start, end)
int start, end;
{
    tputs(tparm(change_scroll_region, start, end), 1 , putchar);
}

void
clr_end_screen()
{
    tputs(clr_eos ,1,putchar);
}


void
clr_screen()
{
    tputs(clear_screen, lines, putchar);
    Term_ClrScreen = clear_screen;
}

void
clr_line1()
{
    tputs(clr_eol, 1, putchar);
    Term_ClrEofLine = clr_eol;
}

void
throw_cur_raw(col,row)
int col, row;
{
    tputs(tparm(cursor_address, row, col), 1, putchar);
}

void
h_r_on_raw()
{
    tputs(enter_standout_mode, 1, putchar);
}

void
h_r_off_raw()
{
    tputs(exit_standout_mode, 1, putchar);
}

void
u_s_on_raw()
{
    tputs(enter_underline_mode, 1, putchar);
}

void
u_s_off_raw()
{
    tputs(exit_underline_mode, 1, putchar);
}

void
b_s_on_raw()
{
    if (bold_mode_fun)
	tputs(enter_bold_mode, 1, putchar);
    else
	tputs(enter_underline_mode, 1, putchar);
}

void
b_s_off_raw()
{
    if (bold_mode_fun)
	tputs(exit_attribute_mode, 1, putchar);
    else
	tputs(exit_underline_mode, 1, putchar);
}

void
ring_bell()
{
    tputs(bell, 1, putchar);
    flush();
}

void
save_cursor_raw()
{
  tputs(save_cursor, 1, putchar);
}

void
restore_cursor_raw()
{
    tputs(restore_cursor, 1, putchar);
}

void
cursor_invisible_raw()
{
  tputs(cursor_invisible , 1 ,putchar);
}
 
void
cursor_normal_raw()
{
  tputs(cursor_normal , 1 , putchar);
}

#endif                          /* TERMINFO */
