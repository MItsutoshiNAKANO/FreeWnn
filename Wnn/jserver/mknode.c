/* $Id: mknode.c,v 1.1.2.1 2001-07-08 06:39:08 iwao Exp $ */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000
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

/*********************
 * kaiseki work area
**********************/

#include "commonhd.h"
#include "jslib.h"
#include "jdata.h"
#include "de_header.h"
#include "wnn_malloc.h"
#include "kaiseki.h"

#include <stdio.h>

w_char *bun;                    /* D */

w_char giji_eisuu[20];          /* 擬似「英数」の定義 */

int maxchg;
int initjmt;                    /* I think initjmt is the length of jmt_ */

int *maxj;                      /* maxj is counts to entries in jmt_ */

/* jmt_ptr is used in jmt0.c only.
   but it must be changed by clients.
   jishobiki does not use it. this is sent to jishobiki by arguments
   */

/* j_e_p is used to hold the current point to which jmtw_ is used */
struct jdata *j_e_p;
struct jdata **jmt_;
struct jdata *jmtw_;
struct jdata **jmt_end;
struct jdata *jmtw_end;
struct jdata **jmt_ptr;
struct jdata ***jmtp;

struct FT *ft;

void
init_jmt (x)
      int x;
{
   int n;
   struct jdata **keep_ptr;
   struct jdata **jmt_ptr_org = jmt_ptr;        /* H.T. */

  if (x == 0)
    x = initjmt;                /* H.T. 22/12/89 */

  for (n = x; n < initjmt && (jmtp[n] == (struct jdata **) 0 || jmtp[n] == (struct jdata **) -1); n++)
    ;

  if (n < initjmt)
    {                           /* Tukatte nai nodakara, kesubeki deha... */
      keep_ptr = jmt_ptr;

      if (n == x)
        jmt_ptr = jmtp[x];
      else
        jmt_ptr = jmtp[n] + (maxj[n] - n + 1);

      for (n = 0; (jmt_ptr + n) < keep_ptr && *(jmt_ptr + n) <= (struct jdata *) 0; n++)
        ;
      if ((jmt_ptr + n) < keep_ptr)
        j_e_p = *(jmt_ptr + n);
    }
  else
    {
      jmt_ptr = jmt_;
      j_e_p = jmtw_;
    }
/* this part is added 8/18 by H.T */
  for (keep_ptr = jmt_ptr; keep_ptr < jmt_ptr_org; keep_ptr++)
    {
      *keep_ptr = NULL;
    }
}


int
jmt_set (yomi)
      int yomi;         /* 読み文字列の先頭へのポインタ(逆順) */
{
   int n;
  if ((n = jishobiki (&(bun[yomi]), jmt_ptr)) > 0)
    {
      jmtp[yomi] = jmt_ptr;
      jmt_ptr += n;
      maxj[yomi] = yomi + (n - 1);
    }
  else
    {
      if (n == 0)
        {
          jmtp[yomi] = 0;
          maxj[yomi] = 0;
        }
      else
        {
          jmtp[yomi] = 0;
          maxj[yomi] = 0;
        }
    }
  return (1);
}

static struct BZD *clrnd_sub ();
static void lnk_bzd (), lnk_ichbn ();
static int mknd_sub (), hyoka2 ();
void freebzd ();

static struct BZD *free_bzd_top = NULL;
static struct free_list *free_list_bzd = NULL;
static struct ICHBNP *free_ichbn_top = NULL;
static struct free_list *free_list_ichbn = NULL;

/************************************************/
/*        initialize link struct BZD            */
/************************************************/
int
init_bzd ()
{
  free_area (free_list_bzd);
  if (get_area (FIRST_BZD_KOSUU, sizeof (struct BZD), &free_list_bzd) < 0)
    return (-1);
  lnk_bzd (free_list_bzd);
  return (0);
}

/* free_bzd が 0 でない時に呼んだらあかんよ */
static void
lnk_bzd (list)
     struct free_list *list;
{
   int n;
   struct BZD *wk_ptr;

  free_bzd_top = wk_ptr = (struct BZD *) ((char *) list + sizeof (struct free_list));

  for (n = list->num - 1; n > 0; wk_ptr++, n--)
    wk_ptr->lnk_son = wk_ptr + 1;
  wk_ptr->lnk_son = 0;
}


