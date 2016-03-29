#include <iostream>
#include <vector>
#include "algebraic_structure.h"
#include "condition.h"
#include "parse.h"




int main(int argc, char** argv){
    
    //vector of all predicates we have to check
    std::vector<Condition*> predicates;
    if (argc < 2) {
        std::cout << "Usage: name a file to open in command line, eg: ./a.out myfile.txt" << std::endl;
        return 1;
    }
    
    std::cout << "looking for file " << argv[1] << std::endl;
    
    
    
    //create all predicates and store in a vector
    //change
    Condition *cc = new Condition(4);
    predicates.push_back(cc);
    
    
    //create domain from input
    Algebraic_structure *g = new Algebraic_structure();
    g = parse(argv[1]);
    
    
    //iterate over all predicates, searching all quantifier arrangments
    //boolean values for quantifier + predicate combinations are stored as they are
    //discovered in the "Condition" objects for the particular predicates.
    for(auto &p : predicates){
        g->two_ways_of_analogy_search(p);
    }
    
    
    
    //call prover
    
    
    //output
    
    
}
