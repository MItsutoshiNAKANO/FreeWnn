/*
 *  $Id: de.c,v 1.21 2002-04-02 09:09:55 aono Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2001, 2002
 *
 * Maintainer:  FreeWnn Project   <freewnn@tomo.gr.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
        Jserver         (Nihongo Daemon)
*/
#if defined(HAVE_CONFIG_H)
#  include <config.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#if STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#endif /* STDC_HEADERS */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif

#include "commonhd.h"
#include "wnn_config.h"
#include "jd_sock.h"
#include "demcom.h"
#include "wnn_malloc.h"
#include "wnn_os.h"

#if defined(HAVE_SYS_PARAM_H)
#include <sys/param.h>
#else /* should be revised... */
#if defined(BSD42)
#define NOFILE getdtablesize()
#endif /* BSD42 */
#ifdef HITACHI
#define NOFILE 89
#endif /* HITACHI */
#endif /* HAVE_SYS_PARAM_H */

#ifdef SYSVR2
#ifndef SIGCHLD
#define SIGCHLD SIGCLD
#endif
#endif /* SYSVR2 */

#include "de_header.h"
#ifdef UX386
#include <sys/termio.h>
#undef  AF_UNIX
#endif

#include "msg.h"

#ifdef SOLARIS
#ifdef SO_DONTLINGER
#undef SO_DONTLINGER
#endif
#endif /* SOLARIS */

#if defined(HAVE_SETSOCKOPT) && defined(SOL_SOCKET) && defined(SO_NONBLOCK)
#define USE_SETSOCKOPT 1
#else
#undef USE_SETSOCKOPT
#endif

#define DOFORK	1
#define QUIET	1

#define NOT_QUIET       DEBUG | !QUIET

#ifndef min
#define min(x,y)        ( (x)<(y) ? (x) : (y) )
#endif

/*      Accept Socket   */
#ifdef INET6
#define MAX_ACCEPTS     3
#else
#define MAX_ACCEPTS     2
#endif
#define UNIX_ACPT       0
#define INET_ACPT       1
#ifdef INET6
#define INET6_ACPT      2
#endif

#define PROTO_ALL	0x1
#define PROTO_UN	0x2
#define PROTO_INET	0x4
#ifdef INET6
#define PROTO_INET6	0x8
#endif
static int listen_proto = PROTO_ALL;

jmp_buf client_dead;

static int port;
static int serverNO = 0;

struct cmblk
{
  int sd;                       /** ソケットのfd        **/
};
#define COMS_BLOCK      struct  cmblk

static COMS_BLOCK *cblk;
                /** クライアントごとのソケットを管理するテーブル **/

static COMS_BLOCK accept_blk[MAX_ACCEPTS];      /*accept sock blocks */


/*      Communication Buffers           */
static char snd_buf[R_BUF_SIZ];         /** 送信バッファ **/
static int sbp;                         /** 送信バッファポインタ **/

static int rbc;                         /** 受信バッファcounter **/
static char rcv_buf[S_BUF_SIZ];         /** 受信バッファ **/

#if defined(EAGAIN)
# if defined(EWOULDBLOCK)
# define ERRNO_CHECK(no)        ((no) == EAGAIN || (no) == EWOULDBLOCK)
# else /* defined(EWOULDBLOCK) */
# define ERRNO_CHECK(no)        ((no) == EAGAIN)
# endif /* defined(EWOULDBLOCK) */
#else /* defined(EAGAIN) */
# if defined(EWOULDBLOCK)
# define ERRNO_CHECK(no)        ((no) == EWOULDBLOCK)
# else /* defined(EWOULDBLOCK) */
# define ERRNO_CHECK(no)        (0)
# endif /* defined(EWOULDBLOCK) */
#endif /* defined(EAGAIN) */

/*      Client Table    */
int clientp;            /** cblkの有効なデータの最後を差している **/

int cur_clp;            /** 現在のクライアントの番号 **/

static int *all_socks;          /** ビットパターン
                                 which jserver to select を保持 **/
static int *ready_socks;        /** データのきているソケットの
                                        ビットパターンを保持 **/
static int *dummy1_socks, *dummy2_socks;

static int no_of_ready_socks;
static int sel_bwidth,          /** bit width of all_socks **/
  sel_width;                    /** byte width of all_socks **/

static int nofile;              /** No. of files **/

