OPT ?= -O2
CC = gcc
CFLAGS = -Wall -c -g $(OPT)
LDLIBS = -lSDL2 -lm
OBJS = main.o utils.o
EXE = main 
$(EXE) : $(OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean

clean:
	rm -f main *.o
	
