/*
** Purpose:
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _support_grids
#define _support_grids

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
#define GCURR(n,r,c) ((*(*all_support_grids->grids)[n]->curr_grid)[r][c])
#define GSET(n,r,c,v) (((*(*all_support_grids->grids)[n]->curr_grid)[r][c])=v)

#define GSET_CUM(n,r,c,v) (((*(*all_support_grids->grids)[n]->curr_grid)[r][c])=v+GCURR(n,r,c))

#define CLRAGT(r,c) (((*agent_grid)[r][c])=0)
#define SETAGT(r,c) (((*agent_grid)[r][c])=1)
#define GETAGT(r,c) ((*agent_grid)[r][c])
/*---------------------------------- Types ----------------------------------*/

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

/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /*_support_grids*/



