#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

// zabranjuje prekide
#define lock{\
 asm pushf;\
 asm cli;\
}

// dozvoljava prekide
#define unlock asm popf


int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
	return res;
}