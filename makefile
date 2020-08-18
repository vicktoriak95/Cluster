FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: cluster

clean:
	rm -rf *.o cluster

cluster: NetworkDivision.o LibFuncsHandler.o LinearUtils.o Network.o NodeUtils.o PowerIteration.o SparseMatrix.o
	gcc NetworkDivision.o LibFuncsHandler.o LinearUtils.o Network.o NodeUtils.o PowerIteration.o SparseMatrix.o -o cluster $(LIBS)

NetworkDivision.o: NetworkDivision.c
	gcc $(FLAGS) -c NetworkDivision.c

LibFuncsHandler.o: LibFuncsHandler.c
	gcc $(FLAGS) -c LibFuncsHandler.c

LinearUtils.o: LinearUtils.c
	gcc $(FLAGS) -c LinearUtils.c

Network.o: Network.c
	gcc $(FLAGS) -c Network.c

NodeUtils.o: NodeUtils.c
	gcc $(FLAGS) -c NodeUtils.c

PowerIteration.o: PowerIteration.c
	gcc $(FLAGS) -c PowerIteration.c

SparseMatrix.o: SparseMatrix.c
	gcc $(FLAGS) -c SparseMatrix.c