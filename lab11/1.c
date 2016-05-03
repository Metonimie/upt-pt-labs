#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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

int ** generate_routes(int start, int cities_number) {
  int route_no = 0;
  int ** routes = malloc((cities_number) * (cities_number) * sizeof(int *));
  // int routes[24][4];
  for (int a = 0; a < cities_number; a++) {
    if ( a == start ) { continue; }
    for (int b = 0; b < cities_number; b++) {
      if ( b == start || b == a) { continue; }
      for (int c = 0; c < cities_number; c++) {
        if ( c == start || c == b || c == a) { continue; }
        for (int d = 0; d < cities_number; d++) {
          if ( d == start || d == c || d == b || d == a) { continue; }
          routes[route_no] = malloc(cities_number * sizeof(int));
          routes[route_no][0] = a;
          routes[route_no][1] = b;
          routes[route_no][2] = c;
          routes[route_no][3] = d;
          route_no++;
        }
      }
    }
  }
  return routes;
}

void do_all_routes(int cities_number) {
  int distance;
  int route = 0;
  int min_distance = 0;
  int min_route = -1;
  int ** routes = generate_routes(1, 5);

  int a, b, c, d;
  for (int i = 0; i < cities_number * cities_number - 1; i++) {
    a = routes[route][0];
    b = routes[route][1];
    c = routes[route][2];
    d = routes[route][3];
    distance = city_distance[1][a] + city_distance[a][b] + city_distance[b][c] + city_distance[c][d];
    if (!min_distance) {
      min_distance = distance;
      min_route = route;
    } else if ( distance < min_distance) {
      min_distance = distance;
      min_route = route;
    }
    printf("Route %02d, [%s - %s - %s - %s - %s] Total distance: %d\n",
    route, cities[1], cities[a], cities[b], cities[c], cities[d], distance);
    route += 1;
  }

  printf("Minimum route: %d with the minimum distance %d\n", min_route, min_distance);

  // Prevent memory leak.
  for (int i = 0; i < cities_number * cities_number - 1; i++) {
    free(routes[i]);
  }
  free(routes);
}

int main(void) {
  load_matrix("mat.txt");
  do_all_routes(5);

  return 0;
}
