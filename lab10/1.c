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

int find_min_index(int arr[], size_t size) {
  int index = -1;
  int min = arr[0];
  // if (!min) { min = arr[1]; index = 1; }
  for (int j = 0; j < size; j++) {
    if (arr[j]) {
      min = arr[j];
      index = j;
    }
  }
  for (int i = 0; i < size; i++) {
    if ( arr[i] != 0 && arr[i] < min ) {
      min = arr[i];
      index = i;
    }
  }
  return index;
}

void mark_visited(int city) {
  for (int i = 0; i < 5; i++) {
    city_distance[i][city] = 0;
  }
}

void find_closest_path_from(int starting_city) {
  int distance = 0;
  int current_city = starting_city;
  int next_city;
  for (int i = 0; i < 5; i++) {
    mark_visited(current_city);
    next_city = find_min_index(city_distance[current_city], 5);
    if ( next_city == -1) {
      printf("We stopped in %s\n", cities[current_city]);
      break;
    }
    printf("Traveled from %s to %s.\n", cities[current_city], cities[next_city]);
    distance += city_distance[current_city][next_city];

    current_city = next_city;
  }
  printf("Total distance traveled: %d\n", distance);
}

void print_matrix() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%d ", city_distance[i][j]);
    }
    printf("\n");
  }
}

int main(void) {
  FILE * file;
  if ( !(file = fopen("mat.txt", "r")) ) {
    perror("Can't open file!");
    return errno;
  }

  load_matrix(file);
  find_closest_path_from(1); // 1 - oradea;
  // print_matrix();

  // printf("%d\n", find_min_index(city_distance[1], 5));

  if ( fclose(file) ) {
    perror("File can't be closed!");
    return errno;
  }
  return 0;
}