#define BINTSIZE        (sizeof(int)*8)
#define sock_set(array,pos)     (array[pos/BINTSIZE] |= (1<<(pos%BINTSIZE)))
#define sock_clr(array,pos)     (array[pos/BINTSIZE] &= ~(1<<(pos%BINTSIZE)))
#define sock_tst(array,pos)     (array[pos/BINTSIZE] &  (1<<(pos%BINTSIZE)))

struct msg_cat *wnn_msg_cat;
struct msg_cat *js_msg_cat;

static void daemon_main (), sel_all (), new_client (), daemon_init (), socket_init_un (), socket_init_in (), xerror (), get_options ();
void daemon_fin (), daemon_fin_un (), daemon_fin_in (), del_client (), put2_cur (), putc_cur ();
static int get_client (), rcv_1_client (), socket_accept_un (), socket_accept_in ();
int get2_cur ();
static void usage (void);
static void print_version (void);
#ifdef DEBUG
static void dmp ();
#endif


char cmd_name[16];

/* No arguments are used. Only options. */
int
main (argc, argv)
     int argc;
     char **argv;
{
  extern void _exit ();
  int tmpttyfd;
  char *cswidth_name;
  extern char *get_cswidth_name ();
  extern void set_cswidth ();

  char nlspath[64];

  strcpy (cmd_name, WNN_DAEMON_NAME);
  strcpy (lang_dir, LANG_NAME);
  strcpy (nlspath, LIBDIR);
  strcat (nlspath, "/%L/%N");
  js_msg_cat = msg_open (MESSAGE_FILE, nlspath, lang_dir);
  wnn_msg_cat = msg_open ("libwnn.msg", nlspath, lang_dir);
  if (wnn_msg_cat == NULL)
    {
      log_err ("Cannot open message file libwnn.msg.");
    }
  if (cswidth_name = get_cswidth_name (LANG_NAME))
    set_cswidth (create_cswidth (cswidth_name));

  port = -1;

  setuid (geteuid ());
  get_options (argc, argv);
  print_version();
  log_debug("invoked as %s.", argv[0]);
  if (DOFORK)
    {
      if (fork ())
	{
	  signal (SIGCHLD, _exit);
	  signal (SIGHUP, SIG_IGN);
	  signal (SIGINT, SIG_IGN);
	  signal (SIGQUIT, SIG_IGN);
#ifdef  SIGTSTP
	  signal (SIGTSTP, SIG_IGN);
#endif
	  signal (SIGTERM, _exit);
	  pause ();
	}
    }

  signal (SIGHUP, signal_hand);
  signal (SIGINT, signal_hand);
  signal (SIGQUIT, signal_hand);
  signal (SIGTERM, terminate_hand);
  if (DOFORK)
    {
#ifdef  SIGTSTP
      signal (SIGTSTP, SIG_IGN);
#endif /* SIGTSTP */
    }
  read_default ();
  daemon_init ();

  env_init ();
  file_init ();
  dic_init ();
  get_kaiseki_area (LENGTHCONV + 1);    /* 変換可能文字数 */
  init_work_areas ();
  init_jmt (0);

  read_default_files ();

  if (DOFORK)
    {
      /* End of initialization, kill parent */
      kill (getppid (), SIGTERM);
      fclose (stdin);
      fclose (stdout);
      if (!noisy)
	{
#if !(defined(BSD) && (BSD >= 199306))  /* !4.4BSD-Lite by Taoka */
	  fclose (stderr);
#else /* 4.4BSD-Lite */
	  int fd = open ("/dev/null", O_WRONLY);
	  if (fd < 0)
	    {
	      xerror ("Cannot open /dev/null");
	    }
	  dup2 (fd, 2);
	  close (fd);
#endif /* 4.4BSD-Lite */
	}

#ifdef SETPGRP_VOID
      setpgrp ();
#else /* !SETPGRP_VOID */
# if !defined(TIOCNOTTY) && defined(SVR4)
#  define TIOCNOTTY     _IO('t', 113)
# endif /* !defined(TIOCNOTTY) && defined(SVR4) */
#ifndef HITACHI
      if ((tmpttyfd = open ("/dev/tty", O_RDWR)) >= 0)
	{
	  ioctl (tmpttyfd, TIOCNOTTY, 0);
	  close (tmpttyfd);
	}
#endif /* HITACHI */
#endif /* SETPGRP_VOID */
    }

  daemon_main ();

  daemon_fin ();
  return (0);	/* it is not reached. only for avoiding compiler warning. */
}

