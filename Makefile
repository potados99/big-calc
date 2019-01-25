CC=C:\\MinGW\\bin\\gcc.exe
TARGET=bn.exe

$(TARGET) : main.o bignum.o
	$(CC) -o $(TARGET) main.o bignum.o

main.o : main.c
	$(CC) -o main.o main.c

bignum.o : bignum.c
	$(CC) -o bignum.o bignum.c

clean :
	rm *.o $(TARGET)
