/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

#define UP    -1
#define DOWN   1
#define RIGHT  2
#define LEFT  -2

#define HALT   3

/* 
   One grid is supplied

   grid #0: trail grid
*/
void leader_wander(agent_struct *agent, int action_index)
/*  "safe" wandering, will not allow multiple agents to occupy same location, also
    leaves a trail for others to follow, leader tends to wander in a straight
    direction, turning only occaisionally */
{

  int dir, count, test_val; /* randomly selected direction */
  int row, col, rows, cols, state, action_grid;

  function_struct    temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("leader_wander\n");
#endif

  /* acquire the position of the current agent */
  row = agent->x_pos;
  col = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to leader_wander().\n" );
    error( "One grid is required for leader_wander()\n" );
    exit(1);
  }

  temp_grid = temp_func.update_grids[0];
  
  /* determines which support grid contains pertinent information */
  action_grid = temp_func.update_grids[0].grid;
 
  rows = GGETR(0);
  cols = GGETC(0);


  dir = (int)(100 * ((double)rand()/(double)RAND_MAX));
  count = 0;
  
  switch( dir%4 )
  { 
    while( count < 4 )
    { 
      /* move up */
      case 0:  
	if ( count < 4 && col > 0 ) 
	{ 
	  test_val = GETAGT( col - 1, row );
	  if ( !test_val )
	  {
	    GSET( action_grid, col, row, UP );
	    move_up( agent, 0 ); 
	    return;
	  }
	}
	count++;
	  
      /* move right */
      case 1:  
	if ( count < 4 && row < rows ) 
	{
	  test_val = GETAGT( col, row + 1 );
	  if ( !test_val )
	  {
	    GSET( action_grid, col, row, RIGHT );
	    move_right( agent, 0 ); 
	    return;
	  }
	}
	count++;

      /* move down */
      case 2:  
	if ( count < 4 && col < cols ) 
	{
	  test_val = GETAGT( col + 1, row );
	  if ( !test_val )
	  {
	    GSET( action_grid, col, row, DOWN );
	    move_down( agent, 0 ); 
	    return;
	  }
	}
	count++;
      
      /* move left */  
      case 3:  
	if ( count < 4 && row > 0 ) 
	{
	  test_val = GETAGT( col, row - 1 );
	  if ( !test_val )
	  {
	    GSET( action_grid, col, row, LEFT );
	    move_left( agent, 0 ); 
	    return;
	  }
	}
	count++;
    }
  }
}


/* Two grid is supplied

   grid #0: message grid
   grid #1: trail grid
*/
void leader_recruit(agent_struct *agent, int action_index)
{
  int row, col, state, dir;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("leader_recruit\n");
#endif


  /* acquire the position of the current agent */
  row = agent->x_pos;
  col = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 2 )
  {
    error( "Insufficient number of grids supplied to leader_recruit().\n" );
    error( "One grid is required for leader_recruit()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET( temp_grid.grid, col, row );

  GSET( temp_grid.grid, col + 1, row, HALT );
  GSET( temp_grid.grid, col, row - 1, HALT );
  GSET( temp_grid.grid, col, row + 1, HALT );
  GSET( temp_grid.grid, col - 1, row, HALT );

  temp_grid = temp_func.update_grids[1];

  switch( dir )
  {
    case UP:
      GSET( temp_grid.grid, col - 1, row, DOWN );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col - 1, row, 0 );      
      break;

    case RIGHT:
      GSET( temp_grid.grid, col, row + 1, LEFT );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col, row + 1, 0 );
      break;

    case DOWN:
      GSET( temp_grid.grid, col + 1, row, UP );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col + 1, row, 0 );
      break;

    case LEFT:
      GSET( temp_grid.grid, col, row - 1, RIGHT );

      temp_grid = temp_func.update_grids[0];
      GSET( temp_grid.grid, col, row - 1, 0 );
      break;

    default:
      puts("Uh-oh");printf("dir = %d\n",dir);grid_dump();/*abort()*/;break; 
  }
}


/* One grid is supplied

   grid #0: trail grid
*/
void follower_wander_info( agent_struct *agent, int action_index )
{
  /* This function allows agents to follow a trail left by a "leader".  The directions
     are N=1,E=2,S=3,W=4.  In addition, the code ensures that an agent following a
     trail will NOT move into an occupied cell.  As a result, the trail of followers 
     will not collapse, but the trail will not be lost either.  */

  int row, col, state;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("follower_wander_info\n");
#endif


  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to follower_wander_info().\n" );
    error( "One grid is required for follower_wander_info()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  /* acquire the position of the current agent */
  row = agent->x_pos;
  col = agent->y_pos;   

  GSET( temp_grid.grid, col + 1, row, UP    );
  GSET( temp_grid.grid, col - 1, row, DOWN  );
  GSET( temp_grid.grid, col, row + 1, LEFT  );
  GSET( temp_grid.grid, col, row - 1, RIGHT );
}


/* One grid is supplied

   grid #0: trail grid
*/
void follower_follow(agent_struct *agent, int action_index)
{
  /* 
     This function allows agents to follow a trail left by a "leader".  The directions
     are N=1,E=2,S=3,W=4.  In addition, the code ensures that an agent following a
     trail will NOT move into an occupied cell.  As a result, the trail of followers 
     will not collapse, but the trail will not be lost either. 
 */

  int dir;
  int row, col, state;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("follower_follow\n");
#endif


  /* acquire the position of the current agent */
  row = agent->x_pos;
  col = agent->y_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to follower_follow().\n" );
    error( "One grid is required for follower_follow()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET(temp_grid.grid,col,row);

  /* copy the direction to the new grid for next agent or for self if waiting */
  GSET( temp_grid.grid, col, row, dir );

  switch( dir )
  {
    case UP:      
      if( !GETAGT( col-1, row ) )
      {
	move_up( agent, 0 );
      }
      break;

     case RIGHT:
       if( !GETAGT( col, row+1 ) )
       {
	 move_right( agent, 0 );
       }  
       break;

     case DOWN:      
       if( !GETAGT( col+1, row) )
       {
	 move_down( agent, 0 );
       }
       break;

     case LEFT:      
       if( !GETAGT( col, row-1 ) )
       {
	 move_left(agent,0);
       }
       break;

     default:     
       printf("Invalid direction found -- program aborting\n");
       abort();
       break;
  }
}



/* One grid is supplied

   grid #0: trail grid
*/
void follower_halt( agent_struct *agent, int action_index )
{
  /* This function allows agents to follow a trail left by a "leader".  The directions
     are N=1,E=2,S=3,W=4.  In addition, the code ensures that an agent following a
     trail will NOT move into an occupied cell.  As a result, the trail of followers 
     will not collapse, but the trail will not be lost either.  */

  int row, col, state, dir;

  function_struct temp_func;
  update_grid_struct temp_grid;

#ifdef collector_error
  printf("follower_halt\n");
#endif


  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != 1 )
  {
    error( "Insufficient number of grids supplied to follower_halt().\n" );
    error( "One grid is required for follower_halt()\n" );
    exit(1);
  }

  /* determines which support grids contains pertinent information */
  temp_grid = temp_func.update_grids[0];

  dir = GGET( temp_grid.grid, row, col );

  GSET( temp_grid.grid, row, col, dir );

  /*
    do nothing for now
  */
}



