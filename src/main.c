/*
 * vim: noet
 *
 * Avenida
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "avenida.h"
#include "script.h"

static void usage(void);
static void version(void);

int
main(int argc, char *argv[])
{
	int ch;
	int rv = EXIT_SUCCESS;
	char infile_path[PATH_MAX];
	avnscript *avn = NULL;

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

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		usage();
		return EXIT_FAILURE;
	}

	snprintf(infile_path, PATH_MAX, "%s", argv[0]);

	if ((avn = avnscript_new(infile_path)) == NULL) {
		warnx("couldn't create struct avnscript");
		rv = EXIT_FAILURE;
		goto cleanup;
	}

	avnscript_setup(avn);

	if (!avnscript_execute(avn)) {
		rv = EXIT_FAILURE;
		goto cleanup;
	}

cleanup:
	avnscript_free(avn);
	return rv;
}


static void
usage(void)
{
	warnx("usage: %s [-h] [-v] script", getprogname());
}


static void
version(void)
{
	printf("%s %s\n", getprogname(), AVENIDA_VERSION);
}
