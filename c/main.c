#include "update.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  char *input;
  char *output;
  double alpha;
  int time_steps, output_freq;
  pArray array;
  if(argc != 4){
    printf("Program called incorrectly!\n");
    printf("Please use the program with the following input:\n");
    printf("\t./program input.dat output_freq output\n");
    exit(1);
  } else {
    input = (char *)malloc(strlen(argv[1])*sizeof(char));
    strcpy(input, argv[1]);
    output_freq = atoi(argv[2]);
    output = (char *)malloc(strlen(argv[3])*sizeof(char));
    strcpy(output, argv[3]);
  }

  array = read_input(input, &alpha, &time_steps);
  simulation(array, alpha, time_steps, output_freq, output);
  clear_pArray(array);
  free(input);
  free(output);

  return 0;
}
