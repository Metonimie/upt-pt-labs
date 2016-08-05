#include <stdio.h>
#include <stdlib.h>

typedef struct _list * List;

struct _list
{
	int data;
	List next;
};

List init(int data) {
	List new = (List) malloc(sizeof(struct _list));
	new->next = NULL;
	new->data = data;
	return new;
}

void add(List list, int data) {
	// List head = list;
	while(list->next != NULL) {
		list = list->next;
	}
	list->next = (List) malloc(sizeof (struct _list));
	list->next->next = NULL;
	list->next->data = data;
}

void print_list(List list) {
	while (list) {
		printf("%d%c", list->data, list->next == NULL ? '\n' : ' ');
		list = list->next;
	}
}

int main(int argc, char const *argv[])
{
	List l = init(1);
	add(l, 2);
	add(l, 3);
	add(l, 4);
	add(l, 5);
	print_list(l);
	return 0;
}