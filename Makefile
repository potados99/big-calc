# Makefile boilerplate version 0.0.1 (20190204)

TARGET = big-calc

CC = gcc
CFLAGS = -Wall -lm

SOURCE = $(wildcard *.c)
OBJECT = $(SOURCE:.c=.o)

$(TARGET): $(OBJECT)
	$(LINK.c) -o $@ $^

# Object file wildcard rule.
%.o: %.c
	$(COMPILE.c) $<

clean:
	rm *.o $(TARGET)*
