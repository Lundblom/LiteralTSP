INCLUDE=include
STD=-std=c++11
ARGS= -g -pthread
install : newMain.o Node.o PathStruct.o NodeCalculatorThread.o Traveler.o
	g++ -Wall -I $(INCLUDE) $(ARGS) $(STD) -o bin/out newMain.o Traveler.o Node.o NodeCalculatorThread.o PathStruct.o

newMain.o : newMain.cpp a_star.cpp
	g++ -Wall $(STD) -I $(INCLUDE) $(ARGS) -c newMain.cpp

%.o : %.cpp
	g++ -Wall $(STD) -I $(INCLUDE) $(ARGS) -c $< -o $@

%.o : %.cpp include/%.h
	g++ -Wall $(STD) -I $(INCLUDE) $(ARGS) -c $< -o $@




