#include <iostream>
#include <vector>
#include <map>
#include <cmath>        //pow()
#include <fstream>      //file handling
#include <unistd.h>     //system calls
#include "prover.h"

//change "...your..path...here..." to your absolute path to prover9. Make sure the path begins with a quotation mark (") and ends with (prover9)
//eg: "/Users/Me/Documents/stuff/LADR-2009-11A/bin/prover9
//make sure the space before "-f" remains
//leave the rest of the string untouched

//or to use without a prover add "-noprover" as a final command line argument
//but it will look a lot less impressive

int call_prover(){
    //change this line
    int outcome = system("/...your..path...here..../prover9 -f analogy_prover_file.in > prover_temp.out");
    
    //pass along prover exit code
    return WEXITSTATUS(outcome);
}





//main prover loop:

//for all formulas f in S
    //print out {S-f} as sos, skipping formulas marked as redundant
    //print out f as goal
    //call prover
    //if proof found, f redundant, mark for removal
//return S


/*
 Prover9 return values:
 0      proof found
 1      fatal error
 2      no proof found, premises exhausted
 3      max memory hit
 4      max time hit
 5      max given hit
 6      max kept hit
 7      Prover9 terminated
 101    Prover9 crashed
 
 */


void invoke_prover(std::vector<Formula>* f){
    int outcome;
    unsigned long num_formulas = f->size();
    std::string callstring;
    std::ofstream outfile;
    //std::string path = "/Users/G2/Documents/Logic/project/code/LADR-2009-11A/bin";
    
    //remove
    std::cout << "sending " << num_formulas << " formulas to prover" << std::endl;
    
    //main loop
    for (int i = 0; i < num_formulas; i++) {
        
        //create a new file, or overwrite an existing one
        outfile.open("analogy_prover_file.in", std::ios::trunc);  //open file, overwrite
        
        if (!outfile.is_open()){
            std::cerr << "file error:" << std::endl;
            return;
        }
        
        // --- build prover input file ---- //
        
        //stop after finding one proof (starts counting from zero)
        outfile << "assign(max_proofs, 0).\n";
        
        //give up after 1 sec
        //longer timeouts have made no difference in practice
        outfile << "assign(max_seconds, 1).\n\n";
        
        //suppress console output
        outfile << "set(quiet).\n";
        
        //start list of premises
        outfile << "formulas(sos).\n\n";
        
        for (int j = 0; j < num_formulas; j++){
  
            //skip current formula we're trying to prove
            if (j == i){
                continue;
            }
            
            //output any premises not yet found redundant
            if ((*f)[j].subsumption_include){
                outfile << (*f)[j].prover_name << ".\n";
            }
        }
        //done printing premises
        outfile << "\nend_of_list.\n\n";
        
        //try to prove formula i
        outfile << "formulas(goals).\n\n" << (*f)[i].prover_name << ".\n\n" << "end_of_list." <<std::endl;
        outfile.close();
        
        // --- done printing to file --- //
        
        
        // -- invoke prover -- //
        
        outcome = call_prover();
    
        //outcome = system("/Users/G2/Documents/Logic/project/code/LADR-2009-11A/bin/prover9 -f analogy_prover_file.in > prover_temp.out");
        //outcome  = WEXITSTATUS(outcome);
        
        
        std::cout << "outcome: " << outcome << std::endl;
        
        //exit code 0 means proof found
        //if proof found, this formula redundant, remove from set
        if (outcome == 0){
            (*f)[i].subsumption_include = false;
        }
        if (outcome == 4){
            //std::cout << "timed out for formula " << (*f)[i].prover_name << std::endl;
        }
        
    } //end main loop
    
}





//extra function, just for fun
//generates all 2^n possible subsets of formulas, to find any true minimal subsets
//obviously intractable for large n, but the implementation is fast enough to pull this off
//easily for the example input.
//gives us an indication whether the subsumption algorithm is any good in practice


//this is experimental code, no user functionality





/*
 Generating powersets turns out to be not entirely straightforward in C++.
 Here's the most straightforward way: For a set of size n, just count from
 0 to (2^n -1) in binary numbers of length n, and use 0,1 as a guide to excluding/including
 each element in the set.
 
 So for n = 2, we get:
 
 00 (empty set)
 01 (just the first element)
 10 (just the second element)
 11 (both elements)
 
 For arbitrary n, the code to generate the bitstrings is a little more convoluted, so I've packaged
 most of that code into three helper functions, which follow below.
 */

