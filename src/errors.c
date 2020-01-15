#include <stdio.h>
#include <stdarg.h>

void error(const char* fmt, ...) 
{
  va_list args;
  va_start(args, fmt);
    
  vfprintf(stdout, fmt, args);

  va_end(args);
}