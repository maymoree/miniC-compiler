.PHONY: all frontend optimization clean test valgrind $(TARGET)

CC = g++
CFLAGS = -g -Wall -I./frontend -I./optimization -I./ast -I /usr/include/llvm-c-15/
LDFLAGS = -L./frontend -L./optimization -lfrontend -loptimization

SRC = miniC_compiler.c
OBJ = $(SRC:.c=.o)
TARGET = miniC_compiler.out

all: frontend optimization $(TARGET)

frontend:
	$(MAKE) -C frontend

optimization:
	$(MAKE) -C optimization

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C frontend clean
	$(MAKE) -C optimization clean
	rm -f $(OBJ) $(TARGET)

test: test_frontend test_optimization

test_frontend:
	$(MAKE) -C frontend smta_good
	$(MAKE) -C frontend smta_bad

test_optimization:
	$(MAKE) -C optimization test

valgrind: valgrind_frontend valgrind_optimization

valgrind_frontend:
	$(MAKE) -C frontend smta_good
	$(MAKE) -C frontend smta_bad

valgrind_optimization:
	$(MAKE) -C optimization valgrind
