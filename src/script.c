/*
 * vim: noet
 *
 * script.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <err.h>
#include <limits.h>
#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "script.h"
#include "avnscript-raster.h"
#include "avnscript-vector.h"

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


void
avnscript_setup(avnscript *avn)
{
	luaL_openlibs(avn->L);
	luaopen_raster(avn->L);
	lua_pop(avn->L, 1);
	lua_setglobal(avn->L, "raster");
	luaopen_vector(avn->L);
	lua_pop(avn->L, 1);
	lua_setglobal(avn->L, "vector");
}


/*
 * luaL_dofile() returns true on error, but avnscript_execute returns true
 * on success.
 */
bool
avnscript_execute(avnscript *avn)
{
	if (luaL_dofile(avn->L, avn->path)) {
		warnx("%s", luaL_tolstring(avn->L, -1, NULL));
		return false;
	} else {
		return true;
	}
}
