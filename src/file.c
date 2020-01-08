/* Contains all functions and structs dealing with files */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "errors.h"
#include "trace.h"
#include "constants.h"

extern char* FSA_FILE_NAME;
extern char* AGENT_FILE_NAME;
extern char* AGENT_FUNCTION_FILE_NAME;
extern char* SWARM_FILE_NAME;
extern char* SUPPORT_GRIDS_FILE_NAME;

void clear_nls ( char * strings[], int num_strings ) {
	/* Clears the \n from the end of strings and replaces it with a null terminator */

	int i;

	for ( i = 0; i < num_strings; i++ ) {

		if ( (strings[i])[ strlen( strings[i] ) - 1 ] == '\n' )  {

			(strings[i])[ strlen( strings[i] ) - 1 ] = '\0';

		}

	}

}

void assign_fptrs ( char * file_names[], int start_num, int num_file_names ) {

	int i, error_flag = 0;

	if ( num_file_names != CORRECT_FILE_NUMBER ) {

		error_flag = 1;
		error( "Incorrect number of files\nFiles Missing:\n" );

	}

	if (IsTraceLevelVerbose())
	{
		TraceVerboseLine("Output of filenames in assign_fptrs");
		for (i=start_num;i< num_file_names; i++)
		{   
			TraceVerboseLine("%s\n", file_names[i]);
		}
	}

	for ( i = start_num; i < num_file_names; i++ ) {

		/* FSA file */
		if ( strstr( file_names[i], ".fsa" ) ) {

			FSA_FILE_NAME = malloc( 1 + strlen( file_names[i] ) );
			strcpy( FSA_FILE_NAME, file_names[i] );

		} else if ( strstr( file_names[i], ".agt" ) ) {

			AGENT_FILE_NAME = malloc( 1 + strlen( file_names[i] ) );
			strcpy( AGENT_FILE_NAME, file_names[i] );

		} else if ( strstr( file_names[i], ".lst" ) ) {

			AGENT_FUNCTION_FILE_NAME = malloc( 1 + strlen( file_names[i] ) );
			strcpy( AGENT_FUNCTION_FILE_NAME, file_names[i] );

		} else if ( strstr( file_names[i], ".swm" ) ) {

			SWARM_FILE_NAME = malloc( 1 + strlen( file_names[i] ) );
			strcpy( SWARM_FILE_NAME, file_names[i] );

		} else if ( strstr( file_names[i], ".sgd" ) ) {

			SUPPORT_GRIDS_FILE_NAME = malloc( 1 + strlen( file_names[i] ) );
			strcpy( SUPPORT_GRIDS_FILE_NAME, file_names[i] );

		}

	}

	if ( error_flag ) {

		if ( !FSA_FILE_NAME )            error( "Fsa File\n"            );
		if ( !AGENT_FILE_NAME )          error( "Agent File\n"          );
		if ( !SWARM_FILE_NAME )          error( "Swarm File\n"          );
		if ( !AGENT_FUNCTION_FILE_NAME ) error( "Agent Function File\n" );
		if ( !SUPPORT_GRIDS_FILE_NAME )  error( "Support Grids File\n"  );

	}

	TraceVerboseLine("FSA: %s", FSA_FILE_NAME );
	TraceVerboseLine("AGT: %s", AGENT_FILE_NAME );
	TraceVerboseLine("SWM: %s", SWARM_FILE_NAME );
	TraceVerboseLine("AFF: %s", AGENT_FUNCTION_FILE_NAME );
	TraceVerboseLine("SGD: %s", SUPPORT_GRIDS_FILE_NAME );
}

void build_file_names ( char* base ) {

	char temp[MAX_BUFFER];

	strcpy( temp, base );
	FSA_FILE_NAME = malloc( strlen( strcat( temp, ".fsa" ) ) );
	strcpy( FSA_FILE_NAME, temp );

	strcpy( temp, base );
	AGENT_FILE_NAME = malloc( strlen( strcat( temp, ".agt") ) );
	strcpy( AGENT_FILE_NAME, temp );

	strcpy( temp,base );
	SWARM_FILE_NAME = malloc( strlen( strcat( temp, ".swm") ) );
	strcpy( SWARM_FILE_NAME, temp );

	strcpy( temp,base );
	AGENT_FUNCTION_FILE_NAME = malloc( strlen( strcat( temp, ".lst") ) );
	strcpy( AGENT_FUNCTION_FILE_NAME, temp );

	strcpy( temp,base );
	SUPPORT_GRIDS_FILE_NAME = malloc( strlen( strcat( temp, ".sgd") ) );
	strcpy( SUPPORT_GRIDS_FILE_NAME, temp );

}






