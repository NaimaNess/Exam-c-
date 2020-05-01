
CC := g++
CFLAGS := -std=c++17 -g -Wall -Wextra -O3
INC := .

PRJ := bst bst_ptr
SRC := $(wildcard *.cc) 
OBJ := $(SRC:%.cc=%.o)
EXE := $(PRJ:%=%.x)

all: $(EXE)

%.x:
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC)

clean:
	rm -f *.o *.x 

.PHONY: all clean

bst.x: main_BST.o
bst_ptr.x: main_BSTSmartPtr.o
