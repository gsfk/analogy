#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "algebraic_structure.h"
#include "condition.h"
#include "parse.h"
#include "prover.h"






void no_prover_warning(){
    std::cout << "\nProver not used, expect redundant formulas.\nYou should really user the prover! It's way more impressive that way.";
}





int main(int argc, char** argv){
    bool use_prover = true;
    bool second_file = false;
    bool shared_formulas = false;
    bool shared_axioms = false;
    std::string filename1, filename2;
    
    //vector of all predicates we have to check
    std::vector<Condition*> predicates;
    std::vector<Condition*> predicates2;
    
    //all true formulas found (all true quantifier / predicate combinations)
    std::vector<Formula> *formulas = new std::vector<Formula>;
    std::vector<Formula> *formulas2 = new std::vector<Formula>;
    
    std::map<std::string, int> all_sufficient_subsets;
    
    
    //user help
    if (argc < 2) {
        std::cout << "Usage: name any input files in command line arguments:\n";
        std::cout << "To find axioms for a single file: ./analogy yourfilename.txt\n";
        std::cout << "To compare axioms for two structures, enter two file names: ./analogy file1 file2\n";
        std::cout << "Requires Prover9 installation: https://www.cs.unm.edu/~mccune/mace4/download/\n";
        std::cout << "To use without a prover add \"-noprover\" as a final argument... but it won't look nearly as cool.\n";
        std::cout << "See README for more details." << std::endl;
        return 1;
    }
    
    //arg handling
    if (argc > 2 && std::string(argv[2]) == "-noprover"){
        use_prover = false;
    } else if (argc > 2) {
        second_file  = true;
        filename2 = argv[2];
    }
    if (argc > 3 && std::string(argv[3]) == "-noprover"){
        use_prover = false;
    }
    
    filename1 = argv[1];
    
    //create domain from input
    Algebraic_structure *g = new Algebraic_structure();
    g = parse(argv[1]);
    
    //create all predicates and store in a vector
    predicates = return_all_predicates(g->relation);
    
    //search all quantifier / predicate / variable order arrangments, saving truth values as you go
    for(auto &p : predicates){
        g->two_ways_of_analogy_search(p);
    }
    
    //explore select properties not expressible with only 2 quantifiers (commutativity, associativity, etc)
    //see algebraic_structure.h and .cpp for full details
    g->extra_properties_search();
    
    
    //get all true formulas to send to the prover
    //todo: still need substitution code for uniqueness quantifier
    
    //ADD G SOMEWHERE
    formulas = &generate_formulas(predicates, *g);
    
    
    //print formulas
    int formula_count = 1;
    std::cout << "\nFound formulas for " << filename1 << ":\n" <<std::boolalpha;
    for (auto f : *formulas){
        std::cout << formula_count << ": " << f.pretty_name << " " << std::endl;
        formula_count++;
    }
    
    
    
    //call prover
    if (use_prover){
        invoke_prover(formulas);
        
        //experimental code: generate all 2^n subsets in order to search for true minimum sets of formulas.
        //No user output, this is used for comparison to the "subsumption" algorithm used in practice for
        //redundancy elimination. See prover.cpp comments for full details.
        
        //all_sufficient_subsets = find_sufficient_subsets(formulas);
    }
    

    if (!use_prover){
        no_prover_warning();
    }
    
    
    

    if (!second_file){
        if (use_prover){
            
            //output for single file mode
            std::cout << "\nMinimal set for " << filename1 << ":\n";
            formula_count = 1;
            for (auto &f : *formulas){
                if (f.subsumption_include){
                    std::cout << formula_count << ": " << f.pretty_name << "\n";
                    formula_count ++;
                }
            }
        }
        
    } else {    //else two input files
        
        //repeat all the search code above for second file:
        
        //generate domain from input
        Algebraic_structure *g2 = new Algebraic_structure();
        g2 = parse(argv[2]);
        
        //get all predicates for this relation
        predicates2 = return_all_predicates(g2->relation);
        
        //check predicates against domain for differing quantification
        for(auto &p : predicates2){
            g2->two_ways_of_analogy_search(p);
        }
        
        //extended cases
        g2->extra_properties_search();
        
        //save all formulas found true, send to prover for redundancy elimination
        formulas2 = &generate_formulas(predicates2, *g2);
        if (use_prover){
            invoke_prover(formulas2);
        }
        
        
        //show formulas unique to file 1
        std::cout << "\nFormulas unique to " << filename1 << "\n";
        formula_count = 1;
        for (auto f1 : *formulas){
            bool this_formula_shared = false;
            for (auto f2 : *formulas2){
                if (f1.prover_name.compare(f2.prover_name)==0){
                    this_formula_shared = true;
                }
            }
            if (!this_formula_shared){
                std::cout << formula_count << ": " << f1.pretty_name << "\n";
                formula_count ++;
            }
        }
        if (formula_count ==1 ){
            std::cout << "(none)\n";
        }
        
        //show formulas unique to file 2
        std::cout << "\nFormulas unique to " << filename2 << "\n";
        formula_count = 1;
        for (auto f2 : *formulas2){
            bool this_formula_shared = false;
            for (auto f1 : *formulas){
                if (f1.prover_name.compare(f2.prover_name)==0){
                    this_formula_shared = true;
                }
            }
            if (!this_formula_shared){
                std::cout << formula_count << ": " << f2.pretty_name << "\n";
                formula_count ++;
            }
            
        }
        if (formula_count ==1 ){
            std::cout << "(none)\n";
        }
        
        
        //show all shared formulas
        std::cout << "\nFormulas shared between " << filename1 << ", " << filename2 << ": \n";
        formula_count = 1;
        for (auto f1 : *formulas){
            for (auto f2 : *formulas2){
                if (f1.prover_name.compare(f2.prover_name)==0){
                    shared_formulas = true;
                    std::cout << formula_count << ": " << f1.pretty_name << "\n";
                    formula_count ++;
                }
            }
        }
        if (!shared_formulas){
            std::cout << "(none)" << "\n";
        }
        
        
        //show minimal sets if prover attached
        if (use_prover){
            
            //show file 1 minimal set
            std::cout << "\nMinimal set for " << filename1 << ": \n";
            formula_count = 1;
            for (auto &f : *formulas){
                if (f.subsumption_include){
                    std::cout << formula_count<< ": " << f.pretty_name << "\n";
                    formula_count ++;
                }
            }
            
            //show file 2 minimal set
            std::cout << "\nMinimal set for " << filename2 << ": \n";
            formula_count = 1;
            for (auto &f : *formulas2){
                if (f.subsumption_include){
                    std::cout << formula_count<< ": " << f.pretty_name << "\n";
                    formula_count ++;
                }
            }
            
            //show any shared axioms (formulas shared between minimal sets)
            std::cout << "\nFormulas shared between minimal sets: \n";
            formula_count = 1;
            for (auto f1 : *formulas){
                for (auto f2 : *formulas2){
                    if (f1.subsumption_include && f2.subsumption_include && (f1.prover_name.compare(f2.prover_name)==0)){
                        shared_axioms = true;
                        std::cout << formula_count << ": " << f1.pretty_name << "\n";
                        formula_count ++;
                    }
                }
            }
            if (!shared_axioms){
                std::cout << "(none)" << "\n";
            }
        } else {no_prover_warning();}
        
        
    }
    
    std::cout << "\nend of main" << std::endl;
    
}











