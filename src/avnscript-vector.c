/*
 * vim: noet
 *
 * avnscript-vector.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <stdbool.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "vector.h"

#define AVNVECTOR_ARG1 ((avnvector*)luaL_checkudata(L, 1, "avnvector"))

static int avenida_lineto(lua_State *);
static int avenida_moveto(lua_State *);
static int avenida_new(lua_State *);
static int avenida_setcap(lua_State *);
static int avenida_setcolor(lua_State *);
static int avenida_setwidth(lua_State *);
static int avenida_stroke(lua_State *);
static int avenida_write(lua_State *);

int luaopen_vector(lua_State *L);

/* */

/*
 * bool = avenida.lineto(avnvector, x, y)
 */
static int
avenida_lineto(lua_State *L)
{
	avnvector *avn;
	int x, y;

	avn = AVNVECTOR_ARG1;
	x = luaL_checknumber(L, 2);
	y = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnvector_lineto(avn, x, y));
	return 1;
}


/*
 * bool = avenida.moveto(avnvector, x, y)
 */
static int
avenida_moveto(lua_State *L)
{
	avnvector *avn;
	int x, y;

	avn = AVNVECTOR_ARG1;
	x = luaL_checknumber(L, 2);
	y = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnvector_moveto(avn, x, y));
	return 1;
}


/*
 * avnvector = avenida.new(width, height)
 */
static int
avenida_new(lua_State *L)
{
	avnvector *avn, *tmp;
	size_t width, height;

	width = (size_t)luaL_checkinteger(L, 1);
	height = (size_t)luaL_checkinteger(L, 2);
	lua_pop(L, 2);

	avn = (avnvector*)lua_newuserdata(L, sizeof(avnvector));
	tmp = avnvector_new(width, height);

	memmove(avn, tmp, sizeof(avnvector));
	luaL_setmetatable(L, "avnvector");

	return 1;
}


static int
avenida_setcap(lua_State *L)
{
	return 0;
}


static int
avenida_setcolor(lua_State *L)
{
	return 0;
}


static int
avenida_setwidth(lua_State *L)
{
	return 0;
}


static int
avenida_stroke(lua_State *L)
{
	return 0;
}


/*
 * bool = avenida.write(avnvector, path)
 */
static int
avenida_write(lua_State *L)
{
	avnvector *avn;
	char *path = NULL;

	avn = AVNVECTOR_ARG1;
	path = (char *)luaL_checkstring(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnvector_write(avn, path));
	return 1;
}

/* */

int
luaopen_vector(lua_State *L)
{
	luaL_Reg funcs[] = {
		{"lineto", avenida_lineto},
		{"moveto", avenida_moveto},
		{"new", avenida_new},
		{"setcap", avenida_setcap},
		{"setcolor", avenida_setcolor},
		{"setwidth", avenida_setwidth},
		{"stroke", avenida_stroke},
		{"write", avenida_write},
		{NULL, NULL},
	};

	luaL_newlib(L, funcs);
	luaL_newmetatable(L, "avnvector");
	return 2;
}
