/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

//#define _KILL_AMBIGUITY


/*
  Three grids are supplied to wander_info().
  
  grid #0: neigbor information grid
  grid #1: vertical vortex information
  grid #2: horizontal vortex information
*/
void wander_info( agent_struct *agent, int action_index )
{
  int                row, col, state;
  function_struct    temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  if ( temp_func.number_grids != 3 )
    {
      error( "Insufficient number of grids supplied to wander_info().\n" );
      error( "Three grids are required for wander_info()" );
      exit(1);
    }

  if ( GGET( temp_func.update_grids[1].grid, row, col ) == 0 &&
       GGET( temp_func.update_grids[2].grid, row, col ) == 0    )
    {
      temp_grid = temp_func.update_grids[0];
      
      GSET(temp_grid.grid, row+1, col, GCURR(temp_grid.grid, row+1, col) + 1  );
      GSET(temp_grid.grid, row-1, col, GCURR(temp_grid.grid, row-1, col) + 1  );
      GSET(temp_grid.grid, row, col+1, GCURR(temp_grid.grid, row, col+1) + 1  );
      GSET(temp_grid.grid, row, col-1, GCURR(temp_grid.grid, row, col-1) + 1  );
    }

# ifdef vortex_error

  printf("\nwander_info");

# endif

}               

void neighbor_info( agent_struct *agent, int action_index )
{
  int                row, col, state, i;
  function_struct    temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  for (i=0; i<temp_func.number_grids; i++)
  {
      temp_grid = temp_func.update_grids[i];

      GSET(temp_grid.grid, row+1, col, GCURR(temp_grid.grid, row+1, col) + 1  );      
      GSET(temp_grid.grid, row-1, col, GCURR(temp_grid.grid, row-1, col) + 1  );
      GSET(temp_grid.grid, row, col+1, GCURR(temp_grid.grid, row, col+1) + 1  );
      GSET(temp_grid.grid, row, col-1, GCURR(temp_grid.grid, row, col-1) + 1  );

      GSET(temp_grid.grid, row+1, col+1, GCURR(temp_grid.grid, row+1, col+1) - 3  );
      GSET(temp_grid.grid, row+1, col-1, GCURR(temp_grid.grid, row+1, col-1) - 3  );
      GSET(temp_grid.grid, row-1, col+1, GCURR(temp_grid.grid, row-1, col+1) - 3  );
      GSET(temp_grid.grid, row-1, col-1, GCURR(temp_grid.grid, row-1, col-1) - 3  );
  }

# ifdef vortex_error

  printf("\nneighbor_info");

# endif

}               

#define ORTHOGONAL_FACTOR 2
#define UP    -1
#define DOWN   1
#define RIGHT  1
#define LEFT  -1

void vortex_end_info( agent_struct *agent, int action_index )
{
  int                row, col, state;
  function_struct    temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* Grid 0 is the vertical movement grid */
  temp_grid = temp_func.update_grids[0];
  GSET(temp_grid.grid, row+1, col-1, GCURR(temp_grid.grid, row+1, col-1) + UP  );
  GSET(temp_grid.grid, row,   col-1, GCURR(temp_grid.grid, row,   col-1) + ( UP * ORTHOGONAL_FACTOR )  );

  GSET(temp_grid.grid, row-1, col+1, GCURR(temp_grid.grid, row-1, col+1) + DOWN  );
  GSET(temp_grid.grid, row,   col+1, GCURR(temp_grid.grid, row,   col+1) + ( DOWN * ORTHOGONAL_FACTOR ) );

  /* Grid 1 is the horizontal movement grid */
  temp_grid = temp_func.update_grids[1];
  GSET(temp_grid.grid, row+1, col,   GCURR(temp_grid.grid, row+1, col)   + ( LEFT * ORTHOGONAL_FACTOR ) );
  GSET(temp_grid.grid, row+1, col+1, GCURR(temp_grid.grid, row+1, col+1) + LEFT  );

  GSET(temp_grid.grid, row-1, col-1, GCURR(temp_grid.grid, row-1, col-1) + RIGHT  );
  GSET(temp_grid.grid, row-1, col,   GCURR(temp_grid.grid, row-1, col)   + ( RIGHT * ORTHOGONAL_FACTOR ) );

# ifdef vortex_error

  printf("\nvortex_end_info");

# endif

}               