/***********************************************/
/*       initialize link struct ICHBNP         */
/***********************************************/
int
init_ichbnp ()
{
  free_area (free_list_ichbn);
  if (get_area (FIRST_ICHBN_KOSUU, sizeof (struct ICHBNP), &free_list_ichbn) < 0)
    return (-1);
  lnk_ichbn (free_list_ichbn);
  return (0);
}

/* free_ichbn が 0 でない時に呼んだらあかんよ */
static void
lnk_ichbn (list)
     struct free_list *list;
{
   int n;
   struct ICHBNP *wk_ptr;

  free_ichbn_top = wk_ptr = (struct ICHBNP *) ((char *) list + sizeof (struct free_list));

  for (n = list->num - 1; n > 0; wk_ptr++, n--)
    wk_ptr->next_p = wk_ptr + 1;
  wk_ptr->next_p = 0;
}

/*********************************************************/
/*   struct BZD & ICHBNP   free area sakusei routine     */
/*********************************************************/
void
clr_node (bzd)
      struct BZD *bzd;  /*clear node top pointer */
{
   struct BZD *return_p;        /*clrnd_sub return pointer */

  return_p = clrnd_sub (bzd->lnk_son);
  if (return_p != 0)
    {
      freebzd (return_p);
    }
  freebzd (bzd);
}

static struct BZD *
clrnd_sub (bzd)
      struct BZD *bzd;  /* node pointer */
{
  struct BZD *return_p;         /* clrnd_sub return pointer */

  if (bzd != 0)
    {
      return_p = clrnd_sub (bzd->lnk_son);
      if (return_p != 0)
        {
          freebzd (return_p);
        }
      return_p = clrnd_sub (bzd->lnk_br);
      if (return_p != 0)
        freebzd (return_p);
      return (bzd);
    }
  else
    return (0);
}

void
freebzd (bzd)                   /* struct BZD o free_area e link */
      struct BZD *bzd;  /* クリアするノードのポインタ */
{
  bzd->lnk_son = free_bzd_top;
  free_bzd_top = bzd;
  clr_sbn_node (bzd->sbn);
}

void
freeibsp (ichbnp_p)             /* struct ICHBNP o free_area e link  */
     struct ICHBNP *ichbnp_p;   /* BZD ni link shiteiru ICHBNP eno pointer */
{
   struct ICHBNP *wk_ptr;

  if (ichbnp_p == 0)
    return;
  for (wk_ptr = ichbnp_p; wk_ptr->next_p != 0; wk_ptr = wk_ptr->next_p)
    ;
  wk_ptr->next_p = free_ichbn_top;
  free_ichbn_top = ichbnp_p;
}

/******************************************/
/* BZD area no kakutoku routine           */
/******************************************/
struct BZD *
getbzdsp ()
{
   struct BZD *rtnptr;

  if (free_bzd_top == 0)
    {
      if (get_area (BZD_KOSUU, sizeof (struct BZD), &free_list_bzd) < 0)
        return ((struct BZD *) -1);
      lnk_bzd (free_list_bzd);
    }

  rtnptr = free_bzd_top;
  free_bzd_top = free_bzd_top->lnk_son;
  rtnptr->lnk_br = 0;
  rtnptr->lnk_son = 0;
  rtnptr->son_v = 0;
  return (rtnptr);
}

/******************************************/
/* ICHBNP area no kakutoku routine        */
/******************************************/
struct ICHBNP *
getibsp ()
{
   struct ICHBNP *rtnptr;

  if (free_ichbn_top == 0)
    {
      if (get_area (ICHBN_KOSUU, sizeof (struct ICHBNP), &free_list_ichbn) < 0)
        return ((struct ICHBNP *) -1);
      lnk_ichbn (free_list_ichbn);
    }

  rtnptr = free_ichbn_top;
  free_ichbn_top = free_ichbn_top->next_p;
  rtnptr->next_p = 0;
  return (rtnptr);
}


/*******************************************/
/* ICHBNP table からのオフセットの取りだし */
/*******************************************/
struct fzkkouho *
getfzkoh_body (ichbnp_p, no)
      struct ICHBNP *ichbnp_p;
     int no;                    /* table index 0 org */
{
   int cnt, no1;

  no1 = no / FZKIBNO;
  for (cnt = 0; cnt < no1; cnt++)
    ichbnp_p = ichbnp_p->next_p;
  return (&(ichbnp_p->fzkib[no % FZKIBNO]));
}

