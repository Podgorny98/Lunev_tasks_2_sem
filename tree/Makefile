CC = gcc
CFLAGS = -Wall -O0 -fprofile-arcs -ftest-coverage -MMD
LDFLAGS = -fprofile-arcs -ftest-coverage

all: test

test: tree.o test.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include *.d

clean:
	rm -rf test dump *.o *.d *.gcda *.gcno *.gcov
