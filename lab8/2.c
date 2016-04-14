#include <stdio.h>

#define ROW_LEN 50
#define IMEI_LEN 10

typedef struct Db_tag {
  int i;
  int blacklisted; // 1 yes, 0 no
  char repair_type[ROW_LEN];
  char imei[IMEI_LEN];
  int price;
  int investment;
  int profit;
  int total_profit;
  int total_influx;
} Database;

int profit_displayed = 0;

int isEmpty(FILE *file){
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0){
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

void insert_entry(FILE * file, Database * entry) {
  fseek(file, 0, SEEK_END);
  fwrite(entry, sizeof(Database), 1, file);
}

void read_entry(FILE * file, Database * entry) {
  fread(entry, sizeof(Database), 1, file);
}

void display_entry(Database * entry) {
  printf("|%3d|%50s|%10s|%5d|%10d|%10d|%12d|%12d|\n",
    entry->i,
    entry->repair_type,
    entry->imei,
    entry->price,
    entry->investment,
    entry->profit,
    entry->total_profit,
    entry->total_influx
 );
}

void display_header() {
  printf("|%3s|%50s|%10s|%5s|%10s|%10s|%12s|%12s|\n",
    "i", "Repair Type", "IMEI", "Price", "Investment",
    "Profit", "Total Profit", "Total Influx");
}

void display_database_entries(FILE * file) {
  fseek(file, 0, SEEK_SET);
  Database entry;
  while ( !feof(file) ) {
    read_entry(file, &entry);
    display_entry(&entry);
    profit_displayed = 1;
  }
  profit_displayed = 0;
}

void display_menu() {
  printf("\n");
  printf("0. Exit program\n");
  printf("1. Display the database.\n");
  printf("2. Add an entry.\n");
  printf("3. Update an entry.\n");
  printf("4. Delete an entry.\n");
  printf("5. Search by IMEI\n");
  printf("\n");
}

int main(void) {
  FILE * fp = fopen("data.bdb", "r+b");
  Database nokia = {
    .i = 0,
    .blacklisted = 6,
    .repair_type = "mester",
    .imei = "nuavem",
    .price = 1,
    .investment = 2,
    .profit = 3,
    .total_profit = 4,
    .total_influx = 5
  };
  Database e;
  // display_header();
  // display_database_entries(fp);
  // insert_entry(fp, &nokia);
  // printf("after ins %ld\n", ftell(fp));
  // fseek(fp, -sizeof(Database), SEEK_CUR);
  // printf("before read %ld\n", ftell(fp));
  // read_entry(fp, &e);
  // printf("after read %ld\n", ftell(fp));
  // printf("%s\n", e.repair_type);
  return 0;
}
