/*
 * vim: noet
 *
 * status.c
 * Christian Koch <cfkoch@sdf.lonestar.org>
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "status.h"

avnstatus
avnstatus_ok(void)
{
	avnstatus stat;
	stat.ok = true;
	snprintf(stat.msg, LINE_MAX, "%s", "");
	return stat;
}


avnstatus
avnstatus_bad(const char *msg)
{
	avnstatus stat;
	stat.ok = false;
	snprintf(stat.msg, LINE_MAX, "%s", msg);
	return stat;
}
