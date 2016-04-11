#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// won't work on windaws.
void unix_to_dos(char * filename) {
  FILE * file = fopen(filename, "r");
  FILE * out;
  // open
  if ( !file ) {
    fprintf(stderr, "Error opening file.\n");
    exit(errno); // exit
  }

  // It would have been easier if we worked on the current file,
  // without creating a separated file for the converted file.
  char * out_filename = malloc( 5 + strlen(filename) ); // dos_ (4) + filename (strlen) + NULL (1)
  memcpy( out_filename, "dos_", 4 );
  memcpy( out_filename + 4, filename, strlen(filename) + 1 ); // + 1 to copy the null
  // we want to perform the following command:
  // touch dos_filename.ext
  // 5 + 1 + strlen(out_filename) + 1
  char * command = malloc( 7 + strlen(out_filename) );
  memcpy(command, "touch ", 6);
  memcpy(command + 6, out_filename, strlen(out_filename) + 1); // + 1 to copy the terminating NULL

  system(command); // create new file.
  if ( !(out = fopen(out_filename, "w")) ) {
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
  free(out_filename);
  free(command);
}

int main(int argc, char *argv[]) {
  if ( argc < 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    fprintf(stderr, "Correct ussage: %s filename\n", argv[0]);
    return 1;
  }

  // *.c will expand to all the files in the current dir. eg: 1.c 2.c 3.c et.c
  for (int i = 1; i < argc; i++) {
    unix_to_dos( argv[i] );
  }

  return 0;
}
