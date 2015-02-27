/*
 * vim: noet
 *
 * media.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include "media.h"
#include "raster.h"
#include "vector.h"

char *
avnmedia_history_json(const avnmedia *avn)
{
	switch (avn->species) {
	case AVENIDA_AUDIO:
		return NULL;
	case AVENIDA_RASTER:
		return avnraster_history_json(avn->raster);
	case AVENIDA_VECTOR: /* FALLTHROUGH */
	case AVENIDA_VIDEO: 
	default: 
		return NULL; /* NOTREACHED */
	}
}
