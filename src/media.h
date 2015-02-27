/*
 * vim: noet
 *
 * media.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#ifndef AVENIDA_MEDIA_H
#define AVENIDA_MEDIA_H

#include "raster.h"
#include "vector.h"
#include "video.h"

enum avnspecies {
	AVENIDA_AUDIO,
	AVENIDA_RASTER,
	AVENIDA_VECTOR,
	AVENIDA_VIDEO,
};

struct avnmedia {
	enum avnspecies species;
	union {
		avnraster *raster;
		avnvector *vector;
		avnvideo *video;
	};
};
typedef struct avnmedia avnmedia;

char *avnmedia_history_json(const avnmedia *);

#endif /* AVENIDA_MEDIA_H */
