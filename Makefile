# Avenida Makefile
# Christian Koch <cfkoch@sdf.lonestar.org>

.PHONY: all clean

MAKE= bmake

all:
	(cd src && $(MAKE) all)

clean:
	(cd src && $(MAKE) clean)
