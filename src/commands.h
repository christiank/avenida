/*
 * vim: noet
 *
 * commands.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#ifndef AVENIDA_COMMANDS_H
#define AVENIDA_COMMANDS_H

#define AVENIDA_CMD_MAX_ARGS 16

#include <stdarg.h>

#include "cJSON.h"

enum avncmdname {
	/* Audio commands */
	/* ... */

	/* Raster commands */
	RASTER_BORDER,
	RASTER_BRIGHTNESS,
	RASTER_CHARCOAL,
	RASTER_CROP,
	RASTER_DESPECKLE,
	RASTER_EMBOSS,
	RASTER_EQUALIZE,
	RASTER_GAMMA,
	RASTER_GAUSSIANBLUR,
	RASTER_HORIZONTALFLIP,
	RASTER_HUE,
	RASTER_IMPLODE,
	RASTER_LEVELS,
	RASTER_MOTIONBLUR,
	RASTER_NEGATE,
	RASTER_NEGATEGRAYS,
	RASTER_NORMALIZE,
	RASTER_OILPAINT,
	RASTER_RADIALBLUR,
	RASTER_RESIZE,
	RASTER_ROLL,
	RASTER_ROTATE,
	RASTER_SATURATION,
	RASTER_SCALE,
	RASTER_SHARPEN,
	RASTER_SWIRL,
	RASTER_TINT,
	RASTER_VERTICALFLIP,
	RASTER_WAVE,

	/* Vector commands */
	VECTOR_CLOSEPATH,
	VECTOR_LINETO,
	VECTOR_MOVETO,
	VECTOR_OPENPATH,
	VECTOR_RELLINETO,
	VECTOR_RELMOVETO,
	VECTOR_SETCAP,
	VECTOR_SETCOLOR,
	VECTOR_SETWIDTH,
	VECTOR_STROKE,

	/* Video commands */
	/* ... */
};

enum avncmdargtype {
	AVN_UINT,
	AVN_INT,
	AVN_DOUBLE,
	AVN_STRING,
};

/*
 * The avncmdarg structure is a tagged union which represents one argument
 * to an Avenida operation.
 *
 * XXX anticipate a "smpte" argtype when durative media comes along
 */
struct avncmdarg {
	enum avncmdargtype type;
	union {
		unsigned int arg_uint;
		int arg_int;
		double arg_double;
		char *arg_str;
	};
};

/*
 * An Avenida operation consists of zero or more arguments.
 */
struct avnop {
	enum avncmdname name;
	unsigned int nargs;
	struct avncmdarg *args[AVENIDA_CMD_MAX_ARGS];
};

struct avncmdarg *avncmdarg_new(void);
struct avncmdarg *avncmdarg_new_uint(const unsigned int);
struct avncmdarg *avncmdarg_new_int(const int);
struct avncmdarg *avncmdarg_new_double(const double);
struct avncmdarg *avncmdarg_new_str(const char *);
void avncmdarg_free(struct avncmdarg *);

char *stravncmdname(const enum avncmdname cmdname);
struct avnop *avnop_new(const enum avncmdname);
void avnop_add_arg(struct avnop *, const enum avncmdargtype, ...);
void avnop_free(struct avnop *);
cJSON *avnop_to_json(const struct avnop *);

#endif /* AVENIDA_COMMANDS_H */
