#include <iostream>
#include <sstream>
#include <vector>
#include "condition.h"
#include "algebraic_structure.h"


/*
  check_condition() is just a wrapper function around the predicate.
 it takes the domain g, and two values x and y as input, and passes back the true/false results
 
 */
bool Condition::check_condition(Algebraic_structure& g, char x, char y){
    return this->f(g,x,y);
}


//function-style prover name, assumes arity 3
std::string Condition::prover_name(){
    std::string s = "";
    s += '(';
    s += ordered_variables[0];
    s += " ";
    s += relation;
    s += " ";
    s += ordered_variables[1];
    s += " = ";
    s += ordered_variables[2];
    s += ')';
    return s;
}


//number of x variables in condition
int num_x(Condition *p){
    int x_count = 0;
    for (int i = 0; i < p->ordered_variables.size(); i++){
        if (p->ordered_variables[i] == 'x'){
            x_count++;
        }
    }
    return x_count;
}

//number of y variables in condition
int num_y(Condition *p){
    int y_count = 0;
    for (int i =0; i < p->ordered_variables.size(); i++){
        if (p->ordered_variables[i] == 'y'){
            y_count++;
        }
    }
    return y_count;
}


//distinguish uniqueness quantifier cases
bool not_unique(Quantifiers q){
    bool result;
    switch(int(q)){
        case FOR_ALL_X_FOR_ALL_Y:
        case FOR_ALL_X_EXISTS_Y:
        case EXISTS_X_FOR_ALL_Y:
        case EXISTS_X_EXISTS_Y:
            result = true;
            break;
        case FOR_ALL_X_EXISTS_UNIQUE_Y:  
        case EXISTS_X_EXISTS_UNIQUE_Y:
        case EXISTS_UNIQUE_X_FOR_ALL_Y:
        case EXISTS_UNIQUE_X_EXISTS_Y:
        case EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y:
            result  = false;
            break;
        default:
            std::cerr << "quantifier read error" << std::endl;
            return false;
    }
    return result;
}



std::pair<std::string, std::string> quantifier_names(Quantifiers q){
    std::pair<std::string, std::string> names;
    switch(int(q)){
        case FOR_ALL_X_FOR_ALL_Y:
            names.first = "∀x ∀y ";
            names.second = "all x all y";
            break;
            
        case FOR_ALL_X_EXISTS_Y:
            names.first = "∀x ∃y ";
            names.second = "all x exists y";
            break;
            
        case FOR_ALL_X_EXISTS_UNIQUE_Y:
            names.first = "∀x ∃!y ";
            names.second = "";
            break;
            
        case EXISTS_X_FOR_ALL_Y:
            names.first = "∃x ∀y ";
            names.second = "exists x all y";
            break;
            
        case EXISTS_X_EXISTS_Y:
            names.first = "∃x ∃y ";
            names.second = "exists x exists y";
            break;
            
            //no simple prover quantifier name for this and following quantifiers
            //requires substitution code
        case EXISTS_X_EXISTS_UNIQUE_Y:
            names.first = "∃x ∃!y ";
            names.second = "";
            break;
            
        case EXISTS_UNIQUE_X_FOR_ALL_Y:
            names.first = "∃!x ∀y ";
            names.second = "";
            break;
            
        case EXISTS_UNIQUE_X_EXISTS_Y:
            names.first = "∃!x ∃y ";
            names.second = "";
            break;
            
        case EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y:
            names.first = "∃!x ∃!y ";
            names.second = "";
            break;
        default:
            ; //do nothing
    }
    return names;
}





// ---- Static functions ---- //

/*
 
 Return all possible predicates of the form "v1 * v2 = v3", where any of the vs can be x or y.
 
 This is the most brute-force-by-hand code in the project, look away!
 There are several permutations that look redundant to each other, such as "x * x = y" and "y * y = x",
 but we always work with a fixed quantifier order (x first, then y), so redundancy is not as straightforward
 as it appears. The only clearly redudant cases for our purposes are "x * x = x" and "y * y = y", so only one
 of those appears. Any other redundacies are left to the prover to eliminate.
 
 */

