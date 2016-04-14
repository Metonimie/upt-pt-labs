CC = gcc
TARGET1 = 1
TARGET2 = 2
CFLAGS = -Wall -O3
all: clean $(TARGET1) $(TARGET2)

1:
	$(CC) $(CFLAGS) -c cipher.c
	ar rcs libcipher.a cipher.o
	$(CC) $(CFLAGS) $(TARGET1).c -o $(TARGET1) -L. -lcipher

2:
	$(CC) $(CFLAGS) $(TARGET2).c -o $(TARGET2)

clean:
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)
