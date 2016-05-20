#include <stdio.h>
#include <stdlib.h>

/* Single Linked list related code */
typedef struct _selist * Slist;

struct _selist {
    int el;
    Slist next;
};

int head(Slist lst) {
    return lst->el;
}

Slist tail(Slist lst) {
    return lst->next;
}

Slist insert(int element, Slist current) {
    Slist head = current;

    while(current->next != head) {
        current = current->next;
    }
    current->next = malloc(sizeof(struct _selist));
    current = current->next;

    current->el = element;
    current->next = head;
    return current;
}

void print_list(Slist list) {
    printf("SLIST PRINT\n");
    Slist head = list;
    list = list->next;
    while (list != head) {
      printf("%d ", list->el);
      list = list->next;
    }
    printf("\n");
}

size_t size(Slist list) {
    size_t elements = 0;
    Slist head = list;
    list = list->next;
    while (list != head) {
        elements += 1;
        list = list->next;
    }
    return elements;
}

void delete_middle(Slist list) {
    unsigned long middle = size(list) / 2;
    Slist prev = list;
    list = list->next;
    for (int i = 0; i <= middle; i++) {
        if (i + 1 == middle) {
            prev = list;
        }
        if (i == middle) {
            prev->next = list->next;
            free(list);
        }
        list = list->next;
    }
}

int main(void) {
  Slist head = malloc(sizeof(struct _selist));
  head->next = head;
  insert(1, head);
  insert(2, head);
  insert(3, head);
  insert(4, head);
  insert(5, head);
  delete_middle(head);
  print_list(head);
  return 0;
}
