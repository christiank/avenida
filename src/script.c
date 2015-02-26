/*
 * vim: noet
 *
 * script.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <limits.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "raster.h"
#include "script.h"

avnscript *
avnscript_new(const char *path)
{
	avnscript *avn = NULL;
	lua_State *L = NULL;

	if ((avn = malloc(sizeof(avnscript))) == NULL)
		goto cleanup;

	if ((L = luaL_newstate()) == NULL)
		goto cleanup;

	avn->L = L;
	snprintf(avn->path, PATH_MAX, "%s", path);
	return avn;

cleanup:
	if (avn != NULL) free(avn);
	if (L != NULL) lua_close(L);
	return NULL;
}


void
avnscript_free(avnscript *avn)
{
	if (avn != NULL) {
		lua_close(avn->L);
		free(avn);
	}
}
