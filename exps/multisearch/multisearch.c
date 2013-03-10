void scan(agent_struct *agent, int action_index)
{

  int i, j;  /*Row and column counters*/

  int row;     /*Starting positions for the scan area*/
  int column;

  int row_max;
  int col_max;  /*Upper bounds for row and col, set by GGETR and GGETC*/

  int there = 0;   /*0 if the target is not found, 1 if found */

  int state;

  function_struct temp_func;
  update_grid_struct temp_grid;

  row_max = GGETR(0);
  col_max = GGETC(0);

 /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */  

  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */

  temp_grid = temp_func.update_grids[0];


  row = (int)(40 * ((double)rand()/(double)RAND_MAX));
  column  = (int)(40 * ((double)rand()/(double)RAND_MAX)); 

  for(i = row; i < (row + 5); i++){
    for(j = column; j < (column + 5); j++){
	
      if(i >= row_max || j >= col_max){

	break;

      }

      if(GGET(temp_grid.grid, i, j) == 1){

        there = 1;
	break;

      }

      GSET(temp_grid.grid, i, j, 2);

    }

    if (there != 0){
      break;
    }
  }

  if (there == 0){

    //    printf("The target was not located in the grid starting at (%d,%d)\n", row, column);

  } else {

    printf("The target was located in the grid starting at (%d,%d)\n", row, column);

    GSET(temp_grid.grid, agent->y_pos, agent->x_pos, 3);

  }

}










