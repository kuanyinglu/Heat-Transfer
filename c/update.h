#ifndef __UPDATE__
#define __UPDATE__

typedef struct {
  double u;
  int hold;
} particle;

typedef struct {
  int *info;//dimension, size of element pointer
  int *size;//size of first dimension, second, third...
  particle **p;
  void (*add_value)(particle **p, int, int, double, int);
}pArray;

void array_free(void *pointer, int *info, int *size);//

void simulation(pArray array, double alpha, int steps, int output_freq, char *output);//

particle particle_update(particle **p_last, int x, int y, int xdim, int ydim, double alpha);//

void clear_pArray(pArray array);//free the arrays

void pArray_add_value(particle **p, int x, int y, double temp, int h);//

pArray create_pArray(int xdim, int ydim);//

#endif
