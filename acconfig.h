/* $Id: acconfig.h,v 1.3 2002-06-15 10:19:27 hiroo Exp $ */

/* Define `socklen_t' to int if <sys/socket.h> does not define. */
#undef socklen_t

/* Define to `long' if <sys/types.h> does not define.  */
#undef time_t

/* Define to `RAND_MAX' if <stdlib.h> does not define. */
#undef HAVE_RAND_MAX

/*
 * Defined if you allow creating a file in an arbitrary path
 * which is the traditional feature.
 */
#undef WNN_ALLOW_UNSAFE_PATH

/* Define if the signal function returns void. */
#undef RETSIGTYPE_VOID
