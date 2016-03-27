#include <iostream>
#include "algebraic_structure.h"
#include "condition.h"
#include "parse.h"

int main(int argc, char** argv){
    
    if (argc < 2) {
        std::cout << "Usage: name a file to open in command line, eg: ./a.out myfile.txt" << std::endl;
        return 1;
    }
    
    std::cout << "looking for file " << argv[1] << std::endl;
    
    Algebraic_structure *g = new Algebraic_structure();
    g = parse(argv[1]);
    
    //generate conditions here and then loop?
    //need to generate condition objects somewhere
    //then iterate over all of them
    //there are only five so we could do this quick here.
    
    
    g->two_ways_of_analogy_search();
    
    
    //results stored in each particular condition object
    //including string values for prover interface
    
    
    //call prover
    
    
    //output
    
    
}
