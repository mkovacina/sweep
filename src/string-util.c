#include <ctype.h>
#include <string.h>

#include "string-util.h"

void trim_right_inplace( char* str )
{
    size_t length = strlen(str);
    char* end = str+length-1;
    while(isspace(*end)) { end--; }
    *(end+1) = '\0';
}