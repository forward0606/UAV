
compile_parameter = g++ -g -Wall -Wextra 

input.out: input.o input/test_input.cpp
	$(compile_parameter) input.o input/test_input.cpp -o input.out
input.o: input/input.h input/input.cpp
	$(compile_parameter) -c input/input.h input/input.cpp

clean: 
	rm -rf *.out *.o