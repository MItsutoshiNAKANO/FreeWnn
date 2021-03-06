/*
 *  $Id: dic_atojis.c,v 1.5 2013-09-02 11:01:39 itisango Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#if STDC_HEADERS
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#endif /* STDC_HEADERS */

#include "commonhd.h"
#include "jslib.h"
#include "jdata.h"
#include "wnn_os.h"
#include "wnn_string.h"

#include "etc.h"

static int keta_4;

static w_char _tatojis[] = {
  0xa1a1, 0xa1aa, 0xa1c9, 0xa1f4, 0xa1f0, 0xa1f3, 0xa1f5, 0xa1c7,
  0xa1ca, 0xa1cb, 0xa1f6, 0xa1dc, 0xa1a4, 0xa1dd, 0xa1a5, 0xa1bf,
  0xa3b0, 0xa3b1, 0xa3b2, 0xa3b3, 0xa3b4, 0xa3b5, 0xa3b6, 0xa3b7,
  0xa3b8, 0xa3b9, 0xa1a7, 0xa1a8, 0xa1e3, 0xa1e1, 0xa1e4, 0xa1a9,
  0xa1f7, 0xa3c1, 0xa3c2, 0xa3c3, 0xa3c4, 0xa3c5, 0xa3c6, 0xa3c7,
  0xa3c8, 0xa3c9, 0xa3ca, 0xa3cb, 0xa3cc, 0xa3cd, 0xa3ce, 0xa3cf,
  0xa3d0, 0xa3d1, 0xa3d2, 0xa3d3, 0xa3d4, 0xa3d5, 0xa3d6, 0xa3d7,
  0xa3d8, 0xa3d9, 0xa3da, 0xa1ce, 0xa1ef, 0xa1cf, 0xa1b0, 0xa1b2,
  0xa1ae, 0xa3e1, 0xa3e2, 0xa3e3, 0xa3e4, 0xa3e5, 0xa3e6, 0xa3e7,
  0xa3e8, 0xa3e9, 0xa3ea, 0xa3eb, 0xa3ec, 0xa3ed, 0xa3ee, 0xa3ef,
  0xa3f0, 0xa3f1, 0xa3f2, 0xa3f3, 0xa3f4, 0xa3f5, 0xa3f6, 0xa3f7,
  0xa3f8, 0xa3f9, 0xa3fa, 0xa1d0, 0xa1c3, 0xa1d1, 0xa1b1, 0x007f
};


static int
atojis (c)
     register w_char c;
{
  if (c >= ' ' && c <= (unsigned char) '\376')
    c = _tatojis[c - ' '];
  return (c);
}

static w_char _tatokan[] = {
  0xa1bb, 0xb0ec, 0xc6f3, 0xbbb0, 0xbbcd,       /* �����󻰻� */
  0xb8de, 0xcfbb, 0xbcb7, 0xc8ac, 0xb6e5        /* ��ϻ��Ȭ�� */
};

static w_char _tatokan_o[] = {
  0xceed, 0xb0ed, 0xc6f5, 0xbbb2, 0xbbcd,       /* ���������� */
  0xb8e0, 0xcfbb, 0xbcb7, 0xc8ac, 0xb6e5        /* ��ϻ��Ȭ�� */
};

static w_char
atokan (c)
     register w_char c;
{
  if (c >= '0' && c <= '9')
    c = _tatokan[c - '0'];
  return (c);
}

static w_char
atokan_o (c)
     register w_char c;
{
  if (c >= '0' && c <= '9')
    c = _tatokan_o[c - '0'];
  return (c);
}

static w_char _tatoket[] = {
  0xbdbd, 0xc9b4, 0xc0e9,       /* ��ɴ�� */
  0xcbfc, 0xb2af, 0xc3fb, 0xb5fe,       /* �������� */
  0xd4b6, 0xa4b7, 0xbef7, 0xb9c2,       /* Զ������ */
  0xb4c2, 0xc0b5, 0xbadc, 0xb6cb        /* �����ܶ� */
/* ���Ϻ� *//* ���ε� *//* ��ͳ¾ *//* �ԲĻ׵� *//* ̵����� */
};

static w_char _tatoket_o[] = {
  0xbda6, 0xeff9, 0xeff4,       /* ������ */
  0xe8df                        /* �� */
};

