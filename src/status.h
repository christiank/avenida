/*
 * vim: noet
 *
 * status.h
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#ifndef AVENIDA_STATUS_H
#define AVENIDA_STATUS_H

#include <limits.h>
#include <stdbool.h>

struct avnstatus {
	bool ok;
	char msg[LINE_MAX];
};
typedef struct avnstatus avnstatus;

avnstatus avnstatus_ok(void);
avnstatus avnstatus_bad(const char *msg);

#endif /* AVENIDA_STATUS_H */