struct fzkkouho *
getfzkoh1_body (ichbnp_p, no)
      struct ICHBNP *ichbnp_p;
      int no;           /* table index 0 org */
{
   int cnt, no1;

  no1 = no / FZKIBNO;
  for (cnt = 0; cnt < no1; cnt++)
    ichbnp_p = ichbnp_p->next_p;
  return (&(ichbnp_p->fzkib1[no % FZKIBNO]));
}


/*********************************************/
/*            make_node routine              */
/*********************************************/
int
#ifdef  NO_FZK
mknode (ptr, yomi_eno, beginvect, endvect, mx_syo_bnst)
#else
mknode (ptr, yomi_eno, beginvect, fzkchar, endvect, mx_syo_bnst)
#endif                          /* NO_FZK */
     struct BZD *ptr;           /* struct BZD node top pointer */
     int yomi_eno;              /* kaiseki bunsyou end index (end char no tugi) */
     int beginvect;             /* 前端ベクタ(-1:文節先頭、-2:なんでも)品詞No. */
#ifndef NO_FZK
     w_char *fzkchar;
#endif /* NO_FZK */
     int endvect;               /* shuutan vector */
     int mx_syo_bnst;           /* 解析を止める小文節の数 */
{
   int flg = 1;         /* call or nocall value */

  flg = mknd_sub (ptr, yomi_eno, beginvect,
#ifndef NO_FZK
                  fzkchar,
#endif /* NO_FZK */
                  endvect, flg, mx_syo_bnst);
  switch (flg)
    {
    case 1:
      ptr->kbcnt++;             /* new_node ga tsukurareta toki */
      return (1);
    case 0:
      return (0);               /* new_node ga tsukurarenakatta toki */
    default:
      return (-1);              /* free_area ga nakatta toki */
    }
}

static int
#ifdef  NO_FZK
mknd_sub (ptr, yomi_eno, beginvect, endvect, flg, mx_syo_bnst)
#else
mknd_sub (ptr, yomi_eno, beginvect, fzkchar, endvect, flg, mx_syo_bnst)
#endif                          /* NO_FZK */
     struct BZD *ptr;           /* struct BZD node pointer */
     int yomi_eno;              /* kaiseki bunsyou end index (end char no tugi) */
     int beginvect;             /* 前端ベクタ(-1:文節先頭、-2:なんでも)品詞No. */
#ifndef NO_FZK
     w_char *fzkchar;
#endif /* NO_FZK */
     int endvect;               /* shuutan vector */
     int flg;                   /* call or nocall value */
     int mx_syo_bnst;           /* 解析を止める小文節の数 */
{
   int keep_v;          /* max 評価値 */
   struct BZD *wk_ptr;  /* work pointer */

  if (ptr->lnk_son != 0)
    {
      for (wk_ptr = ptr->lnk_son, keep_v = 0; wk_ptr != 0; wk_ptr = wk_ptr->lnk_br)
        {
          switch (mknd_sub (wk_ptr, yomi_eno, beginvect,
#ifndef NO_FZK
                            fzkchar,
#endif /* NO_FZK */
                            endvect, flg, mx_syo_bnst))
            {
            case 1:
              if (flg == 1)
                {               /* 解析文節数 */
                  wk_ptr->kbcnt++;
                  /* brothers max hyoka value */
                  if (wk_ptr->son_v > keep_v)
                    keep_v = wk_ptr->son_v;
                  wk_ptr->son_v /= wk_ptr->kbcnt;
                }
              break;
            case 0:
              if (flg == 1)
                {
                  keep_v = wk_ptr->son_v;
                  flg = 0;
                }
              else
                {               /* brothers max hyoka value */
                  if (wk_ptr->son_v > keep_v)
                    keep_v = wk_ptr->son_v;
                }
              wk_ptr->son_v += wk_ptr->v_jc;
              break;
            default:
              return (-1);
            }
        }
      if (keep_v != 0)
        ptr->son_v = hyoka2 (keep_v, ptr->v_jc);
      return (flg);
    }
  else
    {
      if (ptr->j_c + 1 < yomi_eno)
        {
          if (flg == 1)
            {                   /* make new node */
              if (dbn_kai (ptr->j_c + 1, yomi_eno, beginvect,
#ifndef NO_FZK
                           fzkchar,
#endif /* NO_FZK */
                           endvect, WNN_VECT_NO, mx_syo_bnst, &(ptr->lnk_son)) < 0)
                return (-1);    /* ERROR */
              for (wk_ptr = ptr->lnk_son, keep_v = 0; wk_ptr != 0; wk_ptr = wk_ptr->lnk_br)
                {
                  if (wk_ptr->v_jc > keep_v)
                    keep_v = wk_ptr->v_jc;
                  /* brothers max hyoka value */
                }
              ptr->son_v = hyoka2 (keep_v, ptr->v_jc);
            }
          return (1);           /* make flg */
        }
      else
        return (0);             /* no make flg */
    }
}

