
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "cipher.h"

void encrypt_file(char * filename, u8 * password) {
  FILE * in = fopen(filename, "rb");
  FILE * out = fopen("encrypted.speck", "wb");

  if ( !in ) {
    perror("Error opening file.");
  }
}

void decrypt_file(char * filename, u8 * password) {
  FILE * in = fopen(filename, "rb");
  FILE * out = fopen("decrypted.speck", "wb");


  if ( !in ) {
    perror("Error opening file.");
  }
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

  u8 plainText[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};


	//
	// 	printf("PlainText: %c, %c, %c, %c, %c, %c, %c, %c\n",
	// 		plainText[0], plainText[1], plainText[2], plainText[3],
	// 		plainText[4], plainText[5], plainText[6], plainText[7]);
	//
  // encrypt(plainText, keys);
	//
	// 	printf("After encryption: %c, %c, %c, %c, %c, %c, %c, %c\n",
	// 		plainText[0], plainText[1], plainText[2], plainText[3],
	// 		plainText[4], plainText[5], plainText[6], plainText[7]);
	//
  // decrypt(plainText, keys);
	// 	printf("After decryption: %c, %c, %c, %c, %c, %c, %c, %c\n",
	// 		plainText[0], plainText[1], plainText[2], plainText[3],
	// 		plainText[4], plainText[5], plainText[6], plainText[7]);

	return 0;
}
