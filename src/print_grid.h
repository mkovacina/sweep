/*
** Purpose: To display the various support grids on the terminal screen
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _INCLUDED_PRINT_GRID_H_
#define _INCLUDED_PRINT_GRID_H_

/*--------------- Includes Needed for Definitions Below --------------*/

#include "useful.h"

/*---------------------------- Constants -----------------------------*/

/*------------------------------ Macros ------------------------------*/

/*------------------------------- Types ------------------------------*/

/*------------------------- Function Prototypes ----------------------*/
/* void sample (char *, int, Real);                                   */

void print_support_grid PROTO(( int ));
void print_agent_grid   PROTO(( void ));

void grid_dump          PROTO(( void ));

/*------------------------------ Globals -----------------------------*/
/* GLOBAL  float  example;                                            */

/* NOTHING GOES BEYOND THIS LINE !!!! */
#endif /* _INCLUDED_PRINT_GRID_H_ */
