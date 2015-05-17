/*
 * vim: noet
 *
 * raster.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <wand/magick_wand.h>

#include "cJSON.h"

#include "commands.h"
#include "raster.h"

static PixelWand *pixel_wand_with_color(const char *color);

static bool __avnraster_brightness(avnraster *avn, const double);
static bool __avnraster_border(avnraster *, const size_t, const size_t,
	const char *);
static bool __avnraster_charcoal(avnraster *, const double );
static bool __avnraster_crop(avnraster *, const unsigned int,
	const unsigned int, const size_t, const size_t);
static bool __avnraster_despeckle(avnraster *);
static bool __avnraster_emboss(avnraster *, const double);
static bool __avnraster_equalize(avnraster *);
static bool __avnraster_gamma(avnraster *, const double);
static bool __avnraster_gaussianblur(avnraster*, const double);
static bool __avnraster_horizontalflip(avnraster *);
static bool __avnraster_hue(avnraster *, const double);
static bool __avnraster_motionblur(avnraster *avn, const double,
	const double);
static bool __avnraster_negate(avnraster *);
static bool __avnraster_negategrays(avnraster *);
static bool __avnraster_normalize(avnraster *);
static bool __avnraster_oilpaint(avnraster *, const double);
static bool __avnraster_resize(avnraster *, const size_t, const size_t);
static bool __avnraster_roll(avnraster *, const int, const int);
static bool __avnraster_rotate(avnraster *, const double, const char *);
static bool __avnraster_saturation(avnraster *, const double);
static bool __avnraster_scale(avnraster *, const double);
static bool __avnraster_sharpen(avnraster *, const double);
static bool __avnraster_swirl(avnraster *, const double);
static bool __avnraster_verticalflip(avnraster *);
static bool __avnraster_wave(avnraster *, const double, const double);

/* */

avnraster *
avnraster_new(const char *path)
{
	avnraster *avn;
	MagickWand *wand;

	if ((avn = malloc(sizeof(avnraster))) == NULL)
		return NULL;

	if ((wand = NewMagickWand()) == NULL)
		return NULL;

	avn->image = wand;
	avn->info = (avnrasterinfo){ .width = 0, .height = 0, };
	snprintf(avn->info.path, PATH_MAX, "%s", path);
	avn->nops = 0;

	return avn;
}


void
avnraster_free(avnraster *avn)
{
	int i;

	if (avn == NULL)
		return;

	for (i = 0; i < avn->nops; i++)
		avnop_free(avn->ops[i]);

	DestroyMagickWand(avn->image);
	free(avn);
}


/*
 * XXX should somehow be a general method for all avn*species...
 * Does this mean avn*species is a tagged union?
 */
inline void
avnraster_add_op(avnraster *avn, const struct avnop *op)
{
	avn->ops[avn->nops] = (struct avnop *)op;
	(avn->nops)++;
}


/*
 * Opening the image gives us the opportunity to suck in as much metadata
 * as possible about it. This way, any time anyone needs to know something,
 * we don't need to call a GraphicsMagick function, we can just look up
 * inside the avnraster structure.
 *
 * XXX Actually, to be even more performant: we don't NEED to get any
 * metadata until we need it...
 */
bool
avnraster_open(avnraster *avn)
{
	if (MagickReadImage(avn->image, avn->info.path) == MagickPass) {
		avn->info.width = (size_t)MagickGetImageWidth(avn->image);
		avn->info.height = (size_t)MagickGetImageHeight(avn->image);
		snprintf(avn->info.codec, LINE_MAX, "%s",
			MagickGetImageFormat(avn->image));
		return true;
	} else {
		return false;
	}
}


#define ARG(n) (avn->ops[i]->args[n])

/*
 * XXX This has the opportunity to be multithreaded with range locking
 * and everything!
 *
 * XXX "Verbose" should return a string instead? or it should log somewhere
 * specific? or is it just a Lua thing?
 */
