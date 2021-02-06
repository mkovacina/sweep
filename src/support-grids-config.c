#include <stdio.h>
#include <stdlib.h>

#include "trace.h"
#include "string-util.h"
#include "support_grids.h"
#include "support-grids-config.h"
#include "input-data-source.h"

void nextLine(InputDataSource* source, char buffer[], size_t length);
void parseUniformInitialization(UniformInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );
void parseFileInitialization(FileInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );
void parseRandomInitialization(RandomInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length );

void ParseSupportGridConfig(InputDataSource *source)
{
	char buffer[MAX_BUFFER];

	SupportGridConfiguration config;

	/* Determine how many support grids there are */
	nextLine(source, buffer, MAX_BUFFER);

	config.NumberOfSupportGrids = atoi(buffer);

	if ( config.NumberOfSupportGrids > MAX_GRIDS )
	{
		TraceError("Too many grids defined");
		// XXX: find a better way to error besides exit()...errno?? or just define better error codes
		exit(-1);
	}

	TraceDebug("Number of support grids: %d", config.NumberOfSupportGrids);

	/* Get row dimension of the support grids */
	nextLine(source, buffer, MAX_BUFFER);
	config.NumberOfRows = atoi(buffer);
	TraceDebug("Number of rows: %d", config.NumberOfRows); 

	/* Get column dimension of the support grids */
	nextLine(source, buffer, MAX_BUFFER);
	config.NumberOfColumns = atoi(buffer);
	TraceDebug("Number of columns: %d", config.NumberOfColumns);

	for (int entry = 0; entry < config.NumberOfSupportGrids; entry++)
	{
		nextLine(source, buffer, MAX_BUFFER);

		int id = atoi(buffer);

		// this enforces that the support grids are number sequentially
		// from 0 in the input file
		if (entry != id)
		{
			TraceError("Processing support grid with supplied ID of %d but expected %d", id, entry);
			exit(-1);
		}

		SupportGridDefinition* definition = &(config.GridDefinitions[id]);
		definition->GridID = id;

		nextLine(source, buffer, MAX_BUFFER);

		/* determines method of INITIALIZATION for each support grid */
		switch (buffer[0])
		{
			/* uniform - all grid locations get same initial value */
			case 'U': 
				{
					TraceDebug("Initializing grid %d using Uniform initialization", definition->GridID);

					definition->InitializationMethod = Uniform;
					parseUniformInitialization(&definition->InitializationParameters.Uniform, source, buffer, MAX_BUFFER);
				}
				break;

			/* file - all grid locations specified in a named file */
			case 'F': 
				{
					TraceDebug("Initializing grid %d using File Initialization", definition->GridID);

					definition->InitializationMethod = File;
					parseFileInitialization(&definition->InitializationParameters.File, source, buffer, MAX_BUFFER);

				}
				break;

				/* random - all grid locs get a random value in given range */
			case 'R': 
				{
					TraceDebug("Initializing grid %d using Random Initialization", definition->GridID);

					definition->InitializationMethod = Random;
					parseRandomInitialization(&definition->InitializationParameters.Random, source, buffer, MAX_BUFFER);
				}
				break;
#if 0

				/* computed - as yet unimplemented */
			case 'C': 
				{
					TraceVerbose("Initializing grid %d using Computed Initialization", support_ptr->id);
					TraceError("Computed Initialization not yet implemented");
					exit(1);
				}
				break;

				/* distributed - all grid locs get one of a list of values */
			case 'D': 
				{
					TraceVerbose("Initializing grid %d using Distributed Initialization", support_ptr->id);

					strip_read_file(buffer, support_file);
					int num_dis_vals = atoi(buffer);

					if (num_dis_vals > 50)
					{
						TraceError("The specified number of values to distribute (%d) is greater than the maximum number allowed (%d)", num_dis_vals, 50);
						exit(1);
					}

					float dist_vals[50];
					for (int count=0;count<num_dis_vals;count++)
					{
						strip_read_file(buffer, support_file);
						dist_vals[count] = atof(buffer);
						TraceDebug("Stored %f for distribution in grid %d", dist_vals[count], support_ptr->id); 
					}

					distribute_init(support_ptr, num_dis_vals, dist_vals);
				}
				break;

				/* proportional - all locs get a user-specified proportion */
			case 'P': 
				{
					TraceVerbose("Initializing grid %d using Proportional Initialization", support_ptr->id);
					strip_read_file(buffer, support_file);
					int num_prop_vals = atoi(buffer);

					if (num_prop_vals > 50)
					{
						TraceError("The specified number of values (%d) is greater than the maximum number allowed (%d)", num_prop_vals, 50);
						exit(1);
					}

					float init_prop = 0;

					float prop_vals[30][2];
					for (int count=0;count<num_prop_vals;count++)
					{
						strip_read_file(buffer, support_file);
						prop_vals[count][0] = atof(buffer);

						strip_read_file(buffer, support_file);
						init_prop  = init_prop + atof(buffer);
						prop_vals[count][1] = init_prop;

						TraceDebug("Stored %f for %f distribution in grid %d", prop_vals[count][0], prop_vals[count][1], support_ptr->id);
					}

					proportion_init(support_ptr, num_prop_vals, prop_vals);
				}
				break;

				/* "toss" - randomly places a set number of values in a region*/
			case 'T': 
				{
					TraceVerbose("Initializing grid %d using Toss Initialization", support_ptr->id);
					/* number of values to place */
					strip_read_file(buffer, support_file);
					int num_vals = atoi(buffer);

					/* numeric value to place */
					strip_read_file(buffer, support_file);
					int val_number = atoi(buffer);

					/* default value to place */
					strip_read_file(buffer, support_file);
					int default_val = atoi(buffer);

					/* region in which to place the values */
					strip_read_file(buffer, support_file);
					int low_left_row = atoi(buffer);
					strip_read_file(buffer, support_file);
					int low_left_col = atoi(buffer);
					strip_read_file(buffer, support_file);
					int up_right_row= atoi(buffer);
					strip_read_file(buffer, support_file);
					int up_right_col= atoi(buffer);


					toss_init(support_ptr, num_vals, val_number, default_val,
							low_left_row, low_left_col, up_right_row,
							up_right_col);
				}
				break;

				/* "restricted-toss" - randomly places a set number of values in a 
				   within the difference between 2 regions */
			case 'H':   /* H is for "hole" places the objects in a 'holed' region */
				{
					TraceVerbose("Initializing grid %d using Restricted Toss Initialization", support_ptr->id);
					/* number of values to place */
					strip_read_file(buffer, support_file);
					int num_vals = atoi(buffer);

					/* numeric value to place */
					strip_read_file(buffer, support_file);
					int val_number = atoi(buffer);

					/* default value to place */
					strip_read_file(buffer, support_file);
					int default_val = atoi(buffer);

					/* region in which to place the values */
					strip_read_file(buffer, support_file);
					int low_left_row = atoi(buffer);
					strip_read_file(buffer, support_file);
					int low_left_col = atoi(buffer);
					strip_read_file(buffer, support_file);
					int up_right_row= atoi(buffer);
					strip_read_file(buffer, support_file);
					int up_right_col= atoi(buffer);

					/* region in which to place the values */
					strip_read_file(buffer, support_file);
					int hole_ll_row = atoi(buffer);
					strip_read_file(buffer, support_file);
					int hole_ll_col = atoi(buffer);
					strip_read_file(buffer, support_file);
					int hole_ur_row= atoi(buffer);
					strip_read_file(buffer, support_file);
					int hole_ur_col= atoi(buffer);


					restricted_toss_init(support_ptr, num_vals, val_number, 
							default_val, low_left_row, low_left_col, up_right_row,
							up_right_col, hole_ll_row, hole_ll_col, hole_ur_row,
							hole_ur_col);
				}
				break;

				///#if 0
				///				/* block - grid broken into regions that match given values */
				///			case 'B': 
				///				/* get number of blocks in a row */
				///				strip_read_file(buffer, support_file);
				///				rowblox = atoi(buffer);
				///
				///				/* get number of blocks in a column */
				///				strip_read_file(buffer, support_file);
				///				colblox = atoi(buffer);
				///
				///				/* get the values that correspond to each block */
				///				for (i=0; i<rowblox;i++){
				///					for (j=0; j<colblox; j++){
				///
				///						strip_read_file(buffer, support_file);
				///						block_vals[i][j] = atof(buffer);
				///					}
				///				}
				///
				///				/* put block values in the grid */
				///				block_init(support_ptr, rowblox, colblox, block_vals);
				///				break;
				///#endif
#endif
			default: 
				{
					TraceError("Unknown initialization type encountered: '%s'", buffer);
					exit(1);
				}
				break;
		}
#if 0

		fgets(buffer, MAX_BUFFER, support_file);

		/* determines method of UPDATE for each support grid */
		switch (buffer[0])
		{
			/*  "S"tatic updates */
			case 'S': 
				{
					TraceVerbose("Grid update method set to Static Update");
					support_ptr->update_grid = no_update;
					support_ptr->num_params = 0;
				}
				break;

				/* Clear grid update */		 
			case 'X': 
				{
					TraceVerbose("Grid update method set to Clear Update");
					support_ptr->update_grid = clear_update;
					support_ptr->num_params = 0;
				}
				break;

				/* "M"ovement update */
			case 'M': 
				{
					TraceVerbose("Grid update method set to Move Update");
					support_ptr->update_grid = move_update;
					support_ptr->num_params = 2;

					strip_read_file(buffer, support_file);

					/* copies x-direction movement to parameter buffer */
					char *str_temp = (char *)malloc(strlen(buffer));
					strcpy(str_temp, buffer);

					support_ptr->update_params[0] = str_temp;

					strip_read_file(buffer, support_file);

					/* copies y-direction movement to parameter buffer */
					str_temp = (char *)malloc(strlen(buffer));
					strcpy(str_temp, buffer);

					support_ptr->update_params[1] = str_temp;
				}
				break;

				/* "D"iffusion update */
			case 'D': /* chemical diffusion with decay update */
				{
					TraceVerbose("Grid update method set to Diffusion Update");
					support_ptr->update_grid = diffuse_update;
					support_ptr->num_params = 2;

					strip_read_file(buffer, support_file);

					/* copies diffusion rate parameter buffer */
					char* str_temp = (char *)malloc(strlen(buffer));
					strcpy(str_temp, buffer);

					support_ptr->update_params[0] = str_temp;

					strip_read_file(buffer, support_file);

					/* copies decay rate to parameter buffer */
					str_temp = (char *)malloc(strlen(buffer));
					strcpy(str_temp, buffer);

					support_ptr->update_params[1] = str_temp;
				}
				break;
			case 'C': 
				{
					TraceError("Cycle Update not yet implemented");
					exit(1);
					//support_ptr->update_grid = cycle_update;
				}
				break;
			case 'L': 
				{
					TraceError("Library Update not yet implemented");
					exit(1);
					// support_ptr->update_grid = library_update;
				}
				break;
			case 'E': 
				{
					TraceError("'E' Update not yet implemented");
					exit(1);
				}
				break; 
			default:
				{
					TraceError("Unknown update method '%c' (%s) encountered", buffer[0], buffer);
					exit(1);
				}
		}
	}

	// todo: see if there is a better way than `exit(1)` to gracefully exit on an error
	return all_grids_ptr;
#endif
}
}

