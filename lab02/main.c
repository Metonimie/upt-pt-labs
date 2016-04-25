#include <stdio.h>
#include <stdlib.h>
#include "str.h"

int main(void) {
  printf("%d\n", my_strcmp("abc", "abc"));
  printf("%d\n", my_strcmp("adc", "abc"));
  printf("%d\n", my_strcmp("abc", "adc"));
  printf("%d\n", my_strcmp("abcd", "abc"));
  printf("%d\n", my_strcmp("abc", "abcd"));

  char * dest = malloc(12);
  char * dest2 = malloc(6);
  char * src = "Ana are mere.";
  my_strcpy(dest, src);
  my_strncpy(dest2, src, 5);

  printf("%s\n", dest);
  printf("%s\n", dest2);
  return 0;
}
