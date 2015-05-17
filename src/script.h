/*
 * vim: noet
 *
 * script.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#ifndef AVENIDA_SCRIPT_H
#define AVENIDA_SCRIPT_H

#include <limits.h>
#include <stdbool.h>

#include <lua.h>

/*
 * The avnscript structure represents a file with Avenida commands and a
 * means to interpret it.
 */
struct avnscript {
	lua_State *L;
	char path[PATH_MAX];
};
typedef struct avnscript avnscript;

avnscript *avnscript_new(const char *path);
void avnscript_free(avnscript *avn);
void avnscript_setup(avnscript *avn);
bool avnscript_execute(avnscript *avn);

#endif /* AVENIDA_SCRIPT_H */
