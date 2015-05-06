#include "io.h"
#include <stdio.h>
#include <stdlib.h>

void print_result(int current_step, char *output, pArray array){
  FILE *output_file;
  char *output_name;
  int i, j;
  output_name = (char *)malloc((strlen(output) + 10)*sizeof(char));
  sprintf(output_name, "%s_%.5d.dat", output, current_step);
  output_file = fopen(output_name, "w");
  for(i = 0; i < array.size[0]; i++){
    for(j = 0; j < array.size[1]; j ++){
      fprintf(output_file, "%d %d %lf\n", i, j, array.p[i][j].u);
    }
    fprintf(output_file,"\n");
  }
  fclose(output_file);
}

pArray read_input(char *input, double *alpha, int *time_steps){
  FILE *input_file;
  pArray array;
  int xdim, ydim, hold, i, j;
  char x[10], y[10];//assumes the positions are less than 10^10-1
  double temp;
  input_file = fopen(input, "r");
  fscanf(input_file, "%d %d %lf %d", &xdim, &ydim, alpha, time_steps);
  array = create_pArray(xdim, ydim);
  while(fscanf(input_file, "%s %s %lf %d", x, y, &temp, &hold) != EOF){
    if(strcmp("*", x) == 0){//x is wildcard
      if(strcmp("*", y) == 0){//x and y are both wildcard
        for(i = 0; i < xdim; i++){
          for(j = 0; j < ydim; j++){
            array.add_value(array.p, i, j, temp, hold);
          }
        }
      } else{
        for(i = 0; i < xdim; i++){
          array.add_value(array.p, i, atoi(y), temp, hold);
        }
      }
    } else if(strcmp("*", y) == 0){//only y is wildcard
      for(j = 0; j < ydim; j++){
        array.add_value(array.p, atoi(x), j, temp, hold);
      }
    } else {
      array.add_value(array.p, atoi(x), atoi(y), temp, hold);
    }
  }
  fclose(input_file);
  return array;
}
