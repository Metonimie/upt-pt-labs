#include <stdio.h>

unsigned long int fib(unsigned long int n) {
    unsigned int i;
    static long int f[1000] = {0}; // better make it static than global...
    f[0] = 0;
    f[1] = 1;

    if (f[n]) {
        printf("Reused\n");
        return (unsigned long int ) f[n];
    }

    if (n > 2) {
        for (i = 2; i <= n; i++) {
            if (!f[i]) f[i] = f[i - 1] + f[i - 2];
        }
    }
    return (unsigned long int ) f[n];
}

int main() {
    printf("%ld\n", fib(11));
    printf("%ld\n", fib(10));
    printf("%ld\n", fib(12));
    printf("%ld\n", fib(12));
    return 0;
}
