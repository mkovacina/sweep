#pragma once
//
// about the naming convention used
//
// Going with Trace{Level} and TraceLine{Level} because if this would be implemented
// in the common fashion of Trace(Level) and TraceLine(Level), they read the same.
//
// 20201217: inspired by rxi/log.c on github, just going to treat this logging as 
// being line-oriented.  also this is the beginning of a slow shift to an attempt
// to do a more structured logging approach

enum TraceLevel
{
	TraceLevelError,
	TraceLevelVerbose
};

#define TraceVerbose(...) (TraceMessageEmit(TraceLevelVerbose, __FILE__, __LINE__, __VA_ARGS__ ))
#define TraceError(...) (TraceMessageEmit(TraceLevelError, __FILE__, __LINE__, __VA_ARGS__ ))

void TraceMessageEmit(enum TraceLevel traceLevel, const char* filename, int line, const char* format, ...);
