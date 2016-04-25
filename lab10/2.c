#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int city_distance[5][5];
char *cities[] = {"Timisoara", "Oradea", "Cluj", "Iasi", "Bucuresti"};

void load_matrix(FILE * file) {
  int t, o, c, i, b; // timisoara, oradea ...
  for (int j = 0; fscanf(file, "%d %d %d %d %d\n", &t, &o, &c, &i, &b) == 5; j++ ) {
    city_distance[j][0] = t;
    city_distance[j][1] = o;
    city_distance[j][2] = c;
    city_distance[j][3] = i;
    city_distance[j][4] = b;
  }
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
    printf("Route %d, [%s - %s - %s - %s - %s] Total distance: %d\n",
    i, cities[1], cities[a], cities[b], cities[c], cities[d], distance);
  }
  printf("Minimum route: %d with the minimum distance %d\n", min_route, min_distance);
  if ( fclose(file) ) {
    perror("Can't close file!");
    exit(0);
  }
}

int main(void) {
  FILE * file;
  if ( !(file = fopen("mat.txt", "r")) ) {
    perror("Can't open file!");
    return errno;
  }

  load_matrix(file);
  do_all_routes();

  if ( fclose(file) ) {
    perror("File can't be closed!");
    return errno;
  }
  return 0;
}
