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
		break;
	case AVENIDA_RASTER:
		return avnraster_history_json(avn->raster);
		break;
	case AVENIDA_VECTOR: /* FALLTHROUGH */
	case AVENIDA_VIDEO: 
		return NULL;
		break;
	default: 
		return NULL; /* NOTREACHED */
	}
}
