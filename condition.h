#ifndef condition_h
#define condition_h

#include <map>

class Condition {
 private:

 public: 
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
    std::map<Quantifiers, bool> values;

  //+ lambda abstraction for C;
  //what does it look like? 
  //loop provides specific values of x,y
  //lambda returns one of  op(x,y) = x, op(y,x), op(x,x),  op(y,y) , etc.
  //mult. talble lookup operations

    //default constructor
    Condition(){};
    
    //constructor.... change to lambda? something?
    Condition(int x){};
    //constructor: condition(lamdba for c){
  //lamdba = lambda
  // cases_checked = false
  // bools?

    
    
    bool check_condition(Quantifiers);
    
    
    
    
};


#endif
