#include <stdio.h>
#define N 3

void movedisks(int ndisks, char fromPeg, char toPeg, char auxPeg) {
if (ndisks == 1)
    printf("Move disk 1 from [%c] to [%c]\n", fromPeg, toPeg);
else {
  movedisks ( ndisks - 1 , fromPeg , auxPeg , toPeg ) ;
  printf("Move disk %d from [%c] to [%c]\n", ndisks, fromPeg, toPeg);
  movedisks ( ndisks  - 1 , auxPeg , toPeg , fromPeg ) ;
  }
}
int main(void) {
    movedisks(N, 'A', 'C', 'B');
}
