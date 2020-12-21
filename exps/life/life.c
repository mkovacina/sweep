void set_neighborhood(agent_struct *agent, int action_index)
/*  Generates a firefly "flash" within a limited area of the grid */
{
  /* acquire the position of the current agent */
  int Xloc = agent->x_pos;
  int Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  int state = agent->fsa->current_state;

  /* determines which support grid contains pertinent information */
  /* by accessing the support grid to be updated by this function */
  int action_grid = agent->state_functions[state].function_list[action_index].update_grids[0].grid;

  int rows = GGETR(0);
  int cols = GGETC(0);

  for (int row = Yloc-1; row < Yloc+2; row++)
  {
    for  (int col = Xloc-1; col < Xloc+2; col++)
	{
      if (row >= 0 && row < rows && col >= 0 && col < cols && (row != Yloc || col != Xloc))
	  {
		  GSET_CUM(action_grid, row, col, 1);
      }
    }
  }
}
