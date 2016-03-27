
#include <vector>
#include "condition.h"
#include "algebraic_structure.h"


void Algebraic_structure::two_ways_of_analogy_search(Condition c){
    //call all cases for this condition
    //update condition bool map here
    
    
    /*
     if ∀∀ true, skip ∀∃ and ∃∀
     if ∀∃! true, skip ∀∃
     if ∃!∀ true, skip ∃∀
     if ∃!∃! true, skip ∃∃
     
     */
    
    
}



void Algebraic_structure::forall_x_forall_y(Condition c){
    char x,y;
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        for (int j = 0; i < this->order; i++){
            y = this->elements[j];
            
            //check condition, quit if false
            if (!c.check_condition(x, y)){
                c.values[FOR_ALL_X_FOR_ALL_Y] = false;
                return;
            }
        }
    }
    
    //made it all the way without quitting, all conditions true
    c.values[FOR_ALL_X_FOR_ALL_Y] = true;
}



//handles both exists y and exists unique y
//harder case analysis but eliminates redundant checking

void Algebraic_structure::forall_x_exists_y(Condition c){
    char x,y;
    bool exists, exists_unique;
    int unique_counter = 0;
    
    //flag to mark if condition is met only once each time through inner loop
    //set to false the first time false, never re-examine
    exists_unique = true;
    
    //outer loop x
    for (int i = 0; i < this->order; i++){
        x = this->elements[i];
        
        //flag to make sure we satisfy condition at least once in inner loop
        //update to true when it applies
        exists = false;
        
        //inner loop y
        for (int j = 0; i < this->order; i++){
            y = this->elements[j];
            if (c.check_condition(x, y)){
                exists = true;
                unique_counter ++;
            }
        } //end inner loop
        if (!exists) {
            //found a value of x where no y value meets the condition
            //mark exists_y and exists_unique_y as false, return
            c.values[FOR_ALL_X_EXISTS_Y] = false;
            c.values[FOR_ALL_X_EXISTS_UNIQUE_Y] = false;
            return;
        }
        //update exists_unique status, if it became false, set to false forever
        else if (exists_unique && unique_counter != 1){
            exists_unique = false;
        }
        
    }//end outer loop

    //if we made it here, "for all x, exists y" is met for this condition
    c.values[FOR_ALL_X_EXISTS_Y] = true;
    
    //and possibly also "for all x, exists unique y"
    if (exists_unique) {
        c.values[FOR_ALL_X_EXISTS_UNIQUE_Y] = true;
    }
    
}




//at centre of loop, call condition.check_condition(Quantifiers);


//CAN EITHER: iterate through conditions, and check each one in turn
//or, at the centre of each loop, iterate through all the conditions.

// first one is probably more generalizable?


//for all conditions:
    //check quantifier arrangements for this condition

        //OR

//for all quantifier arrangements:
    //check all conditions


//.... doesn't make a difference to complexity





/* forall_x_forall_y() */

/* forall_x_exists_y(); */
/* forall_x_exists!_y(); //handle in forallexists */

/* exists_x_forall_y(); */
/* exists_x_exists_y(); */
/* exists_x_exists!_y(); //handle in existsexists */

/* exists!_x_forall_y(); //handle in existsforall? */
/* exists!_x_exists_y(); //handle in existsexists? */
/* exists!_x_exists!_y(); //handle in existsexists? */

/* check_all_cases(); //call all cases, fill Quantifier lookup table */
/* //lookup table only applies to a particular condition */



