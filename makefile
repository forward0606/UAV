compile_parameter = g++ -g -Wall -Wextra 

all: MyAlgo.o


test_input.out: input.o input/test_input.cpp
	$(compile_parameter) input.o input/test_input.cpp -o test_input.out
input.o: input/input.h input/input.cpp
	$(compile_parameter) -c input/input.h input/input.cpp

AlgorithmBase.o: Algorithm/AlgorithmBase/AlgorithmBase.h Algorithm/AlgorithmBase/AlgorithmBase.cpp
	$(compile_parameter) -c Algorithm/AlgorithmBase/AlgorithmBase.h Algorithm/AlgorithmBase/AlgorithmBase.cpp

MyAlgo.o: Algorithm/MyAlgo/Parameter.h Algorithm/MyAlgo/MyAlgo.h Algorithm/MyAlgo/MyAlgo.cpp Algorithm/MyAlgo/Square.h Algorithm/MyAlgo/Square.cpp
	$(compile_parameter) -c Algorithm/MyAlgo/Parameter.h Algorithm/MyAlgo/MyAlgo.h Algorithm/MyAlgo/MyAlgo.cpp Algorithm/MyAlgo/Square.h Algorithm/MyAlgo/Square.cpp

Square.o: Algorithm/MyAlgo/Parameter.h Algorithm/MyAlgo/Square.h Algorithm/MyAlgo/Square.cpp
	$(compile_parameter) -c Algorithm/MyAlgo/Parameter.h Algorithm/MyAlgo/Square.h Algorithm/MyAlgo/Square.cpp

test_MyAlgo.out: input.o AlgorithmBase.o MyAlgo.o Algorithm/MyAlgo/testMyAlgo.cpp Square.o
	$(compile_parameter) Algorithm/MyAlgo/testMyAlgo.cpp input.o AlgorithmBase.o MyAlgo.o Square.o -o test_MyAlgo.out 
	
run: test_MyAlgo.out
	./test_MyAlgo.out
	./test_MyAlgo.out > test
	python3 plot.py
clean: 
	rm -rf *.out *.o
	rm -rf *.gch