void nextLine(InputDataSource* source, char buffer[], size_t length)
{
	ReadLine(source, buffer, length);
	trim_comments(buffer, length);
}

void parseUniformInitialization(UniformInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length )
{
	TraceVerbose("Parsing information for Uniform initialization");
	nextLine(source,buffer,length);
	
	// todo: we read in a float, cast it to and int, then pass it as a float...
	// currently keeping for "backwards compatiblity" or until I figure out why
	parameters->InitializationValue = atof(buffer);
}

void parseFileInitialization(FileInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length )
{
	TraceVerbose("Parsing information for File initialization");
	nextLine(source,parameters->FilePath, MAX_FILEPATH_LENGTH);

	// XXX: this should probably be read into a buffer first and verified
	// XXX: can we reasonably check that the file exists here (look up stat()/access()/open())
}

void parseRandomInitialization(RandomInitializationParameters* parameters, InputDataSource *source, char buffer[], size_t length )
{
	TraceVerbose("Parsing information for Random initialization");

	nextLine(source,buffer,length);

	// xxx: again, reading in floats and casting down to int...why?
	// int beg_range = atof(buffer);
	//
	parameters->MinimumRandomValue = atof(buffer);

	nextLine(source,buffer,length);

	parameters->MaximumRandomValue = atof(buffer);
	TraceVerbose("RandomInitializationParameters.MinimumRandomValue = %f", parameters->MinimumRandomValue);
	TraceVerbose("RandomInitializationParameters.MaximumRandomValue = %f", parameters->MaximumRandomValue);

}
