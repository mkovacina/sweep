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

  /* 80% bias for going straight */
  if (motion_action < 80.0){
     dir = agent->dir;
  }
  else if (motion_action < 90.0){  /* 15% for turning right */
     dir = (agent->dir + 1) % 4;
  }
  else { /* 10% remaining for left turn */
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

void flash(agent_struct *agent, int action_index)
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
	 GSET(action_grid, row, col, 2);
      }
    }
  }
}

void lflash(agent_struct *agent, int action_index)
/*  Generates a leader firefly "flash" within a limited area of the grid */
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
