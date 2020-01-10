#include <stdio.h>
#include <stdarg.h>

extern FILE* ERROR_FILE;

void start_log() 
{
  ERROR_FILE = fopen("error.log", "w");
}

void error(const char* fmt, ...) 
{
  va_list args;
  va_start(args, fmt);
    
  fprintf(ERROR_FILE, fmt, args);
  fprintf(stdout, fmt, args);

  va_end(args);
}

void stop_log() 
{
  fclose(ERROR_FILE);
}

  
