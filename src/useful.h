/*
** Purpose:     This file contains handy constants and macros.
**
*/

/* NOTHING GOES ABOVE THIS LINE!!!  (Multiple include protection) */
#ifndef _INCLUDED_USEFUL_H
#define _INCLUDED_USEFUL_H

/*------------------- Includes Needed for Definitions Below -----------------*/

#include <stdio.h>
#include <math.h>

/*-------------------------------- Constants --------------------------------*/

/* Logic */

#define SQRT_2          (1.414213562)
#define COS30           (0.866025403)
#define SIN60           COS30
#define TWO_OVER_SQRT3  (1.154700538)
#define SQRT3_OVER_FOUR (0.433012701)
#define EPSILON         (1e-7)
#define PI              (3.141592654)

/* Return Codes */
#define SUCCESS (0)    
#define FAILURE (1)


/*--------------------------------- Macros ----------------------------------*/

/* abs, min and max are obvious */
/* BOUND is the result of a clamp in the range a<=x<=b. */
/* WITHIN is true if a and b are within lim of being equal. */

#define ABS(a)          (((a)<0) ? -(a) : (a))
#define MIN(x,y)        (((x)<(y)) ? (x) : (y))
#define MAX(x,y)        (((x)>(y)) ? (x) : (y))
#define BOUND(x,a,b)    (((x)<(a)) ? (a) :  (((x)>(b)) ? (b) : (x)) )
#define WITHIN(lim,a,b) ((a)-(b) <= (lim) && (b)-(a) <= (lim))
#define INRANGE(val,low,hi) ( (low) <= (val) && (val) < (hi) )
#define EVEN(x)         (((x)%2) == 0)         
#define ODD(x)          (((x)%2) == 1)
#define TESTBIT(flags, bit)        ((flags) & (bit))
#define SET(flags, bit)         ((flags) |= (bit))
#define CLEAR(flags, bit)       ((flags) &= ~(bit))

/* Swap two values using the third argument as the temporary variable. */
#define SWAP(a, b, tmp) \
{ \
    (tmp) = (a); \
    (a) = (b); \
    (b) = (tmp); \
}

/* You can use these to define global variables in a header file. */
/* INIT can give an initial value if it's a simple type (number, string...) */

/* Example:     GLOBAL int var INIT(1);                                 */
/* if DEFINE_GLOBALS is set, translates into                            */
/*   int var = 1;                                                       */
/* and if DEFINE_GLOBALS is not set, translates into                    */
/*   extern int var;                                                    */

/* The compile time variable DEFINE_GLOBALS must be defined in one and  */
/* only one .c file for all this to work correctly.                     */

#ifdef DEFINE_GLOBALS
#  define GLOBAL
#  define INIT(initval) = initval
#else
#  define GLOBAL extern
#  define INIT(initval)
#endif


/* Define a function prototype with something like...                   */
/*   float Coolo_Function PROTO((int, float *, int [3]));               */
/* This turns into the full function prototype for ANSI compilers and   */
/* it turns into the old style kind otherwise.                          */

#ifdef __STDC__
#  define PROTO(args)   args
#else
#  define PROTO(args)   ()
#endif

/* generate random numbers between 0 and 1 or a specified range */
#if 0
                                /* This really returns a long int... */
extern int random();
#endif
#define RND()             ((float) (random() % 1000000) / 1000000.0)
#define RAND(x,y)         (x + (int) (RND()*(y-x+1)))
/*---------------------------------- Types ----------------------------------*/

/*---------------------------- Function Prototypes --------------------------*/

/*--------------------------------- Globals ---------------------------------*/



/* NOTHING GOES BEYOND THIS LINE!!! */
#endif /*_INCLUDED_USEFUL_H*/


