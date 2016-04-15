#include <vector>
#include "condition.h"
#include "algebraic_structure.h"

//debug print fns
void print_elements(std::vector<char>);
void print_facts (std::vector < std::vector<char> >);


/*
 Fact table lookup
 currently works for binary operators only (facts with three elements)
 
 However it's written in a general style that would not be difficult to expand to arbitrary
 arity: it simply iterates through facts looking for a match, rather than using a faster
 implementation that would be specific to a particular arity, the most obvious example being
 a 2D array akin to a multiplication table for binary operators.
 
 Complexity is not terrible, though, each lookup is linear in the size of the fact table (number of facts * arity)
 */
char Algebraic_structure::table_lookup(char a, char b){
    std::vector<char> current_fact;
    
    //iterate through fact table
    for(int i = 0; i < facts.size(); i++){
        
        //get fact, which is a vector of size arity
        current_fact = facts[i];
        
        //pattern match on a fact (a,b,x), return x
        if (current_fact[0] == a && current_fact[1] == b){
            return current_fact[2];
        }
    }
    
    //if you made it here, fact not found
    //input domain is required to be complete and fully definied,
    //so as long as we ask about elements of the domain, we should always find facts
    
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



//main brute force search function, calls all cases.
//void function, updates boolean values associated with the condition given as input
void Algebraic_structure::two_ways_of_analogy_search(Condition* c){
    forall_x_forall_y(c);
    exists_unique_x_exists_y(c);
    exists_x_exists_unique_y(c);
    exists_unique_x_exists_unique_y(c);
    forall_x_exists_y(c);
    exists_x_forall_y(c);
    exists_x_exists_y(c);
}


//search for extra properties not expressible with only two quantifiers
void Algebraic_structure::extra_properties_search(){
    commutativity();
    associativity();
    closure();
    left_identity_left_inverse();
    left_identity_right_inverse();
    right_identity_left_inverse();
    right_identity_right_inverse();
}







void Algebraic_structure::forall_x_forall_y(Condition* c){
    char x,y;
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            //check condition, quit if false
            if (!c->check_condition(*this, x, y)){
                c->values[FOR_ALL_X_FOR_ALL_Y] = false;
                return;
            }
        }
    }
    //made it all the way without quitting, all conditions true
    c->values[FOR_ALL_X_FOR_ALL_Y] = true;
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
        
        //inner loop counter for "exists unique" quantifier
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
    } else {
        c->values[EXISTS_UNIQUE_X_EXISTS_UNIQUE_Y] = false;
    }
}

//end of double quantifier cases



//interesting triple quantifier cases:

void Algebraic_structure::commutativity(){
    char x,y;
    bool commutes = false;
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        for (int j = 0; j < this->order; j++){
            y = this->elements[j];
            
            //check commutativity, quit if false
            commutes = (table_lookup(x,y) == table_lookup(y,x));
            if (!commutes){
                this->extra_properties[COMMUTATIVITY] = false;
                return;
            }
        }
    }
    //else true
    this->extra_properties[COMMUTATIVITY] = true;
}

void Algebraic_structure::associativity(){
    char x,y,z, left, right;
    bool associative = false;
    
    //triple nested loop, check x(yz) = (xy)z for all x,y,z
    for (int i = 0; i<this->order; i++ ){
        x = this->elements[i];
        for (int j=0; j< this->order; j++){
            y = this->elements[j];
            for (int k = 0; k< this->order; k++){
                z = this->elements[k];
                left = table_lookup(x, table_lookup(y, z));     //x(yz)
                right = table_lookup(table_lookup(x,y),z);      //(xy)z
                associative = (left == right);
                
                if(!associative){
                    this->extra_properties[ASSOCIATIVITY] = false;
                    return;
                }
            } //end z
        }//end y
    }// end x
    
    //else true
    this->extra_properties[ASSOCIATIVITY] = true;
}

//check that x * y results in one of the member elements
void Algebraic_structure::closure(){
    char x,y,z, element;
    bool found_element;
    for (int i = 0; i<this->order; i++ ){
        x = this->elements[i];
        for (int j=0; j< this->order; j++){
            y = this->elements[j];
            
            //z = x * y
            z = table_lookup(x,y);
            found_element = false;
            
            //check membership of z
            for (int k= 0; k < this->order; k++){
                element = this->elements[k];
                if (z == element){
                    found_element = true;
                    break;
                }
            }//end z
            
            //if z not found in elements for this x*y
            if (found_element == false){
                this->extra_properties[CLOSURE] = false;
                return;
            }
        }//end y
        
    }//end x
    this->extra_properties[CLOSURE] = true;
}



//existence of identity elements is already checked by the two quantifier case
//these are simple helper functions to find which particular element is identity
//needed for finding inverses

bool Algebraic_structure::is_left_identity_element(char iden){
    char y;
    for (int i= 0; i < this->order; i++){
        y = this->elements[i];
        
        //if you find a non-identity case, return false
        if (!(table_lookup(iden,y) == y)){
            return false;
        }
    }
    //else return true
    return true;
}

bool Algebraic_structure::is_right_identity_element(char iden){
    char y;
    for (int i= 0; i < this->order; i++){
        y = this->elements[i];
        
        //if you find a non-identity case, return false
        if (!(table_lookup(y,iden) == y)){
            return false;
        }
    }
    //else return true
    return true;
}



