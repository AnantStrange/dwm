include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: options dwm

#LDFLAGS="-Wno-unused-variable -g3 -I/usr/include/freetype2"
#CFLAGS="-I/usr/include/freetype2"

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo -e "\n"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo -e "\n"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz *.orig *.rej

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R Makefile config.mk\
		dwm.1 drw.h util.h ${SRC} transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar
	rm -rf dwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1
	mkdir -p ${DESTDIR}${PREFIX}/share/dwm

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

.PHONY: all options clean dist install uninstall
