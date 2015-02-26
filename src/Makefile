# Avenida Makefile
# Christian Koch <cfkoch@sdf.lonestar.org>

.PHONY: all clean tests raster-tests vector-tests
.SUFFIXES: .c .o
.MAIN: all

#CC?= clang -Wall
CC= clang -Wall

LUA=lua5.2
LUA_CFLAGS= -I/usr/pkg/include/lua-5.2
LUA_LDFLAGS= -L/usr/pkg/lib
LUA_LIBS= -llua5.2 -lm

GM_CONFIG= GraphicsMagickWand-config
GM_CFLAGS= $$($(GM_CONFIG) --cflags) $$($(GM_CONFIG) --cppflags)
GM_LDFLAGS= $$($(GM_CONFIG) --ldflags)
GM_LIBS= $$($(GM_CONFIG) --libs)

CAIRO_CONFIG= pkg-config cairo
CAIRO_CFLAGS= $$($(CAIRO_CONFIG) --cflags)
CAIRO_LDFLAGS=
CAIRO_LIBS= $$($(CAIRO_CONFIG) --libs)

SOX_CFLAGS=
SOX_LDFLAGS=
SOX_LIBs=

LIBAV_CFLAGS=
LIBAV_LDFLAGS=
LIBAV_LIBS=

CFLAGS= $(LUA_CFLAGS) $(GM_CFLAGS) $(CAIRO_CFLAGS)
LDFLAGS= $(LUA_LDFLAGS) $(GM_LDFLAGS) $(CAIRO_LDFLAGS)
LIBS= $(LUA_LIBS) $(GM_LIBS) $(CAIRO_LIBS)

OBJS= \
	cJSON.o \
	commands.o \
	main.o \
	raster.o \
	script.o \
	vector.o 

OUTBIN= avenida
LUA_RASTER_MODULE= avnscript-raster.so
LUA_VECTOR_MODULE= avnscript-vector.so
LUA_MODULES= $(LUA_RASTER_MODULE) $(LUA_VECTOR_MODULE)

##########

all: $(OUTBIN) $(LUA_MODULES)

# XXX should be split into two steps, like normal...
$(LUA_RASTER_MODULE): cJSON.o commands.o raster.o avnscript-raster.c
	$(CC) $(CFLAGS) -shared -o $(.TARGET) $(.ALLSRC) $(LDFLAGS) $(LIBS)

# XXX should be split into two steps, like normal...
$(LUA_VECTOR_MODULE): cJSON.o commands.o vector.o avnscript-vector.c
	$(CC) $(CFLAGS) -shared -o $(.TARGET) $(.ALLSRC) $(LDFLAGS) $(LIBS)

$(OUTBIN): $(OBJS)
	$(CC) -o $(.TARGET) $(.ALLSRC) $(LDFLAGS) $(LIBS)

.c.o:
	$(CC) -c $(CFLAGS) -o $(.TARGET) $(.ALLSRC)

clean:
	rm -f $(OUTBIN) $(LUA_MODULES) *.o *.so *.core *.png

tests: raster-tests vector-tests

raster-tests:
	$(LUA) ../example/hello-raster.lua ../example/test.png

vector-tests:
	$(LUA) ../example/hello-vector.lua
