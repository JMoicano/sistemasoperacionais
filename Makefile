compile:
	gcc -o runsim runsim.c
	gcc -o testsim testesim.c
run: compile
	./runsim 2 < testing.data
all: run
