void staysearch(agent_struct *agent, int action_index)
     /* causes an agent to stay in the region that they are currently in, and
	search for a second region specified by the input parameter to the
	function.  Additionally, the agent will perform only "safe" moves */
{
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, curr_region;

  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  rows = GGETR(0);
  cols = GGETC(0);

  /* tests the four possible directions to determine if search value is
     reachable -- if it is, the agent moves there */

  /* check "down" square */
  if (Yloc > 0){
     test_val = GGET(temp_grid.grid, Yloc-1, Xloc);
     if (test_val == temp_grid.grid_value){
       move_up(agent, 0);  /* move agent to Xloc, Yloc-1 */
       return;
     }
  }

  /* check "left" square */
  if (Xloc > 0){
     test_val = GGET(temp_grid.grid, Yloc, Xloc-1);
     if (test_val == temp_grid.grid_value){
       move_left(agent, 0);  /* move agent to Xloc-1, Yloc */
       return;
     }
  }

  /* check "up" square */
  if (Yloc < rows){
     test_val = GGET(temp_grid.grid, Yloc+1, Xloc);
     if (test_val == temp_grid.grid_value){
       move_down(agent, 0);    /* move agent to Xloc, Yloc+1 */
       return;
     }
  }

  /* check "right" square */
  if (Xloc < cols){
     test_val = GGET(temp_grid.grid, Yloc, Xloc+1);
     if (test_val == temp_grid.grid_value){
       move_right(agent, 0); /* move agent to Xloc+1, Yloc */
       return;
     }
  }

  /* if search value was not reachable, go to a random location within the
     same region */

  dir = (int)(4 * ((double)rand()/(double)RAND_MAX));
  count = 0;

  /* current region the agent is in and the one in which it should stay */
  /* agent goes to first adjacent square that is in the same region */

  curr_region = GGET(temp_grid.grid, Yloc, Xloc);

     switch (dir){
         while (count < 4){
         case 0:  if (count<4 && Yloc > 0 && 
		      GGET(temp_grid.grid,Yloc-1,Xloc) == curr_region){
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 1:  if (count<4 && Xloc < cols &&
		      GGET(temp_grid.grid, Yloc, Xloc+1) == curr_region) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 2:  if (count<4 && Yloc < rows &&
		      GGET(temp_grid.grid, Yloc+1, Xloc) == curr_region) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 3:  if (count<4 && Xloc > 0 &&
		      GGET(temp_grid.grid, Yloc, Xloc-1) == curr_region) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        move_left(agent,0); 
		        return;
		     }
	          }
	          count++;
     }
  }
}

void wander_avoid(agent_struct *agent, int action_index)
     /* causes an agent to safely wander while avoiding a particular region */
{
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, avoid_region;

  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  rows = GGETR(0);
  cols = GGETC(0);

  /* tests the four possible directions to determine if avoid value is
     there -- if it is not, then the agent moves there */

  dir = (int)(4 * ((double)rand()/(double)RAND_MAX));
  count = 0;

  /* current region the agent is in and the one in which it should stay */
  /* agent goes to first adjacent square that is in the same region */

  avoid_region = temp_grid.grid_value;

#if 0
  printf("Value to avoid is: %d\n", avoid_region);
  printf("Surrounding values are:\n");
  printf("\n\n");
  printf("            %d\n", (int)GGET(temp_grid.grid,Yloc-1,Xloc));
  printf("          %d   %d\n", (int)GGET(temp_grid.grid,Yloc,Xloc-1),
                         	(int)GGET(temp_grid.grid,Yloc,Xloc+1) );
  printf("            %d\n", (int)GGET(temp_grid.grid,Yloc+1,Xloc));
  printf("\n\n");
#endif
     switch (dir){
         while (count < 4){
         case 0:  if (count<4 && Yloc > 0 && 
		      GGET(temp_grid.grid,Yloc-1,Xloc) != avoid_region) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 1:  if (count<4 && Xloc < cols &&
		      GGET(temp_grid.grid, Yloc, Xloc+1) != avoid_region) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 2:  if (count<4 && Yloc < rows &&
		      GGET(temp_grid.grid, Yloc+1, Xloc) != avoid_region) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 3:  if (count<4 && Xloc > 0 &&
		      GGET(temp_grid.grid, Yloc, Xloc-1) != avoid_region) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        move_left(agent,0); 
		        return;
		     }
	          }
	          count++;
     }
  }
}
