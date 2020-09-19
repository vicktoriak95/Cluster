FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -g -O0
LIBS = -lm

all: cluster

clean:
	rm -rf *.o cluster

cluster: NetworkDivision.o LibFuncsHandler.o LinearUtils.o Network.o NodeUtils.o PowerIteration.o SparseMatrix.o ModularityMax.o Group.o main.o
	gcc NetworkDivision.o LibFuncsHandler.o LinearUtils.o Network.o NodeUtils.o PowerIteration.o SparseMatrix.o ModularityMax.o Group.o main.o -o cluster $(LIBS)

main.o: main.c
	gcc $(FLAGS) -c main.c

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

ModularityMax.o: ModularityMax.c
	gcc $(FLAGS) -c ModularityMax.c

Group.o: Group.c
	gcc $(FLAGS) -c Group.c
