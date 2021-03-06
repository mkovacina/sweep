/* gives access to error() */
#include "errors.h"

/* give access to print_*_grid() */
#include "print_grid.h"

#define UP     1
#define DOWN   2
#define RIGHT  3
#define LEFT   4

#define UP_GRID 0
#define DOWN_GRID 1
#define RIGHT_GRID 2
#define LEFT_GRID 3

#define MAX_STUCK_COUNT 4

static enum field_index { PROXIMITY_STRENGTH,
			  BUFFER,
			  PROXIMITY_RADIUS,
			  VELOCITY,
			  THRUST,
			  THETA,
			  OMEGA,
			  /* END OF FILE INITIALIZED FIELDS */
			  X_POS,
			  Y_POS,
			  STUCK,
			  COUNT,
			  WAIT,
			  FIRST_TIME } index;

void AF_move( agent_struct *agent, int action_index );
void AF_put( int action_grid, int row, int col, int value );
void runge_kutta( float *xx, float *yy, float *vv, float *thetaT, float T, float w, float dt );


void AF_random_move( agent_struct *agent, int action_index )
{
  if ( agent->field[ COUNT ] >= agent->field[ WAIT ] )
    {
      agent->field[ COUNT ] = 0.0;
      agent->field[ WAIT  ] = RAND( 15, 300 );

      agent->field[ THRUST ] = RAND( 1, 100 );
      agent->field[ THETA  ] = RAND( -15, 15 );
      agent->field[ OMEGA  ] = (float)RAND( -300, 300 ) / 1000.0;
    } 

  AF_move( agent, action_index );

  agent->field[ COUNT ] += 1.0;
}

void AF_move( agent_struct *agent, int action_index )
{
  float *x;
  float *y;
  float *v;
  float *theta;
  float thrust;
  float omega;
  float dt;

  if ( agent->field[ FIRST_TIME ] < 1.0 )
    {
      agent->field[ X_POS ] = agent->x_pos;
      agent->field[ Y_POS ] = agent->y_pos;
      agent->field[ FIRST_TIME ] = 2.0;
    }

  x = &(agent->field[ X_POS ]);
  y = &(agent->field[ Y_POS ]);
  v = &(agent->field[ VELOCITY ]);
  theta = &(agent->field[ THETA ]);
  thrust = agent->field[ THRUST ];
  omega = agent->field[ OMEGA ];
  dt = 0.1;

  runge_kutta( x, y, v, theta, thrust, omega, dt );

  agent->x_pos = (int)(*x);
  agent->y_pos = (int)(*y);
}
 
void runge_kutta( float *xx, float *yy, float *vv, float *thetaT, float T, float w, float dt ) 
{
  float f11, f12, f13, f14;
  float f21, f22, f23, f24;
  float f31, f32, f33, f34;
  float f41, f42, f43, f44;

  float x = *xx;
  float y = *yy;
  float v = *vv;
  float theta = *thetaT;

  int k = 10;

  f11 = dt*v*cos(theta);
  f12 = dt*v*sin(theta);
  f13 = dt*(T-k*v);
  f14 = dt*w;
  
  f21 = dt*(v+f13/2)*cos(theta+f14/2);
  f22 = dt*(v+f13/2)*sin(theta+f14/2);
  f23 = dt*(T-k*(v+f13/2));
  f24 = dt*w;
  
  f31 = dt*(v+f23/2)*cos(theta+f24/2);
  f32 = dt*(v+f23/2)*sin(theta+f24/2);
  f33 = dt*(T-k*(v+f23/2));
  f34 = dt*w;
  
  f41 = dt*(v+f33)*cos(theta+f34);
  f42 = dt*(v+f33)*sin(theta+f34);
  f43 = dt*(T-k*(v+f33));
  f44 = dt*w;
  
  *xx=x+f11/6+f21/3+f31/3+f41/6;
  *yy=y+f12/6+f22/3+f32/3+f42/6;
  *vv=v+f13/6+f23/3+f33/3+f43/6;
  *thetaT=theta+f14/6+f24/3+f34/3+f44/6;
}


