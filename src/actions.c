/*
These functions will move the cooperative agent and update the world grid
Input: agent The cooperative agent, unupdated
Output: agent The updated agent

*/

#include "support_grids.h"
#include "agent.h"
#include "useful.h"
#include "trace.h"
#include <stdlib.h>

extern int iter_count;

void show_support_grid(int index)
{

  float val;
  int row, col;
  char dummy;

  for (row=0;row<GGETR(index);row++){
      for (col=0;col<GGETC(index);col++){

	  val = GGET(index, row, col);
	  printf("%4.2f ", val);
      }
      printf("\n");
  }
  scanf("%c", &dummy);
}	   

void verify_grid_access()
{
  static int first_flag = 1;
  static s_grids_ptr st1;
  static int st2;
  static all_supports st3;
  static sgrid_ptr st4;
  static fgrid_ptr st5;
  static fgrid_ptr st6;

  if (first_flag){
     st1 = all_support_grids; 
     st2 = all_support_grids->num_grids; 
     st3 = all_support_grids->grids;
     st4 = (*(all_support_grids->grids))[0]; 
     st5 = (*(all_support_grids->grids))[0]->curr_grid; 
     st6 = (*(all_support_grids->grids))[0]->prev_grid;

     first_flag = 0;
  }

  if (st1 != all_support_grids){
     printf("all_support_grids pointer has been corrupted\n");
     printf("Standard verification of all_support_grids: FAILED\n");
     exit(1);
  }; 
  if (st2 != all_support_grids->num_grids){
     printf("the recorded number of support grids has changed\n");
     printf("Standard verification of all_support_grids: FAILED\n");
     exit(1);
  }; 
  if (st3 != (all_support_grids->grids)){
     printf("The array of support grid pointers has been corrupted\n");
     printf("Standard verification of all_support_grids: FAILED\n");
     exit(1);   
  };
  if (st4 != ((*(all_support_grids->grids))[0])){
     printf("Pointer to the first support grid has been corrupted\n");
     printf("Standard verification of all_support_grids: FAILED\n");
     exit(1);
  };
  if (st5 != ((*(all_support_grids->grids))[0]->curr_grid) &&
      st5 != ((*(all_support_grids->grids))[0]->prev_grid)){
     printf("Ptr to curr_grid of support grid 0 has been corrupted\n");
     printf("Standard verification of all_support_grids: FAILED\n");
     exit(1);
  }; 
  if (st6 != ((*(all_support_grids->grids))[0]->prev_grid) && 
      st6 != ((*(all_support_grids->grids))[0]->curr_grid)){
     printf("Ptr to prev_grid of support grid 0 has been corrupted\n");
     printf("Standard verification of all_support_grids: FAILED\n");
     exit(1);
  };

  printf("Standard verification of all_support_grids: PASSED\n");
}

void deactivate(agent_struct *agent, int ignore)
  {

  agent->active = 0;

  }
	
void no_op(agent_struct *agent, int ignore)
  {
  }

void move_up(agent_struct *agent, int ignore)
  {
#if 1
  CLRAGT(agent->y_pos, agent->x_pos);
  
  agent->dir = 0;

  if (agent->y_pos == 0) agent->y_pos = 0; /* GGETR(0) - 1; */
  else agent->y_pos -= 1;

  SETAGT(agent->y_pos, agent->x_pos);

#endif

#if 0   /* for an open world */
  agent->y_pos -= 1;
#endif

# ifdef error

  printf("\nMoving up");

# endif

  }

void move_down(agent_struct *agent, int ignore)
  {

#if 1
  CLRAGT(agent->y_pos, agent->x_pos);

  agent->dir = 2;

  if (agent->y_pos == (GGETR(0)-1) ) agent->y_pos = GGETR(0)-1; /* 0; */
  else agent->y_pos += 1;

  SETAGT(agent->y_pos, agent->x_pos);
#endif

#if 0  /* for an open world */
  agent->y_pos += 1;  
#endif

# ifdef error

  printf("\nMoving down");

# endif
 
  }

void move_left(agent_struct *agent, int ignore)
  {
#if 1
  CLRAGT(agent->y_pos, agent->x_pos);

  agent->dir = 3;

  if (agent->x_pos == 0) agent->x_pos = 0; /* GGETC(0) - 1; */
  else agent->x_pos -= 1;

  SETAGT(agent->y_pos, agent->x_pos);
#endif

#if 0
  agent->x_pos -= 1;
#endif
# ifdef error

  printf("\nMoving left");

# endif

  }

