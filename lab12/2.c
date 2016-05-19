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

Slist insert(int element, Slist list) {
    Slist * addr = &list;
    while (*addr) { // While addr' not null, go to the end.
        addr = &(*addr)->next; // the list is modified by this. I think...
    } // we should be at the end now.
    Slist node = malloc(sizeof(struct _selist));
    node->el = element;
    node->next = NULL; // we set to head if we want circular, I think?
    *addr = node;
    return list;
}

Slist delete(int element, Slist list) {
    Slist * addr = &list;
    while (*addr) {
        if ( (*addr)->el == element) {
            Slist temp = (*addr)->next;
            free(*addr);
            *addr = temp;
            break;
        }
        addr = &(*addr)->next;
    }
    return list;
}

size_t size(int elem, Slist list) {
    size_t elements = 0;
    while (list) {
      if (list->el == elem) {
          return elements;
      }
      elements += 1;
      list = list->next;
    }
    return elements;
}

void print_list(Slist list) {
    printf("SLIST PRINT\n");
    Slist * addr = &list;
    while (*addr) {
      printf("%d ", (*addr)->el);
      addr = &(*addr)->next;
    }
    printf("\n");
}

/* Double Linked list related code */

typedef struct _dilist * Dlist;

struct _dilist {
    int element;
    Dlist next;
    Dlist prev;
};

Dlist insert_dl(int element, Dlist list) {
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

void print_dlist(Dlist list) {
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

/* Code related to the split function */

void split_li_to_di(int elem, Slist sli, Dlist * dli1, Dlist * dli2) {
    short first_insertion;
    size_t sizzle = size(elem, sli) + 1; // ma fizzle, drizzle

    // Handle first double linked list.
    *dli1 = malloc(sizeof(struct _dilist));
    first_insertion = 1;
    for (int i = 0; i < sizzle; i++) {
        if ( first_insertion) {
            *dli1 = insert_dl(sli->el, NULL);
            first_insertion = 0; // :>
        } else {
            *dli1 = insert_dl(sli->el, *dli1);
        }
        sli = sli->next;
    }

    // Handle seccond double linked list.
    *dli2 = malloc(sizeof(struct _dilist));
    first_insertion = 1;
    while (sli) {
        if ( first_insertion) {
            *dli2 = insert_dl(sli->el, NULL);
            first_insertion = 0; // :>
        } else {
            *dli2 = insert_dl(sli->el, *dli2);
        }
        sli = sli->next;
    }
}

int main(void) {
    // Write a function to split a simple linked list into two double linked lists at a
    // specific point in the original list. The specific point is provided by the key value.
    Slist li = insert(1, insert(2, insert(3, insert(4, insert(5, NULL)))));

    Dlist dli1 = NULL;
    Dlist dli2 = NULL;

    // can be done without passing & by using auxiliary variable
    // like addr as demonstrated in the frist insert function.
    split_li_to_di(2, li, &dli1, &dli2);

    print_list(li);
    print_dlist(dli1);
    print_dlist(dli2);

    // Test
    // Dlist dli = insert_dl(1, insert_dl(2, insert_dl(3, insert_dl(4, NULL))));
    // print_dlist(dli);
    // Slist li = insert(1, insert(2, insert(3, insert(4, NULL))));
    // li = delete(1, li);
    // print_list(li);
    return 0;
}
