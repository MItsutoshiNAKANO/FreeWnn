# $Id: Makefile.FreeBSD4.x,v 1.1.2.1 2001-07-08 08:43:27 iwao Exp $

CFLAGS = -g -Iinclude -DJSERVER -DLIBDIR=\"${PWD}\" -DMAXPATHLEN=1024

OBJS = atojis.o bnsetu_kai.o daibn_kai.o \
de.o dispatch.o do_dic_env.o do_dic_no.o do_env.o do_filecom.o \
do_henkan.o do_henkan1.o do_hindo_s.o error.o fzk.o \
hinsi_list.o initjserv.o jbiki.o jikouho.o jikouho_d.o jishoop.o \
jishosub.o malloc.o mknode.o rd_jishoop.o readfile.o renbn_kai.o w_string.o \
jlib/wnnerrmsg.o \
misc/dic_atojis.o misc/bdic.o misc/gethinsi.o misc/hindo.o misc/msg.o misc/pwd.o \
misc/revdic.o misc/sstrings.o misc/strings.o misc/xutoj.o 

NOUSED = dictoasc.o jbiki_b.o b_index.o sisheng.o

LDLIBS = -lcrypt

TARGET = jserver

all: ${TARGET}

jserver: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
	$(RM) $(TARGET) $(OBJS) *~ *.bak core *.core

# End of Makefile
