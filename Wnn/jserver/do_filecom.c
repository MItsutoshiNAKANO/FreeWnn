/*
 *  $Id: do_filecom.c,v 1.14 2013-09-02 11:01:39 itisango Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002, 2003
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
 *      Do Global File command
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include <stdio.h>
#if STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#else
#  if HAVE_MALLOC_H
#    include <malloc.h>
#  endif
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#endif /* STDC_HEADERS */
#include <sys/types.h>
#include <sys/stat.h>
#if STDC_HEADERS
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif

#include "demcom.h"
#include "commonhd.h"
#include "de_header.h"
#include "jdata.h"

static int  add_fid_to_env (int, int);
static int  del_fid_from_env (int, int);
static void file_already_read (int, int);
static int  file_discard (int, int);
static int  file_loaded_local (void);
static int  file_remove (char*, char*);
static int  find_file_by_uniq (struct wnn_file_uniq *);
static int  new_fid_in_env (int, int);
static int  x_file_already_read (int, int);

int
file_init (void)
{
  int i;
  files = (struct wnn_file *)calloc(MAX_FILES, sizeof(struct wnn_file));
  if ( files == NULL )
    {
      log_err ("file_init: malloc faild.");
      return (0);
    }
  for (i = 0; i < MAX_FILES; i++)
    {
      files[i].ref_count = -1;
    }
  return (SUCCESS);
}

/*
        Global File ope.
 */

/*      mkdir   */

/* #define MODE (0000000 | 0000777) */
/* #define      MODE (0040000 | 0000731) */
/* #define      MODE (0000000 | 0000733) */
#define      MODE (0000000 | 0000755)

void
js_mkdir (void)
{
  char path[FILENAME];
  int x;
  int err = 0;
  err = envhandle ();
  get_file_name (path, FILENAME);

  if (err == -1)
    {
      error_ret ();
      return;
    }
  x = mkdir (path, MODE);
  error1 ("mkdir=%d\n", x);
  if (x != 0)
    {
      wnn_errorno = WNN_MKDIR_FAIL;
      error_ret ();
      return;
    }
  chmod (path, MODE);
  put4_cur (0);
  putc_purge ();
}

/*      access  */
void
js_access (void)
{
  char path[FILENAME];
  int amode;
  envhandle ();
  amode = get4_cur ();
  get_file_name (path, FILENAME);

#ifdef WRITE_CHECK
  check_backup (path);
#endif
  put4_cur (access (path, amode));
  putc_purge ();
}


/*      js_file_list_all        */
void
js_file_list_all (void)
{
  int i, c;

  for (c = 0, i = 1; i < MAX_FILES; i++)
    {
      if (files[i].ref_count == -1)
        continue;
      c++;
    }
  put4_cur (c);
  for (i = 1; i < MAX_FILES; i++)
    {
      if (files[i].ref_count == -1)
        continue;
      put4_cur (i);             /* fid */
      put4_cur (files[i].localf);
      put4_cur (files[i].ref_count);
      put4_cur (files[i].file_type);
      puts_cur (files[i].name);
    }
  putc_purge ();
}

/*      js_file_list    */
void
js_file_list (void)
{
  int i, c, env_id, fid;

  env_id = envhandle ();

  for (c = 0, i = 0; i < WNN_MAX_FILE_OF_AN_ENV; i++)
    {
      if ((env[env_id]->file)[i] == -1)
        continue;
      c++;
    }
  put4_cur (c);
  for (i = 0; i < WNN_MAX_FILE_OF_AN_ENV; i++)
    {
      if ((fid = (env[env_id]->file)[i]) == -1)
        continue;
      put4_cur (fid);           /* fid */
      put4_cur (files[fid].localf);
      put4_cur (files[fid].ref_count);
      put4_cur (files[fid].file_type);
      puts_cur (files[fid].name);
    }
  putc_purge ();
}

