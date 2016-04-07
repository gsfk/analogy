
#include <vector>
#include "condition.h"
#include "algebraic_structure.h"

//debug helper fns
void print_elements(std::vector<char> v){
    std::cout << "element vector is: ";
    
    for (int i= 0; i < v.size(); i++){
        std::cout <<"["<< v[i] << "]";
    }
    std::cout << std::endl;
}

void print_facts (std::vector < std::vector<char> > v){
    std::vector<char> current;
    std::cout << "facts vector is: ";
    for (int i = 0; i < v.size(); i++) {
        std::cout << "{";
        current = v[i];
        std::cout << "(" << current[0] << ", " << current[1] << ", " << current[2] << ")";
    }
    std::cout << "}"<< std::endl;
}


void Algebraic_structure::two_ways_of_analogy_search(Condition* c){
    
    std::cout << "\nSTART BRUTE FORCE SEARCH for predicate: " << c->name << std::endl;
    
    
    //check all cases
    //void functions update boolean values associated with this particular predicate
    
    //if this one is true, the rest are either redundant or false
    forall_x_forall_y(c);
    
    //for any of these that are true, the corresponding arrangment without "unique"
    //is also true
    exists_unique_x_exists_y(c);
    exists_x_exists_unique_y(c);
    exists_unique_x_exists_unique_y(c);
    
    forall_x_exists_y(c);
    exists_x_forall_y(c);
    exists_x_exists_y(c);
}


//table lookup
//currently implemented for binary operators only (given x and y, returns a result z)

//but I've tried to write everything with the goal of arbitrary arity, so this shouldn't be difficult 
//to generalize. As a bonus the complexity of C++ vector lookup is faster than I thought (O(1)), so lookup is 
//faster than expected.  

//TODO: generalize to arbitrary arity
char Algebraic_structure::table_lookup(char a, char b){
    std::vector<char> current_fact;
    
    //iterate through fact table
    for(int i = 0; i < facts.size(); i++){
        
        //get fact, which is a vector of size arity
        current_fact = facts[i];
        
        //  std::cout << "table lookup: " << current_fact[0] << ", " << current_fact[1] << ", " << current_fact[2] << std::endl;
        
        //pattern match on a fact (a,b,x), return x
        if (current_fact[0] == a && current_fact[1] == b){
            return current_fact[2];
        }
    }
    
    //if you made it here, fact not found
    //input domain is required to be complete and fully definied,
    //so as long as we ask about elements of the domain, we should always find facts
    //even if the operator does not have the closure property

    //TODO: exception?
    std::cerr << "fact not found, x = " << a <<", y = " << b << std::endl;
    return 0;
}




//CODE FOR EXHAUSTIVE CHECKING OF QUANTIFICATION FOR A GIVEN PREDICATE

/* Checks all cases of the form Q1x Q2y: P(v1, v2, v3)
 where: the Qs are any of the universal, existential or "exists exactly one" quantifiers
 v1, v2, v3 are either x or y
 
 "Algebraic_structure" objects represent the domain
 "Condition" objects represent the predicate
 
 With three quantifiers and two positions, there are nine separate cases for quantification.
 Each case can easily be represented by nested loops; the most straightforward case is
 "for all x for all y" where we iterate through all possible x,y combinations and require
 the predicate to be true in all cases. Other cases are more detailed.
 
 Some of the cases are similar enough that they can be checked simultaneously. So rather than wasteful
 separate checks for "for all x exists y" and "for all x exists a unique y", we can so some small case
 analysis inside the loop and cut down on the number of nested loops we need to run.
 
 */



void Algebraic_structure::forall_x_forall_y(Condition* c){
    char x,y;
    
    std::cout << " ∀x∀y: " << c->name << std::endl;
    std::cout << "domain has " << this->order<< " elements" << std::endl;
    std::cout << " and " << this->facts.size() << " facts" << std::endl;
    
    std::cout << "elements: " << std::endl;
    print_elements(this->elements);
    
    std::cout << "facts:" <<std::endl;
    print_facts(this->facts);
    
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            std::cout<< "checking x = " << x << ", y = " << y << std::endl;
            
            //check condition, quit if false
            if (!c->check_condition(*this, x, y)){
                std::cout << "found false for x = " << x << ", y = " << y << std::endl;
                c->values[FOR_ALL_X_FOR_ALL_Y] = false;
                return;
            }
        }
    }
    //made it all the way without quitting, all conditions true
    c->values[FOR_ALL_X_FOR_ALL_Y] = true;
    std::cout << "for all for all true for " << c->name << std::endl;
}




//handles both "exists y" and "exists unique y"

