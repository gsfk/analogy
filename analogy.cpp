#include <iostream>
#include <vector>
#include "algebraic_structure.h"
#include "condition.h"
#include "parse.h"

//TODO
std::vector<Condition*> return_all_predicates(){}





int main(int argc, char** argv){
    bool use_prover = true;
    
    //vector of all predicates we have to check
    //std::vector<Condition*> predicates;
    std::vector<Condition*> predicates;
    
    if (argc < 2) {
        std::cout << "Usage: name a file to open in command line, eg: ./a.out myfile.txt\n";
        std::cout <<" To use without a prover add \"-noprover\" as a final argument" << std::endl;
        return 1;
    }
    
    if (argc > 2 && std::string(argv[2]) == "-noprover"){
        use_prover = false;
    }
    
    std::cout << "looking for file " << argv[1] << std::endl;
    
    
    
    //create domain from input
    Algebraic_structure *g = new Algebraic_structure();
    g = parse(argv[1]);
    
    
    //create all predicates and store in a vector
    //change
    //Condition *cc = new Condition(4);
    //predicates.push_back(cc);
    predicates = return_all_predicates();
    std::cout << "all predicates returned, first predicate is " << predicates[0]->name << std::endl;
    
    
    
    //iterate over all predicates, searching all quantifier arrangments
    //boolean values for quantifier + predicate combinations are stored as they are
    //discovered in the "Condition" objects for the particular predicates.
    for(auto &p : predicates){
        g->two_ways_of_analogy_search(p);
    }
    
    
    
    //call prover
    if (use_prover){
        
        //all prover code inside here, or at least called from here
    }
    
    
    
    //output
    if (!use_prover){
        //tell user prover not used
    }
    
    std::cout << "end of main" << std::endl;
    
}