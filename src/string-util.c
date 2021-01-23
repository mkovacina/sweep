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

void trim_comments(char buffer[], size_t length)
{
	int i = 0;
     while( i < (length-1) && buffer[i] != ' ' && buffer[i]!='\n' ) {i++;};
     buffer[i] = '\0';
	 // XXX: need to figure out if this can just use 'isspace'
}
