// todo: define the trace level enum
// todo: replace int placeholder for the trace level enum
// todo: implement a "global" trace level to filter on
// todo: consider looking as vprintf return codes (and adding in subsequent return codes)

#include "trace.h"

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

static const char* TraceLevelNames[] = 
{
	"ERROR",
	"VERBOSE"
};

void TraceMessageEmit(enum TraceLevel traceLevel, const char* filename, int line, const char* fmt, ...)
{
	// todo: guard that tracelevel is a valid value
	//
	time_t t = time(NULL);
	struct tm tm;
	localtime_r(&t, &tm);

	char timestamp[32];
	strftime(timestamp, sizeof(timestamp), "%H:%M:%S", &tm);

	va_list args;
	va_start(args, fmt);
	// note the format string
	// - space and alignment used to make columns fit
	// - tabs used so that the data can be parsed in a tab-separated format
	//		- this is a "good enough for now" step towards structured logging
	fprintf(stdout, "%-5s\t%s\t%20s:%-4d\t", TraceLevelNames[traceLevel], timestamp, filename, line);
	vprintf(fmt, args);
	// fwiw, trying `puts` with the "hope" that the correct line ending marker will be used
	puts("");
	va_end(args);
}
