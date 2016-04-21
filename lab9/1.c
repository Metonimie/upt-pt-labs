#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// <rect x="25" y="25" width="200" height="200" fill="lime" stroke-width="4" stroke="pink" />
// <circle cx="125" cy="125" r="75" fill="orange" />
// <polyline points="50,150 50,200 200,200 200,100" stroke="red" stroke-width="4" fill="none" />
// <line x1="50" y1="50" x2="200" y2="200" stroke="blue" stroke-width="4" />

void draw_disks(FILE * file, int disks, int peg, int width, int y) {
  if ( !disks ) { return; } // no more disks to draw
  if ( y < 120) { return; } // keeps the peg from overflowing;
  char *colors[] = { "#7fffd4", "#ec8484", "#149dca", "#e4d93c", "#222d24",
                     "#ef8a48", "#ee6517", "#bbe200", "#ba1d1e", "#ffc4c4"};
  int random_color = rand() % 10;
  int x = 70 + ((peg - 1) * 100);
  fprintf(file, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"20\" "\
                "fill=\"%s\" stroke=\"black\" stroke-width=\"1\" />\r\n",
                x - width / 2 + 10, y, width, colors[random_color]);
  draw_disks(file, disks - 1, peg, width - width / 4, y - 21);
}

void draw_pegs(FILE * file, int pegs, int width) {
  if ( !pegs ) { return; } // exit function
  fprintf(file, "<rect x=\"%d\" y=\"120\" width=\"15\" height=\"130\" "\
                "fill=\"firebrick\" />\r\n", width);
  draw_pegs(file, pegs - 1, width + 100);
}

void draw_peg_scene(FILE * file, int pegs, int disks) {
  int scene_width = pegs * 100;
  fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n"); // start
  // draw scene
  fprintf(file, "<rect x=\"30\" y=\"250\" width=\"%d\" height=\"20\" "\
                "fill=\"firebrick\" />\r\n", scene_width);
  draw_pegs(file, pegs, 70); // draw pegs
  draw_disks(file, disks, rand() % pegs + 1, 80, 230);
  fprintf(file, "</svg>"); // end
}

int main(int argc, char *argv[]) {
  srand(time(NULL)); // seed random num generator to make colors unique each time
  FILE * file;
  if ( !(file = fopen("test.svg", "w")) ) {
    perror("Can't open file!");
    return -1;
  }
  if ( argc < 3 ) {
    printf("Invalid ussage. Correct usage: %s pegs disks\n", argv[0]);
    return -1;
  }
  draw_peg_scene(file, atoi(argv[1]), atoi(argv[2]));

  if ( fclose(file) ) {
    perror("Can't close file!");
  }

  return 0;
}
