
#include <vector>
#include "condition.h"
#include "algebraic_structure.h"


void two_ways_of_analogy_search(Condition c){
    //call all cases for this condition


    
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


//.... doesn't make a differenc to complexity





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



