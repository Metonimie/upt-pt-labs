#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "cipher.h"

u8 * read_from_file(FILE * stream, long * read_o) {
  long u8_size = sizeof(u8);
  long size = u8_size * 1000;
  long read = 0;
  long left_over = 0;
  u8 * buffer = malloc( size );
  for (int i = 0; fread(buffer + i, u8_size, 1, stream); i++ ) {
    read += u8_size;
    if ( read > size - 100) { // realloc buffer in case it overflows
      size *= 2; // double the size
      u8 * tmp = realloc(buffer, size);
      if (!tmp) {
        perror("Error, not enough memory!");
        exit(0);
      }
      buffer = tmp;
    }
  };
  left_over = read % 8;
  for (int j = 1; j <= left_over; j++) { // make 8 bytes
    buffer[read + j] = (char) 32; // by adding spaces.
  }
  *read_o = read + left_over;
  return buffer; // buffer needs to be divisible by 8 for this to work.
}

void encrypt_file(char * filename, u8 * password) {
  FILE * in = fopen(filename, "rb");
  FILE * out = fopen("encrypted.speck", "wb");
  if ( !in ) {
    perror("Error opening file.");
    return;
  }
  long read = 0;
  u8 * buffer = read_from_file(in, &read);
  for (int i = 0; i < read/8; i++) {
    encrypt(buffer+i*8, password);
  }

  fwrite(buffer, sizeof(u8), read, out);
  printf("%s\n", buffer);
  if ( fclose(in) || fclose(out) ) {
    perror("Error closing files!");
  }
}

void decrypt_file(char * filename, u8 * password) {
  FILE * in = fopen(filename, "rb");
  FILE * out = fopen("decrypted.speck", "wb");
  if ( !in ) {
    perror("Error opening file.");
    return;
  }
  long read = 0;

  u8 * buffer = read_from_file(in, &read);
  for (int i = 0; i < read/8; i++) {
    decrypt(buffer+i*8, password);
  }

  fwrite(buffer, sizeof(u8), read, out);
  printf("%s\n", buffer);
  if ( fclose(in) || fclose(out) ) {
    perror("Error closing files!");
  }
}

int main (int argc, char *argv[]) {

  if ( argc != 4) {
    printf("Invalid usage! Correct usage %s filname password [1/0 which represent"\
    "encrypt or decrypt]\n", argv[0]);
    return 1;
  }

  char * password = argv[2];
  if (strlen(password) < 12) {
    perror("Password must be at least 12 characters long!");
    return 0;
  }
  u8 inputKey[strlen(password) - 1];

  for (int i = 0; password[i]; i++) {
    inputKey[i] = password[i];
  }

  u8 keys[SPECK_BLOCK_SIZE/16*SPECK_ROUNDS];
  encryptKeySchedule(inputKey, keys);

  if ( !strcmp(argv[3], "1") ) {
    encrypt_file(argv[1], keys);
  } else {
    decrypt_file(argv[1], keys);
  }

  return 0;
}
