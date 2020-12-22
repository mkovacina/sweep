/* This program reads in an update support file, and initializes the support
   grids to correspond to the characteristics given in the file 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trace.h"
#include "support_grids.h"


#include "sweep.h"


#define CGRID(g,r,c) ((*((g)->curr_grid))[r][c])
#define PGRID(g,r,c) ((*((g)->prev_grid))[r][c])

sgrid_ptr cleargrids[50];
int cleargrid = 0;

void strip_read_file(char buffer[], FILE *file)
/* This function reads a value from the given file and strips the comments */
{
     int i;

     /* reads the next line from the specified file */
     fgets(buffer, MAX_BUFFER, file);

     /* strips comments from the message buffer */
     for(i=0;buffer[i]!=' ' && buffer[i]!='\n';i++);
     buffer[i] = '\0';
}


void uniform_init(sgrid_ptr grid, float val)
/* initializes a support grid to a given value */
{
	TraceVerbose("Initializing support grid %d with value %f", grid->id, val);

   for( int row=0; row<grid->rows;row++ )
   {
      for( int col=0; col<grid->cols;col++ )
	  {
		  (*(grid->curr_grid))[row][col] = val;
      }
   }
}

void file_init(sgrid_ptr grid, FILE *infile)
/* initializes a support grid to values specfied in a file */
{
   int row, col;
   int val;

   for(row=0; row<grid->rows;row++){
      for(col=0; col<grid->cols; col++){
	 fscanf(infile, "%d", &val);
	 
	 /* put values in both the past and future support grids */
	 //(*(grid->curr_grid))[row][col] = val;
	 // why are we writing to curr_grid
	 // the rule is "read from the past, write to the future"
	 (*(grid->prev_grid))[row][col] = val;
      }
   }
}

void random_init(sgrid_ptr grid, int first, int last)
{
   int row, col;

   for(row=0; row<grid->rows;row++){
      for(col=0; col<grid->cols; col++){
	 (*(grid->curr_grid))[row][col] = first+
	                                  ((double)rand()/(double)RAND_MAX)*
	                                  (last-first+1);
      }
   }
}

void distribute_init(sgrid_ptr grid, int num_vals, float list[])
/* randomly distributes a given set of values across support grid */
{
   int row, col;

   for(row=0; row<grid->rows;row++){
      for(col=0; col<grid->cols; col++){
	 (*(grid->curr_grid))[row][col] = 
	         list[(int)(num_vals*((double)rand()/(double)RAND_MAX))];
      }
   }
}

void proportion_init(sgrid_ptr grid, int size, float val_prob[][2])
/* distributes a given set of values across the support grid in specified
   proportions */
{
   int row, col, count;
   int rand_val;

   for(row=0; row<grid->rows;row++){
      for(col=0; col<grid->cols; col++){
         rand_val = (int) (100 * ((double)rand()/(double)RAND_MAX));

	 count = 0;
         while ((rand_val > val_prob[count][1]) && (count < size)){
	       count++;
	 }
	 (*(grid->curr_grid))[row][col] = val_prob[count][0];
      }
   }
}

void toss_init(sgrid_ptr grid, int num_items, int item, int bkgrd, 
	       int llr, int llc, int urr, int urc)
/* randomly "tosses" a specific number of values into a given region of a support grid */
{
   int row, col, count;

   if (llr < 0 || llc < 0 || urr > grid->rows || urc > grid->cols){
      printf("Poorly defined tossing region, program ending\n");
      exit(1);
   }

   for (row=0; row<grid->rows; row++){
      for (col=0; col<grid->cols; col++){
	     (*(grid->curr_grid))[row][col] = bkgrd;
      }
   }

   count = 0;
   
   while (count < num_items){
      
         row = llr + (int) ( (urr-llr) * ((double)rand()/(double)RAND_MAX));
         col = llc + (int) ( (urc-llc) * ((double)rand()/(double)RAND_MAX));
	
	 if ((*(grid->curr_grid))[row][col] == bkgrd){
	    (*(grid->curr_grid))[row][col] = item;
	    count++;
	 }
   }
}

