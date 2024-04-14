
############################################################################
# Files to be installed by make.

BIN1    = gamepad_test
BINS    = ${BIN1}

LIB1    = libgamepad.a
LIBS    = ${LIB1}

HIDLIB  = usbhid
HEADERS = gamepad.h gamepad_usbhid.h gamepad_analog.h gamepad_libhid.h
MAN3    = libgamepad.3

############################################################################
# List object files that comprise BIN1, BIN2, LIB1, LIB2, etc.

OBJS1   = gamepad.o gamepad_usbhid.o gamepad_analog.o gamepad_libhid.o
OBJS    = ${OBJS1}

############################################################################
# Compile, link, and install options
# Override from the command line with "make VAR=value"
# or by setting in the parent Makefile.

# Install in /usr/local, unless defined by the parent Makefile, the
# environment, or a command line option such as PREFIX=/opt/local.
PREFIX      ?= ../local
MANPREFIX   ?= ${PREFIX}
LOCALBASE   ?= ${PREFIX}

############################################################################
# Build flags
# Override with CC=gcc, CC=icc, etc.
# Do not add non-portable options (such as -Wall) using +=

# Portable defaults.  Can be overridden by mk.conf or command line.
CC      ?= cc
CFLAGS  ?= -O

INCLUDES+= -I${LOCALBASE}/include
CFLAGS  += ${INCLUDES}
LFLAGS1 += -L. -L${LOCALBASE}/lib -l${HIDLIB} -lgamepad
LFLAGS2 += -L. -L${LOCALBASE}/lib -lgamepad

############################################################################
# Assume first command in PATH.  Override with full pathnames if necessary.
# E.g. make INSTALL=/usr/local/bin/ginstall

INSTALL ?= install
LN      ?= ln
RM      ?= rm
AR      ?= ar
LD      ?= cc
LDSHARE ?= -shared

############################################################################
# Standard targets required by ports

all:    ${LIBS} ${BINS}

# Link rules
${BIN1}:        gamepad_test.c ${HEADERS} ${LIBS}
	${CC} -o ${BIN1} ${CFLAGS} gamepad_test.c ${LFLAGS1}

# Dynamic library
${LIB1}:        ${OBJS1}
	${AR} r ${LIB1} ${OBJS1}


############################################################################
# Include dependencies generated by "make depend", if they exist.
# These rules explicitly list dependencies for each object file.
# See "depend" target below.  If Makefile.depend does not exist, use
# generic source compile rules.  These have some limitations, so you
# may prefer to create explicit rules for each target file.  This can
# be done automatically using "cpp -M" or "cpp -MM".  Run "man cpp"
# for more information, or see the "depend" target below.

include Makefile.depend

############################################################################
# Self-generate dependencies the old-fashioned way

depend:
	rm -f Makefile.depend
	touch Makefile.depend
	for file in *.c; do \
		cpp ${INCLUDES} -MM $${file} >> Makefile.depend; \
		echo -e "\t\$${CC} -c \$${CFLAGS} $${file}" >> Makefile.depend; \
		echo "" >> Makefile.depend; \
	done


############################################################################
# Generate a header containing prototypes for C files.  Requires
# the cproto command, which is freely available on the WEB.

protos:
	(cproto ${INCLUDES} *.c > temp_protos.h && mv -f temp_protos.h protos.h)


############################################################################
# Remove generated files (objs and nroff output from man pages)

clean:
	rm -f ${OBJS} ${BINS} ${LIBS} *.nr gamepad_test

# Keep backup files during normal clean, but provide an option to remove them
realclean: clean
	rm -f .*.bak *.bak *.BAK *.gmon


############################################################################
# Install all target files (binaries, libraries, docs, etc.)

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin ${DESTDIR}${PREFIX}/lib \
		 ${DESTDIR}${PREFIX}/include/libgamepad \
		 ${DESTDIR}${MANPREFIX}/man/man3
	for file in ${BINS} ; do \
	    ${INSTALL} -s -c -m 0555 $${file} ${DESTDIR}${PREFIX}/bin; \
	done
	for file in ${HEADERS} ; do \
	    ${INSTALL} -c -m 0444 $${file} ${DESTDIR}${PREFIX}/include/libgamepad; \
	done
	for file in ${LIBS} ; do \
	    ${INSTALL} -c -m 0444 $${file} ${DESTDIR}${PREFIX}/lib; \
	done
	for file in ${MAN3} ; do \
	    ${INSTALL} -c -m 0444 $${file} ${DESTDIR}${MANPREFIX}/man/man3; \
	done