void Algebraic_structure::forall_x_exists_y(Condition* c){
    char x,y;
    bool exists, exists_unique;
    int unique_counter;
    
    //flag to mark if condition is met only once each time through inner loop
    //set to false the first time false, never re-examine
    exists_unique = true;
    
    //outer loop x
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        
        //flag to make sure we satisfy condition at least once in inner loop
        //update to true when it applies
        exists = false;
        
        //counter for "exists unique" quantifier
        unique_counter = 0;
        
        //inner loop y
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            if (c->check_condition(*this, x, y)){
                exists = true;
                unique_counter ++;
            }
        } //end inner loop
        if (!exists) {
            //found a value of x where no y value meets the condition
            //mark exists_y and exists_unique_y as false, exit
            c->values[FOR_ALL_X_EXISTS_Y] = false;
            c->values[FOR_ALL_X_EXISTS_UNIQUE_Y] = false;
            return;
        }
        //update exists_unique status, if it became false, set to false forever
        else if (exists_unique && unique_counter != 1){
            exists_unique = false;
        }
        
    }//end outer loop
    
    //if we made it here, "for all x, exists y" is met for this condition
    c->values[FOR_ALL_X_EXISTS_Y] = true;
    
    //remove
    // std::cout<< "for all exists found true for " << c->name << std::endl;
    
    //and possibly also "for all x, exists unique y"
    if (exists_unique) {
        c->values[FOR_ALL_X_EXISTS_UNIQUE_Y] = true;
    }
    
}



//handles both "exists x for all y" and "exists unique x for all y"
void Algebraic_structure::exists_x_forall_y(Condition* c){
    int x_count = 0;
    char x,y;
    bool current_x;
    
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        current_x = true;
        
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            // if y ever fails, skip to next x value
            if (!c->check_condition(*this, x, y)){
                current_x = false;
                break;
            }
        } //end inner loop
        
        //completed inner loop. if all ys true, increment counter
        if (current_x){
            x_count++;
        }
    } //end outer loop
    
    //all loops finished, update truth values accordingly
    if (x_count > 0){
        c->values[EXISTS_X_FOR_ALL_Y] = true;
    }
    if (x_count == 1){
        c->values[EXISTS_UNIQUE_X_FOR_ALL_Y] = true;
    }
}






//condition is met at least once

void Algebraic_structure::exists_x_exists_y(Condition* c){
    char x,y;
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            //if condition ever holds, we're done
            if (c->check_condition(*this, x, y)){
                c->values[EXISTS_X_EXISTS_Y] = true;
                return;
            }
        }
    }
}





//condition is true for some y values only for a unique x

void Algebraic_structure::exists_unique_x_exists_y(Condition* c){
    char x,y;
    bool x_found = false;
    bool y_found;
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        y_found = false;
        
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            if (c->check_condition(*this, x, y)){
                y_found = true;
            } //end condition check
            
        } //end y loop
        
        if (y_found){
            //if an x already found, no unique x, set to false and exit
            if (x_found){
                c->values[EXISTS_UNIQUE_X_EXISTS_Y] = false;
                return;
            }
            else {
                x_found = true;}
        }
        
    } //end x loop
    
    //if we made it to the end and x only found once, set to true
    if (x_found){
        c->values[EXISTS_UNIQUE_X_EXISTS_Y] = true;
    }
    
    //else the condition was never met
    else {
        c->values[EXISTS_UNIQUE_X_EXISTS_Y] = false;
    }
}







//there's some x where the condition is met for a single y

void Algebraic_structure::exists_x_exists_unique_y(Condition* c){
    char x,y;
    int y_count;
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        y_count = 0;
        
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            if (c->check_condition(*this, x, y)){
                y_count++;
            }
            
        } // end y loop
        
        //if found a unique y for this x, set value and exit
        if (y_count == 1){
            c->values[EXISTS_X_EXISTS_UNIQUE_Y] = true;
            return;
        }
    } //end x loop
    
    //made it to the end without finding a unique y for any x
    c->values[EXISTS_X_EXISTS_UNIQUE_Y] = false;
    
}



//condition met exactly once
//should be able to sneak this condition in somewhere else

void Algebraic_structure::exists_unique_x_exists_unique_y(Condition* c){
    char x,y;
    bool found_unique_xy = false;
    
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            if (c->check_condition(*this, x, y)){
                //if already found an xy, there's no unique xy, set false and exit
                if (found_unique_xy){
                    c->values[EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y] = false;
                    return;
                } else {
                    found_unique_xy = true;
                }
            }
        } //end y loop
        
    } // end x loop
    
    //made it to the end, xy found either one or zero times
    if (found_unique_xy) {
        c->values[EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y] = true;
        //std::cout << "unique_unique found true for " << c->name <<std::endl;
    } else {
        c->values[EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y] = false;
        //std::cout << "unique_unique found false for " << c->name <<std::endl;
        
    }
}







