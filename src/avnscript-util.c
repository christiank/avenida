/*
 * vim: noet
 *
 * avnscript-util.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <lua.h>
#include <lauxlib.h>

#include "errors.h"
#include "avnscript-util.h"

static int avenida_cd(lua_State *);
static int avenida_files(lua_State *);
static int avenida_ls(lua_State *);
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


/*
 * Just like util.ls(), but it only lists regular files.
 */
static int
avenida_files(lua_State *L)
{
	DIR *dir;
	struct dirent *f;
	struct stat sb;
	int i = 0;

	lua_newtable(L);
	dir = opendir(getwd(NULL));
	rewinddir(dir);

	while ((f = readdir(dir)) != NULL) {
		stat(f->d_name, &sb);
		if (S_ISREG(sb.st_mode)) {
			i++;
			lua_pushinteger(L, i);
			lua_pushstring(L, f->d_name);
			lua_settable(L, -3);
		}
	}

	closedir(dir);
	return 1;
}


static int
avenida_ls(lua_State *L)
{
	DIR *dir;
	struct dirent *f;
	int i = 0;

	lua_newtable(L);

	dir = opendir(getwd(NULL));
	rewinddir(dir);

	while ((f = readdir(dir)) != NULL) {
		i++;
		lua_pushinteger(L, i);
		lua_pushstring(L, f->d_name);
		lua_settable(L, -3);
	}

	closedir(dir);

	return 1;
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
		{"files", avenida_files},
		{"ls", avenida_ls},
		{"pwd", avenida_pwd},
		{NULL, NULL}
	};

	luaL_newlib(L, funcs);
	luaL_newmetatable(L, "util");
	return 2;
}
