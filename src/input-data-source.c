#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "input-data-source.h"

void InitializeInputDataSourceFromFile(InputDataSource *source, const char filename[] )
{
	TraceVerbose("Initializing input from file '%s'", filename);
	FILE* file = fopen(filename, "r");
	if (file == NULL) 
	{
		TraceError("Can't open file: %s%c", filename, 'z');
		return;
	}

	source->Type = FileSourceType;
	source->File = file;
	source->NumberOfLines = 0;
}

void InitializeInputDataSourceFromMemory(InputDataSource *source)
{
	TraceVerbose("Initializing input from memory");

	source->Type = MemorySourceType;
	source->File = NULL;
	source->NumberOfLines = 0;
}

void AddInputData(InputDataSource *source, const char* line)
{
	if (source->NumberOfLines > MAX_INPUT_BUFFER_LINES)
	{
		exit(-1);
	}

	strncpy(source->Data[source->NumberOfLines], line, MAX_INPUT_BUFFER_LINE_LENGTH);
	source->NumberOfLines++;

}

void ReadLine(InputDataSource* source, char buffer[], size_t length)
{
	switch(source->Type)
	{
		case FileSourceType:
			{
				fgets(buffer, length, source->File);
			}
			break;
		case MemorySourceType:
			{
				if ( source->NumberOfLines < 1 )
				{
					TraceError("Insufficient input data from the memory source");
					exit(-1);
				}

				strncpy( buffer, source->Data[source->NumberOfLines - 1], length );
				source->NumberOfLines--;
			}
			break;
		default:
			{
				// XXX: all places that use exit replace with a new stop(code, msg) method
				TraceError("Unsupported data source specified (%d)");
				exit(-1);
			}
			break;
	}
}
