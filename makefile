
install : main.o
	g++ -o bin/out main.o

main.o : main.cpp
	g++ -c main.cpp
