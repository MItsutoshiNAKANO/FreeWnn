/* $Id: acconfig.h,v 1.2 2001-09-16 11:35:57 hiroo Exp $ */

/* Define to `long' if <sys/types.h> does not define.  */
#undef time_t

/* Define to 'socklen_t' if <sys/socket.h> does not define. */
#undef socklen_t

/* Define to 'RAND_MAX' if <stdlib.h> does not define. */
#undef HAVE_RAND_MAX

/*
 * Defined if you allow creating a file in an arbitrary path
 * which is the traditional feature.
 */
#undef WNN_ALLOW_UNSAFE_PATH
