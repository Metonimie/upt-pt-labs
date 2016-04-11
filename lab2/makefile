CC = gcc
TARGET = my_string
CFLAGS = -Wall -O3
all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -c str.c
	ar rcs libstr.a str.o
	$(CC) $(CFLAGS) main.c -o $(TARGET).x -L. -lstr

clean:
	$(RM) $(TARGET).x