static void
getketa (k, kouho, col)
     int k;
     w_char kouho[];
     int *col;
{
  if (k % 4)
    {
      if (keta_4 != 0)
        {
          keta_4 = 0;
          getketa (k / 4 * 4, kouho, col);
        }
      kouho[(*col)++] = _tatoket[k % 4 - 1];
    }
  else if ((k / 4 > 0) && (k / 4 < 13))
    {
      kouho[(*col)++] = _tatoket[k / 4 + 2];
    }
  else if (k / 4 == 13)
    {
      kouho[(*col)++] = 0xbabb;
      kouho[(*col)++] = 0xb2cf;
      kouho[(*col)++] = 0xb9b1; /* ���Ϻ� */
    }
  else if (k / 4 == 14)
    {
      kouho[(*col)++] = 0xb5c0;
      kouho[(*col)++] = 0xc1ce;
      kouho[(*col)++] = 0xb0a4; /* ���ε� */
    }
  else if (k / 4 == 15)
    {
      kouho[(*col)++] = 0xc2be;
      kouho[(*col)++] = 0xcdb3;
      kouho[(*col)++] = 0xc6e1; /* ��ͳ¾ */
    }
  else if (k / 4 == 16)
    {
      kouho[(*col)++] = 0xb5c4;
      kouho[(*col)++] = 0xbbd7;
      kouho[(*col)++] = 0xb2c4;
      kouho[(*col)++] = 0xc9d4; /* �ԲĻ׵� */
    }
  else if (k / 4 == 17)
    {
      kouho[(*col)++] = 0xbff4;
      kouho[(*col)++] = 0xc2e7;
      kouho[(*col)++] = 0xcecc;
      kouho[(*col)++] = 0xccb5; /* ̵����� */
    }
  else
    {
      kouho[(*col)++] = 0xa1a9; /* �� */
    }
  keta_4 = 0;
}

static void
getketa_o (k, kouho, col)
     int k;
     w_char kouho[];
     int *col;
{
  if (k % 4)
    {
      if (keta_4 != 0)
        {
          keta_4 = 0;
          if (k / 4 == 1)
            kouho[(*col)++] = _tatoket_o[3];    /* �� */
          else
            getketa (k / 4 * 4, kouho, col);
        }
      kouho[(*col)++] = _tatoket_o[k % 4 - 1];
    }
  else if (k / 4 == 1)
    {
      kouho[(*col)++] = _tatoket_o[3];  /* �� */
    }
  else
    {
      getketa (k, kouho, col);
    }
  keta_4 = 0;
}

int
substr (c1, c2)
     char *c1;
     w_char *c2;
{
  for (; *c1; c1++, c2++)
    {
      if (*c1 != *c2)
        break;
    }
  if (*c1)
    return (0);
  return (1);
}

