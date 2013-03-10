void pick_up(agent_struct *agent, int action_index)
     /* collects an object underneath and removes it from the grid */
{
  int object; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state;

  function_struct temp_func;
  update_grid_struct temp_grid;

  if (agent->field[1]==1) return;

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

  object = GGET(temp_grid.grid, Yloc, Xloc);
  if (object == 1){
     // printf("Looking at position row:%d col:%d\n", Yloc, Xloc);
     // printf("Found a %d there\n", object);
     // printf("Error in system, attempting to pick up a non-existant object\n");
     // exit(1); 
  
     /* "put" the object onboard the agent */
     agent->field[1] = object;

     /* remove the object from the support grid */
     GSET(temp_grid.grid, Yloc, Xloc, 0);
  }
  else if (object == 2){
    /* "accidently pick up an object from the target area */
     /* "put" the object onboard the agent */
     agent->field[1] = 1;

     /* remove the object from the support grid */
     GSET(temp_grid.grid, Yloc, Xloc, 3);
  }
}

void put_down(agent_struct *agent, int action_index)
     /* causes an agent to stay in the region that they are currently in, and
	search for a second region specified by the input parameter to the
	function.  Additionally, the agent will perform only "safe" moves */
{
  int Xloc, Yloc, rows, cols, state, object;

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

  /* make sure the agent is carrying an object */
  object = agent->field[1]; 
  if (object == 1){
     // printf("Error in system, attempting to put down a non-existant object\n");
     // exit(1);
  
     /* place the object on the support grid */
     /* if over the target area, mark with 2, otherwise mark with 1 */
     if (GGET(temp_grid.grid, Yloc, Xloc) == 3){
        /* "drop" the object from the agent */
        agent->field[1] = 0;
        GSET(temp_grid.grid, Yloc, Xloc, 2);
     }
     else if (GGET(temp_grid.grid, Yloc, Xloc)==0){
        /* "drop" the object from the agent */
        agent->field[1] = 0;
        GSET(temp_grid.grid, Yloc, Xloc, 1);
     }
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

  /* check "up" square */
  if (Yloc > 0){
     test_val = GGET(temp_grid.grid, Yloc-1, Xloc);
     if (test_val == temp_grid.grid_value && !GETAGT(Yloc-1,Xloc)){
       move_up(agent, 0);  /* move agent to Xloc, Yloc-1 */
       return;
     }
  }

  /* check "left" square */
  if (Xloc > 0){
     test_val = GGET(temp_grid.grid, Yloc, Xloc-1);
     if (test_val == temp_grid.grid_value && !GETAGT(Yloc, Xloc-1)){
       move_left(agent, 0);  /* move agent to Xloc-1, Yloc */
       return;
     }
  }

  /* check "down" square */
  if (Yloc < rows){
     test_val = GGET(temp_grid.grid, Yloc+1, Xloc);
     if (test_val == temp_grid.grid_value && !GETAGT(Yloc+1,Xloc)){
       move_down(agent, 0);    /* move agent to Xloc, Yloc+1 */
       return;
     }
  }

  /* check "right" square */
  if (Xloc < cols){
     test_val = GGET(temp_grid.grid, Yloc, Xloc+1);
     if (test_val == temp_grid.grid_value && !GETAGT(Yloc, Xloc+1)){
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

void locate_region(agent_struct *agent, int action_index)
     /* checks for a specific region, if found, then moves there, else does safe-moves 
	in a straight-biased pattern regardless of region */
{
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state;
  int dir2, dir3, motion_action, i;

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

  /* check "up" square */
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

  /* check "down" square */
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
  motion_action = (int)(100 * ((double)rand()/(double)RAND_MAX));
  
  /* 70% bias for going straight */
  if (motion_action < 70.0){
     dir = agent->dir;
     dir2 = (agent->dir+1) % 4;
     dir3 = (agent->dir+3) % 4;
  }
  else if (motion_action < 85.0){  /* 15% for turning right */
     dir = (agent->dir + 1) % 4;
     dir2 = (agent->dir + 3) % 4;
     dir3 = agent->dir;
  }
  else { /* 15% remaining for left turn */
     dir = (agent->dir + 3) % 4;
     dir2 = (agent->dir + 1) % 4;
     dir3 = agent->dir;
  }

  //  printf("Direction attempts are: %d %d %d actually went: ", dir, dir2, dir3);

  count = 0;

  /* tries moving in three different directions forward, left and right.  There is
     a strong bias towards moving forward with a cascading bias towards either of the
     other directions */
  for (i=0;i<3;i++){
  switch (dir){
         case 0:  if (Yloc > 0) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
		        return;
		     }
	          }
	          break;
         case 1:  if (Xloc < cols) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0); 
		        return;
		     }
	          }
	          break;
         case 2:  if (Yloc < rows) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
		        return;
		     }
	          }
	          break;
         case 3:  if (Xloc > 0) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        move_left(agent,0); 
		        return;
		     }
	          }
	          break;
         }
         if (i==0){
            dir = dir2;
         }
         if (i==1){
            dir = dir3;
         }
  }
}
void findtarget(agent_struct *agent, int action_index)
     /* THIS IS A HARDCODED FUNCTION that should be modified when the multiple parameter
	capability is developed.  It will search for 1 of 2 hardcoded regions.  It's 
	looking for either the target region or the delivered blocks both are 
	significant.  If neither are found, it will do a straight-biased safe wander */

