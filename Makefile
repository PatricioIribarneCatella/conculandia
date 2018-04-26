CFLAGS := -g -Wall
CC := gcc
EXEC := main
BIN := $(filter-out $(EXEC).c, $(wildcard *.c))
BINFILES := $(BIN:.c=.o)

all: $(EXEC).out

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(EXEC).out: $(BINFILES) $(EXEC).c
	$(CC) $(CFLAGS) $^ -o $@

run: all
	bash run.sh

run-debug: all
	bash run-debug.sh

valgrind: all
	valgrind --leak-check=full ./$(EXEC).out

format: .clang-files
	xargs clang-format -style=file -i <$<

clean:
	rm -f $(EXEC).out *.txt *.o

.PHONY: clean run
