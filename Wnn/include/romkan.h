#ifndef FRWNN_ROMKAN_H
#define FRWNN_ROMKAN_H 1

#if __STDC__
#define FRWNN_PARAMS(paramlist)	paramlist
#else
#define FRWNN_PARAMS(paramlist)	()
#endif	/* __STDC__ */


extern void romkan_set_lang FRWNN_PARAMS((char *lang));

#endif	/* FRWNN_ROMKAN_H  */