std::vector<Condition*> return_all_predicates(std::string relation){
    std::vector<Condition*>* v  = new std::vector<Condition*>;
    std::string readable_predicate_name;
    std::vector<char> ordered_variables;
    
    auto p1 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(x,x) == x; };  // x * x = x
    readable_predicate_name = "x ";
    readable_predicate_name += relation;
    readable_predicate_name += " x = x";
    
    ordered_variables.push_back('x');
    ordered_variables.push_back('x');
    ordered_variables.push_back('x');
    Condition *c1 = new Condition(p1, readable_predicate_name, ordered_variables, relation);
    v->push_back(c1);
    ordered_variables.clear();
    
    auto p2 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(x,x) == y; };  // x * x = y
    readable_predicate_name = "x ";
    readable_predicate_name += relation;
    readable_predicate_name += " x = y";

    ordered_variables.push_back('x');
    ordered_variables.push_back('x');
    ordered_variables.push_back('y');
    Condition *c2 = new Condition(p2, readable_predicate_name, ordered_variables, relation);
    v->push_back(c2);
    ordered_variables.clear();
    
    auto p3 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(x,y) == x; };  // x * y = x
    readable_predicate_name = "x ";
    readable_predicate_name += relation;
    readable_predicate_name += " y = x";
    ordered_variables.push_back('x');
    ordered_variables.push_back('y');
    ordered_variables.push_back('x');
    Condition *c3 = new Condition(p3, readable_predicate_name, ordered_variables, relation);
    v->push_back(c3);
    ordered_variables.clear();
    
    auto p4 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(x,y) == y; };  // x * y = y
    readable_predicate_name = "x ";
    readable_predicate_name += relation;
    readable_predicate_name += " y = y";
    ordered_variables.push_back('x');
    ordered_variables.push_back('y');
    ordered_variables.push_back('y');
    Condition *c4 = new Condition(p4, readable_predicate_name, ordered_variables, relation);
    v->push_back(c4);
    ordered_variables.clear();
    
    auto p5 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(y,x) == x; };  // y * x = x
    readable_predicate_name = "y ";
    readable_predicate_name += relation;
    readable_predicate_name += " x = x";
    ordered_variables.push_back('y');
    ordered_variables.push_back('x');
    ordered_variables.push_back('x');
    Condition *c5 = new Condition(p5, readable_predicate_name, ordered_variables, relation);
    v->push_back(c5);
    ordered_variables.clear();
    
    auto p6 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(y,x) == y; };  // y * x = y
    readable_predicate_name = "y ";
    readable_predicate_name += relation;
    readable_predicate_name += " x = y";
    ordered_variables.push_back('y');
    ordered_variables.push_back('x');
    ordered_variables.push_back('y');
    Condition *c6 = new Condition(p6, readable_predicate_name, ordered_variables, relation);
    v->push_back(c6);
    ordered_variables.clear();
    
    auto p7 = [](Algebraic_structure a, char x, char y){ return a.table_lookup(y,y) == x; };  // y * y = x
    readable_predicate_name = "y ";
    readable_predicate_name += relation;
    readable_predicate_name += " y = x";
    ordered_variables.push_back('y');
    ordered_variables.push_back('y');
    ordered_variables.push_back('x');
    Condition *c7 = new Condition(p7, readable_predicate_name, ordered_variables, relation);
    v->push_back(c7);
    
    return *v;
}




/*
 
 Build all true formulas for this domain.
 
 Formulas are proper first order logic formulas, ie, with quantification and predicates together.
 Previously predicates and quantifiers were packaged separately, in order to check all combinations.
 The Formula class is defined in prover.h, and is largely based on string representations, since
 its main use is for output to users and the prover.
 
 Formulas have both a "pretty" representation for end users and a less readable version intended for the prover.
 
 For simple cases: existential or universal quantifiers with a single predicate, these are simply concatenated
 together and stored as a string.
 
 For formulas that include a uniqueness quantifier, the pretty version concatenates them together, while 
 the prover version involves detailed substitution code that produces an expression using only 
 universal and existential quantifiers.
 
 For "extended property" formulas such as associativity, these are given their standard function-style 
 representation (eg: "∀x ∀y ∃z x * y = z"), using the appropriate relation name.
 
 
 */

