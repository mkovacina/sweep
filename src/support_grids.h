#pragma once

/*------------------- Includes Needed for Definitions Below -----------------*/

#include "useful.h"
#include "constants.h"

/*-------------------------------- Constants --------------------------------*/
/* #define MAX_BUFFER 120 */
#define MAX_SIZE 100
#define MAX_GRIDS 25

/*--------------------------------- Macros ----------------------------------*/

/* gets a value out of a feature grid */
#define FGET(f,r,c) ((f)[(r)][(c)])

/* gets values out of support_grids */
#define SGETC(s) ((s).curr_grid)
#define SGETP(s) ((s).prev_grid)
#define SGETR(s) ((s).rows)
#define SGETL(s) ((s).cols)
#define SGETN(s) ((s).num_params)
#define SGETU(s) ((s).update_grid)

/* Macros to read and write values in the support grids */
#define GGETR(n) ((*all_support_grids->grids)[n]->rows)
#define GGETC(n) ((*all_support_grids->grids)[n]->cols)

#define GGET(n,r,c) ((*(*all_support_grids->grids)[n]->prev_grid)[r][c])
#define GCURR(n,r,c)       ((*(*all_support_grids->grids)[n]->curr_grid)[r][c])
#define GSET(n,r,c,v)     (((*(*all_support_grids->grids)[n]->curr_grid)[r][c])=v)
#define GSET_CUM(n,r,c,v) (((*(*all_support_grids->grids)[n]->curr_grid)[r][c])=v+GCURR(n,r,c))

#define CLRAGT(r,c) (((*agent_grid)[r][c])=0)
#define SETAGT(r,c) (((*agent_grid)[r][c])=1)
#define GETAGT(r,c) ((*agent_grid)[r][c])
/*---------------------------------- Types ----------------------------------*/

#define MAX_SUPPORT_GRIDS (20)

typedef enum
{
	Uniform,
	File,
	Random,
	Computed,
	Distributed,
	Proportional,
	Toss,
	RestrictedToss,
}
GridInitializationMethod;

typedef struct
{
	float InitializationValue;
}
UniformInitializationParameters;

#define MAX_FILEPATH_LENGTH (256)

typedef struct 
{
	char filepath[MAX_FILEPATH_LENGTH];
}
FileInitializationParameters;

typedef struct
{
	float MinimumRandomValue;
	float MaximumRandomValue;
}
RandomInitializationParameters;

typedef struct
{
}
ComputedInitializationParameters;

#define MAX_DISTRIBUTED_VALUES (50)

typedef struct
{
	unsigned int NumberOfValues;
	float Values[MAX_DISTRIBUTED_VALUES];
}
DistributedInitializationParameters;

#define MAX_PROPORTIONAL_VALUES (30)

typedef struct
{
	unsigned int NumberOfValues;
	float Values[MAX_PROPORTIONAL_VALUES][2];
}
ProportionalInitializationParameters;

typedef struct
{
	unsigned int NumberOfValues;
	unsigned int ValueToPlace;
	unsigned int DefaultValue;
	unsigned int LowerLeftRow;
	unsigned int LowerLeftColumn;
	unsigned int UpperRightRow;
	unsigned int UpperRightColumn;
}
TossInitializationParameters;

typedef struct
{
	unsigned int NumberOfValues;
	unsigned int ValueToPlace;
	unsigned int DefaultValue;
	unsigned int LowerLeftRow;
	unsigned int LowerLeftColumn;
	unsigned int UpperRightRow;
	unsigned int UpperRightColumn;
	unsigned int HoleLowerLeftRow;
	unsigned int HoleLowerLeftColumn;
	unsigned int HoleUpperRightRow;
	unsigned int HoleUpperRightColumn;
}
RestrictedTossInitializationParameters;

typedef struct
{
}
StaticUpdateParameters;

typedef struct
{
}
ClearUpdateParameters;

typedef struct
{
	int RowOffest;
	int ColumnOffset;
}
MovementUpdateParameters;

typedef enum
{
	Static,
	Clear,
	Move,
	Diffusion,
	Cycle,
	Library,
	E,
}
GridUpdateMethod;

typedef struct 
{
	unsigned int GridID;
	GridInitializationMethod InitializationMethod;
	GridUpdateMethod UpdateMethod;
}
SupportGridDefinition;

typedef struct 
{
	unsigned int NumberOfSupportGrids;
	unsigned int NumberOfRows;
	unsigned int NumberOfColumns;
	SupportGridDefinition SupportGridDefinitions[MAX_GRIDS];	
}
SupportGridConfiguration;

typedef float feature_grid[MAX_SIZE][MAX_SIZE];

typedef feature_grid  *fgrid_ptr;

typedef struct s_grid_hold *sgrid_ptr;

typedef struct s_grid_hold{
			   int id;
               fgrid_ptr curr_grid, prev_grid;
               sgrid_ptr co_grid;
               int xoffset, yoffset;
               int rows, cols;
               int grid_type;
               int num_params;
               int radius;
               char *update_params[8];
               void (*update_grid)(sgrid_ptr);
        } support_grid;

typedef sgrid_ptr s_grids[MAX_GRIDS]; 

typedef s_grids *all_supports;

typedef struct {
               int          num_grids;
               all_supports grids;
        } all_grids;

typedef all_grids *s_grids_ptr;

typedef char (*pri_func_ptr)(s_grids_ptr, int, int);

/*---------------------------- Function Prototypes --------------------------*/

/* void         Sample PROTO((char *, int, Real)); */

   char         support_update PROTO((s_grids_ptr, int, int));
   void         init_priority_funcs PROTO((pri_func_ptr []));
   void         update_all_grids(void);

/*--------------------------------- Globals ---------------------------------*/
/* GLOBAL    float    example; */

extern s_grids_ptr all_support_grids;
extern fgrid_ptr agent_grid;
