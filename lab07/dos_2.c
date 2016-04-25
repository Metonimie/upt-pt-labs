#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

void count_letter_frequency_from_file(char * filename) {
  FILE * file;
  file = fopen(filename, "r"); // filename, mode

  // open
  if ( !file ) {
    fprintf(stderr, "Error opening file.\n");
    exit(errno); // exit
  }

  int frequencies[128] = { 0 }; // Contains character frequencies from ascii table, is initialized to 0;
  char * line = malloc( 200 ); // becauase 1 char = 1 byte, 200 chars = 200 bytes
  while ( fgets(line, 200, file) ) { // max 200 chars pe line, gets the line
    for ( int i = 0; line[i]; i++ ) { // process the line char by char
      frequencies[ (int) line[i] ]++; // make the letter and decimal and increments it's possition in the array.
    }
  }

  printf("Characters - Frequencies\n");
  for (int i = 0; i < 128; i++) {
    if ( frequencies[i] && isprint( (char) i ) ) { // different from zero and printable
      printf( "\t %c - %d\n", i, frequencies[i] );
    }
  }

  // close
  if ( fclose(file) ) {
    fprintf(stderr, "Error closing file\n");
    exit(errno);
  }

  free(line);
}

int main() {
  count_letter_frequency_from_file("test");
  return 0;
}