//existence of left inverse: ∃x ∀y ∃z (x is identity & z * y = x)
void Algebraic_structure::left_identity_left_inverse(){
    char x,y,z;
    char left_iden = '0'; //dummy initialization
    bool found_left_identity = false;
    bool is_inverse = false;
    int y_counter = 0;
    
    //is x an identity element?
    for (int i = 0; i<this->order; i++ ){
        x = this->elements[i];
        found_left_identity = is_left_identity_element(x);
        
        if (found_left_identity){
            left_iden = x;
        }
    }
    
    //quit if you never find an identity element
    if (!found_left_identity){
        this->extra_properties[LEFT_IDENTITY_LEFT_INVERSE] = false;
        return;
    }
    
    
    for (int j=0; j< this->order; j++){
        y = this->elements[j];
        
        for (int k=0; k< this->order; k++){
            z = this->elements[k];
            
            is_inverse = (table_lookup(z,y) == left_iden);
            
            //if you find an inverse for this y, increment counter and move to the next one
            if (is_inverse){
                y_counter++;
                break;
            }
            
        }//end z
    }//end y
    
    //true if some inverse found for all y
    if (y_counter == this->order){
        this->extra_properties[LEFT_IDENTITY_LEFT_INVERSE] = true;
        
    } else {
        this->extra_properties[LEFT_IDENTITY_LEFT_INVERSE] = false;
        
    }
    
}



//existence of left inverse: ∃x ∀y ∃z (x is identity & y * z = x)
void Algebraic_structure::left_identity_right_inverse(){
    char x,y,z;
    char left_iden = '0'; //dummy initialization
    bool found_left_identity = false;
    bool is_inverse = false;
    int y_counter = 0;
    
    //is x an identity element?
    for (int i = 0; i<this->order; i++ ){
        x = this->elements[i];
        found_left_identity = is_left_identity_element(x);
        if (found_left_identity){
            left_iden = x;
        }
    }
    
    //quit if you never find an identity element
    if (!found_left_identity){
        this->extra_properties[LEFT_IDENTITY_RIGHT_INVERSE] = false;
        return;
    }
    
    for (int j=0; j< this->order; j++){
        y = this->elements[j];
        
        for (int k=0; k< this->order; k++){
            z = this->elements[k];
            
            is_inverse = (table_lookup(y, z) == left_iden);
            
            //if you find an inverse for this y, increment counter and move to the next one
            if (is_inverse){
                y_counter++;
                break;
            }
        }//end z
        
    }//end y
    
    //true if some inverse found for all y
    if (y_counter == this->order){
        this->extra_properties[LEFT_IDENTITY_RIGHT_INVERSE] = true;
        
    } else {
        this->extra_properties[LEFT_IDENTITY_RIGHT_INVERSE] = false;
        
    }
    
}




//existence of left inverse: ∃x ∀y ∃z (x is identity & z * y = x)
void Algebraic_structure::right_identity_left_inverse(){
    char x,y,z;
    char right_iden = '0'; //dummy initialization
    bool found_right_identity = false;
    bool is_inverse = false;
    int y_counter = 0;
    
    //is x an identity element?
    for (int i = 0; i<this->order; i++ ){
        x = this->elements[i];
        found_right_identity = is_right_identity_element(x);
        if (found_right_identity){
            right_iden = x;
        }
    }
    
    //quit if you never find an identity element
    if (!found_right_identity){
        this->extra_properties[RIGHT_IDENTITY_LEFT_INVERSE] = false;
        return;
    }
    
    
    for (int j=0; j< this->order; j++){
        y = this->elements[j];
        
        for (int k=0; k< this->order; k++){
            z = this->elements[k];
            
            is_inverse = (table_lookup(z,y) == right_iden);
            
            //if you find an inverse for this y, increment counter and move to the next one
            if (is_inverse){
                y_counter++;
                break;
            }
            
        }//end z
    }//end y
    
    //true if some inverse found for all y
    if (y_counter == this->order){
        this->extra_properties[RIGHT_IDENTITY_LEFT_INVERSE] = true;
        
    } else {
        this->extra_properties[RIGHT_IDENTITY_LEFT_INVERSE] = false;
        
    }
    
}



//existence of left inverse: ∃x ∀y ∃z (x is identity & y * z = x)
void Algebraic_structure::right_identity_right_inverse(){
    char x,y,z;
    char right_iden = '0'; //dummy initialization
    bool found_right_identity = false;
    bool is_inverse = false;
    int y_counter = 0;
    
    //is x an identity element?
    for (int i = 0; i<this->order; i++ ){
        x = this->elements[i];
        found_right_identity = is_left_identity_element(x);
        if (found_right_identity){
            right_iden = x;
        }
    }
    
    //quit if you never find an identity element
    if (!found_right_identity){
        this->extra_properties[RIGHT_IDENTITY_RIGHT_INVERSE] = false;
        return;
    }
    
    for (int j=0; j< this->order; j++){
        y = this->elements[j];
        y_counter = 0;
        
        for (int k=0; k< this->order; k++){
            z = this->elements[k];
            
            is_inverse = (table_lookup(y, z) == right_iden);
                        
            //if you find an inverse for this y, increment counter and move to the next one
            if (is_inverse){
                y_counter++;
                break;
            }
        }//end z
        
    }//end y
    
    //true if some inverse found for all y
    if (y_counter == this->order){
        this->extra_properties[RIGHT_IDENTITY_RIGHT_INVERSE] = true;
        
    } else {
        this->extra_properties[RIGHT_IDENTITY_RIGHT_INVERSE] = false;
        
    }
    
}
//end looping code








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
    std::cout << "facts vector is: {";
    for (int i = 0; i < v.size(); i++) {
        current = v[i];
        std::cout << "(" << current[0] << ", " << current[1] << ", " << current[2] << ")";
    }
    std::cout << "}"<< std::endl;
}



