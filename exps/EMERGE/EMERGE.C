void safe_str(agent_struct *agent, int action_index)
/*  "safe" wandering, will not allow multiple agents to occupy same location, also tends 
    to wander in a straight direction, turning only occaisionally */
{

  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, action_grid, motion_action;

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

  if (agent->dir == -1){
     agent->dir = (int)(4 * ((double)rand()/(double)RAND_MAX));
  }

  /* selects a random direction (straight, turn right or turn left */
  motion_action = (int)(100 * ((double)rand()/(double)RAND_MAX));

  /* 70% bias for going straight */
  if (motion_action < 70.0){
     dir = agent->dir;
  }
  else if (motion_action < 85.0){  /* 15% for turning right */
     dir = (agent->dir + 1) % 4;
  }
  else { /* 15% remaining for left turn */
     dir = (agent->dir + 3) % 4;
  }

  count = 0;

  switch (dir){
         while (count < 4){
         case 0:  if (count<4 && Yloc > 0) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 1:  if (count<4 && Xloc < cols) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 2:  if (count<4 && Yloc < rows) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
		        return;
		     }
	          }
	          count++;
         case 3:  if (count<4 && Xloc > 0) {
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

void hill_climb(agent_struct *agent, int action_index)
     /* causes an agent to stay in the region that they are currently in, and
	search for a second region specified by the input parameter to the
	function.  Additionally, the agent will perform only "safe" moves */
{
  int dir, alt_count, max; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, alt[5], do_action;
  float north, south, east, west;

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

  north = GGET(temp_grid.grid,Yloc-1,Xloc);

  alt_count = 0;

  max = -1.0;
  dir = -1;
  if (Yloc > 0 && !GETAGT(Yloc-1, Xloc)) { 
     max = GGET(temp_grid.grid,Yloc-1,Xloc);
     dir = 0;
     alt[alt_count++] = 0;
  }
  if (Xloc < rows && !GETAGT(Yloc, Xloc+1)) { 
     east  = GGET(temp_grid.grid,Yloc, Xloc+1);
     alt[alt_count++] = 1;
     if (max < east){
        max = east;
        dir = 1;
     }
  }
  if (Yloc < cols && !GETAGT(Yloc+1, Xloc)) { 
     south = GGET(temp_grid.grid,Yloc+1, Xloc);
     alt[alt_count++] = 2;
     if (max < south){
        max = south;
        dir = 2;
     }
  }
  if (Xloc > 0 && !GETAGT(Yloc, Xloc-1)) { 
     west  = GGET(temp_grid.grid,Yloc, Xloc-1);
     alt[alt_count++] = 3;
     if (max < west){
        max = west;
        dir = 3;
     }
  }

  /* there are no viable directions, so don't move this turn */
  if (dir == -1){
     return;
  }

  /* allows for only partially accurate climbing */
  do_action = (int)(100 * ((double)rand()/(double)RAND_MAX));
     
  if (do_action > 87.5){
     dir = alt[(int)(alt_count * ((double)rand()/(double)RAND_MAX))];
  }

  /* actually performs the agent move */
  switch (dir){
         case 0:  move_up(agent,0); 
		  return;
         case 1:  move_right(agent,0);
	          return;
         case 2:  move_down(agent,0);
	          return;
         case 3:  move_left(agent,0);
	          return;
  }

}

void hill_down(agent_struct *agent, int action_index)
     /* allows an agent to find the direction that is "down" */
{
  int dir, alt[5], alt_count, min; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state, do_action;

  float north, south, east, west;

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

  north = GGET(temp_grid.grid,Yloc-1,Xloc);

  alt_count = 0;

  min = 100000.0;
  dir = -1;
  if (Yloc > 0 && !GETAGT(Yloc-1, Xloc)) { 
     min = GGET(temp_grid.grid,Yloc-1,Xloc);
     dir = 0;
     alt[alt_count++] = 0;
  }
  if (Xloc < rows && !GETAGT(Yloc, Xloc+1)) { 
     east  = GGET(temp_grid.grid,Yloc, Xloc+1);
     alt[alt_count++] = 1;
     if (min > east){
        min = east;
        dir = 1;
     }
  }
  if (Yloc < cols && !GETAGT(Yloc+1, Xloc)) { 
     south = GGET(temp_grid.grid,Yloc+1, Xloc);
     alt[alt_count++] = 2;
     if (min > south){
        min = south;
        dir = 2;
     }
  }
  if (Xloc > 0 && !GETAGT(Yloc, Xloc-1)) { 
     west  = GGET(temp_grid.grid,Yloc, Xloc-1);
     alt[alt_count++] = 3;
     if (min > west){
        min = west;
        dir = 3;
     }
  }

  /* there are no viable directions, so don't move this turn */
  if (dir == -1){
     return;
  }

  /* allows for only partially accurate climbing */
  do_action = (int)(100 * ((double)rand()/(double)RAND_MAX));
     
  if (do_action > 87.5){
     dir = alt[(int)(alt_count * ((double)rand()/(double)RAND_MAX))];
  }

  /* actually performs the agent move */
  switch (dir){
         case 0:  move_up(agent,0); 
		  return;
         case 1:  move_right(agent,0);
	          return;
         case 2:  move_down(agent,0);
	          return;
         case 3:  move_left(agent,0);
	          return;
  }

}

void pick_up(agent_struct *agent, int action_index)
     /* causes an agent to stay in the region that they are currently in, and
	search for a second region specified by the input parameter to the
	function.  Additionally, the agent will perform only "safe" moves */
{
  int object; /* randomly selected direction */
  int Xloc, Yloc, rows, cols, state;

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

  object = GGET(temp_grid.grid, Yloc, Xloc);
  if (object != 1){
     printf("Looking at position row:%d col:%d\n", Yloc, Xloc);
     printf("Found a %d there\n", object);
     printf("Error in system, attempting to pick up a non-existant object\n");
     exit(1);
  }
  
  /* "put" the object onboard the agent */
  agent->field[1] = object;

  /* remove the object from the support grid */
  GSET(temp_grid.grid, Yloc, Xloc, 0);
  
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
  if (object != 1){
     printf("Error in system, attempting to put down a non-existant object\n");
     exit(1);
  }
  
  /* "drop" the object from the agent */
  agent->field[1] = 0;

  /* place the object on the support grid */
  GSET(temp_grid.grid, Yloc, Xloc, 1);
  
}

void locate_obj(agent_struct *agent, int action_index)
     /* causes an agent to stay in the region that they are currently in, and
	search for a second region specified by the input parameter to the
	function.  Additionally, the agent will perform only "safe" moves */
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
  
  //  printf("@ location row %d, col %d: ", Yloc, Xloc);
  
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

