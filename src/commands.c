/*
 * vim: noet
 *
 * commands.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"

#include "commands.h"

struct avncmdarg *
avncmdarg_new(void)
{
	struct avncmdarg *cmdarg;
	cmdarg = malloc(sizeof(struct avncmdarg));
	return cmdarg;
}


struct avncmdarg *
avncmdarg_new_uint(const unsigned int arg)
{
	struct avncmdarg *cmdarg;

	if ((cmdarg = avncmdarg_new()) == NULL)
		return NULL;

	cmdarg->type = AVN_UINT;
	cmdarg->arg_uint = arg;

	return cmdarg;
}


struct avncmdarg *
avncmdarg_new_int(const int arg)
{
	struct avncmdarg *cmdarg;

	if ((cmdarg = avncmdarg_new()) == NULL)
		return NULL;

	cmdarg->type = AVN_INT;
	cmdarg->arg_int = arg;

	return cmdarg;
}


struct avncmdarg *
avncmdarg_new_double(const double arg)
{
	struct avncmdarg *cmdarg;

	if ((cmdarg = avncmdarg_new()) == NULL)
		return NULL;

	cmdarg->type = AVN_DOUBLE;
	cmdarg->arg_double = arg;

	return cmdarg;
}


struct avncmdarg *
avncmdarg_new_str(const char *arg)
{
	struct avncmdarg *cmdarg;

	if ((cmdarg = avncmdarg_new()) == NULL)
		return NULL;

	cmdarg->type = AVN_STRING;
	cmdarg->arg_str = (char *)arg;

	return cmdarg;
}


void
avncmdarg_free(struct avncmdarg *cmdarg)
{
	free(cmdarg);
}


char *
stravncmdname(const enum avncmdname cmdname)
{
	char *s;

	switch (cmdname) {
	case RASTER_BORDER: s = "border"; break;
	case RASTER_BRIGHTNESS: s = "brightness"; break;
	case RASTER_CHARCOAL: s = "charcoal"; break;
	case RASTER_CROP: s = "crop"; break;
	case RASTER_DESPECKLE: s = "despeckle"; break;
	case RASTER_EMBOSS: s = "emboss"; break;
	case RASTER_EQUALIZE: s = "equalize"; break;
	case RASTER_GAMMA: s = "gamma"; break;
	case RASTER_GAUSSIANBLUR: s = "gaussianblur"; break;
	case RASTER_HORIZONTALFLIP: s = "horizontalflip"; break;
	case RASTER_HUE: s = "hue"; break;
	case RASTER_INFO: s = "info"; break;
	case RASTER_LEVELS: s = "levels"; break;
	case RASTER_MOTIONBLUR: s = "motionblur"; break;
	case RASTER_NEGATE: s = "negate"; break;
	case RASTER_NEGATEGRAYS: s = "negategrays"; break;
	case RASTER_NORMALIZE: s = "normalize"; break;
	case RASTER_OILPAINT: s = "oilpaint"; break;
	case RASTER_RADIALBLUR: s = "radialblur"; break;
	case RASTER_RESIZE: s = "resize"; break;
	case RASTER_ROLL: s = "roll"; break;
	case RASTER_ROTATE: s = "rotate"; break;
	case RASTER_SATURATION: s = "saturation"; break;
	case RASTER_SCALE: s = "scale"; break;
	case RASTER_SHARPEN: s = "sharpen"; break;
	case RASTER_SWIRL: s = "swirl"; break;
	case RASTER_TINT: s = "tint"; break;
	case RASTER_VERTICALFLIP: s = "verticalflip"; break;
	case RASTER_WAVE: s = "wave"; break;
	case VECTOR_LINETO: s = "lineto"; break;
	case VECTOR_MOVETO: s = "moveto"; break;
	case VECTOR_SETCAP: s = "setcap"; break;
	case VECTOR_SETCOLOR: s = "setcolor"; break;
	case VECTOR_SETWIDTH: s = "setwidth"; break;
	case VECTOR_STROKE: s = "stroke"; break;
	default:
		s = NULL;
	}

	return s;
}


struct avnop *
avnop_new(const enum avncmdname name)
{
	struct avnop *op;

	if ((op = malloc(sizeof(struct avnop))) == NULL)
		return NULL;

	op->name = name;
	op->nargs = 0;

	return op;
}


void
avnop_add_arg(struct avnop *op, const enum avncmdargtype type, ...)
{
	va_list ap;
	struct avncmdarg *arg;

	va_start(ap, type);

	switch (type) {
	case AVN_UINT:
		arg = avncmdarg_new_uint(va_arg(ap, unsigned int));
		break;
	case AVN_INT:
		arg = avncmdarg_new_int(va_arg(ap, int));
		break;
	case AVN_DOUBLE:
		arg = avncmdarg_new_double(va_arg(ap, double));
		break;
	case AVN_STRING:
		arg = avncmdarg_new_str(va_arg(ap, char *));
		break;
	default:
		return; /* NOTREACHED */
	}

	if (arg == NULL)
		return;

	va_end(ap);

	op->args[op->nargs] = arg;
	(op->nargs)++;
}


void
avnop_free(struct avnop *op)
{
	int i;

	for (i = 0; i < op->nargs; i++)
		avncmdarg_free(op->args[i]);

	free(op);
}


/*
 * Serializes the given avnop to a cJSON* object. It needs to be eventually
 * freed with cJSON_Delete().
 *
 * Example:
 *
 *     {"name":"crop","args":[100,100,523,750]}
 *
 * XXX Be prepared for named arguments in a future version...
 */
cJSON *
avnop_to_json(const struct avnop *op)
{
	int i;
	cJSON *json;
	cJSON *args_ary;

	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "name", stravncmdname(op->name));

	args_ary = cJSON_CreateArray();

	for (i = 0; i < op->nargs; i++) {
		switch (op->args[i]->type) {
		case AVN_UINT: 
			cJSON_AddItemToArray(args_ary,
				cJSON_CreateNumber(op->args[i]->arg_uint));
			break;
		case AVN_INT:
			cJSON_AddItemToArray(args_ary,
				cJSON_CreateNumber(op->args[i]->arg_int));
			break;
		case AVN_DOUBLE:
			cJSON_AddItemToArray(args_ary,
				cJSON_CreateNumber(op->args[i]->arg_double));
			break;
		case AVN_STRING:
			cJSON_AddItemToArray(args_ary,
				cJSON_CreateString(op->args[i]->arg_str));
			break;
		}
	}

	cJSON_AddItemToObject(json, "args", args_ary);

	return json;
}
