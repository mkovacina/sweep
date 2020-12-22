void set_neighborhood(agent_struct *agent, int action_index)
/*  Generates a firefly "flash" within a limited area of the grid */
{
  int Xloc, Yloc, rows, cols, state, action_grid;
  int rad, rad_sqrd, row, col;

  function_struct temp_func;

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  action_grid = temp_func.update_grids[0].grid;

  rows = GGETR(0);
  cols = GGETC(0);

  /* fills in a circular region (of radius 6) around the agent with 1's */
  rad = 8;
  rad_sqrd = rad * rad;

  for (row = Yloc+rad; row > Yloc-rad; row--){
    for  (col = Xloc+rad; col > Xloc-rad; col--){
      if (row >= 0 && row < rows && col >= 0 && col < cols &&
	  (row-Yloc)*(row-Yloc) + (col-Xloc)*(col-Xloc) <= rad_sqrd &&
	  (row != Yloc || col != Xloc)){
	 GSET(action_grid, row, col, 1);
      }
    }
  }
}
