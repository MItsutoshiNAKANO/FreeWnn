#ifndef FRWNN_CONV_H
#define FRWNN_CONV_H 1

#if __STDC__
#define FRWNN_PARAMS(paramlist)	paramlist
#else
#define FRWNN_PARAMS(paramlist)	()
#endif	/* __STDC__ */

extern int convert_key_setup FRWNN_PARAMS((char *, int));
#ifdef TERMCAP
extern int convert_getterm FRWNN_PARAMS((char *termname, int flg));
#endif	/* TERMCAP  */

#endif /* FRWNN_CONV_H  */
