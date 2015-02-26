/*
 * vim: noet
 *
 * Avenida
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "raster.h"
#include "script.h"

int
main(int argc, char *argv[])
{
	avnraster *r;
	avnscript *s;

	r = avnraster_new("ohoh");
	avnraster_free(r);

	s = avnscript_new("ojojo");
	avnscript_free(s);

	return EXIT_SUCCESS;
}
