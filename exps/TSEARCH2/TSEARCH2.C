void initialize(agent_struct *agent, int action_index){

  int row;     /*Starting positions for the scan area*/
  int column;

  row = (int)(40 * ((double)rand()/(double)RAND_MAX));
  column  = (int)(40 * ((double)rand()/(double)RAND_MAX)); 

  agent->field[1] = row;
  agent->field[2] = column;

  agent->field[3] = 2;    /*Row offset*/
  agent->field[4] = 2;    /*Colum offset*/

}

void bounce(agent_struct *agent, int action_index){

  int wall; /* Holds value indicating which wall the agent hit. */

  /* acquire the FSA state of the current agent */
  wall = agent->fsa->current_state;   /*This determines which wall was hit */

  if(wall == 1 || wall == 3){

    switch (agent->dir) {
    case 1: agent->dir = 3; 
      break;
    case 3: agent->dir = 1; 
      break;
    }

    switch (agent->switch_dir) {
    case 1: agent->switch_dir = 3; 
      break;
    case 3: agent->switch_dir = 1; 
      break;
    }
  }
  if(wall == 2 || wall == 3){

    switch (agent->dir) {
    case 0: agent->dir = 2; 
      break;
    case 2: agent->dir = 0; 
      break;
    }
    
    switch (agent->switch_dir) {
    case 0: agent->switch_dir = 2; 
      break;
    case 2: agent->switch_dir = 0; 
      break;
    }
  }

  if(wall == 4){

    if(agent->dir == 0){

      agent->dir = 2;

    } 
    if(agent->switch_dir == 0){
      
      agent->switch_dir = 2;

    }
  }
}


void scan(agent_struct *agent, int action_index){

  int i, j;  /*Row and column counters*/

  int row;     /*Starting positions for the scan area*/
  int column;

  int row_max;
  int col_max;  /*Upper bounds for row and col, set by GGETR and GGETC*/

  int there = 0;   /*0 if the target is not found, 1 if found */

  int state;

  float distance;

  float radius = 1.5;  /*Radius of search area */

  function_struct temp_func;
  update_grid_struct temp_grid;

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

  for(i = (row - radius); i <= (row + radius); i++){
    for(j = (column - radius); j <= (column + radius); j++){

      /*Check to make sure agent is looking at searchable area*/
      if((i < 1 || j < 0) || (i >= row_max || j >= col_max)){
	continue;
      }

      distance = sqrt(((row - i) * (row - i)) + ((column - j) * (column - j)));

      //  printf("Distance is %f, row, col: %d %d, i, j: %d %d\n", distance, row, column, i, j);

      if( distance > radius ){
	continue;
      }

      if(GGET(temp_grid.grid, i, j) == 1){
        there = 1;
      }

      if(GGET(temp_grid.grid, i, j) == 0 || GGET(temp_grid.grid, i, j) == 2){
	GSET(temp_grid.grid, i, j, 2);
      }

    }
  }

  if (there == 1){

    //  printf("%d  %d %d\n", (int)agent->field[5], row, column);
   
    GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 5);

    //    printf("Target has been found, %d %d\n", row, column);

  } else {


    if((agent->field[6] > 5) && GCURR(temp_grid.grid, agent->y_pos, agent->x_pos) != 3){
      
      GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 4);

    }  

    agent->field[2] = agent->field[2] + agent->field[4];    /* Moves the search area over by the offset amount */

    if(agent->field[2] >= col_max || agent->field[2] < 0 ){   /*This handles the case where the search area is off the grid*/

      agent->field[4] = -agent->field[4];

      if(agent->field[2] < 0){

	agent->field[2] = 0;
      
      }  else {

	agent->field[2] = agent->field[2] + agent->field[4];   

      }

      agent->field[1] = agent->field[1] + agent->field[3];

    }

    if(agent->field[1] >= row_max  || agent->field[1] < 2 ){   

      agent->field[3] = -agent->field[3];

      if(agent->field[1] < 2){

	agent->field[1] = 2;
      
      }  else {

	agent->field[1] = agent->field[1] + agent->field[3];

      }

      agent->field[2] = agent->field[2] + agent->field[4];

    }
  }
}

