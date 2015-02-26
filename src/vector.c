/*
 * vim: noet
 *
 * vector.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <stdbool.h>
#include <stdlib.h>

#include "vector.h"

static bool __avnvector_lineto(avnvector *, const int, const int);
static bool __avnvector_moveto(avnvector *, const int, const int);
static bool __avnvector_setcap(avnvector *);
static bool __avnvector_setcolor(avnvector *, const char *);
static bool __avnvector_setwidth(avnvector *, const unsigned int);
static bool __avnvector_stroke(avnvector *);

avnvector *
avnvector_new(const size_t width, const size_t height)
{
	avnvector *avn;
	cairo_surface_t *surf;

	if ((avn = malloc(sizeof(avnvector))) == NULL)
		return NULL;

	surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	avn->vector = cairo_create(surf);
	cairo_surface_destroy(surf);
	avn->info.width = (size_t)width;
	avn->info.height = (size_t)height;
	avn->nops = 0;

	return avn;
}


void
avnvector_free(avnvector *avn)
{
	int i;

	for (i = 0; i < avn->nops; i++)
		avnop_free(avn->ops[i]);

	cairo_destroy(avn->vector);
	free(avn);
}


inline void
avnvector_add_op(avnvector *avn, const struct avnop *op)
{
	avn->ops[avn->nops] = (struct avnop *)op;
	(avn->nops)++;
}


avnvector *
avnvector_open(avnvector *avn, const char *path)
{
	return NULL;
}


avnvector *
avnvector_write(avnvector *avn, const char *path)
{
	return NULL;
}

#define ARG(n) (avn->ops[i]->args[n])

void
avnvector_render(avnvector *avn)
{
	int i;

	for (i = 0; i < avn->nops; i++) {
		switch (avn->ops[i]->name) {
		case VECTOR_LINETO:
			__avnvector_lineto(avn, ARG(0)->arg_uint, ARG(1)->arg_uint);
			break;
		case VECTOR_MOVETO:
			__avnvector_moveto(avn, ARG(0)->arg_uint, ARG(1)->arg_uint);
			break;
		case VECTOR_SETCAP:
			__avnvector_setcap(avn);
			break;
		case VECTOR_SETCOLOR:
			__avnvector_setcolor(avn, ARG(0)->arg_str);
			break;
		case VECTOR_SETWIDTH:
			__avnvector_setwidth(avn, ARG(0)->arg_uint);
			break;
		case VECTOR_STROKE:
			__avnvector_stroke(avn);
			break;
		default:
			return; /* NOTREACHED */
		}
	}
}

#undef ARG

/* */

static bool
__avnvector_lineto(avnvector *avn, const int x, const int y)
{
	if ((x < 0) || (y < 0) || (x > avn->info.width) || (y > avn->info.height))
		return false;

	cairo_line_to(avn->vector, (double)x, (double)y);
	return true;
}


bool
avnvector_lineto(avnvector *avn, const int x, const int y)
{
	struct avnop *op;

	if ((op = avnop_new(VECTOR_LINETO)) == NULL)
		return false;

	avnop_add_arg(op, AVN_INT, x);
	avnop_add_arg(op, AVN_INT, y);
	avnvector_add_op(avn, op);
	return true;
}


static bool
__avnvector_moveto(avnvector *avn, const int x, const int y)
{
	if ((x < 0) || (y < 0) || (x > avn->info.width) || (y > avn->info.height))
		return false;

	cairo_new_path(avn->vector);
	cairo_move_to(avn->vector, (double)x, (double)y);
	return true;
}


bool
avnvector_moveto(avnvector *avn, const int x, const int y)
{
	struct avnop *op;

	if ((op = avnop_new(VECTOR_MOVETO)) == NULL)
		return false;

	avnop_add_arg(op, AVN_INT, x);
	avnop_add_arg(op, AVN_INT, y);
	avnvector_add_op(avn, op);
	return true;
}


static bool
__avnvector_setcap(avnvector *avn)
{
	return true;
}


bool
avnvector_setcap(avnvector *avn)
{
	return true;
}


static bool
__avnvector_setcolor(avnvector *avn, const char *color)
{
	return true;
}


bool
avnvector_setcolor(avnvector *avn, const char *color)
{
	return true;
}


static bool
__avnvector_setwidth(avnvector *avn, const unsigned int width)
{
	return true;
}


bool
avnvector_setwidth(avnvector *avn, const unsigned int width)
{
	return true;
}


static bool
__avnvector_stroke(avnvector *avn)
{
	return true;
}


bool
avnvector_stroke(avnvector *avn)
{
	return true;
}