static void
daemon_main ()
{
  for (;;)
    {
      sel_all ();
      new_client ();
      for (;;)
        {
#ifdef DEBUG
          error1 ("main loop: ready_socks = %02X\n", ready_socks);
#endif
          if (get_client () == -1)
            break;
          c_c = &client[cur_clp];
/*      error1("cur_clp = %d\n", cur_clp);  Moved into do_command() */
          rbc = 0;
          sbp = 0;
/*      if(rcv_1_client(cur_clp) == 0){ del_client(); continue; }       */
          if (setjmp (client_dead))
            {
              del_client ();
              continue;
            }
          do_command (c_c);
        }
    }
}

/*
        allocs area for selecting socketts
*/
void
socket_disc_init ()
{
  int sel_w;                    /* long word(==int) width of all_socks */

  nofile = NOFILE;
  sel_w = (nofile - 1) / BINTSIZE + 1;
  all_socks = (int *) calloc (sel_w, (sizeof (int)));
  ready_socks = (int *) malloc (sel_w * (sizeof (int)));
  dummy1_socks = (int *) malloc (sel_w * (sizeof (int)));
  dummy2_socks = (int *) malloc (sel_w * (sizeof (int)));
  sel_width = sel_w * sizeof (int);     /* byte width */
  sel_bwidth = sel_width * 8;   /* bit width */
}

/**     全てのソケットについて待つ      **/
static void
sel_all ()
{
  bcopy (all_socks, ready_socks, sel_width);
  bzero (dummy1_socks, sel_width);
  bzero (dummy2_socks, sel_width);

top:
  errno = 0;
  if ((no_of_ready_socks = select (nofile, ready_socks, dummy1_socks, dummy2_socks, 0)) == -1)
    {
      if (errno == EINTR)
        goto top;
      xerror ("select error");
    }
#ifdef DEBUG
  error1 ("select OK, ready_socks[0]=%02X, n-r-s=%x\n", ready_socks[0], no_of_ready_socks);
#endif
}

/**     ready_socksから今やる一人を取り出して返す(cur_clpにもセットする)
        誰も居ない時は-1
        スケジューリングはラウンドロビン        **/
static int
get_client ()
{
  register int i;

  if (no_of_ready_socks == 0)
    return -1;                  /* no client waits service */

  for (i = cur_clp;;)
    {
      if (no_of_ready_socks == 0)
        return -1;
      i++;
      if (i >= clientp)
        i = 0;
      if (sock_tst (ready_socks, cblk[i].sd))
        {
          sock_clr (ready_socks, cblk[i].sd);
          no_of_ready_socks--;
          return cur_clp = i;
        }
    }
}

/**     新しいクライアントが居るか否かを調べる
        居た場合はcblkに登録する        **/
static void
new_client ()                   /* NewClient */
{
  register int sd;
  register int full, i;
  FILE *f[3];
  char gomi[1024];
#ifdef  AF_UNIX
  if ((serverNO == 0) && (sock_tst (ready_socks, accept_blk[UNIX_ACPT].sd)))
    {
      sock_clr (ready_socks, accept_blk[UNIX_ACPT].sd);
      no_of_ready_socks--;
      sd = socket_accept_un ();
    }
  else
#endif
#ifdef INET6
  if (sock_tst (ready_socks, accept_blk[INET6_ACPT].sd))
    {
      sock_clr (ready_socks, accept_blk[INET6_ACPT].sd);
      no_of_ready_socks--;
      sd = socket_accept_in (accept_blk[INET6_ACPT].sd);
    }
  else
#endif
  if (sock_tst (ready_socks, accept_blk[INET_ACPT].sd))
    {
      sock_clr (ready_socks, accept_blk[INET_ACPT].sd);
      no_of_ready_socks--;
      sd = socket_accept_in (accept_blk[INET_ACPT].sd);
    }
  else
    {
      return;
    }
  error1 ("new Client: sd = %d\n", sd);
  /* reserve 2 fd */
  for (full = i = 0; i < 2; i++)
    {
      if (NULL == (f[i] = fopen ("/dev/null", "r")))
        {
          full = 1;
        }
    }
  for (i = 0; i < 2; i++)
    {
      if (NULL != f[i])
        fclose (f[i]);
    }

  if (full || sd >= nofile || clientp >= max_client)
    {
      fprintf (stderr, "%s: no more client\n", cmd_name);
#ifdef HAVE_RECV
      recv (sd, gomi, 1024, 0);
#else
      read (sd, gomi, 1024);
#endif
      shutdown (sd, 2);
#ifdef HAVE_CLOSESOCKET
      closesocket (sd);
#else
      close (sd);
#endif
      return;
    }

  cblk[clientp].sd = sd;
  sock_set (all_socks, sd);
  for (i = 0; i < WNN_MAX_ENV_OF_A_CLIENT; i++)
    {
      (client[clientp].env)[i] = -1;
    }
  clientp++;
}

