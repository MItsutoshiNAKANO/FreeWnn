# $Id: Makefile.FreeBSD4.x,v 1.1.2.1 2001-07-08 06:33:55 iwao Exp $

CFLAGS = -g -I../include -DLIBDIR=\"${PWD}\" -DJAPANESE -DMAXPATHLEN=1024

TARGET = atod.o atof.o atorev.o dic_head.o dtoa.o oldatonewa.o \
sort.o ujisf.o utilbdic.o wddel.o wdreg.o wnnkill.o wnnstat.o wnntouch.o

all: ${TARGET}

clean:
	$(RM) $(TARGET) *~ *.bak core *.core

# End of Makefile

