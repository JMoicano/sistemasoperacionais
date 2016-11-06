%.o: %.c
	gcc -c -o $@

compile: fsh runsim testsim

clean:
	rm testsim runsim fsh

all: compile
