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
#include <string.h>
#include <unistd.h>

#include "lua.h"
#include "lauxlib.h"

#include "avenida.h"
#include "linenoise.h"
#include "script.h"

static void usage(void);
static void version(void);
static int repl(void);
static void repl_completion(const char *buf, linenoiseCompletions *lc);
static void builtin_quit(void);
static void builtin_version(void);

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
			return EXIT_SUCCESS;
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
		return repl();
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


static int
repl(void)
{
	char *line;
	avnscript *avn;
	int i, top;

	avn = avnscript_new("/dev/null");
	avnscript_setup(avn);
	linenoiseHistoryLoad(AVENIDA_HISTORYFILE);
	linenoiseHistorySetMaxLen(AVENIDA_HISTORYFILE_LEN);
	linenoiseSetCompletionCallback(repl_completion);

	for (;;) {
		line = linenoise(AVENIDA_PROMPT);

		if (line == NULL)
			break;

		if (!strcasecmp(line, "/quit")) {
			builtin_quit();
			break;
		} else if (!strcasecmp(line, "/version")) {
			builtin_version();
		} else {
			luaL_dostring(avn->L, line);
		}

		linenoiseHistoryAdd(line);
		linenoiseHistorySave(AVENIDA_HISTORYFILE);

		top = lua_gettop(avn->L);

		for (i = 0; i < top; i++)
			printf("%s\n", lua_tostring(avn->L, i));

		lua_pop(avn->L, top);
		free(line);
	}

	avnscript_free(avn);
	return EXIT_SUCCESS;
}


static void
repl_completion(const char *buf, linenoiseCompletions *lc)
{
	linenoiseAddCompletion(lc, "/quit");
	linenoiseAddCompletion(lc, "/version");
}


static void
builtin_quit(void)
{
	/* NOTHING */
}


static void
builtin_version(void)
{
	version();
}
