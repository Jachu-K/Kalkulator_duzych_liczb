CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
OBJS = kalkulator.o vector.o operations.o io.o
TARGET = kalkulator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

kalkulator.o: kalkulator.c vector.h operations.h io.h
	$(CC) $(CFLAGS) -c kalkulator.c

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c

operations.o: operations.c operations.h vector.h
	$(CC) $(CFLAGS) -c operations.c

io.o: io.c io.h vector.h
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
