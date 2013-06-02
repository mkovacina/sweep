// todo: define the trace level enum
// todo: replace int placeholder for the trace level enum

#include "trace.h"

#include <stdarg.h>

int _Trace(int traceLevel, const char* fmt, va_list args);
int _TraceLine(int traceLevel, const char* fmt, va_list args);

void TraceVerbose(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	_Trace(0, fmt, args);

	va_end(args);
}

void TraceVerboseLine(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);

	_TraceLine(0, fmt, args);

	va_end(args);	
}

int IsTraceLevelVerbose()
{
	return 1;
}

int _Trace(int traceLevel, const char* fmt, va_list args)
{
	// todo: impelment level filtering
	int ret = vprintf(fmt, args);
	return ret;
}

int _TraceLine(int traceLevel, const char* fmt, va_list args)
{
	// todo: implement level filtering
	int ret = vprintf(fmt, args);
	vprintf("\n");
	return ret;
}
