#
# $Id: Makefile,v 1.1.2.4 1999/04/19 05:51:56 nakanisi Exp $
#

# FreeWnn is a network-extensible Kana-to-Kanji conversion system.
# This file is part of FreeWnn.
# 
# Copyright Wnn Consortium. 1993
#
# Author: OMRON SOFTWARE Co., Ltd. <freewnn@rd.kyoto.omronsoft.co.jp>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Emacs; see the file COPYING.  If not, write to the
# Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Commentary:
#
# Change log:
#	'99/04/19	TAOKA Satoshi - 田岡 智志<taoka@infonets.hiroshima-u.ac.jp>
#		インストールディレクトリを変更可能にする
#
# Last modified date: 19,Apr.1999
#

# Makefile for gerodic 1.00
#
#	Jun. 26th '93 Wnn Consortium

PREFIX = /usr/local
WNNBINDIR=${PREFIX}/bin/Wnn4
WNNWNNDIR=${PREFIX}/lib/wnn

WNNOWNER=wnn

JWNNWNNDIR=${WNNWNNDIR}/ja_JP
JWNNDICDIR=${JWNNWNNDIR}/dic

ATOD=${WNNBINDIR}/atod
WNNTOUCH=${WNNBINDIR}/wnntouch
HINSI=${JWNNWNNDIR}/hinsi.data
GERODIR=${JWNNDICDIR}/gerodic

SRC=g-jinmei.u
OBJ=g-jinmei.dic

all: ${SRC}
	${ATOD} -h ${HINSI} ${OBJ} < ${SRC}

install: ${OBJ}
	-mkdir ${GERODIR}
	chown ${WNNOWNER} ${GERODIR}
	install -o ${WNNOWNER} -m 644 ${OBJ} ${GERODIR}
	${WNNTOUCH} ${GERODIR}/${OBJ}
