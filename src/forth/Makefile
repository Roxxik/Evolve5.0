# ------------------------------------------ #
# some evolution stuff                       #
# Stefan Nitz                                #
# created: 21.05.2015                        #
#                                            #
# Makefile                                   #
# ------------------------------------------ #

#Comiler
CC = clang

#Comilerflags
CFLAGS = -c -g -Wall -Wextra -Werror
#Linkerflags
LDFLAGS = -lm -flto

SOURCE = main.c block.c instruction.c builtins.c code.c forth.c cstack.c dstack.c
OBJECTS =$(SOURCE:.c=.o)

#Target name
BINARY = evolve

.PHONY: clean all

default: all

all: $(BINARY)

#Compile a file
%.o : %.c
	@echo "  - Building $@"
	$(CC) $(CFLAGS) -o $@ $<

#Link all files together
$(BINARY): $(OBJECTS)
	@echo "  - linking $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BINARY)

#Clean the project
clean:
	@echo "  - delete object all files"
	rm -f $(OBJECTS) $(BINARY)
