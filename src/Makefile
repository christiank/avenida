# Avenida Makefile
# Christian Koch <cfkoch@sdf.lonestar.org>

.PHONY: all clean install
.SUFFIXES: .c .o
.MAIN: all

#CC?= clang -Wall
CC= clang -Wall
PREFIX=/usr/local

LUADIR=./lua
LUA_PLATFORM?=macosx
LUA_CFLAGS= -I$(LUADIR)/src
LUA_LDFLAGS= -L$(LUADIR)/src
LUA_LIBS= -llua -lm

GM_CONFIG= GraphicsMagickWand-config
GM_CFLAGS= $$($(GM_CONFIG) --cflags) $$($(GM_CONFIG) --cppflags)
GM_LDFLAGS= $$($(GM_CONFIG) --ldflags)
GM_LIBS= $$($(GM_CONFIG) --libs)

CAIRO_CONFIG= pkg-config cairo
CAIRO_CFLAGS= $$($(CAIRO_CONFIG) --cflags)
CAIRO_LDFLAGS=
CAIRO_LIBS= $$($(CAIRO_CONFIG) --libs)

CFLAGS= $(LUA_CFLAGS) $(GM_CFLAGS) $(CAIRO_CFLAGS)
LDFLAGS= $(LUA_LDFLAGS) $(GM_LDFLAGS) $(CAIRO_LDFLAGS)
LIBS= $(LUA_LIBS) $(GM_LIBS) $(CAIRO_LIBS)

OBJS= \
	cJSON.o \
	linenoise.o \
	status.o \
	commands.o \
	main.o \
	media.o \
	raster.o \
	script.o \
	vector.o \
	avnscript-raster.o \
	avnscript-vector.o \
	avnscript-util.o

OUTBIN= avenida

##########

all: $(OUTBIN)

$(OUTBIN): $(OBJS)
	$(CC) -o $(.TARGET) $(.ALLSRC) $(LDFLAGS) $(LIBS)

.c.o:
	$(CC) -c $(CFLAGS) -o $(.TARGET) $(.ALLSRC)

clean:
	rm -f $(OUTBIN) *.o *.core *.png

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/man/man1
	cp $(OUTBIN) $(DESTDIR)$(PREFIX)/bin
	cp ../man/avenida.1 $(DESTDIR)$(PREFIX)/man/man1
