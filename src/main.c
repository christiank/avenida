/*
 * vim: noet
 *
 * Avenida
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "avenida.h"

static void usage(void);
static void version(void);

int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "hv")) != -1) {
		switch (ch) {
		case 'h':
			usage();
			return EXIT_SUCCESS;
			break;
		case 'v':
			version();
			break;
		case '?': /* FALLTHROUGH */
		default:
			usage();
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}


static void
usage(void)
{
	warnx("usage: %s [-h] [-v]", getprogname());
}


static void
version(void)
{
	printf("%s %s\n", getprogname(), AVENIDA_VERSION);
}