/********************************************/
/*           bunsetsu hyoka kansuu          */
/********************************************/
static int
hyoka2 (max_v, par_v)
     int max_v;                 /* brothers max hyoka value */
     int par_v;                 /* parent hyoka value */
{
  return ((max_v + par_v));
}

static void lnk_sbn ();

static struct SYO_BNSETSU *free_sbn_top = NULL;
static struct free_list *free_list_sbn = NULL;
/************************************************/
/* initialize link struct SYO_BNSETSU           */
/************************************************/
int
init_sbn ()
{
  free_area (free_list_sbn);
  if (get_area (FIRST_SBN_KOSUU, sizeof (struct SYO_BNSETSU), &free_list_sbn) < 0)
    return (-1);
  lnk_sbn (free_list_sbn);
  return (0);
}

int
get_area (kosuu, size, list)
      int kosuu;
      int size;
     struct free_list **list;
{
   struct free_list *area;

  if ((area = (struct free_list *) malloc (size * kosuu + sizeof (struct free_list))) == NULL)
    {
      wnn_errorno = WNN_MALLOC_INITIALIZE;
      error1 ("Cannot Malloc in get_area.\n");
      return (-1);
    }
  area->lnk = *list;
  area->num = kosuu;
  *list = area;
  return (0);
}

void
free_area (list)
      struct free_list *list;
{
  if (list == 0)
    return;
  free_area (list->lnk);
  free (list);
}

/* free_sbn が 0 でない時に呼んだらあかんよ */
static void
lnk_sbn (list)
     struct free_list *list;
{
   int n;
   struct SYO_BNSETSU *wk_ptr;

  free_sbn_top = wk_ptr = (struct SYO_BNSETSU *) ((char *) list + sizeof (struct free_list));

  for (n = list->num - 1; n > 0; wk_ptr++, n--)
    wk_ptr->lnk_br = wk_ptr + 1;
  wk_ptr->lnk_br = 0;
}


/*******************************************************/
/* struct SYO_BNSETSU & ICHBNP & KANGO free エリア作成 */
/*******************************************************/
void
freesbn (sbn)                   /* struct SYO_BNSETSU を free_area へリンク */
      struct SYO_BNSETSU *sbn;  /* クリアするノードのポインタ */
{
  if (sbn == 0)
    return;
  sbn->reference--;
  if (sbn->reference <= 0)
    {
      sbn->lnk_br = free_sbn_top;
      free_sbn_top = sbn;
    }
}

void
clr_sbn_node (sbn)
      struct SYO_BNSETSU *sbn;
{
  if (sbn == 0)
    return;
  if ((sbn->reference) <= 1)
    {
      freesbn (sbn);
      clr_sbn_node (sbn->parent);
    }
  else
    {
      sbn->reference--;
    }
}

/******************************************/
/* SYO_BNSETSU area の獲得                */
/******************************************/
struct SYO_BNSETSU *
getsbnsp ()
{
   struct SYO_BNSETSU *rtnptr;

  if (free_sbn_top == 0)
    {
      if (get_area (SBN_KOSUU, sizeof (struct SYO_BNSETSU), &free_list_sbn) < 0)
        return ((struct SYO_BNSETSU *) -1);
      lnk_sbn (free_list_sbn);
    }

  rtnptr = free_sbn_top;
  free_sbn_top = free_sbn_top->lnk_br;
  rtnptr->lnk_br = 0;
  rtnptr->parent = 0;
  rtnptr->son_v = 0;
  rtnptr->reference = 0;
  rtnptr->jentptr = 0;
  rtnptr->status = 0;
  rtnptr->status_bkwd = 0;
  return (rtnptr);
}

