void safe_wander_trail(agent_struct *agent, int action_index)
/*  "safe" wandering, will not allow multiple agents to occupy same location, also
    leaves a trail for others to follow */
{

  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, action_grid, surround;

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

  dir = (int)(4 * ((double)rand()/(double)RAND_MAX));
  count = 0;
  surround = 0;

     switch (dir){
         while (count < 4){
         case 0:  if (count<4 && Yloc > 0) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
                        GSET(action_grid, Yloc, Xloc, 1); /* leave a trail to follow */
		        move_up(agent,0); 
		        count=4;
		     }
		     else {
		       surround++;
		     }
	          }
	          count++;
         case 1:  if (count<4 && Xloc < cols) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
                        GSET(action_grid, Yloc, Xloc, 2); /* leave a trail to follow */
		        move_right(agent,0); 
		        count=4;
		     }
		     else {
		       surround++;
		     }
	          }
	          count++;
         case 2:  if (count<4 && Yloc < rows) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
                        GSET(action_grid, Yloc, Xloc, 3); /* leave a trail to follow */
		        move_down(agent,0); 
		        count=4;
		     }
		     else {
		       surround++;
		     }
	          }
	          count++;
         case 3:  if (count<4 && Xloc > 0) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
                        GSET(action_grid, Yloc, Xloc, 4); /* leave a trail to follow */
		        move_left(agent,0); 
		        count=4;
		     }
		     else {
		       surround++;
		     }
	          }
	          count++;
	 }
     }
     /* if the leader is surrounded, then send agents away */
     /* note, this could cause some "catepillar corners" */
     if (surround == 4){
	GSET(action_grid,Yloc-1,Xloc,1);
	GSET(action_grid,Yloc+1,Xloc,3);
	GSET(action_grid,Yloc,Xloc+1,2);
        GSET(action_grid,Yloc,Xloc-1,4);
     }
}

void stay_search_trail (agent_struct *agent, int action_index)
     /* This function is used by the leader agent to stay within a particular region
	while searching for another value and leaving a trail for others to follow */

{
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, curr_region, surround;

  function_struct temp_func;
  update_grid_struct temp_grid, place_grid;

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* verifies that there are two support grids to act on */
  if (temp_func.number_grids != 2){
     printf("Incorrect number of support grids associated with this action\n");
     abort();
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];  // grid with regions
  place_grid = temp_func.update_grids[1]; // grid with trails

  rows = GGETR(0);
  cols = GGETC(0);

  /* tests the four possible directions to determine if search value is
     reachable -- if it is, the agent moves there */

  /* check "up" square */
  if (Yloc > 0){
     test_val = GGET(temp_grid.grid, Yloc-1, Xloc);
     if (test_val == temp_grid.grid_value){
       GSET(place_grid.grid, Yloc, Xloc, 1);         /* leave a trail to follow */
       move_up(agent, 0);  /* move agent to Xloc, Yloc-1 */
       printf("Found target region %d by going up should be %d\n", test_val, temp_grid.grid_value);
       return;
     }
  }

  /* check "left" square */
  if (Xloc > 0){
     test_val = GGET(temp_grid.grid, Yloc, Xloc-1);
     if (test_val == temp_grid.grid_value){
       GSET(place_grid.grid, Yloc, Xloc, 4);         /* leave a trail to follow */
       move_left(agent, 0);  /* move agent to Xloc-1, Yloc */
       printf("Found target region %d by going left should be %d\n", test_val, temp_grid.grid_value);
       return;
     }
  }

  /* check "down" square */
  if (Yloc < rows){
     test_val = GGET(temp_grid.grid, Yloc+1, Xloc);
     if (test_val == temp_grid.grid_value){
       GSET(place_grid.grid, Yloc, Xloc, 3);         /* leave a trail to follow */
       move_down(agent, 0);    /* move agent to Xloc, Yloc+1 */
       printf("Found target region %d by going down should be %d\n", test_val, temp_grid.grid_value);
       return;
     }
  }

  /* check "right" square */
  if (Xloc < cols){
     test_val = GGET(temp_grid.grid, Yloc, Xloc+1);
     if (test_val == temp_grid.grid_value){
       GSET(place_grid.grid, Yloc, Xloc, 2);         /* leave a trail to follow */
       move_right(agent, 0); /* move agent to Xloc+1, Yloc */
       printf("Found target region %d by going right should be %d\n", test_val, temp_grid.grid_value);
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
		        GSET(place_grid.grid,Yloc,Xloc,1); /* leave a trail to follow */
		        move_up(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 1:  if (count<4 && Xloc < cols &&
		      GGET(temp_grid.grid, Yloc, Xloc+1) == curr_region) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        GSET(place_grid.grid,Yloc,Xloc,2); /* leave a trail to follow */
		        move_right(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 2:  if (count<4 && Yloc < rows &&
		      GGET(temp_grid.grid, Yloc+1, Xloc) == curr_region) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        GSET(place_grid.grid,Yloc,Xloc,3); /* leave a trail to follow */
		        move_down(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 3:  if (count<4 && Xloc > 0 &&
		      GGET(temp_grid.grid, Yloc, Xloc-1) == curr_region) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        GSET(place_grid.grid,Yloc,Xloc,4); /* leave a trail to follow */
		        move_left(agent,0); 
		        return;
		     }
	          }
	          count++;
	 }
     }
     printf("Reached end without choosing a direction\n");
}

void follow(agent_struct *agent, int action_index)
{
  /* This function allows agents to follow a trail left by a "leader".  The directions
     are N=1,E=2,S=3,W=4.  In addition, the code ensures that an agent following a
     trail will NOT move into an occupied cell.  As a result, the trail of followers 
     will not collapse, but the trail will not be lost either.  */

  int dir; /* randomly selected direction */
  int Xloc, Yloc, state;

  function_struct temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET(temp_grid.grid,Yloc,Xloc);

     switch (dir){
     case 1:      if (!GETAGT(Yloc-1,Xloc)){
                     move_up(agent,0);
                  }
	          break;
     case 2:      if (!GETAGT(Yloc, Xloc+1)){
                     move_right(agent, 0);
                  }  
                  break;
     case 3:      if (!GETAGT(Yloc+1, Xloc)){
                     move_down(agent,0);
                  }
                  break;
     case 4:      if (!GETAGT(Yloc,Xloc-1)){
                     move_left(agent,0);
                  }
                  break;
     default:     printf("Invalid direction found -- program aborting\n");
                  abort();
		  break;
     }
}

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
