#ifndef algebraic_structure_h
#define algebraic_structure_h

#include "algebraic_structure.h"
#include <vector>
#include <string>
#include <iostream>

//#include "condition.h"
class Condition;    //forward declaration to resolve circular dependency


class Algebraic_structure {
private:
    std::vector<char> elements;
    int order;   //number of elements
    std::string relation;
    std::vector < std::vector<char> > facts;
    unsigned long num_facts;
    
public:
    //constructors
    Algebraic_structure(){};
    
    Algebraic_structure(std::vector<char> e, std::string r, std::vector < std::vector<char> > f){
        elements = e;
        order = int(elements.size());
        relation = r;
        facts = f;
        num_facts = f.size();
        std::cout << "called Alg constructor: " << elements.size() << " elements, relation '" << relation << "' and " << facts.size() << " facts. " << std::endl;
    };
    
    
    void two_ways_of_analogy_search(Condition* c);
    
    void forall_x_forall_y(Condition*);
    
    void forall_x_exists_y(Condition*);
    
    void exists_x_forall_y(Condition*);
    
    void exists_x_exists_y(Condition*);
    
    void exists_x_exists_unique_y(Condition*);
    
    void exists_unique_x_exists_y(Condition*);
    
    void exists_unique_x_exists_unique_y(Condition*);
    
    char table_lookup(char x, char y);
};




#endif
