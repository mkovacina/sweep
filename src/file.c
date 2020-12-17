/* Contains all functions and structs dealing with files */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "file.h"

#include "trace.h"
#include "constants.h"

int addFileToExperiment(char* filename, ExperimentFiles* experimentFiles)
{
	TraceVerbose("Assigning file to experiment: '%s'", filename);

	/* FSA file */
	if ( strstr( filename, ".fsa" ) ) 
	{
		strncpy( experimentFiles->fsaFileName, filename, MAX_FILENAME_LENGTH );
	} 
	else if ( strstr( filename, ".agt" ) ) 
	{
		strncpy( experimentFiles->agentFileName, filename, MAX_FILENAME_LENGTH );
	}
	else if ( strstr( filename, ".lst" ) )
	{
		strncpy( experimentFiles->agentFunctionFileName, filename, MAX_FILENAME_LENGTH);
	}
	else if ( strstr( filename, ".swm" ) ) 
	{
		strncpy( experimentFiles->swarmFileName, filename, MAX_FILENAME_LENGTH );
	} 
	else if ( strstr( filename, ".sgd" ) )
	{
		strncpy( experimentFiles->supportGridsFileName, filename, MAX_FILENAME_LENGTH );
	}
	else
	{
		TraceError("No experiment file found for file : '%s'", filename);
	}

	return 0;
}
