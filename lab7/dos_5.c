#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define LINE_CHARS 100

// Parser
int zfind(char *str, int c) {
  for (int i = 0; str[i]; i++) {
    if (str[i] == c) { return 1; }
  }
  return 0;
}

// easy solution, no memory leak :)
char * parse_html(char *string){
  char * ptr = malloc( LINE_CHARS );
  int index = 0;
  for (int i = 0; string[i]; i++) {
    if (string[i] == '<') { // found opening bracket
      if ( zfind(string + i, '>')) { // found closing bracket
        while(string[i] != '>') { i++; }; // delete za tag
      } else {
        ptr[index++] = string[i]; // didn't find closing bracket, skip
      }
    } else {
      ptr[index++] = string[i]; // normal character
    }
  }
  ptr[index] = '\0';

  return ptr;
}

// Time shift
void shift_time(int *h, int *m, int *s, int *scr_h, int *scr_m, int *scr_s, int shift) {
  long mhs = (*h * 3600) + (*m * 60) + *s + shift;
  long scr_mhs = (*scr_h * 3600) + (*scr_m * 60) + *scr_s + shift;
  *h = mhs / 3600;
  *m = (mhs / 60) % 60;
  *s = mhs % 60;
  *scr_h = scr_mhs / 3600;
  *scr_m = (scr_mhs / 60) % 60;
  *scr_s = scr_mhs % 60;
}

// Reader
void process_subtitles(char * filename, int time) {
  FILE * file;
  FILE * out;
  if ( !(file = fopen(filename , "r")) ) {
    fprintf(stderr, "Error, can't open file: %s\n", filename);
    exit(errno);
  }
  system("touch working.srt");
  if ( !(out = fopen("working.srt" , "w")) ) {
    fprintf(stderr, "Error, can't open file: working.srt\n");
    exit(errno);
  }

  char * line = malloc ( LINE_CHARS );
  char * temp;
  int line_no;
  int h, m, s, ms, scr_h, scr_m, scr_s, scr_ms;

  while(1) {// infinity. Executes the folowing wile for eternity.
    // read numbers
    while ( fscanf(file, "%d\r\n%d:%d:%d,%d --> %d:%d:%d,%d\r\n",
    &line_no, &h, &m, &s, &ms, &scr_h, &scr_m, &scr_s, &scr_ms) ) {
      if (feof(file)) { break; }

      if(time) {
        shift_time(&h, &m, &s, &scr_h, &scr_m, &scr_s, time);
      }

      // print formatted time.
      fprintf(out, "%d\r\n%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\r\n",
      line_no, h, m, s, ms, scr_h, scr_m, scr_s, scr_ms);

      while ( fgets(line, LINE_CHARS - 1, file) ) { // read sub diaglog
        temp = parse_html(line);
        fprintf(out, "%s", temp);
        free(temp); // prevent memory leak by freeing the pointer.
        if (!strcmp(line, "\r\n")) { break; }
      }
    }

    if (feof(file)) { break; }
  }


  if ( fclose(file) || fclose(out) ) {
    printf("Error, can't close file\n");
    exit(errno);
  }

  free(line);
}

int main(int argc, char *argv[]) {
  if ( argc < 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    fprintf(stderr, "Correct ussage: %s filename [secconds to shift]\n", argv[0]);
    return 1;
  }

  int time_to_shift = 0;
  if ( argc > 2) {
    time_to_shift = atoi(argv[2]);
  }

  process_subtitles(argv[1], time_to_shift);
  return 0;
}