void AF_safe_wander( agent_struct *agent, int action_index )
/*  "safe" wandering, will not allow multiple agents to occupy same location */
{
  char *function_name = "AF_safe_wander";
  int  num_grids      = 1;

  int dir, count, test_val; /* randomly selected direction */
  int row, col, rows, cols, state;

  function_struct    temp_func;
  update_grid_struct temp_grid;
  int                action_grid;

#ifdef AF_error
  puts( function_name );
#endif

  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* acquire the FSA state of the current agent */
  state = agent->fsa->current_state;

  /* access the support grid to be updated by this function */
  temp_func = agent->state_functions[state].function_list[action_index];

  /* check for the correct number of grids */
  if ( temp_func.number_grids != num_grids )
  {
    printf( "Insufficient number of grids supplied to %s().\n", function_name );
    printf( "%d grid is required for %s()\n", num_grids, function_name );
    exit( 1 );
  }

  temp_grid = temp_func.update_grids[0];

  /* determines which support grid contains pertinent information */
  action_grid = temp_grid.grid;
 
  rows = GGETR(0) - 1;
  cols = GGETC(0) - 1;

  dir = (int)(100 * ((double)rand()/(double)RAND_MAX));
  count = 0;

  switch( dir%4 )
  {
    while( count < 4 )
    {
      /* move up */
      case 0:
	if ( count < 4 && row > 0 )
	{
	  test_val = GETAGT( row - 1, col );
	  if ( !test_val )
	  {
	    move_up( agent, 0 );
	    return;
	  }
	}
	count++;

      /* move right */
      case 1:
	if ( count < 4 && col < cols )
	{
	  test_val = GETAGT( row, col + 1 );
	  if ( !test_val )
	  {
	    move_right( agent, 0 );
	    return;
	  }
	}
	count++;

      /* move down */
      case 2:
	if ( count < 4 && row < rows )
	{
	  test_val = GETAGT( row + 1, col );
	  if ( !test_val )
	  {
	    move_down( agent, 0 );
	    return;
	  }
	}
	count++;

      /* move left */
      case 3:
	if ( count < 4 && col > 0 )
	{
	  test_val = GETAGT( row, col - 1 );
	  if ( !test_val )
	  {
	    move_left( agent, 0 );
	    return;
	  }
	}
	count++;
    }
  }
}

void AF_safe_follow(agent_struct *agent, int action_index)
{
  int up, down, left, right, direction=0;
  int row, col;


#ifdef AF_error
  puts( "AF_safe_follow" );
#endif

  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;


  up = GGET( UP_GRID, row, col );
  right = GGET( RIGHT_GRID, row, col );
  down = GGET( DOWN_GRID, row, col );
  left = GGET( LEFT_GRID, row, col );

  if ( up >= left && up >= down && up >= right )
    {
      direction = UP;
    }

  if ( right >= left && right >= down && right >= up )
    {
      direction = RIGHT;
    }

  if ( down >= left && down >= up && down >= right )
    {
      direction = DOWN;
    }

  if ( left >= up && left >= down && left >= right )
    {
      direction = LEFT;
    }


  switch( direction )
    {
    case UP:      
      if( !GETAGT( row-1, col ) )
	{
	  move_up( agent, 0 );
	}
      break;
      
    case RIGHT:
      if( !GETAGT( row, col+1 ) )
	{
	  move_right( agent, 0 );
	}  
      break;
      
    case DOWN:      
      if( !GETAGT( row+1, col ) )
	{
	  move_down( agent, 0 );
	}
      break;
      
    case LEFT:      
      if( !GETAGT( row, col-1 ) )
	{
	  move_left(agent,0);
	}
      break;
      
    default:     
      printf("ERROR# 983\n");
      abort();
      break;
    }

  if ( row == agent->y_pos && col == agent->x_pos )
    {
      agent->field[STUCK] = agent->field[STUCK]+1;
    }
  else
    {
      agent->field[STUCK] = 0;
    }

  if ( agent->field[STUCK] > MAX_STUCK_COUNT )
    {
      AF_safe_wander( agent, action_index );
    }
}

void AF_set_proximity(agent_struct *agent, int action_index)
{
  int row, col;
  int r, c, radius;


#ifdef AF_error
  puts( "AF_set_proximity" );
#endif

  /* acquire the position of the current agent */
  row = agent->y_pos;
  col = agent->x_pos;
   
  /* vain attempt for circularity */

  for( radius = (int)(agent->field[PROXIMITY_RADIUS])+(int)(agent->field[BUFFER]); radius > (int)(agent->field[BUFFER]); radius-- )
    {
      /* TOP: moves left to right */
      for( c = -radius; c < radius; c++ )
	{
	  AF_put( RIGHT_GRID, row-radius, col+c, (int)(agent->field[PROXIMITY_STRENGTH]) );
	}

      /* BOTTOM: moves left to right */
      for( c = -radius+1; c < radius+1; c++ )
	{
	  AF_put( LEFT_GRID, row+radius, col+c, (int)(agent->field[PROXIMITY_STRENGTH]) );
	}

      /* RIGHT: moves up to down */
      for( r = -radius; r < radius; r++ )
	{
	  AF_put( DOWN_GRID, row+r, col+radius, (int)(agent->field[PROXIMITY_STRENGTH]) );
	}

      /* LEFT: moves up to down */
      for( r = -radius+1; r < radius+1; r++ )
	{
	  AF_put( UP_GRID, row+r, col-radius, (int)(agent->field[PROXIMITY_STRENGTH]) );
	}

      /* moves the agent into the next layer of the spiral */
      if ( GGET( RIGHT_GRID, row-radius+1, col-radius ) == 0 )
	{
	  AF_put( RIGHT_GRID, row-radius+1, col-radius, (int)(agent->field[PROXIMITY_STRENGTH]) );
	}
    }
}


void AF_put( int action_grid, int row, int col, int value )
{
  if ( 0 <= row && row < GGETR( action_grid ) )
    {
      if ( 0 <= col && col < GGETC( action_grid ) )
	{
	  GSET_CUM( action_grid, row, col, value );
	}
    }
}
