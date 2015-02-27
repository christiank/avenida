/*
 * vim: noet
 *
 * avnscript-raster.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <stdbool.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "raster.h"

#define AVNRASTER_ARG1 ((avnraster*)luaL_checkudata(L, 1, "avnraster"))

static int avenida_border(lua_State *);
static int avenida_brightness(lua_State *);
static int avenida_charcoal(lua_State *);
static int avenida_crop(lua_State *);
static int avenida_despeckle(lua_State *);
static int avenida_emboss(lua_State *);
static int avenida_equalize(lua_State *);
static int avenida_gamma(lua_State *);
static int avenida_gaussianblur(lua_State *);
static int avenida_horizontalflip(lua_State *);
static int avenida_hue(lua_State *);
static int avenida_info(lua_State *);
static int avenida_levels(lua_State *);
static int avenida_motionblur(lua_State *);
static int avenida_negate(lua_State *);
static int avenida_negategrays(lua_State *);
static int avenida_normalize(lua_State *);
static int avenida_oilpaint(lua_State *);
static int avenida_open(lua_State *);
static int avenida_radialblur(lua_State *);
static int avenida_render(lua_State *);
static int avenida_resize(lua_State *);
static int avenida_roll(lua_State *);
static int avenida_rotate(lua_State *);
static int avenida_saturation(lua_State *);
static int avenida_scale(lua_State *);
static int avenida_sharpen(lua_State *);
static int avenida_swirl(lua_State *);
static int avenida_tint(lua_State *);
static int avenida_verticalflip(lua_State *);
static int avenida_wave(lua_State *);
static int avenida_write(lua_State *);

static int avenida_serialize(lua_State *);

int luaopen_raster(lua_State *L);

/* */

/*
 * bool = avenida.border(avnraster, width, height, color)
 */
static int
avenida_border(lua_State *L)
{
	avnraster *avn;
	char *color;
	size_t width, height;

	avn = AVNRASTER_ARG1;
	width = (size_t)luaL_checkinteger(L, 2);
	height = (size_t)luaL_checkinteger(L, 3);
	color = (char*)luaL_checkstring(L, 4);
	lua_pop(L, 4);

	lua_pushboolean(L, avnraster_border(avn, width, height, color));
	return 1;
}


/*
 * bool = avenida.brightness(avnraster, value)
 */
static int
avenida_brightness(lua_State *L)
{
	avnraster *avn;
	double value;

	avn = AVNRASTER_ARG1;
	value = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_brightness(avn, value));
	return 1;
}


static int
avenida_charcoal(lua_State *L)
{
	avnraster *avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_charcoal(avn, amt));
	return 1;
}


/*
 * bool = avenida.crop(avnraster, x, y, width, height)
 */
static int
avenida_crop(lua_State *L)
{
	avnraster *avn;
	unsigned int x, y;
	size_t width, height;

	avn = AVNRASTER_ARG1;
	x = (unsigned int)luaL_checkinteger(L, 2);
	y = (unsigned int)luaL_checkinteger(L, 3);
	width = (size_t)luaL_checkinteger(L, 4);
	height = (size_t)luaL_checkinteger(L, 5);
	lua_pop(L, 5);

	lua_pushboolean(L, avnraster_crop(avn, x, y, width, height));
	return 1;
}


/*
 * bool = avenida.despeckle(avnraster)
 */
static int
avenida_despeckle(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_despeckle(avn));
	return 1;
}


static int
avenida_emboss(lua_State *L)
{
	avnraster *avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_emboss(avn, amt));
	return 1;
}


/*
 * bool = avenida.equalize(avnraster)
 */
static int
avenida_equalize(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_equalize(avn));
	return 1;
}


/*
 * bool = avenida.gamma(avnraster, gamma)
 */
static int
avenida_gamma(lua_State *L)
{
	avnraster *avn;
	double gamma;

	avn = AVNRASTER_ARG1;
	gamma = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_gamma(avn, gamma));
	return 1;
}


/*
 * bool avenida.gaussianblur(avnraster, amount)
 */
static int
avenida_gaussianblur(lua_State *L)
{
	avnraster *avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_gaussianblur(avn, amt));
	return 1;
}


/*
 * bool = avenida.horizontalflip(avnraster)
 */
static int
avenida_horizontalflip(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_horizontalflip(avn));
	return 1;
}


