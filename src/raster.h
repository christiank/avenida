/*
 * vim: noet
 *
 * raster.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#ifndef AVENIDA_RASTER_H
#define AVENIDA_RASTER_H

#include <limits.h>
#include <stdbool.h>

#include <wand/magick_wand.h>

#include "avenida.h"
#include "commands.h"

struct avnrasterinfo {
	size_t width;
	size_t height;
	char codec[LINE_MAX];
	char path[PATH_MAX];
};
typedef struct avnrasterinfo avnrasterinfo;


/*
 * The avnraster structure is a delegate for a raster graphic.
 */
struct avnraster {
	MagickWand *image;
	avnrasterinfo info;
	unsigned int nops;
	struct avnop *ops[AVNMEDIA_MAX_OPS];
};
typedef struct avnraster avnraster;

avnraster *avnraster_new(const char *path);
void avnraster_free(avnraster *);
inline void avnraster_add_op(avnraster *, const struct avnop *);
bool avnraster_open(avnraster *);
bool avnraster_render(avnraster *, const bool verbose);
bool avnraster_write(avnraster *, const char *path);
char *avnraster_history_json(const avnraster *);

bool avnraster_border(avnraster *, const size_t width, const size_t height,
	const char *color);
bool avnraster_brightness(avnraster *, const double value);
bool avnraster_charcoal(avnraster *, const double amt);
bool avnraster_crop(avnraster *, const unsigned int x, const unsigned int y,
	const size_t width, const size_t height);
bool avnraster_despeckle(avnraster *);
bool avnraster_edge(avnraster *, const double amt);
bool avnraster_emboss(avnraster *, const double amt);
bool avnraster_equalize(avnraster *);
bool avnraster_gamma(avnraster *, const double gamma);
bool avnraster_gaussianblur(avnraster *, const double amt);
bool avnraster_horizontalflip(avnraster *);
bool avnraster_hue(avnraster *, const double value);
bool avnraster_implode(avnraster *, const double radius);
bool avnraster_levels(avnraster *, const double black, const double white,
	const double gamma);
bool avnraster_motionblur(avnraster *, const double amt,
	const double angle);
bool avnraster_negate(avnraster *);
bool avnraster_negategrays(avnraster *);
bool avnraster_normalize(avnraster *);
bool avnraster_oilpaint(avnraster *, const double radius);
/* XXX radialblur doesn't work? */
bool avnraster_radialblur(avnraster *, const double angle); 
bool avnraster_resize(avnraster *, const size_t width, const size_t height);
bool avnraster_roll(avnraster *, const int x_amt, const int y_amt);
bool avnraster_rotate(avnraster *, const double angle, const char *bgcolor);
bool avnraster_saturation(avnraster *, const double value);
bool avnraster_scale(avnraster *, const double factor);
bool avnraster_sharpen(avnraster *, const double amt);
bool avnraster_swirl(avnraster *, const double degrees);
bool avnraster_tint(avnraster *, const char *color, const double opacity);
bool avnraster_verticalflip(avnraster *);
bool avnraster_wave(avnraster *, const double amplitude,
	const double wavelength);

unsigned long avnraster_info_ncolors(const avnraster *);

#endif /* AVENIDA_RASTER_H */