bool
avnraster_render(avnraster *avn, const bool verbose)
{
	int i;

	for (i = 0; i < avn->nops; i++) {
		if (verbose)
			printf("%s\n", cJSON_PrintUnformatted(avnop_to_json(avn->ops[i])));

		switch (avn->ops[i]->name) {
		case RASTER_BORDER:
			__avnraster_border(avn, ARG(0)->arg_double, ARG(1)->arg_double,
				ARG(2)->arg_str);
			break;
		case RASTER_BRIGHTNESS:
			__avnraster_brightness(avn, ARG(0)->arg_double);
			break;
		case RASTER_CHARCOAL:
			__avnraster_charcoal(avn, ARG(0)->arg_double);
			break;
		case RASTER_CROP:
			__avnraster_crop(avn, ARG(0)->arg_uint, ARG(1)->arg_uint,
				ARG(2)->arg_uint, ARG(3)->arg_uint);
			break;
		case RASTER_DESPECKLE:
			__avnraster_despeckle(avn);
			break;
		case RASTER_EMBOSS:
			__avnraster_emboss(avn, ARG(0)->arg_double);
			break;
		case RASTER_EQUALIZE:
			__avnraster_equalize(avn);
			break;
		case RASTER_GAMMA:
			__avnraster_gamma(avn, ARG(0)->arg_double);
			break;
		case RASTER_GAUSSIANBLUR:
			__avnraster_gaussianblur(avn, ARG(0)->arg_double);
			break;
		case RASTER_HORIZONTALFLIP:
			__avnraster_horizontalflip(avn);
			break;
		case RASTER_HUE:
			__avnraster_hue(avn, ARG(0)->arg_double);
			break;
		case RASTER_MOTIONBLUR:
			__avnraster_motionblur(avn, ARG(0)->arg_double, ARG(1)->arg_double);
			break;
		case RASTER_NEGATE:
			__avnraster_negate(avn);
			break;
		case RASTER_NEGATEGRAYS:
			__avnraster_negategrays(avn);
			break;
		case RASTER_NORMALIZE:
			__avnraster_normalize(avn);
			break;
		case RASTER_OILPAINT:
			__avnraster_oilpaint(avn, ARG(0)->arg_double);
			break;
		case RASTER_RESIZE:
			__avnraster_resize(avn, ARG(0)->arg_uint, ARG(1)->arg_uint);
			break;
		case RASTER_ROLL:
			__avnraster_roll(avn, ARG(0)->arg_int, ARG(1)->arg_int);
			break;
		case RASTER_ROTATE:
			__avnraster_rotate(avn, ARG(0)->arg_double, ARG(1)->arg_str);
			break;
		case RASTER_SATURATION:
			__avnraster_saturation(avn, ARG(0)->arg_double);
			break;
		case RASTER_SCALE:
			__avnraster_scale(avn, ARG(0)->arg_double);
			break;
		case RASTER_SHARPEN:
			__avnraster_sharpen(avn, ARG(0)->arg_double);
			break;
		case RASTER_SWIRL:
			__avnraster_swirl(avn, ARG(0)->arg_double);
			break;
		case RASTER_VERTICALFLIP:
			__avnraster_verticalflip(avn);
			break;
		case RASTER_WAVE:
			__avnraster_wave(avn, ARG(0)->arg_double, ARG(1)->arg_double);
			break;
		default:
			return false; /* NOTREACHED */
		}
	}

	return true;
}

#undef ARG


bool
avnraster_write(avnraster *avn, const char *path)
{
	if (MagickWriteImage(avn->image, path) == MagickPass)
		return true;
	else
		return false;
}


/*
 * The returned JSON is always an array, since the order of operations is
 * significant. The returned string is dynamically allocated and needs to be
 * freed.
 */
char *
avnraster_history_json(const avnraster *avn)
{
	int i;
	cJSON *history_ary;
	char *str;

	history_ary = cJSON_CreateArray();

	for (i = 0; i < avn->nops; i++)
		cJSON_AddItemToArray(history_ary, avnop_to_json(avn->ops[i]));

	str = cJSON_PrintUnformatted(history_ary);

	return str;
}


/* */

static bool
__avnraster_border(avnraster *avn, const size_t width, const size_t height,
	const char *color)

{
	PixelWand *colorw;
	int ret;

	if ((width == 0) && (height == 0))
		return true;

	if ((colorw = pixel_wand_with_color(color)) == NULL)
		return false;

	ret = MagickBorderImage(avn->image, colorw, width, height);

	DestroyPixelWand(colorw);
	return ret == MagickPass ? true : false;
}


bool
avnraster_border(avnraster *avn, const size_t width, const size_t height,
	const char *color)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_BORDER)) == NULL)
		return false;

	avnop_add_arg(op, AVN_UINT, width);
	avnop_add_arg(op, AVN_UINT, height);
	avnop_add_arg(op, AVN_STRING, color);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_brightness(avnraster *avn, const double value)
{
	if (value == 0.0)
		return true;

	if (MagickModulateImage(avn->image, (value+100), 100, 100) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_brightness(avnraster *avn, const double value)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_BRIGHTNESS)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, value);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_charcoal(avnraster *avn, const double amt)
{
	if (amt < 0.0)
		return false;

	if (amt == 0.0)
		return true; /* XXX dunno if this is correct */

	if (MagickCharcoalImage(avn->image, 0, amt) == MagickPass)
		return true;
	else;
		return false;
}