/*
 * bool = avenida.hue(avnraster, value)
 */
static int
avenida_hue(lua_State *L)
{
	avnraster *avn;
	double value;

	avn = AVNRASTER_ARG1;
	value = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_hue(avn, value));
	return 1;
}


/*
 * table = avenida.info(avnraster)
 */
static int
avenida_info(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_createtable(L, 0, 4);
	lua_pushinteger(L, avn->info.width);
	lua_setfield(L, -2, "width");
	lua_pushinteger(L, avn->info.height);
	lua_setfield(L, -2, "height");
	lua_pushstring(L, avn->info.codec);
	lua_setfield(L, -2, "codec");
	lua_pushstring(L, avn->info.path);
	lua_setfield(L, -2, "path");
	return 1;
}


static int
avenida_levels(lua_State *L)
{
	return 0;
}


static int
avenida_motionblur(lua_State *L)
{
	avnraster *avn;
	double amt, angle;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	angle = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnraster_motionblur(avn, amt, angle));
	return 1;
}


/*
 * bool = avenida.negate(avnraster)
 */
static int
avenida_negate(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_negate(avn));
	return 1;
}


/*
 * bool = avenida.negategrays(avnraster)
 */
static int
avenida_negategrays(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_negategrays(avn));
	return 1;
}


/*
 * bool = avenida.normalize(avnraster)
 */
static int
avenida_normalize(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_normalize(avn));
	return 1;
}


/*
 * bool = avenida.oilpaint(avnraster, radius)
 */
static int
avenida_oilpaint(lua_State *L)
{
	avnraster *avn;
	double radius;

	avn = AVNRASTER_ARG1;
	radius = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_oilpaint(avn, radius));
	return 1;
}


/*
 * avnraster? = avenida.open(path)
 *
 * Returns an "avnraster" userdata if successful, or nil if there's an issue.
 */
static int
avenida_open(lua_State *L)
{
	avnraster *avn, *tmp;
	char *path;

	path = (char*)luaL_checkstring(L, 1);
	lua_pop(L, 1);

	avn = (avnraster*)lua_newuserdata(L, sizeof(avnraster));
	tmp = avnraster_new(path);

	if (avnraster_open(tmp)) {
		memmove(avn, tmp, sizeof(avnraster));
		luaL_setmetatable(L, "avnraster");
	} else {
		free(avn);
		avnraster_free(tmp);
		lua_pushnil(L);
	}

	return 1;
}


/*
 * bool = avenida.radialblur(avnraster, angle)
 */
static int
avenida_radialblur(lua_State *L)
{
	avnraster *avn;
	double angle;

	avn = AVNRASTER_ARG1;
	angle = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_radialblur(avn, angle));
	return 1;
}


/*
 * avenida.render(avnraster)
 */
static int
avenida_render(lua_State *L)
{
	avnraster *avn;
	bool verbose;

	avn = AVNRASTER_ARG1;
	verbose = lua_toboolean(L, 2);
	lua_pop(L, 2);

	avnraster_render(avn, verbose);
	return 0;
}


/*
 * bool = avenida.resize(avnraster, width, height)
 */
static int
avenida_resize(lua_State *L)
{
	avnraster *avn;
	size_t width, height;

	avn = AVNRASTER_ARG1;
	width = (size_t)luaL_checkinteger(L, 2);
	height = (size_t)luaL_checkinteger(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnraster_resize(avn, width, height));
	return 1;
}


/*
 * bool = avenida.roll(avnraster, x_amt, y_amt)
 */
static int
avenida_roll(lua_State *L)
{
	avnraster *avn;
	int x_amt, y_amt;

	avn = AVNRASTER_ARG1;
	x_amt = luaL_checkinteger(L, 2);
	y_amt = luaL_checkinteger(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnraster_roll(avn, x_amt, y_amt));
	return 1;
}


/*
 * bool = avenida.rotate(avnraster, angle, bgcolor?)
 */
static int
avenida_rotate(lua_State *L)
{
	avnraster *avn;
	double angle;
	char *bgcolor = NULL;

	avn = AVNRASTER_ARG1;
	angle = luaL_checknumber(L, 2);

	if (lua_gettop(L) == 2) {
		bgcolor = "#000000";
		lua_pop(L, 2);
	} else {
		bgcolor = (char*)luaL_checkstring(L, 3);
		lua_pop(L, 3);
	}

	lua_pushboolean(L, avnraster_rotate(avn, angle, bgcolor));
	return 1;
}


