/* $Id: acconfig.h,v 1.4 2002-06-22 13:15:25 hiroo Exp $ */

/* Define `socklen_t' to int if <sys/socket.h> does not define. */
#undef socklen_t

/* Define to `long' if <sys/types.h> does not define.  */
#undef time_t

/*
 * Defined if you allow creating a file in an arbitrary path
 * which is the traditional feature.
 */
#undef WNN_ALLOW_UNSAFE_PATH

/* Define if the signal function returns void. */
#undef RETSIGTYPE_VOID
