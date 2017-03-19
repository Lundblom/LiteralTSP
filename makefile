INCLUDE=include
STD=-std=c++11
ARGS= -pthread
install : main.o Node.o PathStruct.o NodeCalculatorThread.o
	g++ -Wall -I $(INCLUDE) $(ARGS) $(STD) -o bin/out main.o Node.o

main.o : main.cpp
	g++ -Wall $(STD) -I $(INCLUDE) $(ARGS) -c main.cpp

%.o : %.cpp include/%.h
	g++ -Wall $(STD) -I $(INCLUDE) $(ARGS) -c $< -o $@


