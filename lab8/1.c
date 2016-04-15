#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "cipher.h"

u8 * read_from_file(FILE * stream, int * read_o) {
  long u8_size = sizeof(u8);
  long size = u8_size * 1000;
  long read = 0;
  u8 * buffer = malloc( size );
  for (int i = 0; fread(buffer + i * u8_size, u8_size, 1, stream); i++ ) {
    read += u8_size;
    if ( read > 1) {
      u8 * tmp = realloc(buffer, size);
      if (!tmp) {
        perror("Error, not enough memory!");
        exit(0);
      }
      buffer = tmp;
    }
  };
  *read_o = read;
  return buffer;
}

void encrypt_file(char * filename, u8 * password) {
  FILE * in = fopen(filename, "rb");
  FILE * out = fopen("encrypted.speck", "wb");
  if ( !in ) {
    perror("Error opening file.");
    return;
  }
  int read = 0;
  u8 * buffer = read_from_file(in, &read);
  encrypt(buffer, password);
  fwrite(buffer, sizeof(u8), read, out);
  printf("%s\n", buffer);
}

void decrypt_file(char * filename, u8 * password) {
  FILE * in = fopen(filename, "rb");
  FILE * out = fopen("decrypted.speck", "wb");
  if ( !in ) {
    perror("Error opening file.");
    return;
  }
  int read = 0;
  u8 * buffer = read_from_file(in, &read);
  decrypt(buffer, password);
  fwrite(buffer, sizeof(u8), read, out);
  printf("%s\n", buffer);

}

int main (int argc, char *argv[]) {

  if ( argc != 4) {
    printf("Invalid usage! Correct usage %s filname password [1/0 which represent"\
    "encrypt or decrypt]\n", argv[0]);
    return 1;
  }

  char * password = argv[2];
  u8 inputKey[strlen(password) - 1];
  int i = 0;
  for (; password[i]; i++) {
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
