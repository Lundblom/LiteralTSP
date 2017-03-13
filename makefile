INCLUDE=include
STD=-std=c++11
install : main.o node.o
	g++ -Wall -I $(INCLUDE) $(STD) -o bin/out main.o node.o

main.o : main.cpp dijkstras.cpp a_star.cpp node.o
	gcc -Wall $(STD) -I $(INCLUDE) -c main.cpp

node.o : node.cpp
	gcc -Wall $(STD) -I $(INCLUDE) -c node.cpp