/*      file info       */
void
js_file_info (void)
{
  int env_id, fid;
  env_id = envhandle ();
  fid = get4_cur ();
  if (find_fid_in_env (env_id, fid) == -1)
    {                           /* valid */
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  put4_cur (0);
  puts_cur (files[fid].name);
  put4_cur (files[fid].localf);
  put4_cur (files[fid].ref_count);
  put4_cur (files[fid].file_type);
  putc_purge ();
}

/*      file stat       */
void
js_file_stat (void)
{
  char n[FILENAME];

  get4_cur ();                  /* env_id */
  get_file_name (n, FILENAME);

  put4_cur (file_stat (n));
  putc_purge ();
}

int
file_stat (char *file_name)
{
  FILE *fp;
  struct wnn_file_head fh;
  struct JT jt;
  struct HJT hjt;
  int ret;

#ifdef WRITE_CHECK
  check_backup (file_name);
#endif
  if ((fp = fopen (file_name, "r")) == NULL)
    {
      return WNN_FILE_NOT_EXIST;
    }
  if (input_file_header (fp, &fh) == -1)
    {
      ret = WNN_NOT_A_JISHO;
      goto end;
    }
  switch (fh.file_type)
    {
    case WNN_FT_DICT_FILE:
      if (input_header_jt (fp, &jt) == -1)
        {
          ret = WNN_NOT_A_JISHO;
          goto end;
        }
      switch (jt.syurui)
        {
        case WNN_UD_DICT:
        case WNN_REV_DICT:
        case WNN_REG_DICT:
        case WNN_STATIC_DICT:
#ifdef  CONVERT_by_STROKE
        case BWNN_REV_DICT:
#endif /* CONVERT_by_STROKE */
#ifdef  CONVERT_with_SiSheng
        case CWNN_REV_DICT:
#endif /* CONVERT_with_SiSheng */
          ret = jt.syurui;
          goto end;
        default:
          ret = WNN_NOT_A_JISHO;
          goto end;
        }
    case WNN_FT_HINDO_FILE:
      if (input_header_hjt (fp, &hjt) == -1)
        {
          ret = WNN_NOT_A_JISHO;
          goto end;
        }
      ret = WNN_HINDO_FILE;
      goto end;
    case WNN_FT_FUZOKUGO_FILE:
      ret = WNN_FUZOKUGO_FILE;
      goto end;
    default:
      ret = WNN_NOT_A_JISHO;
      goto end;
    }
end:
  fclose (fp);
  return (ret);
}

/*      js_file_discard */
void
js_file_discard (void)
{
  int x;
  int env_id, fid;
  env_id = envhandle ();
  fid = get4_cur ();
  if (env_id == -1)
    {
      error_ret ();
      return;
    }
  x = file_discard (env_id, fid);
  if (x == -1)
    {
      error_ret ();
      putc_purge ();
      return;
    }
  put4_cur (x);
  putc_purge ();
  return;
}

static int
file_discard (int env_id, int fid)
{
  if (del_fid_from_env (env_id, fid) == -1)
    {                           /* invalid */
      wnn_errorno = WNN_FID_ERROR;
      return -1;
    }
  used_dic_fuzoku_delete (env_id, fid); /* Delete used dicts and fuzoku. H.T. */
  if (files[fid].ref_count != 0)
    {
      return files[fid].ref_count;
    }
  discardfile (&files[fid]);
  files[fid].ref_count = -1;
  return 0;
}

void
del_all_file_in_env (int env_id)
{
  int i, fid;

  for (i = 0; i < WNN_MAX_FILE_OF_AN_ENV; i++)
    {
      if ((fid = (env[env_id]->file)[i]) == -1)
        continue;
      files[fid].ref_count--;
      if (files[fid].ref_count == 0)
        {
          discardfile (&files[fid]);
          files[fid].ref_count = -1;
        }
    }
}


/*      js_hindo_file_create    */
void
js_hindo_file_create (void)
{
  int env_id, fid;
  char fn[FILE_NAME_L];
  w_char com[1024];
  char hpasswd[WNN_PASSWD_LEN];

  env_id = get4_cur ();         /* env_id */
  fid = get4_cur ();
  get_file_name (fn, FILE_NAME_L);
  getws_cur (com, 1024);
  gets_cur (hpasswd, WNN_PASSWD_LEN);
  if (find_fid_in_env (env_id, fid) == -1)
    {                           /* valid */
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }

  if (files[fid].ref_count == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  if (create_hindo_file1 (&files[fid], fn, com, hpasswd) == -1)
    {
      wnn_errorno = WNN_FILE_CREATE_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  put4_cur (0);
  putc_purge ();
}

/*      js_hindo_file_create_client     */
void
js_hindo_file_create_client (void)
{
  int env_id, fid, i;
  struct wnn_file_uniq *funiq;

  env_id = get4_cur ();         /* env_id */
  fid = get4_cur ();

  if (find_fid_in_env (env_id, fid) == -1)
    {                           /* valid */
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  if (files[fid].ref_count == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  put4_cur (0);
  put4_cur (((struct JT *) (files[fid].area))->maxserial);
  funiq = &files[fid].f_uniq_org;
  put4_cur (funiq->time);
  put4_cur (funiq->dev);
  put4_cur (funiq->inode);
  for (i = 0; i < WNN_HOSTLEN; i++)
    {
      putc_cur (funiq->createhost[i]);
    }
  putc_purge ();
}


/*      js_dic_file_create      */
void
js_dic_file_create (void)
{
  int type, x;
  char fn[FILE_NAME_L];
  w_char com[1024];
  char passwd[WNN_PASSWD_LEN], hpasswd[WNN_PASSWD_LEN];
  get4_cur ();                  /* env_id */
  get_file_name (fn, FILE_NAME_L);
  getws_cur (com, 1024);
  gets_cur (passwd, WNN_PASSWD_LEN);
  gets_cur (hpasswd, WNN_PASSWD_LEN);
  type = get4_cur ();

  if (type != WNN_REV_DICT &&
#ifdef  CONVERT_by_STROKE
      type != BWNN_REV_DICT &&
#endif /* CONVERT_by_STROKE */
#ifdef  CONVERT_with_SiSheng
      type != CWNN_REV_DICT &&
#endif /* CONVERT_with_SiSheng */
      type != WNN_UD_DICT)
    {
      wnn_errorno = WNN_NOT_A_UD;
      error_ret ();
      putc_purge ();
      return;
    }
  x = create_null_dic (fn, com, passwd, hpasswd, type);
  if (x == -1)
    {
      wnn_errorno = WNN_FILE_CREATE_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  put4_cur (0);
  putc_purge ();
}

void
js_file_comment_set (void)
{
  int fid, envi;
  w_char comment[WNN_COMMENT_LEN];
  int dic_no;

  envi = get4_cur ();           /* env_id */
  fid = get4_cur ();
  getws_cur (comment, WNN_COMMENT_LEN);

  if (find_fid_in_env (envi, fid) == -1)
    {                           /* valid */
      wnn_errorno = WNN_FID_ERROR;
      goto ERR_RET;
    }
  if ((dic_no = chk_dic (envi, fid)) == -1 && (dic_no = chk_hindo (envi, fid)) == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      goto ERR_RET;
    }
  if (dic_table[dic_no].body == fid && dic_table[dic_no].rw == WNN_DIC_RDONLY)
    {
      wnn_errorno = WNN_RDONLY;
      goto ERR_RET;
    }
  if (dic_table[dic_no].hindo == fid && dic_table[dic_no].hindo_rw == WNN_DIC_RDONLY)
    {
      wnn_errorno = WNN_RDONLY;
      goto ERR_RET;
    }
  if (file_comment_set (&files[fid], comment) == 0)
    {
      goto ERR_RET;
    }
  put4_cur (0);
  putc_purge ();
  return;
ERR_RET:
  error_ret ();
  putc_purge ();
}


static FILE *
xFopen (char *n, char *mode)
{
  FILE *f;
  if (n == NULL)
    return NULL;
#ifdef WRITE_CHECK
  check_backup (n);
#endif
  if ((f = fopen (n, mode)) == NULL)
    {
      wnn_errorno = WNN_NO_EXIST;
      return (FILE *) - 1;
    }
  return f;
}

static void
xFclose (FILE *f)
{
  if (f == NULL)
    return;
  fclose (f);
}

/*      js_file_loaded_local (in client machine)        */
void
js_file_loaded_local (void)
{

/* get4_cur();   env_id */

  file_loaded_local ();
}

static int
file_loaded_local (void)
{
  int x, i;
  struct wnn_file_uniq fq;

  fq.time = get4_cur ();
  fq.dev = get4_cur ();
  fq.inode = get4_cur ();
  for (i = 0; i < WNN_HOSTLEN; i++)
    {
      fq.createhost[i] = getc_cur ();
    }

/*
fprintf(stderr,"js_file_loaded:ti=%x,dev=%x,inode=%x,%s\n",
        fq.time,fq.dev,fq.inode,fq.createhost);
*/
  x = find_file_by_uniq (&fq);
  if (x == -1)
    put4_cur (-1);
  else
    put4_cur (x);
  putc_purge ();
  return x;
}

/*      js_file_loaded (in jserver machine)     */
void
js_file_loaded (void)
{
  char n[FILE_NAME_L];
  int x;

/*  get4_cur(); env_id */
  get_file_name (n, FILE_NAME_L);

  if ((x = file_loaded (n)) < 0)
    put4_cur (-1);
  else
    put4_cur (x);
  putc_purge ();
}

int
file_loaded (char *n)
{
  int x;
  FILE *f;
  struct wnn_file_head fh;

  f = xFopen (n, "r");
  if (f == (FILE *) - 1 || f == NULL)
    {
      wnn_errorno = WNN_NO_EXIST;
      return -1;
    }
  x = input_file_header (f, &fh);
  if (x == -1)
    {                           /* Must not do check_inode to non-wnn file */
      xFclose (f);
      wnn_errorno = WNN_NOT_A_FILE;
      return -1;
    }
  /* Check I-node H.T. */
  if (check_inode (f, &fh) == -1)
    {
#ifdef WRITE_CHECK
      if (change_file_uniq (&fh, n) == -1)
        {
          xFclose (f);
          wnn_errorno = WNN_INODE_CHECK_ERROR;
          return (-1);
        }
      else
        {
          xFclose (f);
          f = xFopen (n, "r");
          if (f == (FILE *) - 1 || f == NULL)
            {
              wnn_errorno = WNN_NO_EXIST;
              return -1;
            }
          if (check_inode (f, &fh) == -1)
            {
              xFclose (f);
              wnn_errorno = WNN_INODE_CHECK_ERROR;
              return (-1);
            }
        }
#else /* WRITE_CHECK */
      if ((change_file_uniq (&fh, n) == -1) || (check_inode (f, &fh) == -1))
        {
          xFclose (f);
          wnn_errorno = WNN_INODE_CHECK_ERROR;
          return (-1);
        }
#endif /* WRITE_CHECK */
    }

  xFclose (f);

  x = find_file_by_uniq (&fh.file_uniq);
  if (x == -1)
    x = FILE_NOT_READ;
  return x;
}

static int
find_file_by_uniq (struct wnn_file_uniq *uniq)
{
  int i;
  for (i = 1; i < MAX_FILES; i++)
    {
      if (files[i].ref_count == -1)
        continue;
      if (!f_uniq_cmp (&files[i].f_uniq, uniq))
        return i;
    }
  return -1;
}


/*      js_file_send (from Client to Jserver)   */
void
js_file_send (void)
{
  char n[FILE_NAME_L];
  int env_id, fid;

  env_id = get4_cur ();         /* env_id */

  /* file loaded ? */
  fid = file_loaded_local ();

  if (fid != -1)
    {
      if (x_file_already_read (env_id, fid) == -1)
        {                       /* H.T. */
          error_ret ();
          putc_purge ();
          return;
        }
      else
        {
          put4_cur (0);
          putc_purge ();
          return;
        }
    }

  if ((fid = get_new_fid ()) == -1)
    {                           /* no more file */
      wnn_errorno = WNN_NO_MORE_FILE;
      error_ret ();
      putc_purge ();
      return;
    }

  put4_cur (1);
  putc_purge ();

  gets_cur (n, FILE_NAME_L);

  /*     read file       */
  files[fid].localf = REMOTE;
  files[fid].ref_count = 0;
  strcpy (files[fid].name, n);
  if (read_file (&files[fid]) == -1)
    {
/*      wnn_errorno=WNN_FILE_READ_ERROR;  */
      error_ret ();
      putc_purge ();
      files[fid].ref_count = -1;        /* fail */
      return;
    }
  add_fid_to_env (env_id, fid);
  put4_cur (fid);
  putc_purge ();
}

/*      js_file_read    */
void
js_file_read (void)
{
  char n[FILE_NAME_L];
  int env_id, fid;

  env_id = get4_cur ();         /* env_id */
  get_file_name (n, FILE_NAME_L);

  fid = file_loaded (n);
  if (fid == -1)
    {
      error_ret ();
      putc_purge ();
      return;
    }
  if (FILE_NOT_READ != fid)
    {                           /* already read */
      file_already_read (env_id, fid);
      putc_purge ();
      return;
    }
  if ((fid = get_new_fid ()) == -1)
    {                           /* no more file */
      wnn_errorno = WNN_NO_MORE_FILE;
      error_ret ();
      putc_purge ();
      return;
    }
  /*     read file       */
  files[fid].localf = LOCAL;
  files[fid].ref_count = 0;
  strcpy (files[fid].name, n);
  if (read_file (&files[fid]) == -1)
    {
/*      wnn_errorno=WNN_FILE_READ_ERROR;*/
      error_ret ();
      putc_purge ();
      files[fid].ref_count = -1;        /* fail */
      return;
    }
  add_fid_to_env (env_id, fid);
  put4_cur (fid);
  putc_purge ();
}

static void
file_already_read (int env_id, int fid)
{
  if (-1 != find_fid_in_env (env_id, fid))
    {                           /* already exists in env */
      put4_cur (fid);
      return;
    }
  if (-1 == add_fid_to_env (env_id, fid))
    {
      wnn_errorno = WNN_NO_MORE_FILE;
      error_ret ();
      return;
    }
  put4_cur (fid);
  return;
}

static int
x_file_already_read (int env_id, int fid)
{
  if (-1 != find_fid_in_env (env_id, fid))
    {                           /* already exists in env */
      return fid;
    }
  if (-1 == add_fid_to_env (env_id, fid))
    {
      wnn_errorno = WNN_NO_MORE_FILE;
      return -1;
    }
  return fid;
}

/*      js_file_write   */
void
js_file_write (void)
{
  int env_id, fid;
  char n[FILE_NAME_L];
  env_id = get4_cur ();         /* env_id */
  fid = get4_cur ();
  get_file_name (n, FILE_NAME_L);

  if (find_fid_in_env (env_id, fid) == -1)
    {                           /* valid */
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  if (files[fid].ref_count == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  if (strcmp (n, "") == 0)
    strcpy (n, files[fid].name);
  if (write_file (&files[fid], n) == -1)
    {
/*      wnn_errorno=WNN_FILE_WRITE_ERROR; */ error_ret ();
      putc_purge ();
      return;
    }
  put4_cur (fid);
  putc_purge ();
}


/*      js_file_receive (from Jserver to Client) */
void
js_file_receive (void)
{
  int env_id, fid;
  struct wnn_file_uniq fq;
  int mode = 1;
  int i;
  int error = 0;

  env_id = get4_cur ();         /* env_id */
  fid = get4_cur ();
   /**/ if (find_fid_in_env (env_id, fid) == -1)
    {                           /* valid */
      puts_cur ("!");
      putc_purge ();            /* dummy */
      error = 1;
    }
  else
    {
      puts_cur (files[fid].name);
      putc_purge ();
    }
  if (get4_cur () == -1)
    return;                     /* Ack */

  fq.time = get4_cur ();
  fq.dev = get4_cur ();
  fq.inode = get4_cur ();
  for (i = 0; i < WNN_HOSTLEN; i++)
    {
      fq.createhost[i] = getc_cur ();
    }
  if (error || files[fid].ref_count == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  if (fq.time != 0)
    {                           /* Old File Exist */
      mode = must_write_file (&files[fid], &fq);
    }

  if (mode == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }
  else if (mode == 0)
    {                           /* need not saving */
      put4_cur (0);
      putc_purge ();
      return;
    }
  put4_cur (mode);
  putc_purge ();

  if (get4_cur () == -1)
    return;

   /**/ if (rcv_file (&files[fid], mode) == -1)
    {
/*      wnn_errorno=WNN_FILE_WRITE_ERROR; */ error_ret ();
      putc_purge ();
      return;
    }
  if (mode == 1 || mode == 2)
    {
      clear_dirty_bit (&files[fid]);
    }

  put4_cur (0);
/*
 put4_cur(fid);
*/
  putc_purge ();
}


/*      */
int
get_new_fid (void)
{
  int fid;
  for (fid = 1; fid < MAX_FILES; fid++)
    {
      if (files[fid].ref_count == -1)
        return fid;             /* find new area */
    }
  return -1;                    /* no more file */
}

/*
        routines for FID in the ENV
*/

static int
add_fid_to_env (int env_id, int fid)
{
  int i;
  if ((i = new_fid_in_env (env_id, -1)) == -1)
    {
      return -1;
    }
  (env[env_id]->file)[i] = fid;
  files[fid].ref_count++;
  return i;
}

static int
del_fid_from_env (int env_id, int fid)
{
  int i;
  if ((i = find_fid_in_env (env_id, fid)) == -1)
    {
      return -1;
    }
  (env[env_id]->file)[i] = -1;
  files[fid].ref_count--;
  return i;
}


int
find_fid_in_env (int env_id, int fid)
{
  register int i;
  register int *file = env[env_id]->file;
  for (i = 0; i < WNN_MAX_FILE_OF_AN_ENV; i++)
    {
      if ((file)[i] == -1)
        continue;
      if ((file)[i] == fid)
        {
          return i;
        }
    }
  return -1;
}

static int
new_fid_in_env (int env_id, int fid)
{
  int i;
  for (i = 0; i < WNN_MAX_FILE_OF_AN_ENV; i++)
    {
      if ((env[env_id]->file)[i] == -1)
        {
          return i;
        }
    }
  return -1;
}

/*      js_file_remove  (remove file from UNIX file system) H.T. */
void
js_file_remove (void)
{
  char n[FILE_NAME_L];
  char passwd[WNN_PASSWD_LEN];
  int x;

  get_file_name (n, FILE_NAME_L);
  gets_cur (passwd, WNN_PASSWD_LEN);

  x = file_remove (n, passwd);
  if (x == -1)
    {
      error_ret ();
    }
  else
    {
      put4_cur (0);
    }
  putc_purge ();
}

static int
file_remove (char *n, char *passwd)
{
  int fid;
  FILE *fp;
  struct wnn_file_head fh;

  fid = file_loaded (n);
  if (fid == -1)
    {
      return (-1);
    }
  if (FILE_NOT_READ != fid)
    {                           /* already read */
      wnn_errorno = WNN_FILE_IN_USE;
      return (-1);
    }
#ifdef WRITE_CHECK
  check_backup (n);
#endif
  if ((fp = fopen (n, "r")) == NULL)
    {
      wnn_errorno = WNN_FILE_READ_ERROR;
      return (-1);
    }
  if (input_file_header (fp, &fh) == -1)
    {
      fclose (fp);
      return (-1);
    }
  fclose (fp);
  if (!check_pwd (passwd, fh.file_passwd))
    {
      wnn_errorno = WNN_INCORRECT_PASSWD;
      return (-1);
    }
  if (unlink (n) == -1)
    {
      wnn_errorno = WNN_UNLINK;
      return (-1);
    }
  return (0);
}


void
js_file_password_set (void)
{
  int fid, envi, which;
  char old[WNN_PASSWD_LEN];
  char new[WNN_PASSWD_LEN];
  int ret;

  envi = get4_cur ();           /* env_id */
  fid = get4_cur ();
  which = get4_cur ();
  gets_cur (old, WNN_PASSWD_LEN);
  gets_cur (new, WNN_PASSWD_LEN);

  if (find_fid_in_env (envi, fid) == -1)
    {
      wnn_errorno = WNN_FID_ERROR;
      error_ret ();
      putc_purge ();
      return;
    }

  ret = file_password_set (&files[fid], which, old, new);
  if (ret == -1)
    {
      goto ERR_RET;
    }
  put4_cur (0);
  putc_purge ();
  return;
ERR_RET:
  error_ret ();
  putc_purge ();
}