void restricted_toss_init(sgrid_ptr grid, int num_items, int item, int bkgrd, 
	   int llr, int llc, int urr, int urc, int allr, int allc, int aurr, int aurc)
/* randomly "tosses" a specific number of values into a given region but outside of a 
   second given region */
{
   int row, col, count;

   if (llr < 0 || llc < 0 || urr > grid->rows || urc > grid->cols){
      printf("Poorly defined tossing region, program ending\n");
      exit(1);
   }

   if (allr < 0 || allc < 0 || aurr > grid->rows || aurc > grid->cols){
      printf("Poorly defined 'hole' in tossing region, program ending\n");
      exit(1);
   }

   if (allr < llr  || allc < llc || aurr > urr || aurc > urc){
      printf("'Hole' extends outside tossing region, program ending\n");
      exit(1);
   }

   for (row=0; row<grid->rows; row++){
      for (col=0; col<grid->cols; col++){
	     (*(grid->curr_grid))[row][col] = bkgrd;
      }
   }

   count = 0;
   
   while (count < num_items){
      
         /*  randomly selects a location within first region */
         row = llr + (int) ( (urr-llr) * ((double)rand()/(double)RAND_MAX));
         col = llc + (int) ( (urc-llc) * ((double)rand()/(double)RAND_MAX));
	 
	 /* eliminates locations within second region */
	 if (row < allr || row > aurr || col < allc || col > aurc){
	   
 	    /*  if nothing has been placed there yet, place something there */
	    if ((*(grid->curr_grid))[row][col] == bkgrd){
	       (*(grid->curr_grid))[row][col] = item;
	       count++;
	    }
	 }
   }
}

void no_update(sgrid_ptr ignore_grid)
{
   /* this function is a no-op */
}

void app_move_update(sgrid_ptr support_ptr, int r_mov, int c_mov)
{
  int row, col, row_calc, col_calc;
  int d_fault = 0;

  for (row=0;row<support_ptr->rows;row++){
      for (col=0;col<support_ptr->cols;col++){

	  row_calc = row - r_mov;
	  col_calc = col - c_mov;
	  if (row_calc >= 0 && row_calc < support_ptr->rows && 
              col_calc >= 0 && col_calc < support_ptr->cols){

	     (*(support_ptr->prev_grid))[row][col] = 
                       (*(support_ptr->curr_grid))[row_calc][col_calc];
	  }
	  else {
	     (*(support_ptr->prev_grid))[row][col] = d_fault;
	  }
      }
  }

}

void move_update(sgrid_ptr support_ptr)
{
   int r_mov, c_mov;

   r_mov = atoi(support_ptr->update_params[0]);
   c_mov = atoi(support_ptr->update_params[1]);

   app_move_update(support_ptr, r_mov, c_mov);

} 

void app_clear_update(sgrid_ptr support_ptr)
{

#if 0
  int row, col;

  for (row=0;row<support_ptr->rows;row++){
      for (col=0;col<support_ptr->cols;col++){

	  (*(support_ptr->curr_grid))[row][col] = 0;
      }
  }
#endif

  /*
  printf("Setting up a grid to clear: %d\n", cleargrid);
  */

  cleargrids[cleargrid] = support_ptr;
  cleargrid++;

}

void clear_update(sgrid_ptr support_ptr)
{
   app_clear_update(support_ptr);
} 