//makes a vector representing a binary number
std::vector<int>& bin(int n, std::vector<int> *vec){
    std::vector<int> *v = new std::vector<int>;
    v = vec;
    if (n/2 != 0) {
        bin(n/2, vec);
    }
    v->push_back(n %2);
    return *v;
}

//returns a binary number of fixed length as a string
std::string construct_bin(int n, int length){
    std::vector<int> *fixed_length_bin = new std::vector<int>;
    std::string s;
    
    fixed_length_bin = &bin(n, fixed_length_bin);
    
    for (unsigned long i = fixed_length_bin->size(); i < length; i++){
        fixed_length_bin->insert(fixed_length_bin->begin(), 0);
    }
    for (auto b : *fixed_length_bin){
        s+= std::to_string(b);
    }
    return s;
}


//returns all possible 2^n bitstrings of length n
std::vector<std::string>& power_set(int n){
    std::vector<std::string> *v = new std::vector<std::string>;
    std::string s;
    int pset_size = std::pow(2,n);
    
    for (int i =0; i < pset_size ; i++){
        s = construct_bin(i, n);
        v->push_back(s);
    }
    return *v;
}


int num_formulas_this_subset (std::string s){
    int count = 0;
    for (int i= 0; i < s.length(); i++){
        if (s[i] == '1'){
            count++;
        }
    }
    return count;
}








//where n is the number of formulas true for this structure, get a vector of all bistrings of length n
//use bitstrings as a guide to generate all possible subsets of formulas
//for each subset:
//generate a prover file that has that subset as premises, and try to prove all n formulas
//remember the size and bitstring for any subset that can prove all formulas
//when finshed, output the one(s) with minimum size


//requires 2^n prover calls
//returns vector of subset indices

std::map<std::string, int> find_sufficient_subsets(std::vector<Formula>* f){
    std::map<std::string, int> sufficient_subsets;
    int subset_size;
    int n = (int)f->size();
    std::ofstream outfile;
    std::string subset_string;
    char current_bit;
    int outcome;
    
    //vector of all bitstring of length equal to number of formulas
    std::vector<std::string> pset = power_set(n);
    int num_subsets = (int)pset.size();
    
    //iterate through all possible subsets
    for (int i = 0; i < num_subsets; i++){
        
        std::cout << i << "\n"; 
        
        //string for this subset (eg: "001011001")
        subset_string = pset[i];
        
        //size of this subset
        subset_size = num_formulas_this_subset(subset_string);
        
        //create a new file, or overwrite an existing one
        outfile.open("analogy_prover_file.in", std::ios::trunc);  //open file, overwrite
        
        if (!outfile.is_open()){
            std::cerr << "file error:" << std::endl;
            throw 1;
        }
        
        // --- build prover input file ---- //
        
        //stop after finding one proof (starts counting from zero)
        outfile << "assign(max_proofs, 0).\n";
        
        //give up after 5 secs
        outfile << "assign(max_seconds, 5).\n\n";
        
        //suppress console output
        outfile << "set(quiet).\n";
        
        //start list of premises
        outfile << "formulas(sos).\n\n";
        
        //print all premises for this subset
        for (int j = 0; j < subset_string.length(); j++){
            current_bit = subset_string[j];
            if (current_bit == '1'){
                outfile << (*f)[j].prover_name << ".\n";
            } //else skip to next bit
        }
        
        //done printing premises
        outfile << "\nend_of_list.\n\n";
        
        //try to prove entire set of formulas
        outfile << "formulas(goals).\n\n";
        for (int k =0; k < n; k++){
            outfile << (*f)[k].prover_name << ".\n";
        }
        outfile << "end_of_list." <<std::endl;
        
        outfile.close();
        
        //invoke prover
        outcome = call_prover();
        
        //if proof found, this subset is sufficient, so remember it
        if (outcome == 0){
            sufficient_subsets[subset_string] = subset_size;
        }
        if (outcome == 4){
            std::cout << "timed out for formula " << (*f)[i].prover_name << std::endl;
        }
        
        
    } //end main loop, done with subsets
    
    
    
    //todo: minimum set find
    //find smallest subset size
    //save all the subsets with that size in a map
    //return that map instead of all sufficient
    
    
    
    return sufficient_subsets;
}







