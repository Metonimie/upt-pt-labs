#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isbn13(char * isbn) {
  int sum = 0;
  int n = 13;
  for (int i = 0; i < n - 1; i++) {
    if ( (i + 1) % 2 == 0 ) {
      sum += 3 * (isbn[i] - '0');
    } else {
      sum += isbn[i] - '0';
    }
  }
  if ( 10 - ( sum % 10) == isbn[n - 1] - '0' ) {
    return 1;
  }
  return 0;
}

char * conv_isbn13(char * isbn) {
  char * new = malloc(13);
  int n = 10;
  new[0] = '9';
  new[1] = '7';
  new[2] = '8';
  for (int i = 0; i < n; i++) {
    new[i+3] = isbn[i];
  }
  return new;
  new[13] = '\0';
}

int isbn10(char * isbn) {
  int sum = 0;
  for (int i = 0; i < 9; i++) {
    sum += (i + 1) * (isbn[i] - '0');
  }

  if ( ( sum % 11) == isbn[9] - '0' ) {
    return 1;
  }
  return 0;
}

char * conv_isbn10(char * isbn) {
  char * new = malloc(10);
  for (int i = 0; i < 9; i++) {
    new[i] = isbn[i+3];
  }
  new[10] = '\0';
  return new;
}

void guess2(int l, int h);

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
  // printf("%s\n", conv_isbn10("9781435704572"));
  // printf("%s\n", conv_isbn13("0521396549"));
  return 0;
}