bool
avnraster_charcoal(avnraster *avn, const double amt)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_CHARCOAL)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, amt);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_crop(avnraster *avn, const unsigned int x, const unsigned int y,
	const size_t width, const size_t height)
{
	if ((width > avn->info.width) || (height > avn->info.height))
		return false;

	if ((x == 0) && (y == 0) && (width == avn->info.width) &&
		(height == avn->info.height))
			return true;

	if (MagickCropImage(avn->image, width, height, x, y) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_crop(avnraster *avn, const unsigned int x, const unsigned int y,
	const size_t width, const size_t height)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_CROP)) == NULL)
		return false;

	avnop_add_arg(op, AVN_UINT, x);
	avnop_add_arg(op, AVN_UINT, y);
	avnop_add_arg(op, AVN_UINT, width);
	avnop_add_arg(op, AVN_UINT, height);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_despeckle(avnraster *avn)
{
	if (MagickDespeckleImage(avn->image) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_despeckle(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_DESPECKLE)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_emboss(avnraster *avn, const double amt)
{
	if (amt < 0.0)
		return false;

	if (amt == 0.0)
		return true; /* XXX dunno if this correct */

	if (MagickEmbossImage(avn->image, 0, amt) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_emboss(avnraster *avn, const double amt)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_EMBOSS)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, amt);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_equalize(avnraster *avn)
{
	if (MagickEqualizeImage(avn->image) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_equalize(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_EQUALIZE)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


/*
 * XXX Under what conditions is this a noop?
 */
static bool
__avnraster_gamma(avnraster *avn, const double gamma)
{
	if (gamma < 0.0)
		return false;

	if (MagickGammaImage(avn->image, gamma) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_gamma(avnraster *avn, const double gamma)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_GAMMA)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, gamma);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_gaussianblur(avnraster *avn, const double amt)
{
	if (amt < 0.0)
		return false;

	if (amt == 0.0)
		return true; /* XXX dunno if this is correct */

	if (MagickBlurImage(avn->image, 0, amt) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_gaussianblur(avnraster *avn, const double amt)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_GAUSSIANBLUR)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, amt);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_horizontalflip(avnraster *avn)
{
	if (MagickFlopImage(avn->image) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_horizontalflip(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_HORIZONTALFLIP)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


/*
 * See the note about MagickModulateImage() in the documentation for
 * avnraster_brightness().
 */
static bool
__avnraster_hue(avnraster *avn, const double value)
{
	if ((value < -100.0) || (value > 100.0))
		return false;

	if (value == 0.0)
		return true;

	if (MagickModulateImage(avn->image, 100, 100, (value+100)) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_hue(avnraster *avn, const double value)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_HUE)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, value);
	avnraster_add_op(avn, op);
	return true;
}


/*
 * XXX
 */
bool
avnraster_levels(avnraster *avn, const double black, const double white,
	const double gamma)
{
	return true;
}


static bool
__avnraster_motionblur(avnraster *avn, const double amt, const double angle)
{
	if (amt < 0.0)
		return false;

	if (amt == 0.0)
		return true; /* XXX dunno if this correct */

	if (MagickMotionBlurImage(avn->image, 0, amt, angle) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_motionblur(avnraster *avn, const double amt, const double angle)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_MOTIONBLUR)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, amt);
	avnop_add_arg(op, AVN_DOUBLE, angle);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_negate(avnraster *avn)
{
	if (MagickNegateImage(avn->image, 0) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_negate(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_NEGATE)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_negategrays(avnraster *avn)
{
	if (MagickNegateImage(avn->image, 1) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_negategrays(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_NEGATEGRAYS)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_normalize(avnraster *avn)
{
	if (MagickNormalizeImage(avn->image) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_normalize(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_NORMALIZE)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


/*
 * Apparently, 0.0 is an acceptable radius.
 */
static bool
__avnraster_oilpaint(avnraster *avn, const double radius)
{
	if (radius < 0.0)
		return false;

	if (MagickOilPaintImage(avn->image, radius) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_oilpaint(avnraster *avn, const double radius)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_OILPAINT)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, radius);
	avnraster_add_op(avn, op);
	return true;
}


/*
 * XXX doesn't work??
 */
bool
avnraster_radialblur(avnraster *avn, const double angle)
{
	if (MagickRadialBlurImage(avn->image, angle) == MagickPass)
		return true;
	else
		return false;
}


/*
 * XXX should get the "blurry" argument in there somehow.
 *
 * XXX should the user be able to choose a filter?
 */
static bool
__avnraster_resize(avnraster *avn, const size_t width, const size_t height)
{
	int a; 

	if ((width == avn->info.width) && (height == avn->info.height))
		return true;

	a = MagickResizeImage(avn->image, width, height, LanczosFilter, 0);

	return a == MagickPass ? true : false;
}


bool
avnraster_resize(avnraster *avn, const size_t width, const size_t height)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_RESIZE)) == NULL)
		return false;

	avnop_add_arg(op, AVN_UINT, width);
	avnop_add_arg(op, AVN_UINT, height);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_roll(avnraster *avn, const int x_amt, const int y_amt)
{
	if ((abs(x_amt) % avn->info.width == 0) &&
		(abs(y_amt) % avn->info.height == 0))
			return true;

	if (MagickRollImage(avn->image, x_amt, y_amt) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_roll(avnraster *avn, const int x_amt, const int y_amt)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_ROLL)) == NULL)
		return false;

	avnop_add_arg(op, AVN_INT, x_amt);
	avnop_add_arg(op, AVN_INT, y_amt);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_rotate(avnraster *avn, const double angle, const char *bgcolor)
{
	PixelWand *color;
	int ret;

	if (angle == 0.0)
		return true;

	if ((color = pixel_wand_with_color(bgcolor)) == NULL)
		return false;

	ret = MagickRotateImage(avn->image, color, angle);

	DestroyPixelWand(color);
	return ret == MagickPass ? true : false;
}


bool
avnraster_rotate(avnraster *avn, const double angle, const char *bgcolor)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_ROTATE)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, angle);
	avnop_add_arg(op, AVN_STRING, bgcolor);
	avnraster_add_op(avn, op);
	return true;
}


/*
 * See the note about MagickModulateImage() in the documentation for
 * avnraster_brightness().
 */
static bool
__avnraster_saturation(avnraster *avn, const double value)
{
	if ((value < -100.0) || (value > 100.0))
		return false;

	if (value == 0.0)
		return true;

	if (MagickModulateImage(avn->image, 100, (value+100), 100) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_saturation(avnraster *avn, const double value)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_SATURATION)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, value);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_scale(avnraster *avn, const double factor)
{
	unsigned long new_w, new_h;

	if (factor < 0.0)
		return false;

	if (factor == 1.0)
		return true;

	new_w = (avn->info.width *= factor);
	new_h = (avn->info.height *= factor);

	if (MagickScaleImage(avn->image, new_w, new_h) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_scale(avnraster *avn, const double factor)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_SCALE)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, factor);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_sharpen(avnraster *avn, const double amt)
{
	if (amt == 0.0)
		return true; /* XXX dunno if this is correct */

	if (MagickSharpenImage(avn->image, 0, amt))
		return true;
	else
		return false;
}


bool
avnraster_sharpen(avnraster *avn, const double amt)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_SHARPEN)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, amt);
	avnraster_add_op(avn, op);
	return true;
}