{
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state;
  int dir2, dir3, motion_action, i;

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

  /* check "up" square */
  if (Yloc > 0){
     test_val = GGET(temp_grid.grid, Yloc-1, Xloc);
     if (test_val == 3 || test_val == 2){ /* HARDCODED VALUES */
       move_up(agent, 0);  /* move agent to Xloc, Yloc-1 */
       return;
     }
  }

  /* check "left" square */
  if (Xloc > 0){
     test_val = GGET(temp_grid.grid, Yloc, Xloc-1);
     if (test_val == 3 || test_val == 2){ /* HARDCODED VALUES */
       move_left(agent, 0);  /* move agent to Xloc-1, Yloc */
       return;
     }
  }

  /* check "down" square */
  if (Yloc < rows){
     test_val = GGET(temp_grid.grid, Yloc+1, Xloc);
     if (test_val == 3 || test_val == 2){ /* HARDCODED VALUES */
       move_down(agent, 0);    /* move agent to Xloc, Yloc+1 */
       return;
     }
  }

  /* check "right" square */
  if (Xloc < cols){
     test_val = GGET(temp_grid.grid, Yloc, Xloc+1);
     if (test_val == 3 || test_val == 2){ /* HARDCODED VALUES */
       move_right(agent, 0); /* move agent to Xloc+1, Yloc */
       return;
     }
  }
  motion_action = (int)(100 * ((double)rand()/(double)RAND_MAX));
  
  /* 70% bias for going straight */
  if (motion_action < 70.0){
     dir = agent->dir;
     dir2 = (agent->dir+1) % 4;
     dir3 = (agent->dir+3) % 4;
  }
  else if (motion_action < 85.0){  /* 15% for turning right */
     dir = (agent->dir + 1) % 4;
     dir2 = (agent->dir + 3) % 4;
     dir3 = agent->dir;
  }
  else { /* 15% remaining for left turn */
     dir = (agent->dir + 3) % 4;
     dir2 = (agent->dir + 1) % 4;
     dir3 = agent->dir;
  }

  //  printf("Direction attempts are: %d %d %d actually went: ", dir, dir2, dir3);

  count = 0;

  /* tries moving in three different directions forward, left and right.  There is
     a strong bias towards moving forward with a cascading bias towards either of the
     other directions */
  for (i=0;i<3;i++){
  switch (dir){
         case 0:  if (Yloc > 0) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
		        return;
		     }
	          }
	          break;
         case 1:  if (Xloc < cols) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0); 
		        return;
		     }
	          }
	          break;
         case 2:  if (Yloc < rows) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
		        return;
		     }
	          }
	          break;
         case 3:  if (Xloc > 0) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        move_left(agent,0); 
		        return;
		     }
	          }
	          break;
         }
         if (i==0){
            dir = dir2;
         }
         if (i==1){
            dir = dir3;
         }
  }
}

