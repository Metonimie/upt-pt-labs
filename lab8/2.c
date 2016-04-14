#include <stdio.h>
#include <stdlib.h>

#define ROW_LEN 50
#define IMEI_LEN 10

typedef struct Db_tag {
  int blacklisted; // 1 yes, 0 no
  char repair_type[ROW_LEN];
  char imei[IMEI_LEN];
  int price;
  int investment;
  int profit;
} Database;

void display_entry(Database * entry, unsigned index);
void insert_entry(FILE * file, Database * entry);
void read_entry(FILE * file, Database * entry);

int isEmpty(FILE *file){
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0){
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

void delete_entry(FILE * file, unsigned entry_no) {
  fseek(file, 0, SEEK_SET);

  Database entry;
  short found = 0;
  FILE * out = fopen(".temp", "wb");
  for( int i = 0; fread(&entry, sizeof(Database), 1, file); i++ ) {
    if ( i == entry_no ) {
      printf("Entry found and deleted!\n");
      found = 1;
    } else {
      insert_entry(out, &entry);
    }
  }
  insert_entry(out, &entry);
  if ( !found ) {
    printf("Entry not found!\n");
  }

  system("rm -f data.bdb; mv .temp data.bdb"); // copy and remove, ty unix <3
  if ( fclose(file) ) {
    perror("Can't close file!");
  }
  if ( !(file = fopen("data.bdb", "r+b")) ) {
    perror("Can't open file");
  }
}


void insert_entry(FILE * file, Database * entry) {
  fseek(file, 0, SEEK_END);
  fwrite(entry, sizeof(Database), 1, file);
}

void read_entry(FILE * file, Database * entry) {
  fread(entry, sizeof(Database), 1, file);
}

void display_entry(Database * entry, unsigned index) {
  printf("|%3d|%50s|%10s|%6d| %10d| %10d|\n",
    index,
    entry->repair_type,
    entry->imei,
    entry->price,
    entry->investment,
    entry->profit
 );
}

void display_header() {
  printf("|%3s|%50s|%10s|%6s| %10s| %10s|\n",
    "i", "Repair Type", "IMEI", "Price", "Investment",
    "Profit");
}

void display_database_entries(FILE * file) {
  fseek(file, 0, SEEK_SET);
  Database entry;
  display_header();

  long int total_profit = 0;
  long int total_influx = 0;

  for ( int i = 0; fread(&entry, sizeof(Database), 1, file); i++ ) {
    display_entry(&entry, i);
    total_profit += entry.profit;
    total_influx += entry.price;
  }

  printf("TOTAL PROFIT: %ld TOTAL INFLUX: %ld \n", total_profit, total_influx);
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
  // for (int i = 0; i < 10; i++) {
  // Database nokia = {
  //   .blacklisted = 6,
  //   .repair_type = "BATERIE3",
  //   .imei = "nuavema",
  //   .price = i,
  //   .investment = 2,
  //   .profit = 3,
  // };
  // insert_entry(fp, &nokia);
  // }
  Database e;

  display_database_entries(fp);
  // delete_entry(fp, 1);
  display_database_entries(fp);
  // printf("after ins %ld\n", ftell(fp));
  // fseek(fp, -sizeof(Database), SEEK_CUR);
  // printf("before read %ld\n", ftell(fp));
  // read_entry(fp, &e);
  // printf("after read %ld\n", ftell(fp));
  // printf("%s\n", e.repair_type);
  return 0;
}
