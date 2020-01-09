/* Contains all functions and structs dealing with files */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "file.h"

#include "errors.h"
#include "trace.h"
#include "constants.h"


void assign_fptrs ( char * file_names[], int start_num, int num_file_names )
{
	ExperimentFiles experimentFiles = {'\0'};

	TraceVerboseLine("Output of filenames in assign_fptrs");
	for (int i=start_num;i< num_file_names; i++)
	{   
		TraceVerboseLine("%s\n", file_names[i]);
	}

	int error_ExtensionMismatch = 0;
	for ( int i = start_num; i < num_file_names; i++ ) 
	{
		/* FSA file */
		if ( strstr( file_names[i], ".fsa" ) ) 
		{
			strncpy( experimentFiles.fsaFileName, file_names[i], MAX_FILENAME_LENGTH );
		} 
		else if ( strstr( file_names[i], ".agt" ) ) 
		{
			strncpy( experimentFiles.agentFileName, file_names[i], MAX_FILENAME_LENGTH );
		}
		else if ( strstr( file_names[i], ".lst" ) )
		{
			strncpy( experimentFiles.agentFunctionFileName, file_names[i], MAX_FILENAME_LENGTH);
		}
		else if ( strstr( file_names[i], ".swm" ) ) 
		{
			strncpy( experimentFiles.swarmFileName, file_names[i], MAX_FILENAME_LENGTH );
		} 
		else if ( strstr( file_names[i], ".sgd" ) )
		{
			strncpy( experimentFiles.supportGridsFileName, file_names[i], MAX_FILENAME_LENGTH );
		}
		else
		{
			error_ExtensionMismatch = 1;
			// don't break the loop here
			// let it go so we can see if the other files are correct
		}
	}

	if ( !error_ExtensionMismatch && num_file_names != CORRECT_FILE_NUMBER ) 
	{
		error( "Files Missing:\n" );

		if ( strlen(experimentFiles.fsaFileName) == 0 )            
			error( "Fsa File\n"            );
		if ( strlen(experimentFiles.agentFileName) == 0 )          
			error( "Agent File\n"          );
		if ( strlen(experimentFiles.swarmFileName) == 0 )         
			error( "Swarm File\n"          );
		if ( strlen(experimentFiles.agentFunctionFileName) == 0 )
			error( "Agent Function File\n" );
		if ( strlen(experimentFiles.supportGridsFileName) == 0) 
			error( "Support Grids File\n"  );
	}

	TraceVerboseLine("FSA: %s", experimentFiles.fsaFileName );
	TraceVerboseLine("AGT: %s", experimentFiles.agentFileName );
	TraceVerboseLine("SWM: %s", experimentFiles.swarmFileName );
	TraceVerboseLine("AFF: %s", experimentFiles.agentFunctionFileName );
	TraceVerboseLine("SGD: %s", experimentFiles.supportGridsFileName );
}