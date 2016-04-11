#include <stdio.h>

char code64[]="\x48\x31\xFF\x48\x31\xC0\xB0\x3C\x0F\x05";

/*
gcc  fno stack protector  -z execstack  o exit exit . c ;

*/

int main (void) {

  int (*func)(); /* function pointer for a function */
  /* with no arguments but with a return value */

  func = (int (*)()) code64; /* cast from char array to function pointer */

  (int)(*func)(); /* calls exit(0), Intel 64 bit CPU */

  printf("Should exit(0) gracefully before getting here!\n");
return 0;
}
