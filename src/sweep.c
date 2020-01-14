#include <stdlib.h>
#include <string.h>

#include "trace.h"
#include "string-util.h"

#include "useful.h"
#include "file.h"
#include "agent.h"
#include "swarm.h"
#include "errors.h"
#include "probe.h"
#include "constants.h"
#include "support_grids.h"
#include "update_support.h"

/*------------------------ Externs Needed --------------------------- */

/*--------------------------- Globals --------------------------------*/

s_grids_ptr           all_support_grids;
fgrid_ptr             agent_grid;
pri_func_ptr          priority_grid[20];
int                   iter_count;
swarm_element_struct  *swarm;
swarm_element_struct  *master_swarm_types;
int file_flag = 0;

FILE  *SIM_FILE,
	  *PROBE_FILE;

int   RANDOM_NUMBER;

/* REPLACE AT LATER TIME */
int first = 1;
int found = 0;

/*------------------------ Local Constants ---------------------------*/

/*------------------------- Local Macros -----------------------------*/

/*-------------------------- Local Types -----------------------------*/

/*---------------------- Local Function Prototypes -------------------*/
/* void sample (char *, int, Real);                                   */

void display_support_grid ( sgrid_ptr );
void display_support      ( void );
void stop                 ( void );

/* TO BE REPLACED AT LATER TIME */
void write_world ( FILE* );
void write_world_full ( FILE* );
void write_world_stdout ( FILE* );
void write_swarm ( FILE* );

/*---------------------- Function Definitions ------------------------*/

int main ( int argc, char *argv[] ) 
{
	/* Command line arguments:                                            */
	/* The arguments indicate to the main program the files containing    */
	/* the initialization information for the swarm and the environment   */
	/*                                                                    */
	/* Order does not matter but must include sgd, swm, agt, and fsa      */

	int                  i,done=0;
	ExperimentFiles experimentFiles = {'\0'};

	//FILE                 *FILE_NAME_FILE;

	TraceVerboseLine("INITIALIZING SWARM EXPERIMENTATION AND EVALUATION PLATFORM");

	swarm = malloc( sizeof( swarm_element_struct ) );
	swarm->agent = NULL;
	swarm->next_agt = NULL;
	swarm->prev_agt = NULL;

	/* If none CLA supplied, error */
	if (argc != 3) 
	{
		printf( "Usage:  sweep <filename> <random number>\n" );
		stop();
	} 
	else if ( argc == 3 ) 
	{
		FILE* FILE_NAME_FILE = fopen ( argv[1], "r" );

		/* Read file names from file */
		if ( FILE_NAME_FILE ) 
		{
			TraceVerboseLine("Files read in from filename file");
			for ( i = 0; i < 5; i++ ) 
			{
				char filename[MAX_BUFFER];
				fgets( filename, MAX_BUFFER, FILE_NAME_FILE );
				trim_right_inplace(filename);
				addFileToExperiment(filename, &experimentFiles);
			}

			fclose( FILE_NAME_FILE );
		} 
	} 

	init_priority_funcs( priority_grid );

	/* Initialize support grids */
	all_support_grids = init_support_grids( experimentFiles.supportGridsFileName );

	/* Initialize agent grid */
	agent_grid = init_agent_grid();

	/* Initialize swarm */  
	if ( initialize_swarm( agent_grid, &experimentFiles) ) 
	{
		error( "Couldn't initialize swarm\n" );
		stop();
	}

	/* primary "workhorse" loop for the SWEEP system */
	for ( iter_count = 0; !done ; iter_count++ ) 
	{
		update_swarm( swarm );
		update_all_grids();
		probe_swarm(&done, swarm);
	}

	return 0;
}

void stop( ) {
	/* PURPOSE:  Stops the log and quits the program */
	/* INPUT:    NONE                                */
	/* OUTPUT:   NONE                                */
	/* RETURN:   NONE                                */

	exit( 1 );

}

void display_agent_grid()
{
	int i,j,rows,cols;

	rows = GGETR(0);
	cols = GGETC(0);

	for (i=0;i<rows;i++){
		for (j=0;j<cols;j++){
			printf("%3d ", (int)GETAGT(j,i));
		}
		printf("\n");
	}
}

void display_support_grid(sgrid_ptr grd)
{
	int i,j;

	for (i=0;i<grd->rows;i++){
		for (j=0;j<grd->cols;j++){
			printf("%3d ", (int)(* (grd->prev_grid))[i][j]);
		}
		printf("\n");
	}
}

void display_support()
{
	int count;

	count = 0;
	for(count=0; count<all_support_grids->num_grids;count++){

		display_support_grid( (*(all_support_grids->grids))[count]  );

		printf("\n\n\n\n");

	}
}

void write_world(FILE *fp)
{
	int temp;
	int row, col, rows, cols;
	char buffer[150];

	rows = GGETR(0);
	cols = GGETC(0);

	for (row=0; row<rows; row++){
		for (col=0; col<cols; col++){
			temp = priority_grid[0](all_support_grids,row,col);
			buffer[col] = temp;
		}
		buffer[col] = '\0';
		fprintf(fp, "%s\n", buffer);
	}
}

void write_world_stdout(FILE *fp)
{
	int i,j;
	sgrid_ptr grd;

	grd = (*(all_support_grids->grids))[0];
	for (i=0;i<grd->rows;i++){
		for (j=0;j<grd->cols;j++){
			printf("%d", (int)(* (grd->prev_grid))[i][j]);
		}
		printf("\n");
	}
}

void write_world_full(FILE *fp)
{
	int i,j;
	sgrid_ptr grd;

	grd = (*(all_support_grids->grids))[0];
	for (i=0;i<grd->rows;i++){
		for (j=0;j<grd->cols;j++){
			fprintf(fp,"%d", (int)(* (grd->prev_grid))[i][j]);
		}
		fprintf(fp,"\n");
	}
}


void write_swarm ( FILE *fp )
{
	swarm_element_struct *tracer = swarm;

	while ( tracer != NULL ) {
		fprintf(fp, "%d %d %d\n", tracer->agent->y_pos,
				tracer->agent->x_pos,
				tracer->agent->priority_id);
		tracer = tracer->next_agt;
	} 
	fprintf(fp, "*\n");
}