void move_right(agent_struct *agent, int ignore)
  {
#if 1
  CLRAGT(agent->y_pos, agent->x_pos);
  agent->dir = 1;

  if (agent->x_pos == (GGETC(0) - 1) ) agent->x_pos = GGETC(0)-1; /* 0; */
  else agent->x_pos += 1;
  SETAGT(agent->y_pos, agent->x_pos);

#endif

#if 0
  agent->x_pos += 1;
#endif

# ifdef error

  printf("\nMoving right");

# endif

  }

void move_wander(agent_struct *agent, int ignore)
  {
  int dir; /* randomly selected direction to move */

  dir = (int)(4 * ((double)rand()/(double)RAND_MAX));

  switch (dir){
         case 0: move_up(agent,0); break; 
         case 1: move_right(agent,0); break;
         case 2: move_down(agent,0); break;
         case 3: move_left(agent,0); break;
  }

# ifdef error

  printf("\nWandering");

# endif

  }

void pick_any(agent_struct *agent, int ignore)
  {
    int arb_angle;

    float arb_rad;

  arb_angle = (int)(46 * ((double)rand()/(double)RAND_MAX));

  arb_rad = arb_angle/180.0 * 3.141592;

  agent->update_amt = tan(arb_rad);

  agent->dir = (int)(4 * ((double)rand()/(double)RAND_MAX));
  agent->count_amt = 0.0;

  if ( ((double)rand()/(double)RAND_MAX) >= 0.5){
     agent->switch_dir = agent->dir + 1;
     if (agent->switch_dir == 4) {agent->switch_dir = 0;}
  }
  else {
     agent->switch_dir = agent->dir - 1;
     if (agent->switch_dir == -1) {agent->switch_dir = 3;}
  }

# ifdef error

  printf("\nSelecting a random direction - arbitrary\n");

# endif

  }

void move_any(agent_struct *agent, int ignore)
  {
    int dir_store;

#if 0
    printf("data from agent:\n");
    printf("                 dir: %d\n", agent->dir);
    printf("          update_amt: %f\n", agent->update_amt);
    printf("           count_amt: %f\n", agent->count_amt);
    printf("          switch_dir: %d\n\n", agent->switch_dir);
#endif

    if (agent->dir == -1){
       pick_any(agent, 0);
    }

  switch (agent->dir){ 
         case 0: move_up(agent,0); break; 
         case 1: move_right(agent,0); break;
         case 2: move_down(agent,0); break;
         case 3: move_left(agent,0); break;
  }

  agent->count_amt = agent->count_amt + agent->update_amt;
  
  if (agent->count_amt > 1.0){
     agent->count_amt = agent->count_amt - 1.0;

     dir_store = agent->dir;

     /*UGLY HACK!!!!! */
     agent->field[18] += 1;

     switch (agent->switch_dir){
            case 0: move_up(agent,0); break; 
            case 1: move_right(agent,0); break;
            case 2: move_down(agent,0); break;
            case 3: move_left(agent,0); break;
     }

     agent->dir = dir_store;

  }
# ifdef error

  printf("\nMoving in a universal direction\n");

# endif

  }


#if 1
void turn_right(agent_struct *agent, int ignore)
  {
    
  agent->dir = (agent->dir + 1) % 4;

# ifdef error

  printf("\nTurning right");

# endif

  }

void turn_left(agent_struct *agent, int ignore)
  {
    
  agent->dir = (agent->dir - 1) % 4;

  if (agent->dir < 0) { 
     agent->dir = agent->dir + 4; 
  }

# ifdef error

  printf("\nTurning left");

# endif

  }

void move_forward(agent_struct *agent, int ignore)
  {
    
  switch (agent->dir){
         case 0: move_up(agent,0); break; 
         case 1: move_right(agent,0); break;
         case 2: move_down(agent,0); break;
         case 3: move_left(agent,0); break;
  }

# ifdef error

  printf("\nMoving forward");

# endif

  }

void move_backward(agent_struct *agent, int ignore)
  {
    
  switch (agent->dir){
         case 0: move_down(agent,0); break; 
         case 1: move_left(agent,0); break;
         case 2: move_up(agent,0); break;
         case 3: move_right(agent,0); break;
  }

# ifdef error

  printf("\nMoving backward");

# endif

  }
#endif

void reset_field(agent_struct *agent, int action_index)
     /* sets one the agent data fields to 0 */
     
  {
  int state;
  function_struct    temp_func;
  update_grid_struct temp_grid;


  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  agent->field[(int)(temp_grid.grid_value)] = 0;

# ifdef error

  printf("\nSetting Field");

# endif

  }

void set_field_pri(agent_struct *agent, int action_index)
     /* sets one the agent data fields to 0 */
     
  {
  int state, Xloc, Yloc;
  function_struct    temp_func;
  update_grid_struct temp_grid;


  Xloc = agent->x_pos;
  Yloc = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  /* hard coded to the beacon grid */
  agent->field[(int)(temp_grid.grid_value)] = (int)GGET(1,Yloc, Xloc);

# ifdef error

  printf("\nSetting Field to value on world grid based on priority");

# endif

  }


