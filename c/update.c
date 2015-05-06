#include "update.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>


pArray create_pArray(int xdim, int ydim){
  int i;
  int *info, *size;
  pArray array;
  //set up info and size, specific to this project
  array.size = (int *)malloc(2 * sizeof(int));
  array.size[0] = xdim;
  array.size[1] = ydim;
  array.info = (int *)malloc(2 * sizeof(int));
  array.info[0] = 2;
  array.info[1] = sizeof(particle *);
  //create particle array
  array.p = (particle **)malloc(xdim * sizeof(particle *));
  for(i = 0; i < xdim; i++){
    array.p[i] = (particle *)malloc(ydim * sizeof(particle));
  }
  //attempt to be more object oriented
  array.add_value = &pArray_add_value;
  return array;
}

void pArray_add_value(particle **p, int x, int y, double temp, int h){
  p[x][y].hold = h;
  p[x][y].u = temp;
}

void clear_pArray(pArray array){
  array_free(array.p, array.info, array.size);
  free(array.info);
  free(array.size);
}

void array_free(void *pointer, int *info, int *size){ //info format - (dimension, size of element pointer) size -(size of first dimension, size of second, third...)
  int dimension;
  int i, j, pointer_size;
  char *temp = (char *)pointer;
  char *temp2;
  pointer_size = info[1];
  dimension = info[0];
  //printf("Dimension of this array is %d\n", dimension);
  //using recursiong to free arrays
  if(dimension == 1){
    free(pointer);
  } else if(dimension == 2) {
    for(i = 0; i < size[0]; i++){
      temp2 = *(((char **)temp) + i*(pointer_size/sizeof(char *)));
      //printf("Freeing another pointer %d\n", counting++);
      free(temp2);
    }
    //printf("Freeing another pointer %d\n", counting++);
    free(temp);
  } else {
    for(i = 0; i < size[0]; i++){
      temp2 = *(((char **)temp) + i*(pointer_size/sizeof(char *)));
      int *new_info;
      new_info = (int *)malloc(2 *sizeof(int));
      new_info[0] = dimension - 1;
      new_info[1] = info[1];
      int *new_size;
      new_size = (int *)malloc((info[0] - 1)* sizeof(int));
      for(j = 0; j < info[0]-1; j++){
        new_size[j] = size[j+1];
      }
      //printf("Going in a layer\n");
      array_free(temp2, new_info, new_size);
      //printf("finished a layer\n");
      free(new_info);
      free(new_size);
    }
    //printf("Freeing another pointer %d\n", counting++);
    free(temp);
  }
}

void simulation(pArray array, double alpha, int steps, int output_freq, char *output){
  int t,i,j;
  particle **temp;
  pArray prevArray;
  prevArray = create_pArray(array.size[0], array.size[1]);
  t = 0;
  //write initial step
  print_result(t, output, array);
  //update particles
  for(t = 1; t <= steps; t++){
    //switch last time step and current time step, so no new allocations needed
    temp = prevArray.p;
    prevArray.p = array.p;
    array.p = temp;
    for(i = 0; i < array.size[0]; i++){
      for(j = 0; j < array.size[1]; j++){
        array.p[i][j] = particle_update(prevArray.p, i, j, array.size[0], array.size[1], alpha);
      }
    }
    //write new timestep
    if(t % output_freq == 0){
      print_result(t, output, array);
    }
  }
  clear_pArray(prevArray);
}

particle particle_update(particle **p_last, int x, int y, int xdim, int ydim, double alpha){
  particle p;
  if(p_last[x][y].hold == 0){
    p.hold = 0;
    //if at edge
    if(x == 0){
      if(y == 0){//top left corner
        p.u = p_last[x][y].u + alpha*(-3*p_last[x][y].u + p_last[x+1][y].u + p_last[x][y+1].u + p_last[x+1][y+1].u);
      } else if(y == ydim-1){//bottom left corner
        p.u = p_last[x][y].u + alpha*(-3*p_last[x][y].u + p_last[x+1][y].u + p_last[x+1][y-1].u + p_last[x][y-1].u);
      } else {//left edge
        p.u = p_last[x][y].u + alpha*(-5*p_last[x][y].u + p_last[x][y+1].u + p_last[x][y-1].u + p_last[x+1][y].u + p_last[x+1][y-1].u + p_last[x+1][y+1].u);
      }
    } else if(x == xdim-1){
      if(y == 0){//top right corner
        p.u = p_last[x][y].u + alpha*(-3*p_last[x][y].u + p_last[x-1][y].u + p_last[x-1][y+1].u + p_last[x][y+1].u);
      } else if(y == ydim -1){//bottom right corner
        p.u = p_last[x][y].u + alpha*(-3*p_last[x][y].u + p_last[x-1][y].u + p_last[x-1][y-1].u + p_last[x][y-1].u);
      } else {//right edge
        p.u = p_last[x][y].u + alpha*(-5*p_last[x][y].u + p_last[x][y+1].u + p_last[x][y-1].u + p_last[x-1][y].u + p_last[x-1][y-1].u + p_last[x-1][y+1].u);
      }
    } else if(y == 0){//top edge
      p.u = p_last[x][y].u + alpha*(-5*p_last[x][y].u + p_last[x-1][y].u + p_last[x+1][y].u + p_last[x-1][y+1].u + p_last[x][y+1].u + p_last[x+1][y+1].u);
    } else if(y == ydim -1){//bottom edge
      p.u = p_last[x][y].u + alpha*(-5*p_last[x][y].u + p_last[x-1][y].u + p_last[x+1][y].u + p_last[x-1][y-1].u + p_last[x][y-1].u + p_last[x+1][y-1].u);
    } else{//the only remaining case is somewhere in the middle
      p.u = p_last[x][y].u + alpha*(-8*p_last[x][y].u + p_last[x-1][y-1].u + p_last[x-1][y].u + p_last[x-1][y+1].u + p_last[x][y-1].u + p_last[x][y+1].u + p_last[x+1][y-1].u + p_last[x+1][y].u + p_last[x+1][y+1].u);
    }    
    return p;
  } else {//no update needed
    return p_last[x][y];
  }
}
