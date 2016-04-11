#include <stdio.h>
#include <math.h>

double (*primary[7])(double);
char *secondary[] = { "sin(x)", "cos(x)", "ceil(x)", "floor(x)", "fabs(x)", "log(x)", "sqrt(x)" };

void process_choice();
void map_function_pointers(double (**arr)(double), int pos, int n) {
  switch (n) {
    case 0:
      arr[pos] = sin;
      break;
    case 1:
      arr[pos] = cos;
      break;
    case 2:
      arr[pos] = ceil;
      break;
    case 3:
      arr[pos] = floor;
      break;
    case 4:
      arr[pos] = fabs;
      break;
    case 5:
      arr[pos] = log;
      break;
    case 6:
      arr[pos] = sqrt;
      break;
    default:
      fprintf(stderr, "Error\n");
  }
}

void clean_stdin() {
  int c; while ( (c = getchar()) != '\n' );
}

void display_menu() {
  for (int i = 0; i < 7; i++) {
    printf("%d. %s\n", i, secondary[i]);
  }
}

void init() {
  for (int i = 0; i < 7; i++) {
    map_function_pointers(primary, i, i);
  }
}

int is_valid(int flag) {
  if (!flag) {
    fprintf(stderr, "Invalid choice!\n");
    clean_stdin();
    return 0;
  }
  return 1;
}

void process_choice() {
  display_menu();
  int choice = -1;
  double value = 0;

  printf("Choice: ");
  int flag = scanf("%d", &choice);
  if ( !is_valid(flag) ) { return process_choice(); }
  if (choice < 0 || choice > 6) {
    fprintf(stderr, "Out of range choice!\n");
    return process_choice();
  }

  printf("Enter a value: ");
  int value_flag = scanf("%lf", &value);
  if ( !is_valid(value_flag) ) {
    fprintf(stderr, "Invalid value!\n");
    return process_choice();
  }

  printf("RESULT: %f\n", primary[choice](value));

}

int main() {
  init();
  process_choice();
  return 0;
}
