#ifndef prover_h
#define prover_h
#include <vector>




//all true claims are made into "Formula" objects, representing:
//      prettified name (eg: "∀x ∃y x * y = x")
//      a less pretty prover-readable name ("all x exists y *(x,y,x)")
//      an include flag, true if it's part of a minimal set of axioms, false otherwise

class Formula{
private:
    
public:
    std::string pretty_name;
    std::string prover_name;
    bool subsumption_include;
    
    //default constructor
    Formula(){}
    
    //constructor
    Formula(std::string name1, std::string name2){
        pretty_name = name1;
        prover_name = name2;
        if (prover_name.empty()){
            prover_name = name1;
        }
        subsumption_include = true;
    }
    
    void blank(){
        pretty_name = "";
        prover_name = "";
    }
};



//launch external prover
void invoke_prover(std::vector<Formula>*);

//since the implemenation is far faster than expected, generate all possible subsets
//of formulas, to find the true minimal one, if one exists
//this should give us an idea of how good subsumption is in practice

//more complex theorem prover calls in newer versions of the implementation mean this is now deprecated
//no user-servicable code inside
std::map<std::string, int> find_sufficient_subsets(std::vector<Formula>*);



#endif