void app_diffuse_update(sgrid_ptr support_ptr, float diffuse, float decay)
{
   int row, col, rows, cols;
   float diff;

   fgrid_ptr temp_grid;

   rows = support_ptr->rows;
   cols = support_ptr->cols;

   diffuse = diffuse / 4.0;

   for (row=0;row<rows;row++){
       for (col=0;col<cols;col++){
           CGRID(support_ptr,row,col) = 0;
       }	  
   }

   for (row=0;row<rows;row++){
       for (col=0;col<cols;col++){

	   if (row>0) {
	      if (PGRID(support_ptr,row,col) > PGRID(support_ptr,row-1,col)){
		 diff = PGRID(support_ptr,row,col) - 
                              PGRID(support_ptr,row-1,col);
		 CGRID(support_ptr,row,col) -= diffuse * diff;
                 CGRID(support_ptr,row-1,col) += diffuse * diff;
  	      }
	   }

	   if (row<rows-1) {
	      if (PGRID(support_ptr,row,col) > PGRID(support_ptr,row+1,col)){
		 diff = PGRID(support_ptr,row,col) - 
                                    PGRID(support_ptr,row+1,col);
		 CGRID(support_ptr,row,col) -= diffuse * diff;
                 CGRID(support_ptr,row+1,col) += diffuse * diff;
	      }
	   }

	   if (col>0) {
	      if (PGRID(support_ptr,row,col) > PGRID(support_ptr,row,col-1)){
		 diff = PGRID(support_ptr,row,col) - 
                                    PGRID(support_ptr,row,col-1);
		 CGRID(support_ptr,row,col) -= diffuse * diff;
                 CGRID(support_ptr,row,col-1) += diffuse * diff;
	      }
	   }

	   if (col<cols-1) {
	      if (PGRID(support_ptr,row,col) > PGRID(support_ptr,row,col+1)){
		 diff = PGRID(support_ptr,row,col) - 
                                    PGRID(support_ptr,row,col+1);
		 CGRID(support_ptr,row,col) -= diffuse * diff;
                 CGRID(support_ptr,row,col+1) += diffuse * diff;
	      }
	   }

	   
       }	  
   }

   /* update the grid with both the decay factor and the diffusion factor */
   for (row=0;row<rows;row++){
       for (col=0;col<cols;col++){
//	   printf("Loc %d,%d is %f: becomes: ", row, col, 
//                                    PGRID(support_ptr,row,col));
	   PGRID(support_ptr,row,col) -= decay * PGRID(support_ptr,row,col);
	   PGRID(support_ptr,row,col) += CGRID(support_ptr, row, col);

//	   printf("%f\n", PGRID(support_ptr,row,col));

       }	  
   } 

   /* swap grids to override the subsequent swapping of the grids */
   temp_grid = support_ptr->prev_grid;
   support_ptr->prev_grid = support_ptr->curr_grid;
   support_ptr->curr_grid = temp_grid;

}

void diffuse_update(sgrid_ptr support_ptr)
{
   float diffuse_rate, decay_rate;

   diffuse_rate = atof(support_ptr->update_params[0]);
   decay_rate = atof(support_ptr->update_params[1]);

   app_diffuse_update(support_ptr, diffuse_rate, decay_rate);

} 

#if 0
void app_gaus_update(sgrid_ptr support_ptr, float rate, int time)
{
  int rowA, colA, rowB, colB;
  sgrid_ptr aux_grid;
  float sigma, aigma_sqrd;

  aux_grid = support_ptr->co_grid;

  sigma = rate * time;
  sigma_sqrd = sigma * sigma;

  /* re-intialize previous grid */
  for (rowA=0;rowA<support_ptr->rows;rowA++){
      for (colA=0; colA<support_ptr->cols;colA++){
	  support_ptr->prev_grid[rowA][colA] = 0.0;
      }
  }

  for (rowA=0;rowA<aux_grid->rows;rowA++){
      for (colA=0;colA<aux_grid->cols;colA++){

	  /* if there is a source at this location, find its contribution */
	  if (aux_grid->curr_grid[rowA][colA] != 0){

	     /* go through entire grid computing contribution from source */
	     for (rowB = 0; rowB<support_ptr->rows;rowB++){
	         for (colB = 0; colB<support_ptr->cols;colB++){

		     /* compute distance from rowA,colA to rowB, colB */
		     dist = (rowA-rowB)*(rowA-RowB)+(colA-colB)*(colA-colB);

		     /* based on distance, rate & time compute concentration */
		     conc_contr = norm_const * (1/sigma) * 
		                exp(-(1/(2 * sigma_sqrd)) * dist);
		   
		     support_ptr->prev_grid[rowB][colB] += conc_contr;
		 }
	     }
	  }
      }
   }

  /* normalize the concentrations by only taking the maximum value */
  /* this is a "hack" that will be fixed by having repeated injections */
  /* of the chemical into the central region */

  for (rowA=0;rowA<support_ptr->rows;rowA++){
      for (colA=0; colA<support_ptr->cols;colA++){
	  curr_val = support_ptr->curr_grid[rowA][colA];
	  prev_val = support_ptr->prev_grid[rowA][colA];

	  if (curr_val > prev_val){
	     support_ptr->prev_grid[rowA][colA] = curr_val;
	  }
      }
  }
}

