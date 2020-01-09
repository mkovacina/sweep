#pragma once

#define MAX_FILENAME_LENGTH (255)

typedef struct
{
	char fsaFileName[MAX_FILENAME_LENGTH];
	char agentFileName[MAX_FILENAME_LENGTH];
	char agentFunctionFileName[MAX_FILENAME_LENGTH];
	char swarmFileName[MAX_FILENAME_LENGTH];
	char supportGridsFileName[MAX_FILENAME_LENGTH];
} ExperimentFiles;


int addFileToExperiment(char* filename, ExperimentFiles* experimentFiles);
void assign_fptrs ( char[][MAX_FILENAME_LENGTH], int, int, ExperimentFiles* );
