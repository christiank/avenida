/*
 * vim: noet
 *
 * Avenida
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void version(void);

int
main(int argc, char *argv[])
{
	int ch;

	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch (ch) {
		case 'v':
			version();
			break;
		case '?': /* FALLTHROUGH */
		default:
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}


static void
version(void)
{
	printf("v0.0.1\n");
}
