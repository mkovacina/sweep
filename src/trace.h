#ifndef _TRACE_H_
#define _TRACE_H_

// about the naming convention used
//
// Going with Trace{Level} and TraceLine{Level} because if this would be implemented
// in the common fashion of Trace(Level) and TraceLine(Level), they read the same.

void TraceVerbose(const char* fmt, ...);
void TraceVerboseLine(const char* fmt, ...);

int IsTraceLevelVerbose();

#endif 
