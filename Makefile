CC=gcc
TARGET=bn
CFLAGS=-Wall

$(TARGET) : main.o bignum.o test.o
	$(CC) -o $(TARGET) main.o bignum.o test.o $(CFLAGS)

main.o : main.c
	$(CC) -c main.c $(CFLAGS)

bignum.o : bignum.c
	$(CC) -c bignum.c $(CFLAGS)

test.o : test.c
	$(CC) -c test.c $(CFLAGS)

clean :
	rm *.o $(TARGET).*
