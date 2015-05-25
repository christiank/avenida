#ifndef AVENIDA_ERRORS_H
#define AVENIDA_ERRORS_H

#define DEFAULT_ERROR (luaL_error(L, "%s", __func__))

#define RANGE_ERROR(value) \
	(luaL_error(L, "value %f is outside acceptable range", value))

#endif /* AVENIDA_ERRORS_H */
