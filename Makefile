all: analogy

analogy:
	$ g++ -std=c++11 -o analogy analogy.cpp parse.cpp algebraic_structure.cpp condition.cpp prover.cpp

clean: 
	rm -rf *.o 
	rm analogy
