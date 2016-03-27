#ifndef algebraic_structure_h
#define algebraic_structure_h

#include <vector>
#include <string>
#include <iostream>
#include "condition.h"

class Algebraic_structure {
 private:
  std::vector<char> elements;
  std::string relation;
  std::vector < std::vector<char> > facts;

 public: 
  //constructor
    Algebraic_structure(){};
    Algebraic_structure(std::vector<char> e, std::string r, std::vector < std::vector<char> > f){
        elements = e;
        relation = r;
        facts = f;
        std::cout << "called Alg constructor: " << elements.size() << " elements, relation '" << relation << "' and " << facts.size() << " facts. " << std::endl;
        };
   

    void two_ways_of_analogy_search(){};



  //loops forallforall, forallexists, etc
  //save these values in a lookup table so we don't have to repeat them? What does the table look like?

  


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
  
};




#endif
