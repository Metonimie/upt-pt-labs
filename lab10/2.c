#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int city_distance[5][5];
char *cities[] = {"Timisoara", "Oradea", "Cluj", "Iasi", "Bucuresti"};

void load_matrix(char * filename) {
  FILE * file;
  if ( !(file = fopen(filename, "r")) ) {
    fprintf(stderr, "Can't open: %s\n", filename);
    exit(errno);
  }

  int t, o, c, i, b; // timisoara, oradea ...
  for (int j = 0; fscanf(file, "%d %d %d %d %d\n", &t, &o, &c, &i, &b) == 5; j++ ) {
    city_distance[j][0] = t;
    city_distance[j][1] = o;
    city_distance[j][2] = c;
    city_distance[j][3] = i;
    city_distance[j][4] = b;
  }

  if ( fclose(file) ) {
    perror("File can't be closed!");
    exit(errno);
  }
}

void do_all_routes2() {
  int distance;
  int routes = 0;
  int min_distance = 0;
  int min_route = -1;

  for (int a = 0; a < 5; a++) {
    if ( a == 1 ) { continue; }
    for (int b = 0; b < 5; b++) {
      if ( b == 1 || b == a) { continue; }
      for (int c = 0; c < 5; c++) {
        if ( c == 1 || c == b || c == a) { continue; }
        for (int d = 0; d < 5; d++) {
          if ( d == 1 || d == c || d == b || d == a) { continue; }
          distance = city_distance[1][a] + city_distance[a][b] + city_distance[b][c] + city_distance[c][d];
          routes += 1;
          if (!min_distance) {
            min_distance = distance;
            min_route = routes;
          } else if ( distance < min_distance) {
            min_distance = distance;
            min_route = routes;
          }
          printf("Route %02d, [%s - %s - %s - %s - %s] Total distance: %d\n",
          routes, cities[1], cities[a], cities[b], cities[c], cities[d], distance);
        }
      }
    }
  }

  printf("Minimum route: %d with the minimum distance %d\n", min_route, min_distance);
}

void do_all_routes() {
  FILE * file;
  if ( !(file = fopen("routes.txt", "r")) ) { exit(0); }
  int distance ;
  int min_distance = 0;
  int min_route = -1;
  int a, b, c, d;
  for (int i = 0; fscanf(file, "%d %d %d %d\n", &a, &b, &c, &d) == 4; i++) {
    distance = city_distance[1][a] + city_distance[a][b] + city_distance[b][c] + city_distance[c][d];
    if (!min_distance) {
      min_distance = distance;
      min_route = i;
    } else if ( distance < min_distance) {
      min_distance = distance;
      min_route = i;
    }
    printf("Route %2d, [%s - %s - %s - %s - %s] Total distance: %d\n",
    i, cities[1], cities[a], cities[b], cities[c], cities[d], distance);
  }
  printf("Minimum route: %d with the minimum distance %d\n", min_route, min_distance);
  if ( fclose(file) ) {
    perror("Can't close file!");
    exit(0);
  }
}

int main(void) {
  load_matrix("mat.txt");
  // do_all_routes();
  do_all_routes2();

  return 0;
}
