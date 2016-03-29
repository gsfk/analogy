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
    
    
    //predicate P(x,y,z) is represented as a function f(x,y,z) -> bool
    //x and y are members of the domain, z is the result of applying the relation represented
    //in the input on x and y (eg: the result of "x * y" for some relation '*' .
    //in practice we are only starting out considering claims with two quantifiers, so
    //the predicate will only have two bound variables (claims such as "x * y = x").
    std::function<int (Algebraic_structure, char, char)> f;
    
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
    //bool check_condition(Algebraic_structure&, char, char);
    
    
    bool check_condition(Algebraic_structure& g, char x, char y){
        return this->f(g,x,y);
    }
    
};





/*Predicates will be roughly of this form:
 
 auto p1 = [](Algebraic_structure a, char x, char y){ return x == y && x == a.table_lookup(x,y); }; // x * x = x
 auto p2 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(x,y) == x; };     // x * y = x
 auto p3 = [](AlAlgebraic_structureg a, char x, char y){ return a.table_lookup(y,x) == x; };     // y * x = x
 
 ... this uses C++ lambda abstractions, which is not strictly necessary, but is nice and concise
 
 While the predicate explicitly mentions the domain ("Algebraic_structure a" in the input) the predicate is not
 fixed to any particular domain, since this is given as input.
 
 
 check_condition() is just a wrapper function around the predicate.
 it takes the domain g, and two values x and y as input, and passes back the true/false results
 
 */






#endif
