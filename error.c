#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "error.h"

void
error(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	exit(1);
}
