/* gives access to error() */
#include "errors.h"

typedef struct
{
  int x;
  int y;
}
Point;

#define QUADRANT_ONE   1
#define QUADRANT_TWO   2
#define QUADRANT_THREE 3
#define QUADRANT_FOUR  4
#define OUT_OF_RANGE   0

#define HEADING_LEFT  -1
#define HEADING_RIGHT  1

#define MAX_OMEGA 1.0

static enum field_index { VELOCITY,
			  THRUST,
			  THETA,
			  OMEGA,
			  SEARCH_RADIUS,
			  TOO_CLOSE,
			  TOO_FAR,
			  /* END OF FILE INITIALIZED FIELDS */
			  X_POS,
			  Y_POS,

			  // for use with SaD_random_move
			  OUT_OF_BOUNDS,
			  DELTA_OMEGA,
			  COUNT,
			  WAIT,

			  TURNING,
			  FIRST_TIME } index;

void SaD_seek( agent_struct *agent, int action_index );
void SaD_move( agent_struct *agent, int action_index );
void SaD_random_move( agent_struct *agent, int action_index );
int find( agent_struct* agent, Point* p );
int distance_to_closest( agent_struct *agent );
void runge_kutta( float *xx, float *yy, float *vv, float *thetaT, float T, float w, float dt );


void SaD_seek( agent_struct *agent, int action_index ) 
{
  /* !!! N.B. distance is in truncated integer form */
  if ( !INRANGE( distance_to_closest( agent ), agent->field[ TOO_CLOSE ], agent->field[ TOO_FAR ] ) )
    {
      SaD_random_move( agent, action_index );
    }
  else
    {
      SaD_move( agent, action_index );
    }
}

int find( agent_struct* agent, Point *p )
{
  int rows, cols, r, c;
  int x, y, radius;

  x = agent->x_pos;
  y = agent->y_pos;
  radius = (int)agent->field[ SEARCH_RADIUS ];

  rows = GGETR(0);
  cols = GGETC(0);

  for( c = x - radius; c < (x + radius); c++ )
    {
      for( r = y - radius; r < (y + radius); r++ )
	{
	  if ( 0 <= r && r < rows && 0 <= c && c < cols )
	    {
	      if ( GETAGT( r, c ) && x != c && y != r )
		{
		  p->x = c;
		  p->y = r;

		  if ( r >= y )
		    {
		      if ( c > x )
			{
			  return QUADRANT_FOUR;
			}
		      else
			{
			  return QUADRANT_ONE;
			}
		    }
		  else
		    {
		      if ( c > x )
			{
			  return QUADRANT_THREE;
			}
		      else
			{
			  return QUADRANT_TWO;
			}
		    }
		}
	    }
	}
    }

  return OUT_OF_RANGE;
}

int distance_to_closest( agent_struct *agent )
{
  Point p;
  int result = 0;

  if ( find( agent, &p ) != OUT_OF_RANGE )
    {
      result = (int)sqrt( (p.x - agent->x_pos)*(p.x - agent->x_pos) + (p.y - agent->y_pos)*(p.y - agent->y_pos) );
    }

  return result;
} 

void SaD_random_move( agent_struct *agent, int action_index )
{
  if ( agent->field[ COUNT ] >= agent->field[ WAIT ] )
    {
      agent->field[ COUNT ] = 0.0;
      agent->field[ WAIT  ] = RAND( 15, 300 );
      
      agent->field[ THRUST ] = RAND( 1, 100 );
      agent->field[ OMEGA  ] = (float)RAND( -500, 500 ) / 1000.0;
      if ( RAND( 0, 1000 ) > 500 )
	{
	  agent->field[ OMEGA ] = 0.0;
	}
      agent->field[ DELTA_OMEGA  ] = (float)RAND( -300, 300 ) / 10000.0;
    }

  if ( INRANGE( agent->x_pos, 0, GGETC(0) ) && INRANGE( agent->y_pos, 0, GGETR(0) ) )
    {
      agent->field[ OMEGA ] += agent->field[ DELTA_OMEGA ];
      agent->field[ OMEGA ] = BOUND( agent->field[ OMEGA ], -MAX_OMEGA, MAX_OMEGA );
      SaD_move( agent, action_index );
      agent->field[ COUNT ] += 1.0;
    } 
  else
    {
      float temp1 = agent->field[ OMEGA ];
      float temp2 = agent->field[ THRUST ];
      agent->field[ OMEGA  ] = 1;
      agent->field[ THRUST ] = 100;
      SaD_move( agent, action_index );
      agent->field[ OMEGA  ] = temp1;
      agent->field[ THRUST ] = temp2;
    }

  //fprintf( stderr, "(x,y) = ( %d , %d )\n", agent->x_pos, agent->y_pos );
}

void SaD_move( agent_struct *agent, int action_index )
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