std::vector<Formula>& generate_formulas(std::vector<Condition*> predicates, Algebraic_structure g){
    std::vector<Formula> *formulas = new std::vector<Formula>;
    Formula current_formula;
    
    std::stringstream pretty;
    std::stringstream prover;
    
    //readable quantifier names, pretty one first
    std::pair<std::string, std::string> names;
    
    //loop through all conditions
    for(auto &p : predicates){
        
        //inner loop through all quantification cases
        for (auto &f : p->values){
            
            //continue if this formula is true, else skip it
            if (f.second){
                
                //handle simple cases without uniqueness quantifier
                if (not_unique(f.first)){
                    
                    //build pretty expression  ("∃x ∀y x * x = x")
                    names = quantifier_names(f.first);
                    current_formula.pretty_name = names.first;
                    current_formula.pretty_name += p->name;
                    
                    //build uglier expression ("exists x all y *(x,x,x)")
                    current_formula.prover_name = names.second;
                    current_formula.prover_name += " ";
                    current_formula.prover_name += p->prover_name();
                    
                    current_formula.subsumption_include = true;
                    formulas->push_back(current_formula);
                    
                                    }
                /*
                 
                 Substitution code for uniqueness quantifiers
                 
                 A simple pairing of quantifiers and predicate isn't sufficient here, since the prover won't
                 accept the uniqueness quantifier. So we need to substitute each formula containing a uniqueness
                 quantifier with one containing only regular exisitential and universal quantifiers.
                 
                 A simple case-by-case rewriting isn't practical here, since the order of variables in the
                 predicate matters.
                 
                 Example:
                 
                    "∀x ∃!y P(x, x, y)"
                 
                        becomes:
                 
                    ∀x ∃y (P(x,x,y) & ∀z(P(x,x,z) -> (x=z)))
                 
                 So we need more generalizable substitution code.
                 */
                else {
                    std::stringstream ss;
                    
                    //pretty name
                    names = quantifier_names(f.first);
                    current_formula.pretty_name = names.first;
                    current_formula.pretty_name += p->name;
                    
                    //build expression readable by prover
                    char a,b,c;
                    a = p->ordered_variables[0];
                    b = p->ordered_variables[1];
                    c = p->ordered_variables[2];
                    
                    switch(f.first){
                        case FOR_ALL_X_EXISTS_UNIQUE_Y:
                            {

                                //number of appearances of 'y' in the predicate
                                int num_unique_values;
                                num_unique_values= num_y(p);
                                
                                //number of conjunctions we need to print between terms
                                int conjunction = num_unique_values - 1;
                                
                                //counter of terms printed
                                int terms_printed = 0;
                                
                                ss << "all x exists y (";
                                ss << p->prover_name();
                                ss << " & all z (";
                                
                                //print subterms
                                if (a == 'y'){
                                    ss << "((z " << p->relation << " " << b << " = " << c << ") -> z = y)";
                                    terms_printed++;
                                }
                                if (terms_printed && conjunction) {
                                    ss << " & ";
                                    conjunction --;
                                }
                                if (b=='y'){
                                    ss << "((" << a << " " << p->relation << " z " << " = " << c << ") -> z = y)";
                                }
                                if (terms_printed && conjunction) {
                                    ss << " & ";
                                    conjunction --;
                                }
                                if (c == 'y'){
                                    ss << "((" << a << " " << p->relation << " " << b << "= z) -> z = y)";
                                    terms_printed++;
                                }
                                ss << "))";
                                
                                current_formula.subsumption_include = true;
                                current_formula.prover_name = ss.str();
                                formulas->push_back(current_formula);
                                
                                //blank stringstream
                                ss.str("");  //clear characters
                                ss.clear();  //clear bits
                                break;
                            }

                        case EXISTS_X_EXISTS_UNIQUE_Y:
                        {
                            //number of appearances of 'y' in the predicate
                            int num_unique_values;
                            num_unique_values= num_y(p);
                            
                            //number of conjunctions we need to print between terms
                            int conjunction = num_unique_values - 1;
                            
                            //counter of terms printed
                            int terms_printed = 0;
                            
                            ss << "exists x exists y (";
                            ss << p->prover_name();
                            ss << " & all z (";
                            
                            //print subterms
                            if (a == 'y'){
                                ss << "((z " << p->relation << " " << b << " = " << c << ") -> z = y)";
                                terms_printed++;
                            }
                            if (terms_printed && conjunction) {
                                ss << " & ";
                                conjunction --;
                            }
                            if (b== 'y'){
                                ss << "((" << a << " " << p->relation << " z " << " = " << c << ") -> z = y)";
                                terms_printed++;
                            }
                            if (terms_printed && conjunction) {
                                ss << " & ";
                                conjunction --;
                            }
                            if (c == 'y'){
                                ss << "((" << a << " " << p->relation << " " << b << " = z) -> z = y)";
                            }
                            ss << "))";
                            
                            current_formula.subsumption_include = true;
                            current_formula.prover_name = ss.str();
                            formulas->push_back(current_formula);
                            
                            //blank stringstream
                            ss.str("");  //clear characters
                            ss.clear();  //clear bits
                            break;
                        }
                            
                        case EXISTS_UNIQUE_X_FOR_ALL_Y:
                        {
                            //number of appearances of 'x' in the predicate
                            int num_unique_values;
                            num_unique_values= num_x(p);
                            
                            //number of conjunctions we need to print between terms
                            int conjunction = num_unique_values - 1;
                            
                            //counter of terms printed
                            int terms_printed = 0;
                            
                            ss << "exists x all y (";
                            ss << p->prover_name();
                            ss << " & all z (";
                            
                            //print subterms
                            if (a == 'x'){
                                ss << "((z " << p->relation << " " << b << " = " << c << ") -> z = x)";
                                terms_printed++;
                            }
                            if (terms_printed && conjunction) {
                                ss << " & ";
                                conjunction --;
                            }
                            if (b=='x'){
                                ss << "((" << a << " " << p->relation << " z " << " = " << c << ") -> z = x)";
                                terms_printed++;
                            }
                            if (terms_printed && conjunction) {
                                ss << " & ";
                                conjunction --;
                            }
                            if (c == 'x'){
                                ss << "((" << a << " " << p->relation << " " << b << " = z) -> z = x)";
                            }
                            ss << "))";
                            
                            current_formula.subsumption_include = true;
                            current_formula.prover_name = ss.str();
                            formulas->push_back(current_formula);
                            
                            //blank stringstream
                            ss.str("");  //clear characters
                            ss.clear();  //clear bits
                            break;
                        }

                        case EXISTS_UNIQUE_X_EXISTS_Y:
                        {
                            //number of appearances of 'x' in the predicate
                            int num_unique_values;
                            num_unique_values= num_x(p);
                            
                            //number of conjunctions we need to print between terms
                            int conjunction = num_unique_values - 1;
                            
                            //counter of terms printed
                            int terms_printed = 0;

                            ss << "exists x exists y (";
                            ss << p->prover_name();
                            ss << " & all z (";
                            
                            //print subterms
                            if (a == 'x'){
                                ss << "((z " << p->relation << " " << b << " = " << c << ") -> z = x)";
                                terms_printed++;
                            }
                            if (terms_printed && conjunction) {
                                ss << " & ";
                                conjunction --;
                            }
                            if (b=='x'){
                                ss << "((" << a << " " << p->relation << " z " << " = " << c << ") -> z = x)";
                                terms_printed++;
                            }
                            if (terms_printed && conjunction) {
                                ss << " & ";
                                conjunction --;
                            }
                            if (c == 'x'){
                                ss << "((" << a << " " << p->relation << " " << b << "= z) -> z = x)";
                            }
                            ss << "))";
                            
                            current_formula.subsumption_include = true;
                            current_formula.prover_name = ss.str();
                            formulas->push_back(current_formula);
                            
                            //blank stringstream
                            ss.str("");  //clear characters
                            ss.clear();  //clear bits
                            break;
                        }
                        
                        case EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y: {
                            
                            //different approach
                            //don't care about numbers, only a,b,c vals
                            
                            ss << "exists x exists y (" << p->prover_name();
                            ss << " & all u all v all w((";
                            ss << "u " << p->relation << " v = w) -> (u = " << a << " & v = " << b << " & w = " << c << ")))";
                            
                            current_formula.subsumption_include = true;
                            current_formula.prover_name = ss.str();
                            formulas->push_back(current_formula);
                            
                            //blank stringstream
                            ss.str("");  //clear characters
                            ss.clear();  //clear bits
                          break;
                    }

                        default:
                            ;//do nothing
                    
                    }
               }// end unique cases
            }
        }// end loop through quantifiers
        
    }//end loop through all conditions
    
    
    
    //begin code for extended properties:
    for (auto ep : g.extra_properties){
        if (ep.second){             //if formula is true
            switch (ep.first){
                case ASSOCIATIVITY:
                    //build pretty name
                    pretty << "∀x ∀y ∀z ((x " << g.relation << " y) " << g.relation << " z = x " << g.relation << " (y " << g.relation << " z)) (associativity)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover name
                    prover << "all x all y all z ((x " << g.relation << " y) " << g.relation << " z = x " << g.relation << " (y " << g.relation << " z))";
                    current_formula.prover_name = prover.str();
                    break;
                    
                case COMMUTATIVITY:
                    //build pretty
                    pretty << "∀x ∀y ((x " << g.relation << " y) = (y " << g.relation << " x)) (commutativity)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover
                    prover << "all x all y ((x " << g.relation << " y) = (y " << g.relation << " x))";
                    current_formula.prover_name = prover.str();
                    break;
                    
                case CLOSURE:
                    
                    //build pretty
                    pretty << "∀x ∀y ∃z (x " << g.relation << " y = z) (closure)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover
                    prover << "all x all y exists z (x " << g.relation << " y = z)";
                    current_formula.prover_name = prover.str();
                    break;
                    
                case LEFT_IDENTITY_LEFT_INVERSE:
                    
                    //build pretty
                    pretty << "∃x ∀y ∃z ((x " << g.relation << " y = y) & (z " << g.relation << " y = x)) (left identity, left inverse)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover
                    prover << "exists x all y exists z ((x " << g.relation << " y = y) & (z " << g.relation << " y = x))";
                    current_formula.prover_name = prover.str();
                    break;
                    
                case RIGHT_IDENTITY_LEFT_INVERSE:
                    
                    pretty << "∃x ∀y ∃z ((y " << g.relation << " x = y) & (z " << g.relation << " y = x)) (right identity, left inverse)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover
                    prover << "exists x all y exists z ((y " << g.relation << " x = y) & (z " << g.relation << " y = x))";
                    current_formula.prover_name = prover.str();
                    break;
                    
                case LEFT_IDENTITY_RIGHT_INVERSE:
                    
                    //build pretty
                    pretty << "∃x ∀y ∃z ((x " << g.relation << " y = y) & (y " << g.relation << " z = x)) (left identity, right inverse)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover
                    prover << "exists x all y exists z ((x " << g.relation << " y = y) & (y " << g.relation << " z = x))";
                    current_formula.prover_name = prover.str();
                    break;
                    
                case RIGHT_IDENTITY_RIGHT_INVERSE:
                    
                    //build pretty
                    pretty << "∃x ∀y ∃z ((y " << g.relation << " x = y) & (y " << g.relation << " z = x)) (right identity, right inverse)";
                    current_formula.pretty_name = pretty.str();
                    
                    //build prover
                    prover << "exists x all y exists z ((y " << g.relation << " x = y) & (y " << g.relation << " z = x))";
                    current_formula.prover_name = prover.str();
                    break;
                    
                default:
                    ;//do nothing
            }
            
            //add to set of formulas
            current_formula.subsumption_include = true;
            formulas->push_back(current_formula);
            
            //clear stringstreams
            pretty.str("");  //clear characters
            pretty.clear();  //clear bits
            prover.str("");
            prover.clear();
        }
    }
    
    return *formulas;
}