/**     クライアントをcblkから削除する  **/
/* delete Client (please call from JS_CLOSE) */
void
del_client ()
{
  disconnect_all_env_of_client ();
  sock_clr (all_socks, cblk[cur_clp].sd);
#ifdef HAVE_CLOSESOCKET
  closesocket (cblk[cur_clp].sd);
#else
  close (cblk[cur_clp].sd);
#endif
#ifndef IBM
  cblk[cur_clp] = cblk[clientp - 1];
  client[cur_clp] = client[clientp - 1];
#else /* !IBM : IBM's compiler could not copy structure. */
  bcopy (&cblk[cur_clp], &cblk[clientp - 1], sizeof (COMS_BLOCK));
  bcopy (&client[cur_clp], &client[clientp - 1], sizeof (CLINET));
#endif /* !IBM */
  clientp--;
  error1 ("Delete Client: cur_clp = %d\n", cur_clp);
}


/**     サーバをイニシャライズする      **/
static void
daemon_init ()                   /* initialize Daemon */
{
  /*
  signal (SIGHUP, SIG_IGN);
  signal (SIGINT, SIG_IGN);
  signal (SIGQUIT, SIG_IGN);
  */


  if ((cblk = (COMS_BLOCK *) malloc (max_client * sizeof (COMS_BLOCK))) == NULL)
    {
      perror ("Malloc for cblk");
      exit (1);
    }
  if ((client = (CLIENT *) malloc (max_client * sizeof (CLIENT))) == NULL)
    {
      perror ("Malloc for client");
      exit (1);
    }
#ifdef HAVE_DRAND48
  srand48 (time (NULL));
#else
  srand ((int) time (NULL));
#endif
  clientp = 0;                  /* V3.0 */
  cur_clp = 0;                  /* V3.0 */
  socket_disc_init ();
#ifdef INET6
  if (listen_proto&(PROTO_ALL|PROTO_INET|PROTO_INET6))
      socket_init_in ();
#else
  if (listen_proto&(PROTO_ALL|PROTO_INET))
      socket_init_in ();
#endif
#ifdef  AF_UNIX
  if (listen_proto&(PROTO_ALL|PROTO_UN))
      socket_init_un ();
#endif /* AF_UNIX */
}

/**     サーバを終わる  **/
#ifdef  AF_UNIX
void
daemon_fin_un (sock_d_un)
     int sock_d_un;
{
  int trueFlag = 1;
  struct sockaddr_un addr_un;
  socklen_t addrlen;

  if (serverNO == 0)
    {
#ifndef SOLARIS
#if defined(FIONBIO)
      ioctl (sock_d_un, FIONBIO, &trueFlag);
#endif
#else /* !SOLARIS */
      fcntl (sock_d_un, F_SETFL, F_UNLCK);
#endif /* !SOLARIS */
      for (;;)
        {
          addrlen = sizeof (addr_un);
          if (accept (sock_d_un, (struct sockaddr *) &addr_un, &addrlen) < 0)
            break;
          /* EWOULDBLOCK EXPECTED, but we don't check */
        }
      shutdown (sock_d_un, 2);
      close (sock_d_un);
    }
}
#endif /* AF_UNIX */

void
daemon_fin_in (sock_d_in)
     int sock_d_in;
{
  int trueFlag = 1;
  struct sockaddr_in addr_in;
  socklen_t addrlen;
#ifdef USE_SETSOCKOPT
  int on = ~0;
#endif

#ifndef SOLARIS
#ifdef USE_SETSOCKOPT
  setsockopt (sock_d_in, SOL_SOCKET, SO_NONBLOCK, &on, sizeof (int));
#else
#if defined(FIONBIO)
  ioctl (sock_d_in, FIONBIO, &trueFlag);
#endif
#endif /* USE_SETSOCKOPT */
#else /* !SOLARIS */
  fcntl (sock_d_in, F_SETFL, F_UNLCK);
#endif /* !SOLARIS */
  for (;;)
    {
      addrlen = sizeof (addr_in);
      if (accept (sock_d_in, (struct sockaddr *) &addr_in, &addrlen) < 0)
        break;
      /* EWOULDBLOCK EXPECTED, but we don't check */
    }
  shutdown (sock_d_in, 2);
#ifdef HAVE_CLOSESOCKET
  closesocket (sock_d_in);
#else
  close (sock_d_in);
#endif
}