void set_id(agent_struct *agent, int action_index)
     /* sets the id number */
     
  {
  int state;
  function_struct    temp_func;
  update_grid_struct temp_grid;


  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  agent->id_number = temp_grid.grid_value;

# ifdef error

  printf("\nSetting ID");

# endif

  }


void inc_field(agent_struct *agent, int action_index)
     /* increments one of the agent data fields */
     
  {
  int state;
  function_struct    temp_func;
  update_grid_struct temp_grid;


  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  (agent->field[(int)(temp_grid.grid_value)])++;

# ifdef error

  printf("\nIncrementing Field");

# endif

  }


void dec_field(agent_struct *agent, int action_index)
     /* increments one of the agent data fields */
     
  {
  int state;
  function_struct    temp_func;
  update_grid_struct temp_grid;


  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  (agent->field[(int)(temp_grid.grid_value)])--;

# ifdef error

  printf("\nDecrementing Field");

# endif

  }

void message1(agent_struct *agent, int ignore)
     /* prints message giving distance from a beacon to another beacon */
     
  {
    int Xloc, Yloc, test_val;
    float dist, dist1, dist2;

    Xloc = agent->x_pos;
    Yloc = agent->y_pos;

    test_val = (int)GGET(1, Yloc, Xloc);
    
    dist1 = agent->field[18];
    dist2 = agent->field[20];
    dist = (float)sqrt(dist1*dist1 + dist2*dist2);


    if (agent->field[19] != test_val){
       printf("%d %d %f\n", (int)agent->field[19], test_val, dist);
    }

# ifdef error

  printf("\nPrinting message 1");

# endif

  }

void message2(agent_struct *agent, int ignore)
     /* prints message giving distance from a beacon to the edge */
     
  {
    float dist, dist1, dist2;

    dist1 = agent->field[18];
    dist2 = agent->field[20];
    dist = (float)sqrt(dist1*dist1 + dist2*dist2);


    printf("%d E %f\n", (int)agent->field[19], dist);

# ifdef error

  printf("\nPrinting message 2");

# endif

  }

void set_radius(agent_struct *agent, int action_index)
     /* works in conjunction with circle action, sets the radius
	at which to draw the circle on a grid */
  {
  int state;
  function_struct    temp_func;
  update_grid_struct temp_grid;


  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  temp_grid = temp_func.update_grids[0];

  agent->radius = temp_grid.grid_value;

# ifdef error

  printf("\nSetting Radius");

# endif

  }

void circle(agent_struct *agent, int action_index)
/* places a specified value on a grid in a preset sized circular 
   region.  (Radius set with set_radius action).  Two other parameters
   are the value to place and the grid on which to place it */
{
    
	TraceDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  int                row, col, state, i, r, c, rsqd, marker;
  int beg_row,end_row,beg_col,end_col;
  function_struct    temp_func;
  update_grid_struct temp_grid;
  
  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  for (i=0; i<temp_func.number_grids; i++){

      temp_grid = temp_func.update_grids[i];

      rsqd = agent->radius * agent->radius;

      beg_row = MAX(row-agent->radius,0);
      end_row = MIN(row+agent->radius,GGETR(0));
		    
      beg_col = MAX(col-agent->radius,0);
      end_col = MIN(col+agent->radius,GGETC(0));
		  
      
      if (temp_grid.grid_value == 99){
	 marker = agent->id_number;
      }
      else {
	 marker = temp_grid.grid_value;
      }

      for (r=beg_row;r<end_row;r++){
	  for (c=beg_col;c<end_col;c++){
	    if (((r-row)*(r-row) + (c-col)*(c-col)) < rsqd){
	      if (r != row || c != col){
                 GSET(temp_grid.grid, r, c, marker);
	      }
	    }
	  }
      }
  }

# ifdef error

  printf("\nCircle");

# endif

}

