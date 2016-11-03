compile:
	gcc -o runsim runsim.c
	gcc -o testsim testesim.c
run:
	./runsim 2 < testing.data
clean:
	rm testsim runsim
all: run
