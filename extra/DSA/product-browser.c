//razvan.cioarga@cs.upt.ro
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct record_t {
    char product_name[35];
    int product_code;
};

typedef struct record_t Record;

Record * records;
Record * sorted_by_code;
int number_of_records;
FILE * input_file;

int load_records(const char * file_name) {
    size_t size = 500;
    input_file = fopen(file_name, "r");
    if ( !input_file ) {
        printf("Can't open file: %s \n", file_name);
        return 0;
    }

    int counter = 0;

    records = malloc ( size * sizeof(struct record_t));
    if ( records == NULL ) {
        printf("Not enough memory!\n");
        exit(0);
    }

    while (
        (fscanf(input_file, "%s\n", records[counter].product_name) != EOF ) &&
        (fscanf(input_file, "%d\n", &records[counter].product_code) != EOF )
    ) {
        if ( counter > size - 100 ) {
            size = size + 100;
            records = realloc(records, size * sizeof(struct record_t));
            if ( records == NULL ) {
                printf("Reallocation failed!\n");
                exit(0);
            }
        }
        ++counter;
    }

    number_of_records = counter;

    sorted_by_code = malloc(number_of_records * sizeof(struct record_t));
    if ( sorted_by_code == NULL ) {
        printf("Not enough memory!\n");
        exit(0);
    }

    printf("Loaded %d records! \n", counter);
    return 1;
}

void display() {
    int const collumns = 3;
    int tab = 0;
    printf("FORMAT: PRODUCT NAME - PRODUCT CODE\n");
    for (int i = 0; i < number_of_records; ++i) {
        if ( tab % collumns == 0) {
            printf("\t");
        }
        printf("%s - %d ", records[i].product_name,
                          records[i].product_code);
        if ( tab % collumns == 0) {
            printf("\n");
        }
        ++tab;
    }
}

void sort_ascending(Record * array, int n) {
    int i, j, min;
    Record temp;
    for( i= 0; i <= n-2; i ++)
    {
      min= i; temp= array[i];
      for( j= i+1; j < n; j ++)
        if ( tolower(array[j].product_name[0]) < tolower(temp.product_name[0]) )
          {
            min= j; temp= array[j];
          }   /*FOR*/
      array[min]= array[i]; array[i]= temp; /*interschimbarea*/
    }   /*FOR*/
}

void sort_ascending_by_code(Record * array, int n) {
    int i, j, min;
    Record temp;
    for( i= 0; i <= n-2; i ++)
    {
      min= i; temp= array[i];
      for( j= i+1; j < n; j ++)
        if ( array[j].product_code < temp.product_code )
          {
            min= j; temp= array[j];
          }   /*FOR*/
      array[min]= array[i]; array[i]= temp; /*interschimbarea*/
    }   /*FOR*/
}

Record * search_by_code(Record * a, int n, int x) {
    int s, d, gasit, m;
    s=0; d=n; gasit=0;
      while(s<d)
      {
        m=(s+d)/2; /*sau orice valoare cuprinsă între s şi d*/
        if(a[m].product_code<x)
            s=m+1;
          else
                      d=m;
         }
      if(d>=n)
              { /*elementul cautat nu exista*/
}
       else
         if (a[d].product_code==x) {/*avem o coincidenta la indicele d */
             return &a[d];
}
         else
             {/*elementul cautat nu exista*/

}
return NULL;
}

void sort_odd_by_product_code(Record * array, int n) {
    int i, j, min;
    Record temp;
    for( i= 0; i <= n-2; i ++)
    {
      min= i; temp= array[i];
      for( j= i+1; j < n; j ++)
        if ( array[j].product_code > temp.product_code &&
             (array[j].product_code % 2 != 0) &&
             (temp.product_code) % 2 != 0
           )
          {
            min= j; temp= array[j];
          }   /*FOR*/
      array[min]= array[i]; array[i]= temp; /*interschimbarea*/
    }   /*FOR*/

}

void show_menu() {
    printf("1. Display\n");
    printf("2. Sort asscending\n");
    printf("3. Search by product code\n");
    printf("4. Sord desc-odd by product code\n");
    printf("5. Exit\n");
}

void invalid_choice(char * message) {
    printf("%s\n", message);
}

void clear() {
    fflush(stdin);
    int c = getchar();
    while ( c != '\n') {
        c = getchar();
    };
}

enum choices { c_display = 1, c_sort, c_search, c_psort, c_exit, c_quit = 65 };
void make_choice() {
    show_menu();

    printf("Enter choice: ");
    int choice = getchar() - '0';
    clear();

    int product_code;

    switch (choice) {
        case c_display:
            display();
            clear();
            break;
        case c_sort:
            sort_ascending(records, number_of_records);
            printf("Records sorted successfully!\n");
            break;
        case c_search:
            printf("Enter the product code: ");
            int valid = scanf("%d", &product_code);
            if (!valid) {
                invalid_choice("Invalid product_code");
                clear();
                make_choice();
            }
            memcpy(sorted_by_code, records, number_of_records * sizeof(struct record_t));
            sort_ascending_by_code(sorted_by_code, number_of_records);
            char * product = search_by_code(sorted_by_code, number_of_records, product_code)->product_name;
            if (product) {
                printf("Product: %s\n", product);
            } else {
                printf("No product found for the following code %d\n", product_code);
            }

            clear();
            break;
        case c_psort:
            sort_odd_by_product_code(records, number_of_records);
            printf("Records sorted successfully!\n");
            break;
        case c_exit:
        case c_quit:
            exit(0);
        default:
            invalid_choice("Choice is invalid! Please make a valid choice!");
            break;
    }
    make_choice();
}


int main(int argc, char * argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("%s filename\n", argv[0]);
        exit(0);
    }

    if (!load_records(argv[1])) {
        exit(1);
    }

    make_choice();

    return 0;
}
