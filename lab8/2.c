#include <stdio.h>

#define ROW_LEN 50

typedef struct Db_tag {
  int i;
  char repair_type[ROW_LEN];
  char imei[ROW_LEN];
  int price;
  int investment;
  int profit;
  int total_profit;
  int total_influx;
} Database;

int main(void) {
  return 0;
}
