#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* float is automatically promoted to double via ...
because of the stack memory requirements */
double arithmetic_mean(unsigned int n, ...) {
  va_list valist;
  float sum = 0.0;
  unsigned int i;
  /* initialize valist for n arguments */
  va_start(valist , n);

  /* parse n arguments of type float from valist */
  for (i = 0; i < n; i++) {
    sum += va_arg(valist , double);
  }

  /* clean valist */
  va_end( valist );
  return sum/n;
}

void max_and_min(int * max, int * min, int n, ...) {
  va_list valist;
  /* initialize valist for n arguments */
  va_start(valist , n);

  *max = va_arg(valist, int);
  *min = va_arg(valist, int);
  if ( *max < *min) {
    int aux = *max;
    *max = *min;
    *min = aux;
  }

  int el;
  for (int i = 0; i < n-2; i++) {
    el = va_arg(valist , int);
    if ( el >= *max ) {
      *max = el;
    }
    else if ( el < *min ) {
      *min = el;
    }
  }

  /* clean valist */
  va_end( valist );
}

int main() {
  int max;
  int min;
  max_and_min(&max, &min, 10, 1, -1, 1, 3, 4, -6, 6, 8, 9, 10);
  printf("Max: %d  Min: %d\n", max, min);
  return 0;
}
