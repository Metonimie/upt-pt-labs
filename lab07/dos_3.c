#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


// won't work on windaws.
void unix_to_dos(char * filename) {
  // "Use the stdin and stdout file streams." NO.
  FILE * file = fopen(filename, "r");
  FILE * out;
  // open
  if ( !file ) {
    fprintf(stderr, "Error opening file.\n");
    exit(errno); // exit
  }

  system("touch converted.txt"); // create new file.
  if ( !(out = fopen("converted.txt", "w")) ) {
    fprintf(stderr, "Error opening output file. Are you using windows?\n");
    exit(errno); // exit
  }

  int c;
  while ( (c = fgetc(file)) != EOF ) {
    if ( c == '\n') {
      fputc('\r', out); // we're placing the character in the out file.
      fputc(c, out); // c is newline.
    } else {
      fputc(c, out); // c is whatever
    }
  }


  // close
  if ( fclose(file) || fclose(out) ) { // fclose returns zero on successful close
    fprintf(stderr, "Error closing file\n");
    exit(errno);
  }
}

int main(int argc, char *argv[]) {
  if ( argc != 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    fprintf(stderr, "Correct ussage: %s filename\n", argv[0]);
    return 1;
  }

  unix_to_dos(argv[1]);

  return 0;
}
