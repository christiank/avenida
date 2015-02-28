/*
 * vim: noet
 *
 * vector.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#ifndef AVENIDA_VECTOR_H
#define AVENIDA_VECTOR_H

#include <stdbool.h>

#include <cairo.h>

#include "avenida.h"
#include "commands.h"

struct avnvectorinfo {
	size_t width;
	size_t height;
};
typedef struct avnvectorinfo avnvectorinfo;

struct avnvector {
	cairo_t *vector;
	avnvectorinfo info;
	unsigned int nops;
	struct avnop *ops[AVNMEDIA_MAX_OPS];
};
typedef struct avnvector avnvector;

avnvector *avnvector_new(const size_t width, const size_t height);
void avnvector_free(avnvector *);
inline void avnvector_add_op(avnvector *, const struct avnop *);
avnvector *avnvector_open(avnvector *, const char *path);
bool avnvector_write(avnvector *, const char *path);
void avnvector_render(avnvector *);

bool avnvector_closepath(avnvector *);
bool avnvector_lineto(avnvector *, const double x, const double y);
bool avnvector_moveto(avnvector *, const double x, const double y);
bool avnvector_openpath(avnvector *);
bool avnvector_setcap(avnvector *);
bool avnvector_setcolor(avnvector *, const char *color);
bool avnvector_setwidth(avnvector *, const unsigned int width);
bool avnvector_stroke(avnvector *);

#endif /* AVENIDA_VECTOR_H */