void climb_gradient(agent_struct *agent, int action_index)
  {
    
  int Xloc, Yloc, row, col, curr_dir, first_val,i;
  int rows, cols, state, action_grid;

  float curr_max, test_val;
  function_struct temp_func;

  struct choice{int x,y;};
  struct choice  offset[] = {{1,0},{-1,0},{0,1},{0,-1},
			        {1,1},{1,-1},{-1,-1},{-1,1}};

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  action_grid = temp_func.update_grids[0].grid;

  rows = GGETR(action_grid);
  cols = GGETC(action_grid);

  first_val = 1;

  /* scan the four orthogonal directions for the largest value in world grid */
  for (i=0; i<4; i++){
      col = Xloc + offset[i].x;
      row = Yloc + offset[i].y;

      if ((row >= 0) && (col >= 0) && (row < rows) && (col < cols)){

	 test_val = GGET(action_grid, row, col);
#if 0
	 printf(" value there is %f\n", test_val);
#endif
	 /* look at the support grid with the corresponding concentrations */
	 /* to determine which direction to travel */
	 if (first_val){

	    curr_max = test_val;
	    first_val  = 0;
	    curr_dir = 0;
	 }
	 else if (test_val > curr_max){
	    curr_max = test_val;
	    curr_dir = i;
	 }
      }
  }
#if 0
  printf("Agent at %d %d climbing in direction %d\n", Xloc, Yloc, curr_dir);  
#endif
  /* make the move in the direction of the largest value */
  agent->x_pos = Xloc + offset[curr_dir].x;
  agent->y_pos = Yloc + offset[curr_dir].y;
  
# ifdef error

  printf("\nClimbing gradient");

# endif

  }

void time_tag(agent_struct *agent, 
	      int action_index)
{
  int                row, col, state, i;
  function_struct        temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  for (i=0; i<temp_func.number_grids; i++){
      temp_grid = temp_func.update_grids[i];

      /* sets a given location on a given support grid to a specific value */
      GSET(temp_grid.grid, row, col, iter_count);
  }
}



void place(agent_struct *agent, int action_index)
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

      if ( INRANGE( row, 0, GGETR(temp_grid.grid)) && INRANGE( col, 0, GGETC(temp_grid.grid)) )
	{
	  GSET(temp_grid.grid, row, col, temp_grid.grid_value);
	}

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

void safe_wander(agent_struct *agent, int action_index)
/*  "safe" wandering, will not allow multiple agents to occupy same location */
  {
  int dir, count, test_val; /* randomly selected direction */
  int Xloc, Yloc, rows, cols; /*, state, action_grid; 

  function_struct temp_func;
			      */

  /* acquire the position of the current agent */
  Xloc = agent->x_pos;
  Yloc = agent->y_pos;
   
#if 0
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* determines which support grid contains pertinent information */
  action_grid = temp_func.update_grids[0].grid;
#endif

  rows = GGETR(0);
  cols = GGETC(0);

  dir = (int)(4 * ((double)rand()/(double)RAND_MAX));
  count = 0;

     switch (dir){
         while (count < 4){
         case 0:  if (count<4 && Yloc > 0) {
                     test_val = GETAGT(Yloc-1, Xloc);
                     if (!test_val){
		        move_up(agent,0); 
		        count=4;
		     }
	          }
	          count++;
         case 1:  if (count<4 && Xloc < cols) {
                     test_val = GETAGT(Yloc, Xloc+1);
                     if (!test_val){
		        move_right(agent,0); 
		        count=4;
		     }
	          }
	          count++;
         case 2:  if (count<4 && Yloc < rows) {
                     test_val = GETAGT(Yloc+1, Xloc);
                     if (!test_val){
		        move_down(agent,0); 
		        count=4;
		     }
	          }
	          count++;
         case 3:  if (count<4 && Xloc > 0) {
                     test_val = GETAGT(Yloc, Xloc-1);
                     if (!test_val){
		        move_left(agent,0); 
		        count=4;
		     }
	          }
	          count++;
     }
  }
}

void pump_chem(agent_struct *agent, int action_index)
/* places a specified value at the current location of a coag on a specified 
   support grid.  The specified information is all contained in the coag */
{
    
  int                row, col, state;
  function_struct        temp_func;
  update_grid_struct temp_grid;
  float              chem_amt, old_chem, new_chem;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  chem_amt = agent->chem;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  temp_grid = temp_func.update_grids[0];

  /* injects a new amount of chemical into the agent's location */
  old_chem = GGET(temp_grid.grid, row, col);
  new_chem = old_chem + chem_amt;
  GGET(temp_grid.grid, row, col) = new_chem;

#if 0  
  printf("Storing %f in location %d %d on grid %d\n", new_chem, col, row, temp_grid.grid);
  /* ((*(*all_support_grids->grids)[temp_grid.grid]->prev_grid)[row][col]) = new_chem;  */
#endif

  /* updates the amount of chemical to release */
  agent->chem = chem_amt - (INJECT_DECREASE * chem_amt);

# ifdef error

  printf("\nPlace");

# endif

}


void modify(agent_struct *agent, int action_index)
{
    
  int                row, col, state, temp_value, i;
  function_struct        temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];
  for (i=0; i<temp_func.number_grids; i++){
      temp_grid = temp_func.update_grids[i];

      temp_value = GGET(temp_grid.grid, row, col);

      /* sets a given location on a given support grid to a specific value */
      GSET(temp_grid.grid, row, col, temp_value + temp_grid.grid_value);
  }

# ifdef error

  printf("\nModify");

# endif

}

#include "user_actions.c"




