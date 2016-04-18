#ifndef condition_h
#define condition_h
#include <map>
#include <vector>
#include "algebraic_structure.h"
#include "prover.h"


enum Quantifiers {
    FOR_ALL_X_FOR_ALL_Y,
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
    
    
public:
    //map from quantifiers to boolean values for this predicate
    //this is filled in by the search code in algebraic_structure.cpp
    std::map<Quantifiers, bool> values;
    
    //human readable predicate name ("x * y = y" more meaningful than "*(x,y,y)")
    //could probably build this as needed instead
    std::string name;
    
    //predicate variables in order
    //this way we can construct predicate names for arbitary arity,
    //and access terms in the prover call, where we have to construct replacements
    //for the uniqueness quantifier
    std::vector<char> ordered_variables;
    
    //relation name (shared with algebraic structure class)
    std::string relation;
    
    //predicate arity
    int arity;
    
    //default constructor
    Condition(){};
    
    //copy constructor
    Condition(const Condition& c) {
        f = c.f;
        name = c.name;
    }
    
    //constructor 
    Condition(std::function<int (Algebraic_structure, char, char)> func, std::string readable_predicate_name,
              std::vector<char> v, std::string r){
        f = func;
        name = readable_predicate_name;
        //prover_name = predicate_name;
        ordered_variables = v;
        relation = r;
    };
   

    
    //table lookup function
    bool check_condition(Algebraic_structure&, char, char);
    
    //print all bool values to console
    void print_map() const;

    //construct predicate name for prover output
    std::string prover_name();
    
    //generate outcomes in prover syntax
    std::string prover_print();
    
    //helper fns
//    int num_x(Condition);
//    int num_y(Condition);

};


//-- Static functions --//

//get all arity 3 predicates
std::vector<Condition*> return_all_predicates(std::string);

//generate all true formulas for prover output
std::vector<Formula>& generate_formulas(std::vector<Condition*> predicates, Algebraic_structure);

//get readable names for quantifiers
std::pair<std::string, std::string> quantifier_names(Quantifiers);






/*Predicates will be roughly of this form:
 
 auto p1 = [](Algebraic_structure a, char x, char y){ return x == y && x == a.table_lookup(x,y); }; // x * x = x
 auto p2 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(x,y) == x; };     // x * y = x
 auto p3 = [](AlAlgebraic_structureg a, char x, char y){ return a.table_lookup(y,x) == x; };     // y * x = x
 
 ... this uses C++ lambda abstractions, which is not strictly necessary, but they're nice and concise
 
 While the predicate explicitly mentions the domain ("Algebraic_structure a" in the input) the predicate is not
 fixed to any particular domain, since this is given as input.
 
 
 check_condition() is just a wrapper function around the predicate.
 it takes the domain g, and two values x and y as input, and passes back the true/false results
 
 */




#endif
