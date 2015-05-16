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

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "avenida.h"

static void usage(void);
static void version(void);

int
main(int argc, char *argv[])
{
	int ch;
	char infile_path[PATH_MAX];
	lua_State *L = NULL;

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

	if ((L = luaL_newstate()) == NULL) {
		warnx("couldn't create a new Lua state");
		goto cleanup;
	}

	luaL_openlibs(L);

	if (luaL_dofile(L, infile_path)) {
		warnx("%s", luaL_tolstring(L, -1, NULL));
		goto cleanup;
	}

cleanup:
	if (L != NULL)
		lua_close(L);

	return EXIT_SUCCESS;
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
