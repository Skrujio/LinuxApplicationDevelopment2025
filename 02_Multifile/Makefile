.PHONY: all clean test
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -fPIC
PROG_SOURCES = prog.c fun.c const.c
PROG_OBJS = $(PROG_SOURCES:.c=.o)
LIB_SOURCES = fun.c const.c
LIB_OBJS = $(LIB_SOURCES:.c=.o)
EXECUTABLES = prog prog-so prog-a

all: $(EXECUTABLES)

prog: $(PROG_OBJS)

prog-a: prog.o liboutput_static.a($(LIB_OBJS))
	$(CC) $(CFLAGS) $^ -o $@

prog-so: prog.o liboutput.so
	$(CC) $(CFLAGS) -Wl,-rpath=. $^ -o $@

liboutput.so: $(LIB_OBJS)
	$(CC) -shared $^ -o $@

test: test_no_args test_one_arg test_three_args

test_no_args: $(EXECUTABLES)
	./prog > ./$@_prog 2>&1
	./prog-so > ./$@_prog-so 2>&1
	./prog-a > ./$@_prog-a 2>&1
	cmp ./$@_prog ./$@_prog-so
	cmp ./$@_prog ./$@_prog-a
	cmp ./$@_prog-a ./$@_prog-so

ONE_ARG = "ARG"

test_one_arg: $(EXECUTABLES)
	./prog $(ONE_ARG) > ./$@_prog 2>&1
	./prog-so $(ONE_ARG) > ./$@_prog-so 2>&1
	./prog-a $(ONE_ARG) > ./$@_prog-a 2>&1
	cmp ./$@_prog ./$@_prog-so
	cmp ./$@_prog ./$@_prog-a
	cmp ./$@_prog-a ./$@_prog-so

THREE_ARGS = "ARG1" "ARG2" "ARG3"

test_three_args: $(EXECUTABLES)
	./prog $(THREE_ARGS) > ./$@_prog 2>&1
	./prog-so $(THREE_ARGS) > ./$@_prog-so 2>&1
	./prog-a $(THREE_ARGS) > ./$@_prog-a 2>&1
	cmp ./$@_prog ./$@_prog-so
	cmp ./$@_prog ./$@_prog-a
	cmp ./$@_prog-a ./$@_prog-so

clean:
	rm -f *.o *.a *.so prog prog-a prog-so test_*

delete: clean
	rm -f prog prog-a prog-so