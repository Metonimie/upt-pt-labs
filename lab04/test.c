#include <stdio.h>

enum cardsuit {
  CLUBS = 1,
  DIAMONDS = 2 ,
  HEARTS = 3 ,
  SPADES = 4
} custom_deck [ 52 ];

struct {
  unsigned int age : 3;
  unsigned int dayWeek: 3;
  unsigned int flagValid : 1;
} Info;

int main() {
  unsigned int aux;
  scanf ("%u", &aux);
  if (aux == 0) Info.flagValid = 0; else Info.flagValid = 1;
  if (aux > 1) fprintf(stderr , "Sir , You should be more careful with your input!\n");
  return 0;
}