void
daemon_fin ()
{
  int fd;
#ifdef  AF_UNIX
  int sock_d_un = accept_blk[UNIX_ACPT].sd;
#endif /* AF_UNIX */
  int sock_d_in = accept_blk[INET_ACPT].sd;
#ifdef INET6
  int sock_d_in6 = accept_blk[INET6_ACPT].sd;
#endif

  /*
     accept all pending connection from new clients,
     avoiding kernel hangup.
   */
#ifdef  AF_UNIX
  daemon_fin_un (sock_d_un);
#endif
  daemon_fin_in (sock_d_in);
#ifdef INET6
  daemon_fin_in (sock_d_in6);
#endif

  for (fd = nofile - 1; fd >= 0; fd--)
    {
      if ((fd != sock_d_in) &&
#ifdef INET6
          (fd != sock_d_in6) &&
#endif
#ifdef AF_UNIX
          (fd != sock_d_un) &&
#endif /* AF_UNIX */
          sock_tst (all_socks, fd))
        {
          shutdown (fd, 2);
#ifdef HAVE_CLOSESOCKET
          closesocket (fd);
#else
          close (fd);
#endif
        }
    }
}

/*------*/

/**     **/
char *
gets_cur (buffer, buffer_size)
     char *buffer;
     size_t buffer_size;
{
  char *b;

  if (!buffer || !buffer_size)
    return NULL;

  b = buffer;

  while (--buffer_size && (*b = getc_cur ()) != '\0')
    b++;

  if (*b != '\0')
    {
      *b = '\0';
      while (getc_cur () != '\0')
        ;
    }

  return buffer;
}

/**     **/
w_char *
getws_cur (buffer, buffer_size)
     w_char *buffer;
     size_t buffer_size;
{
  w_char *b;

  if (!buffer || !buffer_size)
    return NULL;

  b = buffer;

  while (--buffer_size && (*b = get2_cur ()) != 0)
    b++;

  if (*b != 0)
    {
      *b = 0;
      while (getc_cur () != 0)
        ;
    }

  return buffer;
}

/**     カレント・クライアントから2バイト取る   **/
int
get2_cur ()
{
  register int x;
  x = getc_cur ();
  return (x << 8) | getc_cur ();
}

/**     カレント・クライアントから4バイト取る   **/
int
get4_cur ()
{
  register int x1, x2, x3;
  x1 = getc_cur ();
  x2 = getc_cur ();
  x3 = getc_cur ();
  return (x1 << (8 * 3)) | (x2 << (8 * 2)) | (x3 << (8 * 1)) | getc_cur ();
}

/**     カレント・クライアントから1バイト取る   **/
int
getc_cur ()
{
  static int rbp;
  if (rbc <= 0)
    {
      rbc = rcv_1_client (cur_clp);
      rbp = 0;
    }
  rbc--;
  return rcv_buf[rbp++] & 0xFF;
}

/**     クライアントから1パケット取る   **/
static int
rcv_1_client (clp)
     int clp;   /**     clp=クライアント番号    **/
{
  register int cc = 0;
  while (cc <= 0)
    {
      errno = 0;
#ifdef HAVE_RECV
      cc = recv (cblk[clp].sd, rcv_buf, S_BUF_SIZ, 0);
#else
      cc = read (cblk[clp].sd, rcv_buf, S_BUF_SIZ);
#endif
      if (cc <= 0)
        {
          if (ERRNO_CHECK (errno))
            {
              continue;
            }
          else if (cc == 0)
            {                   /* client dead */
              longjmp (client_dead, 666);
            }
          else
            {                   /* cc == -1 */
              if (errno != EINTR)
                longjmp (client_dead, 666);
              continue;
            }
        }
    }
#ifdef DEBUG
  error1 ("rcv: clp = %d, sd = %d, cc = %d\n", clp, cblk[clp].sd, cc);
  dmp (rcv_buf, cc);
#endif
  return cc;
}