void update_gaus(sgrid_ptr support_ptr)
{
   int x_mov, y_mov;

   x_mov = atoi(support_ptr->update_params[0]);
   y_mov = atoi(support_ptr->update_params[1]);

   app_gaus_update(support_ptr, x_mov, y_mov);

} 
#endif

#if 0
void move_cycle(<pointer to a coag>)
{
   /* read in parameters for cycle */
   /* call actual movement update routine */
   /* modify results on the way out */
} 

void move_library(<pointer to a coag>)
{
   /* read in parameters for library routine */
   /* call actual movement update routine */
   /* modify results on the way out */
} 

#endif
fgrid_ptr init_agent_grid()
{
        fgrid_ptr agent_grid;

	agent_grid = (fgrid_ptr)calloc(1, sizeof(feature_grid));
	return agent_grid;
}


s_grids_ptr init_support_grids(char file_name[])
{
	char buffer[MAX_BUFFER];

	s_grids_ptr all_grids_ptr;

	TraceVerbose("Opening support grid file '%s'", file_name);
	FILE* support_file = fopen(file_name, "r");
	if (support_file == NULL) 
	{
		TraceError("Can't open file: %s%c", file_name, 'z');
		exit(1);
	}

	/* Determine how many support grids there are */
	strip_read_file(buffer, support_file);

	int num_support_grids = atoi(buffer);

	TraceDebug("num_support_grids is: %d", num_support_grids);

	/* Get row dimension of the support grids */
	strip_read_file(buffer, support_file);
	int sg_rows = atoi(buffer);
	TraceDebug("sg_rows is %d", sg_rows); 

	/* Get column dimension of the support grids */
	strip_read_file(buffer, support_file);
	int sg_cols = atoi(buffer);
	TraceDebug("sg_cols  is %d", sg_cols);

	all_grids_ptr = (s_grids_ptr)malloc(sizeof(all_grids));

	all_grids_ptr->num_grids = num_support_grids;

	all_grids_ptr->grids = (all_supports)malloc(num_support_grids*sizeof(sgrid_ptr));

	for (int entries=0; entries<num_support_grids;entries++)
	{
		strip_read_file(buffer, support_file);
		int id = atoi(buffer);

		// this enforces that the support grids are number sequentially
		// from 0 in the input file
		if (entries != id){
			TraceError("Processing support grid with supplied ID of %d but expected %d", id, entries);
			exit(1);
		}

		sgrid_ptr support_ptr = (sgrid_ptr) malloc(sizeof(support_grid));

		(*(all_grids_ptr->grids))[entries] = support_ptr;

		fgrid_ptr c_grid = (fgrid_ptr)calloc(1,sizeof(feature_grid));
		fgrid_ptr p_grid = (fgrid_ptr)calloc(1,sizeof(feature_grid));

		/*
		   c_grid = (fgrid_ptr)malloc(sg_rows * sg_cols *sizeof(float));
		   p_grid = (fgrid_ptr)malloc(sg_rows * sg_cols *sizeof(float));
		   */
		support_ptr->curr_grid = c_grid;
		support_ptr->prev_grid = p_grid;
		support_ptr->rows = sg_rows;
		support_ptr->cols = sg_cols;
		support_ptr->id = id;

		// todo: why are we flushing if we are reading? 
		// (Because Mike didn't know what he was doing back then??)
		fflush(support_file);
		fgets(buffer, MAX_BUFFER, support_file);

		/* determines method of INITIALIZATION for each support grid */
		switch (buffer[0])
		{
			/* uniform - all grid locations get same initial value */
			case 'U': 
				{
					TraceVerbose("Initializing grid %d using Uniform Initialization", support_ptr->id); 
					strip_read_file(buffer, support_file);
					int init_val = atof(buffer);
					uniform_init(support_ptr, init_val);
				}
				break;

				/* file - all grid locations specified in a named file */
			case 'F': 
				{
					TraceVerbose("Initializing grid %d using File Initialization", support_ptr->id);

					strip_read_file(buffer, support_file);

					/* open initialization data file */
					FILE *init_file = fopen(buffer, "r");
					if (init_file == NULL) {
						printf("Can't open file: %s\n", buffer);
						exit(1);
					}	

					file_init(support_ptr, init_file);
					fclose(init_file);
				}
				break;

				/* random - all grid locs get a random value in given range */
			case 'R': 
				{
					TraceVerbose("Initializing grid %d using Random Initialization", support_ptr->id);

					strip_read_file(buffer, support_file);

					/* beginning of range of random values to place */
					int beg_range = atof(buffer);

					strip_read_file(buffer, support_file);

					/* end of range of random values to place in grid */
					int end_range = atof(buffer);

					random_init(support_ptr, beg_range, end_range);
				}
				break;

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

#if 0
				/* block - grid broken into regions that match given values */
			case 'B': 
				/* get number of blocks in a row */
				strip_read_file(buffer, support_file);
				rowblox = atoi(buffer);

				/* get number of blocks in a column */
				strip_read_file(buffer, support_file);
				colblox = atoi(buffer);

				/* get the values that correspond to each block */
				for (i=0; i<rowblox;i++){
					for (j=0; j<colblox; j++){

						strip_read_file(buffer, support_file);
						block_vals[i][j] = atof(buffer);
					}
				}

				/* put block values in the grid */
				block_init(support_ptr, rowblox, colblox, block_vals);
				break;
#endif
			default: 
				{
					TraceError("Unknown initialization type encountered: '%s'", buffer);
					exit(1);
				}
				break;
		}

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
}
 
void update_all_grids(void)
{

   /* all_support_grids is a global pointer to the array of pointers to
      all support grids.  It is accessed through an extern definition in
      the header file support_grids.h */

   sgrid_ptr supp_ref;

   int grid_count, num_grids;
   int row, col, count;

   num_grids = all_support_grids->num_grids;

   /* go through each support grid and have it update itself */
   for (grid_count=0;grid_count<num_grids;grid_count++){

       supp_ref = (*(all_support_grids->grids))[grid_count];
       (supp_ref->update_grid)(supp_ref);
   }

   /* after the updates, transfer the information on the current grid to     */
   /* the previous grid, making them identical at the beginning of a time    */
   /* step. Information should *always* be retrieved from the PREVIOUS grid  */
   /* stored in the CURRENT grid.  This loop rectifies the grids so when the */
   /* change is made, both grids are the same */

   for (grid_count=0;grid_count<num_grids;grid_count++){
       supp_ref = (*(all_support_grids->grids))[grid_count];

       for (row=0;row<supp_ref->rows;row++){
           for (col=0; col<supp_ref->cols;col++){
	       (*(supp_ref->prev_grid))[row][col] = 
                                     (*(supp_ref->curr_grid))[row][col];
           }
       }
   }

#if 0
   /* used to test clear_grid() */

   printf("Number of grids to clear: %d\n", cleargrid);

   /* prints out all grids */ 
   display_support();
#endif 
   for (count=0; count<cleargrid;count++){
      for (row=0;row<(cleargrids[count])->rows;row++){
           for (col=0;col<(cleargrids[count])->cols;col++){
	     
	        (*(cleargrids[count])->curr_grid)[row][col] = 0;
	     
	//	(*(cleargrids[count])->prev_grid)[row][col] = 0;
	   }
      }
  }

  cleargrid = 0;
}


