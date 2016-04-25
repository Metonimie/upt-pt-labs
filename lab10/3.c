#include <stdio.h>
#include <stdlib.h>

/*
 * Linked List
 */
typedef struct int_list_t * list;

struct int_list_t {
  int el;
  list next;
};

list insert(list lst, int el);
list delete(list lst, int el);
list tail(list lst);
int head(list lst);
void print_list(list lst);

list insert(list lst, int el) {
  list * adr = &lst;
  while ( (*adr) != NULL ) { adr = &(*adr)->next; }
  list temp = malloc(sizeof(struct int_list_t));
  temp->el = el;
  temp->next = (*adr);
  (*adr) = temp;
  return lst;
}

list delete(list lst, int el) {
  list * adr = &lst;
  while ( (*adr) != NULL ) {
    if ((*adr)->el == el) {
      list temp = (*adr)->next;
      free(*adr);
      (*adr) = temp;
      break;
    }
    adr = &(*adr)->next;
  }
  return lst;
}

int head(list lst) {
  return lst->el;
}

list tail(list lst) {
  return lst->next;
}

void print_list(list lst) {
  while ( lst != NULL ) {
    printf("%d ", head(lst));
    lst = tail(lst);
  }
  printf("\n");
}

/*
 * Finding peak valuez
 */
int find_min(list lst) {
  int min = head(lst);
  lst = tail(lst);
  while (lst) {
    if ( head(lst) < min ) {
      min = head(lst);
    }
    lst = tail(lst);
  }
  return min;
}

int find_max(list lst) {
  int max = head(lst);
  lst = tail(lst);
  while (lst) {
    if ( head(lst) > max ) {
      max = head(lst);
    }
    lst = tail(lst);
  }
  return max;
}

 void find_peak(list * lst, size_t size) {
   int min = find_min(lst[0]);
   int max = find_max(lst[0]);
   int new_min, new_max;
   for (int i = 1; i < size; i++) {
     new_min = find_min(lst[i]);
     new_max = find_max(lst[i]);
     if (new_min < min) {
       min = new_min;
     }
     if (new_max > max) {
       max = new_max;
     }
   }
   printf("Maximum %d\n", max);
   printf("Minimum %d\n", min);
 }

int main(void) {
  list list1 = insert(insert(insert(insert(NULL, 11), 7), 5), 3);
  list list2 = insert(insert(insert(insert(NULL, 17), -3), 2), -8);
  list list3 = insert(insert(insert(insert(NULL, 14), 21), 10), 32);
  list lists[] = { list1, list2, list3 };
  find_peak(lists, 3);
}