void object_trail(agent_struct *agent, int action_index)
     /* This function moves the agent in a highly straight-biased path does "safe" 
	movements and leaves a reverse trail to follow */
{
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state;
  int dir2, dir3, motion_action, i;

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

  motion_action = (int)(100 * ((double)rand()/(double)RAND_MAX));
  
  /* 92% bias for going straight */
  if (motion_action < 92.0){
     dir = agent->dir;
     dir2 = (agent->dir+1) % 4;
     dir3 = (agent->dir+3) % 4;
  }
  else if (motion_action < 96.0){  /* 4% for turning right */
     dir = (agent->dir + 1) % 4;
     dir2 = (agent->dir + 3) % 4;
     dir3 = agent->dir;
  }
  else { /* 4% remaining for left turn */
     dir = (agent->dir + 3) % 4;
     dir2 = (agent->dir + 1) % 4;
     dir3 = agent->dir;
  }

  count = 0;

  /* tries moving in three different directions forward, left and right.  There is
     a strong bias towards moving forward with a cascading bias towards either of the
     other directions */
  for (i=0;i<3;i++){
  switch (dir){
         case 0:  if (Yloc > 0) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
			if (GCURR(temp_grid.grid, Yloc-1, Xloc) == 0){
			   GSET(temp_grid.grid, Yloc-1, Xloc, 3);
			}
		        return;
		     }
	          }
	          break;
         case 1:  if (Xloc < cols) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0);
			if (GCURR(temp_grid.grid, Yloc, Xloc+1)==0){
			   GSET(temp_grid.grid, Yloc, Xloc+1, 4);
			}
		        return;
		     }
	          }
	          break;
         case 2:  if (Yloc < rows) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
			if (GCURR(temp_grid.grid, Yloc+1, Xloc)==0){
			   GSET(temp_grid.grid, Yloc+1, Xloc, 1);
			}
		        return;
		     }
	          }
	          break;
         case 3:  if (Xloc > 0) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        move_left(agent,0); 
			if (GCURR(temp_grid.grid, Yloc, Xloc-1)==0){
			   GSET(temp_grid.grid, Yloc, Xloc-1, 2);
			}
		        return;
		     }
	          }
	          break;
         }
         if (i==0){
            dir = dir2;
         }
         if (i==1){
            dir = dir3;
         }
  }
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
  if (dir == 0){
     dir = (int)(4 * ((double)rand()/(double)RAND_MAX))+1;
  }

     switch (dir){
     case 1:      if (!GETAGT(Yloc-1,Xloc)){
                     move_up(agent,0);
		     agent->field[0] = 0;
		     return;
                  }
	          break;
     case 2:      if (!GETAGT(Yloc, Xloc+1)){
                     move_right(agent, 0);
		     agent->field[0] = 0;
		     return;
                  }  
                  break;
     case 3:      if (!GETAGT(Yloc+1, Xloc)){
                     move_down(agent,0);
		     agent->field[0] = 0;
		     return;
                  }
                  break;
     case 4:      if (!GETAGT(Yloc,Xloc-1)){
                     move_left(agent,0);
		     agent->field[0] = 0;
		     return;
                  }
                  break;
     default:     printf("Invalid direction found %d -- program aborting\n", dir);
                  printf("At location row: %d; col: %d\n", Yloc, Xloc);
                  abort();
		  break;
     }
     (agent->field[0])++;
}






