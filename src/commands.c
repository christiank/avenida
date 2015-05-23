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
#include <string.h>

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

#define cmp strcasecmp
const enum avncmdname
avncmdnamestr(const char *s)
{
	if (!cmp(s, "border")) return RASTER_BORDER;
	else if (!cmp(s, "brightness")) return RASTER_BRIGHTNESS;
	else if (!cmp(s, "charcoal")) return RASTER_CHARCOAL;
	else if (!cmp(s, "crop")) return RASTER_CROP;
	else if (!cmp(s, "despeckle")) return RASTER_DESPECKLE;
	else if (!cmp(s, "emboss")) return RASTER_EMBOSS;
	else if (!cmp(s, "equalize")) return RASTER_EQUALIZE;
	else if (!cmp(s, "gamma")) return RASTER_GAMMA;
	else if (!cmp(s, "gaussianblur")) return RASTER_GAUSSIANBLUR;
	else if (!cmp(s, "horizontalflip")) return RASTER_HORIZONTALFLIP;
	else if (!cmp(s, "hue")) return RASTER_HUE;
	else if (!cmp(s, "levels")) return RASTER_LEVELS;
	else if (!cmp(s, "motionblur")) return RASTER_MOTIONBLUR;
	else if (!cmp(s, "negate")) return RASTER_NEGATE;
	else if (!cmp(s, "negategrays")) return RASTER_NEGATEGRAYS;
	else if (!cmp(s, "normalize")) return RASTER_NORMALIZEj;
	else if (!cmp(s, "oilpaint")) return RASTER_OILPAINT;
	else if (!cmp(s, "radialblur")) return RASTER_RADIALBLUR;
	else if (!cmp(s, "resize")) return RASTER_RESIZE;
	else if (!cmp(s, "roll")) return RASTER_ROLL;
	else if (!cmp(s, "rotate")) return RASTER_ROTATE;
	else if (!cmp(s, "saturation")) return RASTER_SATURATION;
	else if (!cmp(s, "scale")) return RASTER_SCALE;
	else if (!cmp(s, "sharpen")) return RASTER_SHARPEN;
	else if (!cmp(s, "swirl")) return RASTER_SWIRL;
	else if (!cmp(s, "tint")) return RASTER_TINT;
	else if (!cmp(s, "verticalflip")) return RASTER_VERTICALFLIP;
	else if (!cmp(s, "wave")) return RASTER_WAVE;
	else if (!cmp(s, "lineto")) return VECTOR_LINETO;
	else if (!cmp(s, "moveto")) return VECTOR_MOVETO;
	else if (!cmp(s, "setcap")) return VECTOR_SETCAP;
	else if (!cmp(s, "setcolor")) return VECTOR_SETCOLOR;
	else if (!cmp(s, "setwidth")) return VECTOR_SETWIDTH;
	else if (!cmp(s, "stroke")) return VECTOR_STROKE;
	else
		return -1;
}
#undef cmp


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


struct avnop *
avnop_from_json(const char *str)
{
	return NULL;
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
