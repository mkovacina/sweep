void initialize(agent_struct *agent, int action_index){

  int row;     /*Starting positions for the scan area*/
  int column;

  row = (int)(40 * ((double)rand()/(double)RAND_MAX));
  column  = (int)(40 * ((double)rand()/(double)RAND_MAX)); 

  agent->field[1] = row;
  agent->field[2] = column;

  agent->field[3] = 4;    /*Row offset*/
  agent->field[4] = 4;    /*Colum offset*/

}

void out_search(int target[5][5])
{
  int i, j;
  
  printf("vvvvvvvvvvvvvvvvvvvvvvv\n");
  for (i=0;i<5;i++){
    for (j=0;j<5;j++){
      printf(" %d", target[i][j]);
    }
    printf("\n");
  }
  printf("^^^^^^^^^^^^^^^^^^^^^^^\n");
}

void scan(agent_struct *agent, int action_index){

  int i, j;  /*Row and column counters*/

  int target[5][5];  /* Array to hold info about the target */
  int row_beg, col_beg, row_end, col_end;  /*Store row and column intersections */
  int locator = 0;
  int row_fd, col_fd;
  int found;

  int row;     /*Starting positions for the scan area*/
  int column;

  int row_max;
  int col_max;  /*Upper bounds for row and col, set by GGETR and GGETC*/

  int there = 0;   /*0 if the target is not found, 1 if found */

  int state;

  function_struct temp_func;
  update_grid_struct temp_grid;

  static int count = 0;   /*Used to determine whether to write the location to a file*/

  FILE * loc_file;

  row_max = GGETR(0);
  col_max = GGETC(0);

  row = (int)agent->field[1];
  column = (int)agent->field[2];


 /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */  

  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */

  temp_grid = temp_func.update_grids[0];

  for(i = row; i < (row + 5); i++){
    for(j = column; j < (column + 5); j++){
	
      if(i >= row_max || j >= col_max){

	break;

      } else if(i < 0 || j < 0){

	break;

      }

      if(GGET(temp_grid.grid, i, j) == 1){

        there = 1;
	printf("Found it at location %d %d\n", i,j);
	break;

      }

      GSET(temp_grid.grid, i, j, 2);

    }

    if (there != 0){
      break;
    }
  }

  if (count == 1) {   /* This code segment updates the file and checks to see if the target has been located */

    printf("Agent searching at row %d, column %d\n", row, column);

    if(there == 1){

      printf("Target found in the searched area.\n");

    }

    loc_file = fopen("location.dat", "r");

    if (loc_file == NULL){
      printf("Unsuccessful opening of the file, program ending\n");
      exit(1);
    }

    fscanf(loc_file, "%d %d", &row, &column);

    printf("Searched grid begins at %d %d\n", row, column);

    for(i = 0; i < 5; i++){
      for(j = 0; j < 5; j++){

        fscanf(loc_file, "%d", &target[i][j]);

      }
    }

    fclose(loc_file);

    loc_file = fopen("location.dat", "w");

    if (loc_file == NULL){
      printf("Unsuccessful opening of the search file for writing\n");
      printf("Program ending\n");
      exit(1);
    }    

    fprintf(loc_file, "%d %d\n", row, column);    

    if( row >= agent->field[1] ) {

      row_beg = 0;
      row_end = (int)(agent->field[1]) + 5 - row;

      if(row_end > 5){
	row_end = 5;
      } else if (row_end < 0){
	row_end = 0;
      }

    } else {

      row_beg =  (int)(agent->field[1]) - row;
      row_end = 5;

      if(row_beg < 0){
	row_beg = 0;
      } else if (row_beg > 5){
	row_beg = 5;
      }

    }

    if (column >= agent->field[2]) {

      col_beg = 0;
      col_end = (int)(agent->field[2]) + 5 - column;

      if(col_end > 5){
	col_end = 5;
      } else if(col_end < 0){
	col_end = 0;
      }

    } else {

      col_beg = (int)(agent->field[2]) - column;
      col_end = 5;

      if(col_beg < 0){
	col_beg = 0;
      } else if(col_beg > 5){
	col_beg = 5;
      }

    }

    printf("Intersection is a grid from %d, %d to %d, %d\n", row_beg, col_beg, row_end, col_end);

    if(there == 1){

      for(i = 0; i < 5; i++){
	for(j = 0; j < 5; j++){

	  if((i < row_beg || i >= row_end) || (j < col_beg || j >= col_end)){
	    
	    target[i][j] = 0;
	  }
	}
      }
    } else {
      
      for(i = row_beg; i < row_end; i++){
	for(j = col_beg; j < col_end; j++){
	  
	  target[i][j] = 0;

	}
      }
    }


    for(i = 0; i < 5; i++){
      for(j = 0; j < 5; j++){

        fprintf(loc_file, "%d ", target[i][j]);

	locator = locator + target[i][j];

      }

      fprintf(loc_file, "\n");

    }

    out_search(target);

    fclose(loc_file);

    if (locator == 1) {

      i = 0;
      j = 0;
      found = 0;

      while(i < 5 && found == 0){
	while(j < 5 && found == 0){

	  printf("In while loops, target %d,%d is %d\n", i, j, target[i][j]);

	  if(target[i][j] == 1){

	    found = 1;
	    row_fd = i;
	    col_fd = j;

	    printf("Row found is %d, column found is %d\n", row_fd, col_fd);

	  }

	  j++;
	}
	j = 0;
	printf("In out loop, target %d,%d is %d\n", i, j, target[i][j]);
	i++;
      }

      printf("Location of target has been assessed.\n");
      printf("Target is located at: %d %d\n", (row + row_fd), (column + col_fd));

      for(i = 18; i < 23; i++){
        for(j = 18; j < 23; j++){

	  if(GGET(temp_grid.grid, i, j) != 1){
	  
	    GSET(temp_grid.grid, i, j, 3);

	  }

	  GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 3);
        }
      }

    }


  } else if (there == 1) {      /* This is the case where count != 1 and the target has been found */

    printf("The target was located in the grid starting at (%d,%d)\n", row, column);

    loc_file = fopen("location.dat", "w");


    if (loc_file == NULL){
      printf("Unsuccessful opening of the file for writing - initial position setting\n");
      exit(1);
    }

    fprintf(loc_file, "%d %d\n", (int)(agent->field[1]), (int)(agent->field[2]));

    for(i = 0; i < 5; i++){
      for(j = 0; j < 5; j++){

	fprintf(loc_file, "%d ", 1);
      }

      fprintf(loc_file, "\n");

    }

    fclose(loc_file);
    
    count = 1;
    
    for(i = 18; i < 23; i++){
      for(j = 18; j < 23; j++){
	
	if(GGET(temp_grid.grid, i, j) != 1){
	  
	  GSET(temp_grid.grid, i, j, 3);
	  
	}
      }
    }
    
    GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 3);

  }


  agent->field[2] = agent->field[2] + agent->field[4];    /* Moves the search area over by the offset amount */

  if(agent->field[2] >= col_max || agent->field[2] < 0 ){   /*This handles the case where the search area is off the grid */

    agent->field[4] = -agent->field[4];

    if(agent->field[2] < 0){

      agent->field[2] = 0;
      
    }  else {

      agent->field[2] = agent->field[2] + agent->field[4];   

    }

    agent->field[1] = agent->field[1] + agent->field[3];

  }

  if(agent->field[1] >= row_max  || agent->field[1] < 0 ){   

    agent->field[3] = -agent->field[3];

    if(agent->field[1] < 0){

      agent->field[1] = 0;
      
    }  else {

      agent->field[1] = agent->field[1] + agent->field[3];

    }

    agent->field[2] = agent->field[2] + agent->field[4];

  }

}


void location(agent_struct *agent, int action_index){

  int row, column;
  int row_off, col_off;   /*Row and col offsets*/

  FILE * loc_file;

  loc_file = fopen("location.dat", "r");


  if (loc_file == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  fscanf(loc_file, "%d %d", &row, &column);

  fclose(loc_file);

  row_off = (int)(8 * ((double)rand()/(double)RAND_MAX)) - 4;
  col_off  = (int)(8 * ((double)rand()/(double)RAND_MAX)) - 4; 

  agent->field[1] = row + row_off;
  agent->field[2] = column + col_off;

}








