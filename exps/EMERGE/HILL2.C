#include <stdio.h>
#include <math.h>

int main()
{

  int i,j,k, row, col;
  int row_start, row_end, col_start, col_end;
  int data[100][100];
  int max, rcent, ccent;
  float dist;

  int inc, base;

  printf("Enter the size of the grid: row,col \n");
  scanf("%d%d", &row, &col);

  printf("Enter the increment\n");
  scanf("%d", &inc);

  printf("Enter the start value\n");
  scanf("%d", &base);

  row_start = 0;
  row_end = row;
  col_start = 0;
  col_end = col;

  for (i = col_start; i <col_end; i++){
    for (j = row_start; j<row_end; j++){
        data[i][j] = base;
    }
  }
  
  max = 0;

  rcent = row_end/2;
  ccent = col_end/2;

    for (i = col_start; i <col_end; i++){
       for (j = row_start; j<row_end; j++){
	   dist = sqrt((double)(i - ccent)*(i - ccent) + (j - rcent)*(j - rcent));
           data[i][j] = (int)dist;
	   if (data[i][j] > max){
	      max = data[i][j];
	   }
       }
    }


  for (i = 0; i <col; i++){
    for (j = 0; j<row; j++){
        data[i][j] = max - data[i][j] + base;
        printf("%d ", data[i][j]);
    }
    printf("\n");
  }
}
    
