CC=gcc
EXECUTABLE=ezpswd
hp=./src/hide_pass.c

all: build

clean:
	rm -rf *.o ezpswd

build: ezpswd.o hide_pass.o
	$(CC) hide_pass.o ezpswd.o -o $(EXECUTABLE)

ezpswd.o: ezpswd.c
	$(CC) ezpswd.c -c ezpswd.c

hide_pass.o: $(hp)
	$(CC) $(hp) -c hide_pass.o
