#include <stdio.h>
#include <string.h>
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
void validate_input(int i);

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

void modify_entry(Database * entry, int exists) {
  if (entry->blacklisted && exists) {
    printf("Entry is blacklisted!\n Access Denied\n");
    return;
  }

  printf("Enter new repair type:\n");
  if (exists) printf("Current: %s\n> ", entry->repair_type);
  validate_input(0); scanf("%50[a-zA-Z -_'\"!~?]", entry->repair_type);
  printf("Enter new IMEI:\n");
  if (exists) printf("Current: %s\n> ", entry->imei);
  validate_input(0); scanf("%50[a-zA-Z ]", entry->imei);
  printf("Enter new price:\n");
  if (exists) printf("Current: %d\n> ", entry->price);
  validate_input(0); scanf("%d", &entry->price);
  printf("Enter new investment:\n");
  if (exists) printf("Current: %d\n> ", entry->investment);
  validate_input(0); scanf("%d", &entry->investment);
  printf("Enter new profit:\n");
  if (exists) printf("Current: %d\n> ", entry->profit);
  validate_input(0); scanf("%d", &entry->profit);
}

void update_entry(FILE * file, unsigned entry_no) {
  fseek(file, 0, SEEK_SET);
  Database entry;
  for( int i = 0; fread(&entry, sizeof(Database), 1, file); i++ ) {
    if ( i == entry_no ) {
      printf("Entry found!\n");
      fseek(file, -sizeof(Database), SEEK_CUR);
      modify_entry(&entry, 1);
      fwrite(&entry, sizeof(Database), 1, file);
    }
  }
}

void blacklist_entry(FILE * file, unsigned entry_no) {
  fseek(file, 0, SEEK_SET);
  Database entry;
  for( int i = 0; fread(&entry, sizeof(Database), 1, file); i++ ) {
    if ( i == entry_no ) {
      printf("Entry found!\n");
      fseek(file, -sizeof(Database), SEEK_CUR);
      if (entry.blacklisted) {
        printf("Entry has been Unblacklisted!\n");
        entry.blacklisted = 0;
      } else {
        printf("Entry has been Blacklisted!\n");
        entry.blacklisted = 1;
      }
      fwrite(&entry, sizeof(Database), 1, file);
    }
  }
}

void search_entry(FILE * file, char * imei) {
  fseek(file, 0, SEEK_SET);
  Database entry;
  for( int i = 0; fread(&entry, sizeof(Database), 1, file); i++ ) {
    if ( !strcmp(entry.imei, imei) ) {
      display_entry(&entry, 0);
    }
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
  printf("|%3d|%50s|%10s|%6d| %10d| %10d| %c\n",
    index,
    entry->repair_type,
    entry->imei,
    entry->price,
    entry->investment,
    entry->profit,
    (entry->blacklisted ? 'B' : '_')
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
  printf("6. Blacklist/UnBlacklist\n");
  printf("\n");
}

void validate_input(int i) {
  if (!i) {
    int c;
    while( (c = getchar()) != '\n' );
  }
}

void process_choice(FILE * file) {
  display_menu();
  Database e;

  enum choices { exit_program_c, display_the_database_c, add_entry_c, update_entry_c,
  delete_entry_c, search_by_imei_c, blacklist_c };

  int choice = -1;
  int entry_no = 0;
  char buffer[50];
  printf("Enter your choice\n>> ");
  validate_input(scanf("%d", &choice));
  switch (choice) {
    case exit_program_c:
      printf("Exiting...\n");
      exit(0);
      break;
    case display_the_database_c:
      display_database_entries(file);
      break;
    case add_entry_c:
      modify_entry(&e, 0);
      insert_entry(file, &e);
      break;
    case update_entry_c:
      printf("Enter the entry number: ");
      validate_input(scanf("%d", &entry_no));
      update_entry(file, entry_no);
      break;
    case delete_entry_c:
      validate_input(printf("Enter the entry number: "));
      scanf("%d", &entry_no);
      delete_entry(file, entry_no);
      break;
    case search_by_imei_c:
      printf("Enter the IMEI: ");
      scanf("%s", buffer);
      search_entry(file, buffer);
      break;
    case blacklist_c:
      validate_input(printf("Enter the entry number: "));
      scanf("%d", &entry_no);
      blacklist_entry(file, entry_no);
      break;
    default:
      printf("Invalid choice!\n");
  }
  process_choice(file); // redo the choice, since the user hasn't exited;
}

int main(void) {
  FILE * fp;
  if ( !(fp = fopen("data.bdb", "r+b")) ) {
    perror("Error opening file");
  }
  process_choice(fp);
  return 0;
}
