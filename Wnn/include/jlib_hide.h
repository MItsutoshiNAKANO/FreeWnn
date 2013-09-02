#ifndef FRWNN_JLIB_HIDE_H
#define FRWNN_JLIB_HIDE_H 1

#include "jslib.h"

/* defined in jlib/js.c  */
#if DEBUG
extern void xerror FRWNN_PARAMS((char *));
extern void dmp FRWNN_PARAMS((char *, int));
#endif	/* DEBUG  */


#ifdef  nodef
extern void js_set_lang FRWNN_PARAMS((struct wnn_env *, register char *));
#endif	/* nodef  */

extern char *_wnn_get_machine_of_serv_defs FRWNN_PARAMS((const char *));

#endif	/* FRWNN_JLIB_HIDE_H  */