/**     クライアントへ1パケット送る     **/
static void
snd_1_client (clp, n)
     int clp, n;/**     clp=クライアント番号, n= send n Bytes   **/
{
  register int cc, x;
#ifdef  DEBUG
  error1 ("snd: clp = %d, sd = %d\n", clp, cblk[clp].sd);
  dmp (snd_buf, n);
#endif
  for (cc = 0; cc < n;)
    {
      errno = 0;
#ifdef HAVE_SEND
      x = send (cblk[clp].sd, &snd_buf[cc], n - cc, 0);
#else
      x = write (cblk[clp].sd, &snd_buf[cc], n - cc);
#endif
      if (x < 0)
        {
          if (ERRNO_CHECK (errno) || errno == EINTR)
            {
              errno = 0;
              continue;
            }
          else
            {                   /* client dead */
              longjmp (client_dead, 666);
            }
        }
      cc += x;
    }
}

/**     **/
void
puts_cur (p)
     char *p;
{
  register int c;
  while (c = *p++)
    putc_cur (c);
  putc_cur (0);
}

/**     **/
void
puts_n_cur (p, n)
     char *p;
     int n;
{
  register int c;
  while ((c = *p++) && --n >= 0)
    putc_cur (c);
  putc_cur (0);
}

/**     **/
void
putws_cur (p)
     w_char *p;
{
  register int c;
  while (c = *p++)
    put2_cur (c);
  put2_cur (0);
}

/**     **/
void
putnws_cur (p, n)
     register w_char *p;
     register int n;
{
  register int c;
  for (; n > 0; n--)
    {
      if ((c = *p++) == 0)
        break;
      put2_cur (c);
    }
  put2_cur (0);
}

/**     カレント・クライアントへ2バイト送る     **/
void
put2_cur (c)
     register int c;
{
  putc_cur (c >> (8 * 1));
  putc_cur (c);
}

/**     カレント・クライアントへ4バイト送る     **/
void
put4_cur (c)
     register int c;
{
  putc_cur (c >> (8 * 3));
  putc_cur (c >> (8 * 2));
  putc_cur (c >> (8 * 1));
  putc_cur (c);
}

/**     カレント・クライアントへ1バイト送る     **/
void
putc_cur (c)
     register int c;
{
  snd_buf[sbp++] = c;
  if (sbp >= R_BUF_SIZ)
    {
      snd_1_client (cur_clp, R_BUF_SIZ);
      sbp = 0;
    }
}

/**     カレント・クライアントの送信バッファをフラッシュする    **/
void
putc_purge ()
{
  if (sbp != 0)
    {
      snd_1_client (cur_clp, sbp);
      sbp = 0;
    }
}

/*-----*/

/**     ソケットのイニシャライズ        **/
#ifdef  AF_UNIX
static void
socket_init_un ()
{
  struct sockaddr_un saddr_un;
  int sock_d_un;
  if (serverNO == 0)
    {
      saddr_un.sun_family = AF_UNIX;
      unlink (sockname);
      strcpy (saddr_un.sun_path, sockname);
      if ((sock_d_un = socket (AF_UNIX, SOCK_STREAM, 0)) == ERROR)
        {
          xerror ("Can't create unix domain socket.");
        }
      if (bind (sock_d_un, (struct sockaddr *) &saddr_un, strlen (saddr_un.sun_path) + 2) == ERROR)
        {
          shutdown (sock_d_un, 2);
          xerror ("Can't bind unix domain socket.");
        }
      if (listen (sock_d_un, 5) == ERROR)
        {
          shutdown (sock_d_un, 2);
          xerror ("Can't listen unix domain socket.");
        }
      chmod (sockname, 0777);
      signal (SIGPIPE, SIG_IGN);
#ifdef DEBUG
      error1 ("sock_d_un = %d\n", sock_d_un);
#endif
      accept_blk[UNIX_ACPT].sd = sock_d_un;
      sock_set (all_socks, sock_d_un);
    }
}
#endif /* AF_UNIX */