// --- console print functions for debugging --- //




//print map of truth values for quantifier / predicate combinations
void Condition::print_map() const {
    std::cout << std::boolalpha;    //print bools as "true" or "false"
    for (auto &a : values){
        switch(int(a.first)){
            case FOR_ALL_X_FOR_ALL_Y:
                std::cout << "∀x ∀y: " << this->name << "   " << a.second << "\n";
                break;
            case FOR_ALL_X_EXISTS_Y:
                std::cout << "∀x ∃y: " << this->name << "   " << a.second << "\n";
                break;
                
            case FOR_ALL_X_EXISTS_UNIQUE_Y:
                std::cout << "∀x ∃!y: " << this->name << "   " << a.second << "\n";
                break;
                
            case EXISTS_X_FOR_ALL_Y:
                std::cout << "∃x ∀y: " << this->name << "   " << a.second << "\n";
                break;
                
            case EXISTS_X_EXISTS_Y:
                std::cout << "∃x ∃y: " << this->name << "   " << a.second << "\n";
                break;
                
            case EXISTS_X_EXISTS_UNIQUE_Y:
                std::cout << "∃x ∃!y: " << this->name << "   " << a.second << "\n";
                break;
                
            case EXISTS_UNIQUE_X_FOR_ALL_Y:
                std::cout << "∃!x ∀y: " << this->name << "   " << a.second << "\n";
                break;
                
            case EXISTS_UNIQUE_X_EXISTS_Y:
                std::cout << "∃!x ∃y: " << this->name << "   " << a.second << "\n";
                break;
                
            case EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y:
                std::cout << "∃!x ∃!y: " << this->name << "   " << a.second << "\n";
                break;
            default:
                //do nothing
                ;
        }
    }
}






