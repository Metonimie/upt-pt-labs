//#include <intrin.h> // or x86intrin.h | x64intrin.h ?
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

uint64_t _lrotl(uint64_t val, int shift) {
    uint64_t hibit;                  /* non-zero means hi bit set */
    uint64_t num = val;              /* number to rotate */
    shift &= 0x3f;               	   /* modulo 64 -- this will also make
                                     	  	negative shifts work */
    while (shift--) {
        hibit = num & 0x8000000000000000;  /* get high bit */
        num <<= 1;                         /* shift left one bit */
        if (hibit)
	        num |= 0x1;                        /* set lo bit if hi bit was set */
    }
    return num;
}

uint64_t _lrotr(uint64_t val, int shift) {
    uint64_t lobit;
    uint64_t num = val;
    shift &= 0x3f;
    while (shift--) {
        lobit = num & 0x1;
        num >>= 1;
        if (lobit)
            num |= 0x8000000000000000;
    }
    return num;
}

#define LCS _lrotl // left circular shift
#define RCS _lrotr // right circular shift
#define u64 unsigned long long
#define R(x,y,k) (x=RCS(x,8) , x+=y, x^=k, y=LCS(y,3) , y^=x)
#define INV_R(x,y,k) (y^=x, y=RCS(y,3) , x^=k, x =y, x=LCS(x,8))

void Speck128ExpandKeyAndEncrypt(u64 pt [] , u64 ct [] , u64 K[]) {
  u64 i,B=K[1],A=K[0];
  ct[0]=pt[0]; ct[1]=pt[1];
  for(i=0; i<32; i++) {
    R(ct[1], ct[0], A); R(B, A, i);
  }
}

void Speck128Encrypt(u64 pt[], u64 ct[], u64 k[]) {
  u64 i;
  ct[0]=pt[0]; ct[1]=pt[1];
  for(i=0; i<32; i++) R(ct[1], ct[0], k[i]);
}

void Speck128Decrypt(u64 pt[], u64 ct[], u64 K[]) {
  long long i;
  for(i=31; i>=0; i--) {
    INV_R(ct[1], ct[0], K[i]);
  }
}

static void speck_keyexpand(u64 K[]) // OK
{
  u64 tmp[32], *p;
  memcpy(tmp,K, sizeof tmp);
  // K[0] stays the same
  u64 i;
  for (i=0; i<(31);) {
    p = tmp + (1+( i %(2 - 1)));
    R(*p,tmp[0] , i );
    ++i ;
    K[i] = tmp[0];
  }
}

int main(void) {
  u64 k[32];
  k[1] = 0x0f0e0d0c0b0a0908;
  k[0] = 0x0706050403020100;

  u64 pt[2];
  pt [1] = 0x6c61766975716520;
  pt [0] = 0x7469206564616d20;
  u64 ctverify [2];

  ctverify [1] = 0xa65d985179783265;
  ctverify [0] = 0x7860fedf5c570d18;
  u64 ct[] = {0x0, 0x0};

  // Speck128ExpandKeyAndEncrypt ( pt , ct, k);
  speck_keyexpand (k);
  Speck128Encrypt (pt, ct, k);

  printf("ct[] = %llx %llx\n", ct[0], ct[1]);

  if (( ct [1] == ctverify [1]) && ( ct [0] == ctverify [0]))
    printf("Encryption Test Vector OK\n");

  Speck128Decrypt ( pt, ct, k ) ;
  printf("ct[] = %llx %llx\n", ct[0], ct[1]);

  if ((ct[1] == pt[1]) && (ct[0] == pt[0]))
    printf("Decryption Test Vector OK\n");

  return 0;
}
