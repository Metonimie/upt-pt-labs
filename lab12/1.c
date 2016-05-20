#include <stdio.h>
#include <stdlib.h>

/* Double Linked list related code */

typedef struct _dilist * Dlist;

struct _dilist {
    int element;
    Dlist next;
    Dlist prev;
};

Dlist insert(int element, Dlist list) {
    Dlist * addr = &list;
    Dlist prev = NULL;
    while (*addr) { // While addr' not null, go to the end.
    if ( !(*addr)->next ) {
        prev = *addr;
    }
        addr = &(*addr)->next; // the list is modified by this. I think...
    } // we should be at the end now.
    Dlist node = malloc(sizeof(struct _dilist));
    node->element = element;
    node->next = NULL; // we set to head if we want circular, I think?
    node->prev = prev; // 1 - NULL, 2 - 1, 3- 2, 4 - 3
    // printf("%d", prev->element);
    *addr = node;
    return list;
}

void print_list(Dlist list) {
    printf("DLIST PRINT\n");
    Dlist * addr = &list;
    while (*addr) {
        // if (((*addr)->prev))
            // printf("prev %d\n", ((*addr)->prev)->element);
        printf("%d ", (*addr)->element);
        addr = &(*addr)->next;
    }
    printf("\n");
}

Dlist reverse(Dlist list) {
  short virgin = 1;
  // creeaza spatiu pentru lista aia noua
  Dlist reversed = malloc(sizeof(struct _dilist)); // shoul find better names
  // sa duce la ultimu element
  while(list->next) { list = list->next; }
  while(list) {
    if (virgin) {
      reversed = insert(list->element, NULL);
      virgin = 0;
    } else {
      reversed = insert(list->element, reversed);
    }
    list = list->prev;
  }
  return reversed;
}

int main(void) {
  Dlist dli = insert(1, insert(2, insert(3, insert(4, NULL))));
  Dlist dli2 = reverse(dli);
  print_list(dli);
  print_list(dli2);
  print_list(dli);
  return 0;
}
