/* This program reads in a priority file, and generates C functions that 
   correspond to each set of priorities.  
*/

#include <stdio.h>
#include <stdlib.h>
#include "support_grids.h"

int main()
{
    FILE *priority_file;
    char buffer[MAX_BUFFER];
    int count, pos, start, num_priorities, inc_amt, entries;
    int num_priority_types;
    
    /* open the priorities data file */
    priority_file = fopen("priority.dat", "r");
    if (priority_file == NULL) {
       printf("Can't open file: %s\n", "priority.dat");
       exit(1);
    }

    printf("#include \"support_grids.h\"\n\n");
    printf("\n\n /***** WARNING *****/\n");
    printf(" /***** WARNING *****/\n");
    printf(" /*                 */\n");
    printf(" /* these functions */\n");
    printf(" /* are generated   */\n");
    printf(" /* automatically!! */\n");
    printf(" /* do not modify!! */\n");
    printf(" /*                 */\n");
    printf(" /***** WARNING *****/\n");
    printf(" /***** WARNING *****/\n");
    printf("\n\n");
    printf("\n");

    /* Determine how many sets of priorities there are */
    fgets(buffer, MAX_BUFFER, priority_file);
    num_priority_types = atoi(buffer);

    /* Generate an app_priority function for each set of priorities */
    for (entries=0; entries<num_priority_types;entries++){

        /* number versions of the functions to correspond with priority file */
        printf("char app_priority%d(s_grids_ptr all_support_grids, ",entries); 
        printf("int row, int col)\n");
        printf("\n");
    
        fgets(buffer, MAX_BUFFER, priority_file);
        num_priorities = atoi(buffer);

        printf("{\n");
        printf("   ");

        /* Generate a cascading if-elseif statement for each priority in the */
        /* set of priorities */
        for (count=1; count <= num_priorities; count += inc_amt){

            fgets(buffer, MAX_BUFFER, priority_file);

            /* if line begins with a #, it is a comment, ignore it */
            /* and don't increment the priority count              */
            if (buffer[0] == '#') {
               inc_amt = 0;
               continue;
            }

            /* verify that the priority file is of the proper format */
            /* handles up to 999 priorities in a priority set */
            if (count < 10) {
               if (buffer[0] != '0' + count){
                  printf("Illegal priority file construction at entry %d\n",
                          count);
                  exit(1);
               }
               else {
                  start = 2;
               }
            }
            else if (count < 100) {
               if ((buffer[0]-'0')*10+(buffer[1]-'0') != count){
                  printf("Illegal priority file construction at entry %d\n",
                          count);
                  exit(1);
               }
               else {
                  start = 3;
               }
            }
            else if (count < 1000) {
               if ((buffer[0]-'0')*100+(buffer[1]-'0')
                               *10+(buffer[2]-'0') != count){
                  printf("Illegal priority file construction at entry %d\n",
                          count);
                  exit(1);
               }
               else {
                  start = 4;
               }
            }
            else {
               printf("Illegal priority file - too many priorities!\n");
               exit(1);
            }

            /* found a valid line in priority file, so increment count */
            inc_amt = 1;

            /* Begin generating the cascading if-elseif statement */
            printf("if ( ");

            /* Get the boolean expression from the priority file */
            /* and insert it into the conditional expression */
            for(pos=start;buffer[pos] != ':';pos++){
               printf("%c", buffer[pos]);
               if (buffer[pos] == 'G') {
                  printf("GET(%c,row,col)", buffer[++pos]);
               }
               /* # sign is comment character for priority file, ignore */
               /* everything after it until the end of line             */
               if (buffer[pos] == '#') {
                  break;
               }
            }

            if (buffer[++pos] != 'G'){
               printf(" ) {\n      return \'%c\';\n   }", buffer[pos]);
            }
            else {
               printf(" ) {\n      return GGET(%c,row,col);\n   }", 
                                                           buffer[++pos]);
            }

            printf("\n");
            printf("   else ");
       }

       /* default is to return a question mark if no priority matches */
       printf("{ \n");
       printf("      return '?';\n   }\n} /* app_priority%d*/\n\n\n", entries);
   }

   printf("\n");

   /* generate the function that initializes the array of pointers to */
   /* priority functions */
   printf("void init_priority_funcs(pri_func_ptr priority_grid[])\n");
   printf("{\n");
   
   for (entries=0; entries<num_priority_types; entries++){
       printf("     priority_grid[%d] = app_priority%d;\n", entries, entries);
   }

   printf("}\n");

   return 0;
}











