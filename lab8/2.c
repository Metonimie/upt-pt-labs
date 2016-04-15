#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROW_LEN 50

typedef struct Db_tag {
  int blacklisted : 1; // 1 yes, 0 no
  char repair_type[ROW_LEN];
  long imei;
  int price;
  int investment;
  int profit;
} Database;

// function signatures
void display_entry(Database * entry, unsigned index);
void insert_entry(FILE * file, Database * entry);
void read_entry(FILE * file, Database * entry);
void validate_input(int i);

// Deletes entry by making a temporary file and copying everything in it.
// Except for the deleted entry, after that it deletes the original file
// and renames the temporary one to match the original file's name.
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

// Reads new entry from stdin.
// if exists is 1, it prints the entry's values.
// if the entry is blacklisted it quits;
void modify_entry(Database * entry, int exists) {
  if (entry->blacklisted && exists) {
    printf("Entry is blacklisted!\n Access Denied\n");
    return;
  }

  printf("Enter new repair type:\n");
  if (exists) printf("Current: %s\n> ", entry->repair_type);
  validate_input(0); scanf("%50[a-zA-Z -_'\"!~?]", entry->repair_type);
  printf("Enter new IMEI:\n");
  if (exists) printf("Current: %ld\n> ", entry->imei);
  validate_input(0); scanf("%10ld", &entry->imei);
  printf("Enter new price:\n");
  if (exists) printf("Current: %10d\n> ", entry->price);
  validate_input(0); scanf("%10d", &entry->price);
  printf("Enter new investment:\n");
  if (exists) printf("Current: %d\n> ", entry->investment);
  validate_input(0); scanf("%10d", &entry->investment);
  entry->profit = entry->price - entry->investment;
  entry->blacklisted = 0; // in case there is an 1 that's not touched.
  validate_input(0); // clear remaining trash, if any.
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
      return;
    }
  }
  printf("Entry not found!\n");
}

// Blacklists or Unblacklists an entry.
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

void search_entry(FILE * file, long imei) {
  fseek(file, 0, SEEK_SET);
  Database entry;
  for( int i = 0; fread(&entry, sizeof(Database), 1, file); i++ ) {
    if ( entry.imei == imei ) {
      display_entry(&entry, 0);
      return;
    }
  }
  printf("Entry not found!\n");
}

void insert_entry(FILE * file, Database * entry) {
  fseek(file, 0, SEEK_END);
  entry->profit = entry->price - entry->investment;
  fwrite(entry, sizeof(Database), 1, file);
}

void read_entry(FILE * file, Database * entry) {
  fread(entry, sizeof(Database), 1, file);
}

void display_entry(Database * entry, unsigned index) {
  printf("|%3d|%50s|%10ld|%10d| %10d| %10d| %c\n",
    index,
    entry->repair_type,
    entry->imei,
    entry->price,
    entry->investment,
    entry->profit,
    (entry->blacklisted ? 'B' : '_') // ternary operator, if blaclist is 1 it prints B
 );
}

void display_header() {
  printf("|%3s|%50s|%10s|%10s| %10s| %10s|\n",
    "i", "Repair Type", "IMEI", "Price", "Investment",
    "Profit");
}

// shows all entries and calculates profit + influx
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

// removes all characters from stdin until newline.
// basically it clears all the trash from stdin.
void validate_input(int i) {
  if (!i) {
    int c;
    while( (c = getchar()) != '\n' );
  }
}

// takes user choices.
void process_choice(FILE * file) {
  display_menu();
  Database e;

  enum choices { exit_program_c, display_the_database_c, add_entry_c, update_entry_c,
  delete_entry_c, search_by_imei_c, blacklist_c };

  int choice = -1;
  int entry_no = 0;
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
      printf("Enter the entry number: ");
      validate_input(scanf("%d", &entry_no));
      delete_entry(file, entry_no);
      break;
    case search_by_imei_c:
      printf("Enter the IMEI: ");
      validate_input(scanf("%ld", &e.imei));
      search_entry(file, e.imei);
      break;
    case blacklist_c:
      printf("Enter the entry number: ");
      validate_input(scanf("%d", &entry_no));
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