static bool
__avnraster_swirl(avnraster *avn, const double degrees)
{
	if (degrees == 0.0)
		return true;

	if (MagickSwirlImage(avn->image, degrees) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_swirl(avnraster *avn, const double degrees)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_SWIRL)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, degrees);
	avnraster_add_op(avn, op);
	return true;
}


/*
 * XXX doesn't work??
 */
bool
avnraster_tint(avnraster *avn, const char *color, const double opacity)
{
	PixelWand *colorw;
	int ret;

	if ((colorw = pixel_wand_with_color(color)) == NULL)
		return false;

	PixelSetOpacity(colorw, opacity);
	//ret = MagickTintImage(avn->image, colorw, colorw);
	ret = MagickColorizeImage(avn->image, colorw, colorw);

	DestroyPixelWand(colorw);
	return ret == MagickPass ? true : false;
}


static bool
__avnraster_verticalflip(avnraster *avn)
{
	if (MagickFlipImage(avn->image) == MagickPass)
		return true;
	else
		return false;
}


bool
avnraster_verticalflip(avnraster *avn)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_VERTICALFLIP)) == NULL)
		return false;

	avnraster_add_op(avn, op);
	return true;
}


/*
 * XXX hmm, wavelength of zero yields a blank image
 */
static bool
__avnraster_wave(avnraster *avn, const double amplitude,
	const double wavelength)
{
	if ((amplitude < 0.0) || (wavelength < 0.0))
		return false;

	if (amplitude == 0.0)
		return true;

	if (MagickWaveImage(avn->image, amplitude, wavelength) == MagickPass)
		return true;
	else
		return false;
}

bool
avnraster_wave(avnraster *avn, const double amplitude,
	const double wavelength)
{
	struct avnop *op;

	if ((op = avnop_new(RASTER_WAVE)) == NULL)
		return false;

	avnop_add_arg(op, AVN_DOUBLE, amplitude);
	avnop_add_arg(op, AVN_DOUBLE, wavelength);
	avnraster_add_op(avn, op);
	return true;
}

/* */

/*
 * May return NULL. The wand should be freed with DestroyPixelWand() when
 * you're done with it.
 */
static PixelWand *
pixel_wand_with_color(const char *color)
{
	PixelWand *wand;

	wand = NewPixelWand();

	if (PixelSetColor(wand, color) != MagickPass)
		return NULL;
	else
		return wand;
}
