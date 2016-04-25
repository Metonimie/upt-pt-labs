#include <string.h>

int my_strcmp(char * one, char * two) {
  if ( strlen(one) > strlen(two) ) {
    return 1;
  } else if ( strlen(one) < strlen(two) ) {
    return -1;
  }

  for (int i = 0; one[i]; i++) {
    if ( one[i] > two[i] ) {
      return 1;
    } else if ( one[i] < two[i] ) {
      return -1;
    }
  }
  return 0;
}

char * my_strcpy(char * dest, char * source) {
  int index = 0;
  while ( (dest[index] = source[index]) ) { index++; };
  dest[index] = '\0';
  return dest;
}

char * my_strncpy(char * dest, char * source, size_t len) {
  int index = 0;
  while ( (dest[index] = source[index]) && index < len ) { index++; };
  dest[len] = '\0';
  return dest;
}
