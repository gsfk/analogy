#ifndef algebraic_structure_h
#define algebraic_structure_h

#include <vector>
#include <string>
#include <iostream>
#include <map>

class Condition;    //forward declaration to resolve circular dependency

enum Properties {
    ASSOCIATIVITY,
    COMMUTATIVITY,
    CLOSURE,
    LEFT_IDENTITY_LEFT_INVERSE,
    RIGHT_IDENTITY_LEFT_INVERSE,
    LEFT_IDENTITY_RIGHT_INVERSE,
    RIGHT_IDENTITY_RIGHT_INVERSE,
};



/*
 Class to represent input domain, stores the elements and fact table from input,
 and a few other properties of the domain.
 */

class Algebraic_structure {
private:
    
    //domain elements
    std::vector<char> elements;
    
    //number of elements
    int order;
    
    //fact table generated from input facts
    std::vector < std::vector<char> > facts;
    unsigned long num_facts;
    
public:
    std::string relation;
    
    //relational properties not tied to a particular condition
    //commutativity, associativity, inverses, etc.
    std::map<Properties, bool> extra_properties;
    
    
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
    
    char table_lookup(char, char);
    
    
    //Brute force checks of all two quantifier cases
    void two_ways_of_analogy_search(Condition*);
    
    void forall_x_forall_y(Condition*);
    
    void forall_x_exists_y(Condition*);
    
    void exists_x_forall_y(Condition*);
    
    void exists_x_exists_y(Condition*);

    void exists_x_exists_unique_y(Condition*);
    
    void exists_unique_x_exists_y(Condition*);
    
    void exists_unique_x_exists_unique_y(Condition*);
    
    
    
    //select properties requiring greater than two quantifiers
    void extra_properties_search();
    
    void commutativity();
    
    void associativity();
    
    void closure();
    
    void left_identity_left_inverse();
    
    void left_identity_right_inverse();

    void right_identity_left_inverse();
    
    void right_identity_right_inverse();
    
    
    
    //helper functions
    bool is_left_identity_element(char iden);
    
    bool is_right_identity_element(char iden);
};




#endif
