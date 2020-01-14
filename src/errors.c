#include <stdio.h>
#include <stdarg.h>

void error(const char* fmt, ...) 
{
  va_list args;
  va_start(args, fmt);
    
  fprintf(stdout, fmt, args);

  va_end(args);
}