/*      Inet V3.0 */
static void
socket_init_in ()
{
#ifndef SOLARIS
  int on = 1;
#else /* SOLARIS */
  int on = 0;
#endif /* SOLARIS */
  struct servent *sp;
#if !defined(SO_DONTLINGER) && defined(SO_LINGER)
  struct linger linger;
#endif
#ifdef INET6
  struct addrinfo hints, *res, *res0;
  int error;
  char sport[6];
#else
  struct sockaddr_in saddr_in;
#endif
  int sock_d_in;

  if (port < 0)
    {
      if ((sp = getservbyname (SERVERNAME, "tcp")) == NULL)
        {
          port = WNN_PORT_IN;
        }
      else
        {
          port = ntohs (sp->s_port);
        }
    }

  port += serverNO;

#if DEBUG
  error1 ("port=%x\n", port);
#endif
#ifdef INET6
  memset(&hints, 0, sizeof(hints));
  if (listen_proto&PROTO_INET && !(listen_proto&PROTO_INET6))
      hints.ai_family = PF_INET;
  else if (listen_proto&PROTO_INET6 && !(listen_proto&PROTO_INET))
      hints.ai_family = PF_INET6;
  else
      hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  sprintf(sport, "%d", port);
  error = getaddrinfo(NULL, sport, &hints, &res0);
  if (error)
    {
      xerror (gai_strerror(error));
    }
  for (res = res0; res; res = res->ai_next) {
    if (res->ai_family == AF_INET || res->ai_family == AF_INET6){
      if ((sock_d_in = socket (res->ai_family, res->ai_socktype, res->ai_protocol)) == ERROR)
#else
  saddr_in.sin_family = AF_INET;
  saddr_in.sin_port = htons (port);
  saddr_in.sin_addr.s_addr = htonl (INADDR_ANY);
  if ((sock_d_in = socket (AF_INET, SOCK_STREAM, 0)) == ERROR)
#endif
    {
#ifdef INET6
      if (res->ai_family == AF_INET6)
        xerror ("can't create inet6 socket");
      else if (res->ai_family == AF_INET)
#endif
      xerror ("can't create inet socket");
    }
  setsockopt (sock_d_in, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof (int));
#ifdef SO_DONTLINGER
  setsockopt (sock_d_in, SOL_SOCKET, SO_DONTLINGER, (char *) 0, 0);
#else
# ifdef SO_LINGER
  linger.l_onoff = 0;
  linger.l_linger = 0;
  setsockopt (sock_d_in, SOL_SOCKET, SO_LINGER, (char *) &linger, sizeof linger);
# endif /* SO_LINGER */
#endif /* SO_DONTLINGER */

#ifdef INET6
  if (bind (sock_d_in, res->ai_addr, res->ai_addrlen) == ERROR)
#else
  if (bind (sock_d_in, (struct sockaddr *) &saddr_in, sizeof (saddr_in)) == ERROR)
#endif
    {
      shutdown (sock_d_in, 2);
#ifdef INET6
      if (res->ai_family == AF_INET6)
        xerror ("can't bind inet6 socket");
      else if (res->ai_family == AF_INET)
#endif
      xerror ("can't bind inet socket");
    }
  if (listen (sock_d_in, 5) == ERROR)
    {
      shutdown (sock_d_in, 2);
#ifdef INET6
      if (res->ai_family == AF_INET6)
        xerror ("can't listen inet6 socket");
      else if (res->ai_family == AF_INET)
#endif
      xerror ("can't listen inet socket");
    }
#if DEBUG
  error1 ("sock_d_in = %d\n", sock_d_in);
#endif
  sock_set (all_socks, sock_d_in);
#ifdef INET6
      if (res->ai_family == AF_INET)
	accept_blk[INET_ACPT].sd = sock_d_in;
      else if (res->ai_family == AF_INET6)
	accept_blk[INET6_ACPT].sd = sock_d_in;
    }
  }
  freeaddrinfo(res0);
#else
  accept_blk[INET_ACPT].sd = sock_d_in;
#endif
}


/**     accept new client socket        **/
#ifdef  AF_UNIX
static int
socket_accept_un ()
{
  struct sockaddr_un addr;
  socklen_t addrlen;

  addrlen = sizeof (addr);
  return accept (accept_blk[UNIX_ACPT].sd, (struct sockaddr *) &addr, &addrlen);
}
#endif /* AF_UNIX */

static int
socket_accept_in (fd)
     int fd;
{
  struct sockaddr_in addr;
  socklen_t addrlen;

  addrlen = sizeof (addr);
  return accept (fd, (struct sockaddr *) &addr, &addrlen);
}

