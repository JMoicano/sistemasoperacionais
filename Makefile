compile:
	gcc -c list.c
	gcc -o fsh list.o fsh.c
	gcc -o runsim runsim.c
	gcc -o testsim testsim.c

clean:
	rm -rf list.o testsim runsim fsh

all: compile
