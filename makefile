INCLUDE=include
STD=-std=c++11
install : main.o
	g++ -Wall -I $(INCLUDE) $(STD) -o bin/out main.o 

main.o : main.cpp dijkstras.cpp
	gcc -Wall $(STD) -I $(INCLUDE) -c main.cpp

