
#include <stdio.h>
#include "constants.h"
#include "cipher.h"

int main () {
		/*
		 * Speck 64/96
		 */
		u8 inputKey[] = {0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x11, 0x12, 0x13};
		u8 keys[SPECK_BLOCK_SIZE/16*SPECK_ROUNDS];
		// plain text: 74614620 736e6165
		u8 plainText[] = {0x65, 0x61, 0x6e, 0x73, 0x20, 0x46, 0x61, 0x74};

		encryptKeySchedule(inputKey, keys);

		printf("PlainText: %x, %x, %x, %x, %x, %x, %x, %x\n",
			plainText[0], plainText[1], plainText[2], plainText[3],
			plainText[4], plainText[5], plainText[6], plainText[7]);

		encrypt(plainText, keys);
		// cipher text: 9f7952ec 4175946c
		printf("After encryption: %x, %x, %x, %x, %x, %x, %x, %x\n",
			plainText[0], plainText[1], plainText[2], plainText[3],
			plainText[4], plainText[5], plainText[6], plainText[7]);

		decrypt(plainText, keys);
		printf("After decryption: %x, %x, %x, %x, %x, %x, %x, %x\n",
			plainText[0], plainText[1], plainText[2], plainText[3],
			plainText[4], plainText[5], plainText[6], plainText[7]);
	return 0;
}