w_char *
kanji_giji_str (bun, bunl, c, kouho)
     w_char *bun;
     int bunl;
     w_char *c;
     w_char *kouho;
{
  register int k;
  w_char revkouho[LENGTHYOMI];
  char tmp[LENGTHYOMI];
  int col = 0, keta = 0;

  if (substr (DIC_HIRAGANA, c))
    {
      for (k = bunl - 1; k >= 0; k--)
        {
          *kouho++ = bun[k];
        }
    }
  else if (substr (DIC_KATAKANA, c))
    {                           /* �������� */
      for (k = bunl - 1; k >= 0; k--)
        {
          if ((bun[k] & 0xff00) == 0xa400)
            *kouho++ = bun[k] | 0x100;
          else
            *kouho++ = bun[k];
        }
    }
  else if (substr (DIC_ZENKAKU, c))
    {                           /* ���ѿ��� *//* ������ */
      for (k = bunl - 1; k >= 0; k--)
        {
          *kouho++ = atojis (bun[k]);
        }
    }
  else if (substr (DIC_NUM_KAN, c))
    {                           /* ������ *//* ���� */
      for (k = bunl - 1; k >= 0; k--)
        {
          *kouho++ = atokan (bun[k]);
        }
    }
  else if (substr (DIC_NUM_HANCAN, c))
    {                           /* Ⱦ�ѿ��� *//* 1,234 */
      for (k = 0; k < bunl; k++)
        {
          if ((keta != 0) && (keta % 3 == 0))
            revkouho[col++] = ',';      /* , */
          revkouho[col++] = bun[k];
          keta++;
        }
      while (--col >= 0)
        {
          *kouho++ = revkouho[col];
        }
    }
  else if (substr (DIC_NUM_ZENCAN, c))
    {                           /* ���ѿ��� *//* ���������� */
      for (k = 0; k < bunl; k++)
        {
          if ((keta != 0) && (keta % 3 == 0))
            revkouho[col++] = 0xa1a4;   /* �� */
          revkouho[col++] = atojis (bun[k]);
          keta++;
        }
      while (--col >= 0)
        {
          *kouho++ = revkouho[col];
        }
    }
  else if (substr (DIC_NUM_KANSUUJI, c))
    {                           /* ������ *//* ɴ�󽽻� */
      keta_4 = 0;
      for (k = 0; k < bunl; k++)
        {
          if (bun[k] != '0')
            {
              if (keta != 0)
                getketa (keta, revkouho, &col);
              if (bun[k] != '1' || keta % 4 == 0 || keta % 4 == 3)
                revkouho[col++] = atokan (bun[k]);
            }
          else if ((keta != 0) && (keta % 4 == 0))
            {
              keta_4 = 1;
            }
          keta++;
        }
      if (col == 0)
        *kouho++ = _tatokan[0];
      while (--col >= 0)
        {
          *kouho++ = revkouho[col];
        }
    }
  else if (substr (DIC_NUM_KANOLD, c))
    {                           /* ������ *//* ��ɴ������ chao */
      keta_4 = 0;
      for (k = 0; k < bunl; k++)
        {
          if (bun[k] != '0')
            {
              if (keta != 0)
                getketa_o (keta, revkouho, &col);
              if (bun[k] != '1' || keta % 4 == 0 || keta % 4 == 3)
                revkouho[col++] = atokan_o (bun[k]);
            }
          else if ((keta != 0) && (keta % 4 == 0))
            {
              keta_4 = 1;
            }
          keta++;
        }
      if (col == 0)
        *kouho++ = _tatokan_o[0];
      while (--col >= 0)
        {
          *kouho++ = revkouho[col];
        }
    }
  else if (substr (DIC_ESC, c))
    {
      *kouho++ = '\\';
    }
  else if (substr (DIC_HEX, c) || substr (DIC_HEXc, c))
    {
      int num;
      wnn_sStrcpy (tmp, c + strlen (DIC_HEX));
      sscanf (tmp, "%x", &num);
      *kouho++ = num;
    }
  else if (substr (DIC_OCT, c))
    {
      int num;
      wnn_sStrcpy (tmp, c + strlen (DIC_OCT));
      sscanf (tmp, "%o", &num);
      *kouho++ = num;
    }
  else
    {
      *kouho++ = *c++;
      for (; *c && *c != '\\'; c++)
        {
          *kouho++ = *c;
        }
      if (*c == '\\')
        *kouho++ = *c;
    }
  *kouho = 0;
  return (kouho);
}


void
kanji_esc_str (w, oy, oyl)
     w_char *w, *oy;
     int oyl;
{
  register w_char *ret;
  register w_char *c = w;
  w_char tmp[LENGTHKANJI];

  for (; *c; c++)
    {
      if (*c == '\\')
        break;
    }
  if (!*c)
    return;
  wnn_Strcpy (tmp, w);
  c = tmp;
  while (*c)
    {
      if (*c == '\\' && (ret = kanji_giji_str (oy, oyl, c, w)) != NULL)
        {
          w = ret;
          for (++c; *c; c++)
            {
              if (*c == '\\')
                {
                  c++;
                  break;
                }
            }
        }
      else
        {
          *w++ = *c++;
        }
    }
  *w = 0;
}

void
Get_kanji (kptr, oy, oyl, kanji, yomi, comment)
     UCHAR *kptr;
     w_char *kanji, *comment, *yomi, *oy;
     int oyl;
{
  get_kanji_str (kptr, kanji, yomi, comment);
  if (kanji)
    {
      kanji_esc_str (kanji, oy, oyl);
    }
  if (yomi)
    {
      kanji_esc_str (yomi, oy, oyl);
    }
}


void
Get_knj1 (kptr, oy, oyl, kanji2, kouho, yomi, comment)
     UCHAR *kptr;
     int kanji2;
     w_char *kouho, *comment, *yomi, *oy;
     int oyl;
{
  int tcnt;
  for (tcnt = 0; tcnt < kanji2; tcnt++)
    {
      kptr += *kptr;
    }
  Get_kanji (kptr, oy, oyl, kouho, yomi, comment);
  return;
}
