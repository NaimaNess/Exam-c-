CC = g++

dirB = $(PWD)

exe_name = tree.x

OPT = -DDEFAULT

FLAGS = -g -fsanitize=address

CFLAGS = -I $(dirB)  $(FLAGS) $(OPT) -std=c++14 -Wall -Wextra -O3

LDFLAGS = $(FLAGS) 

INC = $(dirB)/tree.h $(dirB)/node.h $(dirB)/iterator.h

OBJ = main.o

#################################################################

all: $(exe_name)

$(exe_name): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

main.e: main.cc $(INC) $(TPL) Makefile
	$(CC) $(CFLAGS) -E $< -o $@

main.o: main.cc $(INC) $(TPL) Makefile
	$(CC) $(CFLAGS) -c $< -o $@


# eliminate default suffixes

.SUFFIXES:
SUFFIXES =

# just consider our own suffixes

.SUFFIXES: .cpp .tpp .o

#################################################################
# phony rules

.PHONY: clean documentation clean_doc

clean:
	$(info clean up)
	rm -rf $(exe_name) main.o $(OBJ) *~ *# $(dirT)/*~ $(dirI)/*~ $(dirD)/*~ $(PWD)/input/*~

clean_doc:
	$(info clean doc)
	rm -rf  $(dirD)/*~ $(dirD)/html $(dirD)/latex

