#include <stdio.h>
#include <stdlib.h>

void guess2(int, int);
void guessnum() {
  printf("Think of a number between 0 and 100\n");
  guess2(0, 100);
}

void guess2(int l, int h) {
  int n = 0;
  int fitty = (l + h) / 2 ;
  int guess;
  int guesst;
  printf("IS your number %d?\n", fitty);
  scanf("%d", &guesst);
  if (guesst == 1) {
    printf("Your number is %d\n", fitty);
    exit(0);
  }
  while ( fitty != n ) {
    printf("IS you number greather than %d\n", fitty);
    scanf("%d", &guess);
    if (guess == 1) {
      guess2(fitty, h);
    } else {
      guess2(l, fitty);
    }
  }
}

int main(int argc, char const *argv[]) {
  guessnum();
  return 0;
}
