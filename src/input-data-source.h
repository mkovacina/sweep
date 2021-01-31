#pragma once

#include <stdio.h>

#define MAX_INPUT_BUFFER_LINES			(1024)
#define MAX_INPUT_BUFFER_LINE_LENGTH	(1024)
typedef enum
{
	FileSourceType,
	MemorySourceType
}
InputDataSourceType;

typedef struct
{
	InputDataSourceType Type;
	FILE *File;
	size_t NumberOfLines;
	char Data[MAX_INPUT_BUFFER_LINES][MAX_INPUT_BUFFER_LINE_LENGTH];
} 
InputDataSource;

void InitializeInputDataSourceFromFile(InputDataSource *source, const char filename[] );
void InitializeInputDataSourceFromMemory(InputDataSource *source);
void AddInputData(InputDataSource* source, const char* line);
void ReadLine(InputDataSource* source, char buffer[], size_t length);
