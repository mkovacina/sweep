void print_support_grid(int grid_index)
{
   int i,j;
   sgrid_ptr grd;

   printf("#### GRID # %d ####\n", grid_index);
   grd = (*(all_support_grids->grids))[grid_index];
   for (i=0;i<grd->rows;i++){
       for (j=0;j<grd->cols;j++){
           printf("%d ", (int)(* (grd->prev_grid))[i][j]);
       }
       printf("\n");
   }
   printf("###################\n\n");
}

void print_agent_grid()
{
   int i,j,rows,cols;

   rows = GGETR(0);
   cols = GGETC(0);

   printf("#### AGENT GRID ####\n");
   for (i=0;i<rows;i++){
       for (j=0;j<cols;j++){
           printf("%d ", (int)(GETAGT(i,j)));
       }
       printf("\n");
   }
   printf("###################\n\n");
}

