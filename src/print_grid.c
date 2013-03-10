//#include "print_grid.h"
#include "support_grids.h"

void print_support_grid(int grid_index)
{
   int i, j;
   sgrid_ptr grd;

   printf("#### GRID # %d ####\n", grid_index);
   grd = (*(all_support_grids->grids))[ grid_index ];

   /* prints column numbers */
   printf("    ");
   for( j = 0; j < grd->cols; j++ )
     {
       printf("%2d ", j);
     }
   puts("");
   printf("----");
   for( j = 0; j < grd->cols; j++ )
     {
       printf("---");
     }
  puts("");

   for( i=0; i<grd->rows; i++)
     {
       /* print row number */
       printf("%2d| ", i);
      
       for( j=0; j<grd->cols; j++)
	 {
	   printf("%2d ", (int)(* (grd->prev_grid))[i][j]);
	 }
       printf("\n");
     }
   printf("###################\n\n");
}

void print_agent_grid( void )
{
   int i,j, rows,cols;

   rows = GGETR(0);
   cols = GGETC(0);

   printf("#### AGENT GRID ####\n");

   /* prints column numbers */
   printf("    ");
   for( j = 0; j < rows; j++ )
     {
       printf("%2d ", j);
     }
   puts("");
   printf("----");
   for( j = 0; j < cols; j++ )
     {
       printf("---");
     }
   puts("");

   for( i=0; i<rows; i++)
     {
       /* print row number */
       printf("%2d| ", i);

       for( j=0; j<cols; j++)
	 {
           printf("%2d ", (int)(GETAGT(i,j)));
	 }
       printf("\n");
     }
   printf("###################\n\n");
}

void grid_dump( void )
{
  int x;

  puts("\n########### GRID DUMP ###########\n");
  for( x = 0; x < all_support_grids->num_grids; x++ )
    {
      print_support_grid( x );
    }
  
  print_agent_grid();
}