void vortex_mid_info( agent_struct *agent, int action_index )
{
  int                row, col, state;
  function_struct    temp_func;
  update_grid_struct temp_grid;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* Grid 0 is the vertical movement grid */
  temp_grid = temp_func.update_grids[0];
  GSET(temp_grid.grid, row+1, col-1, GCURR(temp_grid.grid, row+1, col-1) + UP  );
  GSET(temp_grid.grid, row,   col-1, GCURR(temp_grid.grid, row,   col-1) + UP  );
  GSET(temp_grid.grid, row-1, col-1, GCURR(temp_grid.grid, row-1, col-1) + UP  );

  GSET(temp_grid.grid, row-1, col+1, GCURR(temp_grid.grid, row-1, col+1) + DOWN  );
  GSET(temp_grid.grid, row,   col+1, GCURR(temp_grid.grid, row,   col+1) + DOWN  );
  GSET(temp_grid.grid, row+1, col+1, GCURR(temp_grid.grid, row+1, col+1) + DOWN  );

  /* Grid 1 is the horizontal movement grid */
  temp_grid = temp_func.update_grids[1];
  GSET(temp_grid.grid, row+1, col,   GCURR(temp_grid.grid, row+1, col)   + LEFT  );
  GSET(temp_grid.grid, row+1, col+1, GCURR(temp_grid.grid, row+1, col+1) + LEFT  );
  GSET(temp_grid.grid, row+1, col-1, GCURR(temp_grid.grid, row+1, col-1) + LEFT  );

  GSET(temp_grid.grid, row-1, col-1, GCURR(temp_grid.grid, row-1, col-1) + RIGHT  );
  GSET(temp_grid.grid, row-1, col,   GCURR(temp_grid.grid, row-1, col) + RIGHT  );
  GSET(temp_grid.grid, row-1, col+1, GCURR(temp_grid.grid, row-1, col+1) + RIGHT  );

# ifdef vortex_error

  printf("\nvortex_mid_info");

# endif

}


/*
  This is the implementation of vortex follow intended for a simulation
  consisting of 2 direction grids, hortizonal and vertical.
*/
void vortex_follow(agent_struct *agent, int action_index)
{ 
  int             row, col, state;
  function_struct temp_func;

  /* acquire the position of the current agent */
  col = agent->x_pos;
  row = agent->y_pos;

  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* cycle through the support grids to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* Since this is the 2 direction grid implementation, only 2 grids are wanted */
  if ( temp_func.number_grids != 2 )
    {
      error("Too many grids in function vortex_follow");
    }
  
  #ifdef _KILL_AMBIGUITY
    if( fabs( GGET( temp_func.update_grids[0].grid, row, col ) ) == fabs( (double)GGET( temp_func.update_grids[1].grid, row, col ) ) )
    {
      error("uh-oh: ambiguity\n");
      printf("grid val: %f\n", fabs( (double)GGET( temp_func.update_grids[0].grid, row, col ) ));
      printf("x: %d    y: %d\n",row,col);
      print_support_grid( 0 );
      print_support_grid( 1 );
      print_support_grid( 2 );
      exit(1);
    }
  #endif

  /* Grid 0 is the vertical grid   Grid 1 is the horizontal grid */
  if( abs( GGET( temp_func.update_grids[0].grid, row, col ) ) > abs( GGET( temp_func.update_grids[1].grid, row, col ) ) )
    {
      /* UP is negative */
      if ( GGET( temp_func.update_grids[0].grid, row, col ) < 0 )
	{
	  move_up( agent, action_index );
	}
      
      /* DOWN is positive */
      if ( GGET( temp_func.update_grids[0].grid, row, col ) > 0 )
	{
	  move_down( agent, action_index );
	}
      
      /* 0 -> ambiguous direction, which should never happen */
#ifdef _KILL_AMBIGUITY
      if ( GGET( temp_func.update_grids[0].grid, row, col ) == 0 )     
	{
	  error("0 recieved in vertical grid in vortex_follow\n");

	        print_support_grid( 0 );
	        print_support_grid( 1 );
	        print_support_grid( 2 );

	        exit(1);
	}
#endif
    }
  else
    {
      /* LEFT is negative */
      if ( GGET( temp_func.update_grids[1].grid, row, col ) < 0 )
	{
	  move_left( agent, action_index );
	}
      
      /* RIGHT is positive */
      if ( GGET( temp_func.update_grids[1].grid, row, col ) > 0 )
	{
	  move_right( agent, action_index );
	}
      
      /* 0 -> ambiguous direction, which should never happen */
      if ( GGET( temp_func.update_grids[1].grid, row, col ) == 0 )     
	{
	  error("0 recieved in horizontal grid in vortex_follow\n");

	  #ifdef _KILL_AMBIGUITY
    	        print_support_grid( 0 );
	        print_support_grid( 1 );
	        print_support_grid( 2 );

	        exit(1);
	  #endif
	}
    }

# ifdef vortex_error
  printf("\nvortex_follow");
# endif

}             
#undef _KILL_AMBIGUITY
