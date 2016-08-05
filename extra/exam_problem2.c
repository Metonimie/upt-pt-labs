#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Sa se genereze un fisier binar cu 100 de numere reale(float/double) arbitrar.
void generate_file(const char * filename) {
    srand(time(NULL));
    FILE * file = fopen(filename, "wb");
    if ( !file ) {
        perror("Can't open file!");
        return;
    }

    double numbers[100];
    for (int i = 0; i < 100; ++i) {
        numbers[i] = rand() % 1000000 / 1000.0;
    }

    fwrite(numbers, sizeof(double), 100, file);
    fseek(file, 0, SEEK_SET);

    if ( fclose(file) ) {
        perror("Can't close file.");
    }
}

//Se va citi continutul acestui fisier intr-o lista dublu inlantuita ordonata crescator.
typedef struct _dl_list * List;
struct _dl_list {
    double data;
    List next;
    List prev;
};

List init() {
    // create dummy list
    List new_list = malloc(sizeof(struct _dl_list));
    new_list->data = -1;
    new_list->next = new_list;
    new_list->prev = new_list;
    return new_list;
}

void _insert_middle(List list, double data) {
    //  head ... prev node | wwwti | cur node | next node ... last node
    //                                   ^
    // wwwti = where we want to insert.
    List cur_node = list;
    List node = malloc(sizeof(struct _dl_list));
    node->data = data;
    node->next = cur_node;
    node->prev = cur_node->prev;

    cur_node->prev->next = node;
    cur_node->next->prev = cur_node;
    cur_node->prev = node;
}

void _insert_first(List list, double data) {
    // head | wwwti
    //  ^
    List node = malloc(sizeof(struct _dl_list));
    node->data = data;
    node->next = list;
    node->prev = list;
    list->next = node;
    list->prev = node;
}

void _insert_last(List list, List head, double data) {
    // cur_node | wwwti | head
    //    ^
    List node = malloc(sizeof(struct _dl_list));
    node->data = data;
    node->next = head;
    node->prev = list;
    list->next = node;
}

/* The statement _insert_middle(...) can be replaced by the actual
 * code from the function _insert_middle. I've chosed to move the
 * code in a different function in order to keep this one small.
 * Same goes for the other two: _insert_first and _insert_last
 */
void add(List list, double data) {
    List head = list;
    int flag = 0;
    while( list->next != head ) { // middle insertion
        list = list->next;
        if ( data < list->data && !flag) {
            _insert_middle(list, data);
            flag = 1;
        }
    }
    if ( list == head ) { // first insertion
        _insert_first(list, data);
        list = list->next;
    } if ( list->next == head && data > list->data) { // last insertion
        _insert_last(list, head, data);
        list = list->next; // move to wwwti
    }

    // make list circular.
    list->next = head;
    head->prev = list;
}

void print_list(List list) {
    List head = list;
    int counter = 0;
    int item_counter = 0;
    while ( list->next != head ) {
        list = list->next;
        ++item_counter;
        // \x1b[31m = color red; %03d; \x1b[0m = color reset;
        printf("\x1b[31m%03d\x1b[0m: %07.3f ", item_counter, list->data);
        ++counter;
        if ( counter % 5 == 0) {
            printf("\n");
            counter = 0;
        }
    }
    printf("\n");
    printf("next %.3f\n", list->next->data); // -1 is dummy value
    printf("next's prev %.3f\n", list->next->prev->data);
    printf("prev %.3f\n", list->prev->data);
    printf("head prev %.3f\n", head->prev->data);

}

void read_from_file(List list, const char * filename) {
    FILE * file = fopen(filename, "rb");
    if ( !file ) {
        perror("Can't open file!");
        return;
    }

    double item;
    while ( fread(&item, sizeof(item), 1, file) != 0) {
        add(list, item);
    }

    if ( fclose(file) ) {
        perror("Can't close file!");
    }
}

// Sa se sorteze fisierul binar in ordine descrescatoare si sa se afisieze continutul listei pe ecran.
// Read the items into a buffer, qsort them, write them to file.
int comparator(const void * a, const void * b) {
    double f = *((double *) a);
    double s = *((double *) b);
    if ( f > s ) {
        return -1;
    } else if ( f < s) {
        return 1;
    }
    return 0;
}

void sort_file(const char * filename) {
    FILE * file = fopen(filename, "rb+");
    if ( !file ) {
        perror("Can't open file!");
        return;
    }

    double items[100];
    for ( int i = 0; i < 100; ++i ) {
        fread(&items[i], sizeof(double), 1, file);
    }
    qsort(items, sizeof(items)/sizeof(items[0]), sizeof(double), comparator);

    fseek(file, 0, SEEK_SET);
    for ( int i = 0; i < 100; ++i ) {
        fwrite(&items[i], sizeof(items[0]), 1, file);
    }

    if ( fclose(file) ) {
        perror("Can't close file!");
    }
}

int main() {
    generate_file("numbers.txt");
    List boss = init();
    read_from_file(boss, "numbers.txt");
    print_list(boss);
    sort_file("numbers.txt");

    return 0;
}
