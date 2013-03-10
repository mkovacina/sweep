#include <stdio.h>

int main()
{

  int i,j,k, row, col;
  int row_start, row_end, col_start, col_end;
  int data[100][100];

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
        data[i][j] = 0.0;
    }
  }

  row_start++;
  row_end--;
  col_start++;
  col_end--;

  while (row_start <= row_end && col_start <= col_end){
    for (i = col_start; i <col_end; i++){
       for (j = row_start; j<row_end; j++){
           data[i][j] = data[i][j] + inc;
       }
    }

    row_start++;
    row_end--;
    col_start++;
    col_end--;
  }

  for (i = 0; i <col; i++){
    for (j = 0; j<row; j++){
        printf("%d ", data[i][j]);
    }
    printf("\n");
  }
}
    