static void
xerror (s)
     register char *s;
{
  fprintf (stderr, "%s: %s\n", cmd_name, s);
#ifdef HAVE_PERROR
  perror (cmd_name);
#endif
  exit (1);
}

#if DEBUG
static void
dmp (p, n)
     char *p;
{
  int i, j;

  for (i = 0; i < n; i += 16)
    {
      for (j = 0; j < 16; j++)
        {
          fprintf (stderr, "%02x ", p[i + j] & 0xFF);
        }
      fprintf (stderr, "n=%d\n", n);
    }
}

int
niide (x)                       /* おまけだよ。niide(3);とコールしてね */
     int x;
{
  switch (x)
    {
    case 0:
      return 1;
    case 1:
      fprintf (stderr, "ゲッターロボは");
      break;
    case 2:
      fprintf (stderr, "ゴレンジャーは");
      break;
    case 3:
      fprintf (stderr, "ロードランナーシリーズは");
      break;
    default:
      return x * niide (x - 1);
    }
  fprintf (stderr, "良い番組だ by 新出。\n");
  return x * niide (x - 1);
}
#endif

static void
get_options (argc, argv)
     int argc;
     char **argv;
{
  int c;
  extern char *optarg;

  strcpy (jserverrcfile, LIBDIR);       /* usr/local/lib/wnn */
  strcat (jserverrcfile, SERVER_INIT_FILE);     /* ja_JP/jserverrc */
#ifndef INET6
  while ((c = getopt (argc, argv, "f:s:h:N:p:vu4")) != EOF)
#else
  while ((c = getopt (argc, argv, "f:s:h:N:p:vu46")) != EOF)
#endif
    {
      switch (c)
        {
        case 'f':
          strcpy (jserverrcfile, optarg);
          break;
        case 's':
          noisy = 1;
          if (strcmp ("-", optarg) != 0)
            {
              if (freopen (optarg, "a", stderr) == NULL)
                {
                  printf ("Error in opening scriptfile %s.\n", optarg);
                  exit (1);
                }
            }
          error1 ("script started\n");
          break;
        case 'h':
          hinsi_file_name = optarg;
          break;
        case 'N':
          serverNO = atoi (optarg);
          break;
        case 'p':
          port = atoi (optarg);
          break;
        case 'v':
          print_version();
          usage();
	  exit (0);
        case 'u':
	  listen_proto &= ~PROTO_ALL;
	  listen_proto |= PROTO_UN;
          break;
        case '4':
	  listen_proto &= ~PROTO_ALL;
	  listen_proto |= PROTO_INET;
          break;
#ifdef INET6
        case '6':
	  listen_proto &= ~PROTO_ALL;
	  listen_proto |= PROTO_INET6;
          break;
#endif
        default:
	  usage();
        }
    }
}



/*
*/
void
js_who ()
{
  register int i, j;

  put4_cur (clientp);
  for (i = 0; i < clientp; i++)
    {
      put4_cur (cblk[i].sd);
      puts_cur (client[i].user_name);
      puts_cur (client[i].host_name);
      for (j = 0; j < WNN_MAX_ENV_OF_A_CLIENT; j++)
        {
          put4_cur ((client[i].env)[j]);
        }

    }
  putc_purge ();
}

void
js_kill ()
{
  if (clientp == 1)
    {
      put4_cur (0);
      putc_purge ();
      terminate_hand ();
    }
  else
    {
      put4_cur (clientp - 1);
      putc_purge ();
    }
}

void
usage ()
{
  fprintf(stderr, 
#ifdef INET6
	  "usage: %s [-f <init_file> -s <log_file(\"-\" for stderr)> -h <pos_file> -N <serverNO> -p <port_base> -u -4 -6]\n",
#else
	  "usage: %s [-f <init_file> -s <log_file(\"-\" for stderr)> -h <pos_file> -N <serverNO> -p <port_base> -u -4]\n",
#endif
	  cmd_name);
  fprintf(stderr,
	  "       %s -v\n",
	  cmd_name);
  exit (1);
}

void
print_version ()
{
#if  defined(CHINESE)
  printf ("%s (%s) Chinese Multi Client Server\n", cmd_name, SER_VERSION);
#elif  defined(KOREAN)
  printf ("%s (%s) Korean Multi Client Server\n", cmd_name, SER_VERSION);
#else
  printf ("%s (%s) Nihongo Multi Client Server\n", cmd_name, SER_VERSION);
#endif /* CHINESE */
}
