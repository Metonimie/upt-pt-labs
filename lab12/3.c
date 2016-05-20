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

Slist init() {
    Slist dummy = malloc(sizeof(struct _selist));
    dummy->next = dummy;
    return dummy;
}

void insert(int element, Slist current) {
    Slist head = current;

    // chestia asta merge pana la ultimul element, si ramane acolo.
    // daca-i head n-are treaba.
    while(current->next != head) {
        current = current->next;
    }
    // creaza un nod nou, la next si schimba adresa nodului curent pe next.
    current->next = malloc(sizeof(struct _selist));
    // se muta pe nod-ul creat
    current = current->next;

    // baga elementu si face ca nodu sa pointeze spre head.
    current->el = element;
    current->next = head;
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
  Slist head = init();
  insert(1, head);
  insert(2, head);
  insert(3, head);
  insert(4, head);
  insert(5, head);
  insert(6, head);
  delete_middle(head);
  print_list(head);
  return 0;
}
