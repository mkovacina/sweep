# include <stdio.h>

extern FILE* ERROR_FILE;

void start_log() {

  ERROR_FILE = fopen("error.log", "w");

}

void error(char* error_message) {

  fprintf(ERROR_FILE, "%s", error_message);
  fprintf(stdout, "%s\n", error_message);

}

void stop_log() {

  fclose(ERROR_FILE);

}

  
