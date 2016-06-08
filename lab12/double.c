#include <stdio.h>
#include <stdlib.h>

typedef struct _dclist * List;

struct _dclist {
    int data;
    List next;
    List prev;
};

List init() {
    List initializer = malloc(sizeof(struct _dclist));
    initializer->data = 0;
    initializer->next = initializer;
    initializer->prev = initializer;
    return initializer;
}

void insert(int data, List list) {
    List head = list;
    while ( list->next != head ) { // if we have next, go to it.
        list = list->next;
    }
    list->next = malloc(sizeof(struct _dclist));
    head->prev = list->next;
    list->next->prev = list; // set the preview value;
    list->next->data = data;
    list->next->next = head;
}

void print(List list) {
    List head = list;
    while ( list->next != head) {
        list = list->next;
        printf("PREVIOUS VALUE: %d\n", list->prev->data);
        printf("CURRENT VALUE: %d\n", list->data);
    }
    printf("Is it circular? %d means yes\n", list->next->data);
}

int main(void) {
    List test = init();
    insert(10, test);
    insert(12, test);
    insert(13, test);
    insert(14, test);
    insert(15, test);
    print(test);
    return 0;
}
