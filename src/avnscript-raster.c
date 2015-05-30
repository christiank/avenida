/*
 * vim: noet
 *
 * avnscript-raster.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>

#include "errors.h"
#include "raster.h"

#define AVNRASTER_ARG1 ((avnraster**)luaL_checkudata(L, 1, "avnraster"))

static int avenida_border(lua_State *);
static int avenida_brightness(lua_State *);
static int avenida_charcoal(lua_State *);
static int avenida_crop(lua_State *);
static int avenida_despeckle(lua_State *);
static int avenida_edge(lua_State *);
static int avenida_emboss(lua_State *);
static int avenida_equalize(lua_State *);
static int avenida_gamma(lua_State *);
static int avenida_gaussianblur(lua_State *);
static int avenida_horizontalflip(lua_State *);
static int avenida_hue(lua_State *);
static int avenida_implode(lua_State *);
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
 * avenida.border(avnraster, width, height, color)
 */
static int
avenida_border(lua_State *L)
{
	avnraster **avn;
	char *color;
	size_t width, height;

	avn = AVNRASTER_ARG1;
	width = (size_t)luaL_checkinteger(L, 2);
	height = (size_t)luaL_checkinteger(L, 3);
	color = (char*)luaL_checkstring(L, 4);
	lua_pop(L, 4);

	if (!avnraster_border(*avn, width, height, color))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.brightness(avnraster, value)
 *
 * Note: GraphicsMagick's documentation is a complete LIE. The range for the
 * args of MagickModulateImage() is (0..200), not (-100..100).
 */
static int
avenida_brightness(lua_State *L)
{
	avnraster **avn;
	double value;

	avn = AVNRASTER_ARG1;
	value = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if ((value < -100.0) || (value > 100.0))
		return RANGE_ERROR(value);

	if (!avnraster_brightness(*avn, value))
		return DEFAULT_ERROR;

	return 0;
}


static int
avenida_charcoal(lua_State *L)
{
	avnraster **avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (amt < 0.0)
		return RANGE_ERROR(amt);

	if (!avnraster_charcoal(*avn, amt))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.crop(avnraster, x, y, width, height)
 */
static int
avenida_crop(lua_State *L)
{
	avnraster **avn;
	unsigned int x, y;
	size_t width, height;

	avn = AVNRASTER_ARG1;
	x = (unsigned int)luaL_checkinteger(L, 2);
	y = (unsigned int)luaL_checkinteger(L, 3);
	width = (size_t)luaL_checkinteger(L, 4);
	height = (size_t)luaL_checkinteger(L, 5);
	lua_pop(L, 5);

	if (width > (*avn)->info.width)
		return luaL_error(L, "requested width %d is greater than original width",
			width);
	if (height > (*avn)->info.height)
		return luaL_error(L, "requested height %d is greater than original height",
			height);

	if (!avnraster_crop(*avn, x, y, width, height))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.despeckle(avnraster)
 */
static int
avenida_despeckle(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_despeckle(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * raster.edge(img, amt?)
 *
 * If no 'amt' is given then default to zero, which GraphicsMagick
 * interprets on its own.
 */
static int
avenida_edge(lua_State *L)
{
	avnraster **avn;
	double amt;

	avn = AVNRASTER_ARG1;

	if (lua_gettop(L) >= 2) {
		amt = luaL_checknumber(L, 2);
		lua_pop(L, 2);
	} else {
		amt = 0.0;
		lua_pop(L, 1);
	}

	if (!avnraster_edge(*avn, amt))
		return DEFAULT_ERROR;

	return 0;
}


static int
avenida_emboss(lua_State *L)
{
	avnraster **avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (amt < 0.0)
		return RANGE_ERROR(amt);

	if (!avnraster_emboss(*avn, amt))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.equalize(avnraster)
 */
static int
avenida_equalize(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_equalize(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.gamma(avnraster, gamma)
 */
static int
avenida_gamma(lua_State *L)
{
	avnraster **avn;
	double gamma;

	avn = AVNRASTER_ARG1;
	gamma = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (gamma < 0.0)
		return RANGE_ERROR(gamma);

	if (!avnraster_gamma(*avn, gamma))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.gaussianblur(avnraster, amount)
 */
static int
avenida_gaussianblur(lua_State *L)
{
	avnraster **avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (amt < 0.0)
		return RANGE_ERROR(amt);

	if (avnraster_gaussianblur(*avn, amt))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.horizontalflip(avnraster)
 */
static int
avenida_horizontalflip(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_horizontalflip(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.hue(avnraster, value)
 *
 * See the note about MagickModulateImage() in the documentation for
 * avnraster_brightness().
 */
static int
avenida_hue(lua_State *L)
{
	avnraster **avn;
	double value;

	avn = AVNRASTER_ARG1;
	value = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if ((value < -100.0) || (value > 100.0))
		return RANGE_ERROR(value);

	if (!avnraster_hue(*avn, value))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * raster.implode(img, radius)
 */
static int
avenida_implode(lua_State *L)
{
	avnraster **avn;
	double radius;

	avn = AVNRASTER_ARG1;
	radius = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (!avnraster_implode(*avn, radius))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * table = avenida.info(avnraster)
 */
static int
avenida_info(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_createtable(L, 0, 5);
	lua_pushinteger(L, (*avn)->info.width);
	lua_setfield(L, -2, "width");
	lua_pushinteger(L, (*avn)->info.height);
	lua_setfield(L, -2, "height");
	lua_pushstring(L, (*avn)->info.codec);
	lua_setfield(L, -2, "codec");
	lua_pushstring(L, (*avn)->info.path);
	lua_setfield(L, -2, "path");
	lua_pushinteger(L, avnraster_info_ncolors(*avn));
	lua_setfield(L, -2, "ncolors");

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
	avnraster **avn;
	double amt, angle;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2);
	angle = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	if (amt < 0.0)
		return RANGE_ERROR(amt);

	if (!avnraster_motionblur(*avn, amt, angle))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.negate(avnraster)
 */
static int
avenida_negate(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_negate(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.negategrays(avnraster)
 */
static int
avenida_negategrays(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_negategrays(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.normalize(avnraster)
 */
static int
avenida_normalize(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_normalize(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.oilpaint(avnraster, radius)
 *
 * Apparently, 0.0 is an acceptable radius.
 */
static int
avenida_oilpaint(lua_State *L)
{
	avnraster **avn;
	double radius;

	avn = AVNRASTER_ARG1;
	radius = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (radius < 0.0)
		return RANGE_ERROR(radius);

	if (!avnraster_oilpaint(*avn, radius))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avnraster? = avenida.open(path)
 *
 * Returns an "avnraster" userdata if successful, or nil if there's an issue.
 */
static int
avenida_open(lua_State *L)
{
	avnraster **avn;
	char *path;

	path = (char*)luaL_checkstring(L, 1);
	lua_pop(L, 1);

	avn = (avnraster**)lua_newuserdata(L, sizeof(avnraster *));
	*avn = avnraster_new(path);

	if (avnraster_open(*avn)) {
		luaL_setmetatable(L, "avnraster");
	} else {
		avnraster_free(*avn);
		luaL_error(L, "couldn't open raster \"%s\"", path);
		return 0;
	}

	return 1;
}


/*
 * avenida.radialblur(avnraster, angle)
 */
static int
avenida_radialblur(lua_State *L)
{
	avnraster **avn;
	double angle;

	avn = AVNRASTER_ARG1;
	angle = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (!avnraster_radialblur(*avn, angle))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.render(avnraster)
 */
static int
avenida_render(lua_State *L)
{
	avnraster **avn;
	bool verbose;

	avn = AVNRASTER_ARG1;
	verbose = lua_toboolean(L, 2);
	lua_pop(L, 2);

	avnraster_render(*avn, verbose);
	return 0;
}


/*
 * avenida.resize(avnraster, width, height)
 */
static int
avenida_resize(lua_State *L)
{
	avnraster **avn;
	size_t width, height;

	avn = AVNRASTER_ARG1;
	width = (size_t)luaL_checkinteger(L, 2);
	height = (size_t)luaL_checkinteger(L, 3);
	lua_pop(L, 3);

	if (!avnraster_resize(*avn, width, height))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.roll(avnraster, x_amt, y_amt)
 */
static int
avenida_roll(lua_State *L)
{
	avnraster **avn;
	int x_amt, y_amt;

	avn = AVNRASTER_ARG1;
	x_amt = luaL_checkinteger(L, 2);
	y_amt = luaL_checkinteger(L, 3);
	lua_pop(L, 3);

	if (!avnraster_roll(*avn, x_amt, y_amt))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.rotate(avnraster, angle, bgcolor?)
 */
static int
avenida_rotate(lua_State *L)
{
	avnraster **avn;
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

	if (!avnraster_rotate(*avn, angle, bgcolor))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.saturation(avnraster, value)
 *
 * See the note about MagickModulateImage() in the documentation for
 * avenida_brightness().
 */
static int
avenida_saturation(lua_State *L)
{
	avnraster **avn;
	double value;

	avn = AVNRASTER_ARG1;
	value = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if ((value < -100.0) || (value > 100.0))
		return RANGE_ERROR(value);

	if (!avnraster_saturation(*avn, value))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.scale(avnraster, factor)
 */
static int
avenida_scale(lua_State *L)
{
	avnraster **avn;
	double factor;

	avn = AVNRASTER_ARG1;
	factor = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (factor <= 0.0)
		return RANGE_ERROR(factor);

	if (!avnraster_scale(*avn, factor))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.sharpen(avnraster, amt)
 */
static int
avenida_sharpen(lua_State *L)
{
	avnraster **avn;
	double amt;

	avn = AVNRASTER_ARG1;
	amt = luaL_checknumber(L, 2); 
	lua_pop(L, 2); 

	if (!avnraster_sharpen(*avn, amt))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.swirl(avnraster, degrees)
 */
static int
avenida_swirl(lua_State *L)
{
	avnraster **avn;
	double degrees;

	avn = AVNRASTER_ARG1;
	degrees = luaL_checknumber(L, 2);
	lua_pop(L, 2);

	if (!avnraster_swirl(*avn, degrees))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.tint(avnraster, color, opacity)
 */
static int
avenida_tint(lua_State *L)
{
	avnraster **avn;
	char *color = NULL;
	double opacity;

	avn = AVNRASTER_ARG1;
	color = (char*)luaL_checkstring(L, 2);
	opacity = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	if (!avnraster_tint(*avn, color, opacity))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.verticalflip(avnraster)
 */
static int
avenida_verticalflip(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	if (!avnraster_verticalflip(*avn))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * avenida.wave(avnraster, amplitude, wavelength)
 */
static int
avenida_wave(lua_State *L)
{
	avnraster **avn;
	double amplitude, wavelength;

	avn = AVNRASTER_ARG1;
	amplitude = luaL_checknumber(L, 2);
	wavelength = luaL_checknumber(L, 3);
	lua_pop(L, 3);

	if (amplitude < 0.0)
		return RANGE_ERROR(amplitude);
	if (wavelength < 0.0)
		return RANGE_ERROR(wavelength);

	if (!avnraster_wave(*avn, amplitude, wavelength))
		return DEFAULT_ERROR;

	return 0;
}


/*
 * bool = avenida.write(avnraster, path)
 */
static int
avenida_write(lua_State *L)
{
	avnraster **avn;
	char *path;

	avn = AVNRASTER_ARG1;
	path = (char*)luaL_checkstring(L, 2);
	lua_pop(L, 2);

	lua_pushboolean(L, avnraster_write(*avn, path));
	return 1;
}


/*
 * str = avenida.serialize(avnraster)
 */
static int
avenida_serialize(lua_State *L)
{
	avnraster **avn;

	avn = AVNRASTER_ARG1;
	lua_pop(L, 1);

	lua_pushstring(L, avnraster_history_json(*avn));
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
		{"edge", avenida_edge},
		{"emboss", avenida_emboss},
		{"equalize", avenida_equalize},
		{"gamma", avenida_gamma},
		{"gaussianblur", avenida_gaussianblur},
		{"horizontalflip", avenida_horizontalflip},
		{"hue", avenida_hue},
		{"implode", avenida_implode},
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