void look(agent_struct *agent, int action_index){

  int i, j;  /*Row and column counters*/

  int row;     /*Starting positions for the scan area*/
  int column;

  int row_max;
  int col_max;  /*Upper bounds for row and col, set by GGETR and GGETC*/

  int there = 0;   /*0 if the target is not found, 1 if found */

  int state;

  float distance;
  float radius = 3.0;

  function_struct temp_func;
  update_grid_struct temp_grid;

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

  for(i = (row - radius); i <= (row + radius); i++){
    for(j = (column - radius); j <= (column + radius); j++){

      /*Check to make sure agent is looking at searchable area*/
      if((i < 1 || j < 0) || (i >= row_max || j >= col_max)){
	continue;
      }

      distance = sqrt(((row - i) * (row - i)) + ((column - j) * (column - j)));

      //  printf("Distance is %f, row, col: %d %d, i, j: %d %d\n", distance, row, column, i, j);

      if( distance > radius ){
	continue;
      }

      if(GGET(temp_grid.grid, i, j) == 1){
        there = 1;
      }

      if(GGET(temp_grid.grid, i, j) == 0 || GGET(temp_grid.grid, i, j) == 2){
	GSET(temp_grid.grid, i, j, 2);
      }

    }
  }

  if (there == 1){

    //  printf("%d  %d %d\n", (int)agent->field[5], row, column);
   
    GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 5);

    //    printf("Target has been found, %d %d\n", row, column);

  } else {


    if((agent->field[6] > 5) && GCURR(temp_grid.grid, agent->y_pos, agent->x_pos) != 3){
      
      GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 4);

    }  

    agent->field[2] = agent->field[2] + agent->field[4];    /* Moves the search area over by the offset amount */

    if(agent->field[2] >= col_max || agent->field[2] < 0 ){   /*This handles the case where the search area is off the grid*/

      agent->field[4] = -agent->field[4];

      if(agent->field[2] < 0){

	agent->field[2] = 0;
      
      }  else {

	agent->field[2] = agent->field[2] + agent->field[4];   

      }

      agent->field[1] = agent->field[1] + agent->field[3];

    }

    if(agent->field[1] >= row_max  || agent->field[1] < 2 ){   

      agent->field[3] = -agent->field[3];

      if(agent->field[1] < 2){

	agent->field[1] = 2;
      
      }  else {

	agent->field[1] = agent->field[1] + agent->field[3];

      }

      agent->field[2] = agent->field[2] + agent->field[4];

    }
  }
}


void offset(int *row, int *column){
 
  int row_off, col_off;   /*Row and col offsets*/
  int amount = 2;  /* Offset amount */

  row_off = (int)((amount * 2) * ((double)rand()/(double)RAND_MAX)) - amount;
  col_off  = (int)((amount * 2) * ((double)rand()/(double)RAND_MAX)) - amount; 

  *row = *row + row_off;
  *column = *column + col_off;

}


void location(agent_struct *agent, int action_index){ 

  int row, column;

  FILE * loc_file;

  //  printf("I am about to read the file\n");

  loc_file = fopen("location.dat", "r");

  if (loc_file == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  fscanf(loc_file, "%d %d", &row, &column);

  // printf("These are the values in the file: %d %d\n", row, column);

  fclose(loc_file);

  offset(&row, &column);

  agent->field[1] = row;
  agent->field[2] = column;

}


void offset2(int *row, int *column){
 
  int row_off = 0, col_off = 0;   /*Row and col offsets*/
  int amount = 5;

  while(((row_off <= 5) && (row_off >= -5)) && ((col_off <= 5) && (col_off >= -5))){
 
    row_off = (int)((amount * 2) * ((double)rand()/(double)RAND_MAX)) - amount;
    col_off  = (int)((amount * 2) * ((double)rand()/(double)RAND_MAX)) - amount; 

  }

  *row = *row + row_off;
  *column = *column + col_off;

}



void location2(agent_struct *agent, int action_index){ 

  int row, column;

  FILE * loc_file;

  //  printf("I am about to read the file\n");

  loc_file = fopen("location.dat", "r");

  if (loc_file == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  fscanf(loc_file, "%d %d", &row, &column);

  // printf("These are the values in the file: %d %d\n", row, column);

  fclose(loc_file);

  offset2(&row, &column);

  agent->field[1] = row;
  agent->field[2] = column;

}


void target(agent_struct *agent, int action_index){ 

  int row, column;
  int i, j;  /*Row and column counters*/    
  int state;

  function_struct temp_func;

  update_grid_struct temp_grid;       

  FILE * loc_file;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */

  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */

  temp_grid = temp_func.update_grids[0];

  loc_file = fopen("location.dat", "w");

  //  printf("About to write the file\n");

  if (loc_file == NULL){
    printf("Unsuccessful opening of the file, ending program\n");
    exit(1);
  }

  row = agent->field[1];
  column = agent->field[2];

  //  printf("Row %d, Column %d\n", row, column);

  fprintf(loc_file, "%d %d", row, column);

  fclose(loc_file);

  for(i = 0; i < 1; i++){
    for(j = 0; j < 20; j++){
      
      if(GCURR(temp_grid.grid, i, j) != 1  && GCURR(temp_grid.grid, i, j) != 5){
	
	GSET(temp_grid.grid, i, j, 3);
	
      }
    }
  }



}


