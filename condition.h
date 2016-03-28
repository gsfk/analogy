#ifndef condition_h
#define condition_h
#include <map>
#include "algebraic_structure.h"


enum Quantifiers {FOR_ALL_X_FOR_ALL_Y,
    FOR_ALL_X_EXISTS_Y,
    FOR_ALL_X_EXISTS_UNIQUE_Y,
    EXISTS_X_FOR_ALL_Y,
    EXISTS_X_EXISTS_Y,
    EXISTS_X_EXISTS_UNIQUE_Y,
    EXISTS_UNIQUE_X_FOR_ALL_Y,
    EXISTS_UNIQUE_X_EXISTS_Y,
    EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y
};


class Condition {
private:
    
    //predicate P(x,y,z) is represented as a function f(x,y) -> z
    std::function<int (char,char)> f;
    
    //string representing this predicate, needed both for human readable output
    //and interaction with the theorem prover
    
    //TODO: can't we construct the string from the relation name alone?
    std::string s;
    
public:
    //map from quantifiers to boolean values for this predicate
    //this is filled in by the search code in algebraic_structure.cpp
    std::map<Quantifiers, bool> values;
    
    //predicate arity
    int arity;
    
    //default constructor
    Condition(){};
    
    //constructor TODO
    Condition(int x){};
    //f = function
    //s = string
    //anything else?
    
    
    //table lookup function
    bool check_condition(Algebraic_structure&, char, char);
    
};


#endif