/*
 * bool = avenida.saturation(avnraster, value)
 */
static int
avenida_saturation(lua_State *L)
{
	avnraster *avn;
	double value;

	avn = AVNRASTER_ARG1;
	value = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_saturation(avn, value));
	return 1;
}


/*
 * bool = avenida.scale(avnraster, factor)
 */
static int
avenida_scale(lua_State *L)
{
	avnraster *avn;
	double factor;

	avn = AVNRASTER_ARG1;
	factor = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_scale(avn, factor));
	return 1;
}


/*
 * bool = avenida.sharpen(avnraster, amt)
 */
static int
avenida_sharpen(lua_State *L)
{
	avnraster *avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2); 
	lua_pop(L, 2); 

	lua_pushboolean(L, avnraster_sharpen(avn, amt));
	return 1;
}


/*
 * bool = avenida.swirl(avnraster, degrees)
 */
static int
avenida_swirl(lua_State *L)
{
	avnraster *avn;
	double degrees;

	avn = AVNRASTER_ARG1;
	degrees = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_swirl(avn, degrees));
	return 1;
}


/*
 * bool = avenida.tint(avnraster, color, opacity)
 */
static int
avenida_tint(lua_State *L)
{
	avnraster *avn;
	char *color = NULL;
	double opacity;

	avn = AVNRASTER_ARG1;
	color = (char*)luaL_checkstring(L, 2);
	opacity = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnraster_tint(avn, color, opacity));
	return 1;
}


/*
 * bool = avenida.verticalflip(avnraster)
 */
static int
avenida_verticalflip(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushboolean(L, avnraster_verticalflip(avn));
	return 1;
}


/*
 * bool = avenida.wave(avnraster, amplitude, wavelength)
 */
static int
avenida_wave(lua_State *L)
{
	avnraster *avn;
	double amplitude, wavelength;

	avn = AVNRASTER_ARG1;
	amplitude = luaL_checknumber(L, 2);
	wavelength = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	lua_pushboolean(L, avnraster_wave(avn, amplitude, wavelength));
	return 1;
}


/*
 * bool = avenida.write(avnraster, path)
 */
static int
avenida_write(lua_State *L)
{
	avnraster *avn;
	char *path;

	avn = AVNRASTER_ARG1;
	path = (char*)luaL_checkstring(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_write(avn, path));
	return 1;
}


/*
 * str = avenida.serialize(avnraster)
 */
static int
avenida_serialize(lua_State *L)
{
	avnraster *avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushstring(L, avnraster_history_json(avn));
	return 1;
}

/* */

int
luaopen_raster(lua_State *L)
{
	luaL_Reg funcs[] = {
		{"border", avenida_border},
		{"brightness", avenida_brightness},
		{"charcoal", avenida_charcoal},
		{"crop", avenida_crop},
		{"despeckle", avenida_despeckle},
		{"emboss", avenida_emboss},
		{"equalize", avenida_equalize},
		{"gamma", avenida_gamma},
		{"gaussianblur", avenida_gaussianblur},
		{"horizontalflip", avenida_horizontalflip},
		{"hue", avenida_hue},
		{"info", avenida_info},
		{"levels", avenida_levels},
		{"motionblur", avenida_motionblur},
		{"negate", avenida_negate},
		{"negategrays", avenida_negategrays},
		{"normalize", avenida_normalize},
		{"oilpaint", avenida_oilpaint},
		{"open", avenida_open},
		{"radialblur", avenida_radialblur},
		{"render", avenida_render},
		{"resize", avenida_resize},
		{"roll", avenida_roll},
		{"rotate", avenida_rotate},
		{"saturation", avenida_saturation},
		{"scale", avenida_scale},
		{"sharpen", avenida_sharpen},
		{"swirl", avenida_swirl},
		{"tint", avenida_tint},
		{"verticalflip", avenida_verticalflip},
		{"wave", avenida_wave},
		{"write", avenida_write},
		{"serialize", avenida_serialize},
		{NULL, NULL},
	};

	luaL_newlib(L, funcs);
	luaL_newmetatable(L, "avnraster");
	return 2;
}