static void lnk_jkdbn (), lnk_jksbn (), lnk_jksone (), freejktsone ();

static struct JKT_DBN *free_jkdbn_top = NULL;
static struct free_list *free_list_jkdbn = NULL;
static struct JKT_SBN *free_jksbn_top = NULL;
static struct free_list *free_list_jksbn = NULL;
static struct JKT_SONE *free_jksone_top = NULL;
static struct free_list *free_list_jksone = NULL;

/************************************************/
/* initialize link struct JKT_SONE           */
/************************************************/
int
init_jktdbn ()
{
  free_area (free_list_jkdbn);
  if (get_area (FIRST_JKDBN_KOSUU, sizeof (struct JKT_DBN), &free_list_jkdbn) < 0)
    return (-1);
  lnk_jkdbn (free_list_jkdbn);
  return (0);
}

/* free_jkdbn が 0 でない時に呼んだらあかんよ */
static void
lnk_jkdbn (list)
     struct free_list *list;
{
   int n;
   struct JKT_DBN *wk_ptr;

  free_jkdbn_top = wk_ptr = (struct JKT_DBN *) ((char *) list + sizeof (struct free_list));

  for (n = list->num - 1; n > 0; wk_ptr++, n--)
    wk_ptr->lnk_br = wk_ptr + 1;
  wk_ptr->lnk_br = 0;
}

int
init_jktsbn ()
{
  free_area (free_list_jksbn);
  if (get_area (FIRST_JKSBN_KOSUU, sizeof (struct JKT_SBN), &free_list_jksbn) < 0)
    return (-1);
  lnk_jksbn (free_list_jksbn);
  return (0);
}

/* free_jksbn が 0 でない時に呼んだらあかんよ */
static void
lnk_jksbn (list)
     struct free_list *list;
{
   int n;
   struct JKT_SBN *wk_ptr;

  free_jksbn_top = wk_ptr = (struct JKT_SBN *) ((char *) list + sizeof (struct free_list));

  for (n = list->num - 1; n > 0; wk_ptr++, n--)
    wk_ptr->lnk_br = wk_ptr + 1;
  wk_ptr->lnk_br = 0;
}

int
init_jktsone ()
{
  free_area (free_list_jksone);
  if (get_area (FIRST_JKSONE_KOSUU, sizeof (struct JKT_SONE), &free_list_jksone) < 0)
    return (-1);
  lnk_jksone (free_list_jksone);
  return (0);
}

/* free_jksone_top が 0 でない時に呼んだらあかんよ */
static void
lnk_jksone (list)
     struct free_list *list;
{
   int n;
   struct JKT_SONE *wk_ptr;

  free_jksone_top = wk_ptr = (struct JKT_SONE *) ((char *) list + sizeof (struct free_list));

  for (n = list->num - 1; n > 0; wk_ptr++, n--)
    wk_ptr->lnk_br = wk_ptr + 1;
  wk_ptr->lnk_br = 0;
}

/*******************************************************/
/* struct JKT_SONE & JKT_SBN free エリア作成 */
/*******************************************************/
void
freejktdbn (dbn)                /* struct JKT_SBN を free_area へリンク */
      struct JKT_DBN *dbn;      /* クリアするノードのポインタ */
{
  if (dbn == 0)
    return;

  freejktsbn (dbn->sbn);
  dbn->lnk_br = free_jkdbn_top;
  free_jkdbn_top = dbn;
}

void
freejktsbn (sbn)                /* struct JKT_SBN を free_area へリンク */
     struct JKT_SBN *sbn;       /* クリアするノードのポインタ */
{
   struct JKT_SONE *sone;
   struct JKT_SONE *next;
  if (sbn == 0)
    return;
  sbn->reference--;
  if (sbn->reference <= 0)
    {
      for (sone = sbn->sbn; sone != 0;)
        {
          next = sone->lnk_br;
          freejktsone (sone);
          sone = next;
        }
      sbn->reference = 0;
      sbn->lnk_br = free_jksbn_top;
      free_jksbn_top = sbn;
      freejktsbn (sbn->parent);
    }
}

