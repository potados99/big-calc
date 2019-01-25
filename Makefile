CC=C:\\MinGW\\bin\\gcc.exe
TARGET=bn.exe
CFLAGS=-Wall

$(TARGET) : main.o bignum.o
	$(CC) -o $(TARGET) main.o bignum.o $(CFLAGS)

main.o : main.c
	$(CC) -c main.c $(CFLAGS)

bignum.o : bignum.c
	$(CC) -c bignum.c $(CFLAGS)

clean :
	rm *.o $(TARGET)
