#include <stdio.h>
#include <math.h>

int is_prime(int);

int populate_array(int * array, size_t size) {
  int index = 0;
  for (int i = 1; i < size; i++) {
    if ( is_prime(i) ) {
      array[index++] = i;
    }
  }
  return index;
}

int is_present(int * array, size_t size, int number);

void parr(int * array, size_t size, size_t numbers) {
  for (int i = 1; i < numbers; i++)
    printf("%d IS PRESENT: %d\n",i, is_present(array, size, i));
}

int is_prime(int number) {
  for (int i = 2; i <= number/2; i++) {
    if ( number % i == 0 ) {
      return 0;
    }
  }
  return 1;
}

int _is_present(int * array, int number, int low, int high) {
  int middle = ((high + low) / 2.0);
  //printf("L: %d H: %d M: %d N: %d\n", low, high, middle, number);
  if ( number > array[middle] ) {
    return _is_present(array, number, middle, high);
  } else if ( number < array[middle] ) {
    return _is_present(array, number, low, middle);
  } else if ( number == array[middle] ) {
    return 1;
  } else {
    return 0;
  }
}

int is_present(int * array, size_t size, int number) {
  if ( is_prime(number) ) {
    return _is_present(array, number, 0, size);
  } else {
    return 0;
  }
}

int main(void) {
  int primes[100];
  int real_primes = populate_array(primes, 100);
  //parr(primes, real_primes, 100);
  printf("%d\n", is_present(primes, real_primes, 6));
  printf("%d\n", is_present(primes, real_primes, 13));
  return 0;
}