static void
freejktsone (sone)              /* struct JKT_SONE を free_area へリンク */
      struct JKT_SONE *sone;    /* クリアするノードのポインタ */
{
  if (sone == 0)
    return;
  sone->lnk_br = free_jksone_top;
  free_jksone_top = sone;
}

/******************************************/
/* JKT_SBN JKT_SONE area の獲得           */
/******************************************/
struct JKT_DBN *
getjktdbn ()
{
   struct JKT_DBN *rtnptr;

  if (free_jkdbn_top == 0)
    {
      if (get_area (JKDBN_KOSUU, sizeof (struct JKT_DBN), &free_list_jkdbn) < 0)
        return ((struct JKT_DBN *) -1);
      lnk_jkdbn (free_list_jkdbn);
    }

  rtnptr = free_jkdbn_top;
  free_jkdbn_top = free_jkdbn_top->lnk_br;
  rtnptr->lnk_br = 0;
  return (rtnptr);
}

struct JKT_SBN *
getjktsbn ()
{
   struct JKT_SBN *rtnptr;

  if (free_jksbn_top == 0)
    {
      if (get_area (JKSBN_KOSUU, sizeof (struct JKT_SBN), &free_list_jksbn) < 0)
        return ((struct JKT_SBN *) -1);
      lnk_jksbn (free_list_jksbn);
    }

  rtnptr = free_jksbn_top;
  free_jksbn_top = free_jksbn_top->lnk_br;
  rtnptr->lnk_br = 0;
  rtnptr->reference = 0;
  rtnptr->status = 0;
  return (rtnptr);
}

struct JKT_SONE *
getjktsone ()
{
   struct JKT_SONE *rtnptr;

  if (free_jksone_top == 0)
    {
      if (get_area (JKSONE_KOSUU, sizeof (struct JKT_SONE), &free_list_jksone) < 0)
        return ((struct JKT_SONE *) -1);
      lnk_jksone (free_list_jksone);
    }

  rtnptr = free_jksone_top;
  free_jksone_top = free_jksone_top->lnk_br;
  rtnptr->lnk_br = 0;
  rtnptr->jentptr = 0;
  return (rtnptr);
}

int
get_kaiseki_area (kana_len)
     int kana_len;
{
  char *c;
  char *area_pter;

  int maxj_len = (kana_len * sizeof (int) + 7) & 0xfffffff8;
  int jmtp_len = (kana_len * sizeof (struct jdata **) + 7) & 0xfffffff8;
  int jmt_len = (SIZE_JISHOTABLE * sizeof (struct jdata *) + 7) & 0xfffffff8;
  int jmtw_len = (SIZE_JISHOHEAP * sizeof (struct jdata) + 7) & 0xfffffff8;

  int bun_len = ((kana_len + 1) * sizeof (w_char) + 7) & 0xfffffff8;

  if ((area_pter = malloc (bun_len + jmtw_len + jmt_len + jmtp_len + maxj_len)) == NULL)
    {
      wnn_errorno = WNN_MALLOC_INITIALIZE;
      error1 ("malloc in get_kaiseki_area");
      return (-1);
    }

  maxj = (int *) area_pter;
  area_pter += maxj_len;
  for (c = (char *) maxj; c < area_pter;)
    {
      *(c++) = 0;
    }

  jmtp = (struct jdata ***) area_pter;
  area_pter += jmtp_len;
  for (c = (char *) jmtp; c < area_pter;)
    {
      *(c++) = 0;
    }

  jmt_ = (struct jdata **) area_pter;
  area_pter += jmt_len;
  jmtw_ = (struct jdata *) area_pter;
  area_pter += jmtw_len;
  jmt_end = jmt_ + SIZE_JISHOTABLE;
  jmt_ptr = jmt_end;            /* H.T. To initialize all in jmt_init */
  jmtw_end = jmtw_ + SIZE_JISHOHEAP;

  bun = (w_char *) area_pter;
  area_pter += bun_len;

  maxchg = kana_len;
  initjmt = maxchg - 1;
  bun[maxchg] = NULL;

  return (0);
}


/* サーバーが起きた時に呼ぶ
        解析ワークエリアをクリアする
        変換がエラーリターンした時にも呼んでね。*/

void
init_work_areas ()
{
  init_ichbnp ();
  init_bzd ();
  init_sbn ();
  init_jktdbn ();
  init_jktsbn ();
  init_jktsone ();
}
