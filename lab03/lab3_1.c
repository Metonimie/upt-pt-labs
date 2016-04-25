#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int (*comp)(const void *, const void *);

int sorter(double * a, double * b) {
  if ( *a > *b ) {
    return 1;
  } else if ( *a < *b ) {
    return -1;
  } else {
    return 0;
  }
}

void populate_array(double * array, size_t size) {
  srand(time(0));
  for (int i = 0; i < size; i++) {
    array[i] = rand() % 101;
    // printf("%f\n", array[i]);
  }
  qsort(array, size, sizeof(array[0]), (comp) sorter);
}

void parr(double * arr, size_t size) {
  for (int i = 0; i < size; i++) printf("%f\n", arr[i]);
}

int main(void) {
  double array[5];
  populate_array(array, 5);
  parr(array, 5);
  return 0;
}
