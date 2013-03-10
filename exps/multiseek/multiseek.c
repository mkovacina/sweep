/* one custom action */

void actual_fill(int grid, int row, int col, float value){

      
  if(GCURR(grid, row, col) == 1){

        GSET(grid, row, col, value);
	
	actual_fill(grid, row+1, col, value);
	actual_fill(grid, row-1, col, value);
	actual_fill(grid, row, col+1, value);
	actual_fill(grid, row, col-1, value);
  }
}


void flood_fill(agent_struct *agent, int action_index)
/* places a specified value at the current location of a coag on a specified 
   support grid.  The specified information is all contained in the coag */
{
    
  int                   row, col, state, i;
  function_struct       temp_func;
  update_grid_struct    temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

#if 0
  printf("row,col location of the agent is: %d %d\n", row, col);
#endif

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

#if 0
  printf("Current state of the FSA is: %d\n", state);
#endif

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  for (i=0; i<temp_func.number_grids; i++){

      temp_grid = temp_func.update_grids[i];

#if 0
      printf("In loop, iteration %d\n", i);
      printf("GRID %d PRIOR TO UPDATE\n", temp_grid.grid);
      verify_grid_access();

      /* show_support_grid(temp_grid.grid); */

      printf("Found temp_grid information to update\n");
      /* sets a given location on a given support grid to a specific value */

      printf("About to modify a support grid with %f\n", temp_grid.grid_value);

      printf("Previous value at that location is:");
      printf("%f\n", 
        ((*(*all_support_grids->grids)[temp_grid.grid]->curr_grid)[row][col])
      );
#endif

      actual_fill(temp_grid.grid, row, col, temp_grid.grid_value);

#if 0
      printf("Updated  value at that location is:");
      printf("%f\n", 
        ((*(*all_support_grids->grids)[temp_grid.grid]->curr_grid)[row][col])
      );

      printf("GRID %d AFTER UPDATING\n",temp_grid.grid);
      /* show_support_grid(temp_grid.grid); */
#endif
  }

# ifdef error

  printf("\nPlace");

# endif

}
















