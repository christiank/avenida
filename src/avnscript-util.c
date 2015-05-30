/*
 * vim: noet
 *
 * avnscript-util.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#include <lua.h>
#include <lauxlib.h>

#include "errors.h"
#include "avnscript-util.h"

static int avenida_cd(lua_State *);
static int avenida_pwd(lua_State *);

static int
avenida_cd(lua_State *L)
{
	char *path;

	path = (char *)luaL_checkstring(L, 1);
	if (chdir(path) == -1)
		luaL_error(L, "couldn't change directory");

	return 0;
}


static int
avenida_pwd(lua_State *L)
{
	char *s;

	if ((s = getwd(NULL)) != NULL) {
		lua_pushstring(L, s);
		return 1;
	} else {
		return DEFAULT_ERROR;
	}
}


int
luaopen_util(lua_State *L)
{
	luaL_Reg funcs[] = {
		{"cd", avenida_cd},
		{"pwd", avenida_pwd},
		{NULL, NULL}
	};

	luaL_newlib(L, funcs);
	luaL_newmetatable(L, "util");
	return 2;
}
