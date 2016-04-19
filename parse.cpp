#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "algebraic_structure.h"
#include "condition.h"
#include "parse.h"

//helper file to target delimiters for deletion
bool isDelimiter(char c){
    switch(c){
        case '(':
        case ')':
        case ' ':
        case ',':
        case '\t':  //tab
            return true;
        default:
            return false;
    }
}

Algebraic_structure *parse(char* filename) {
    std::string input_string;
    std::vector<char>elements;
    std::string relation_name;
    std::vector<char> this_fact;
    std::vector < std::vector<char> > facts;
    
    //open file
    std::ifstream input_file(filename);
    if (!input_file){
        std::cerr << "Error opening file" << std::endl;
    }
    
    //main loop: iterate through file
    while (input_file >> input_string){
        //handle comments:
        //if '%' appears at the beginning of a line or beginning of a word, skip to next line
        if (input_string[0] == '%'){
            input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        if(input_string == "Elements:"){
            
            //remove
            //std::cout << "\nprocessing Elements\n";
            
            //inner read loop to process elements
            //ignore comments in inner loop
            while (input_file >> input_string && input_string != "Relation:"){
                if (input_string[0] == '%'){
                    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                //keep first char, anything else is punctuation
                //number elements starting at zero
                //add to vector
                elements.push_back(input_string[0]);
                //std::cout << "added element " << input_string[0] << "\n";
            }
        } //end element handling
        
        if(input_string == "Relation:"){
            
            
            //std::cout << "\nprocessing Relation\n";
            
            
            //save relation name
            input_file >> input_string;
            relation_name  = input_string;
            
            //std::cout << "read relation " << relation_name << std::endl;
            
            
        } //end relation handling
        
        
        if(input_string == "Facts:"){
            //inner loop to read facts
            //std::cout << "\nbuilding fact table\n";
            
            getline(input_file, input_string);
            
            while (input_string != "end"){
                
                //std::cout << "input string: " << input_string << "size: " << input_string.length() << std::endl;
                
                //remove all delimiters
                input_string.erase(std::remove_if(input_string.begin(), input_string.end(), &isDelimiter), input_string.end());
                //std::cout << "delims removed: " << input_string << " " << input_string.length() << std::endl;
                
                
                //if there's anything left, add it to the table
                //if nothing left, this is just whitespace, so ignore
                if (input_string.length() > 0){
                    if(input_string.length() != 3){
                        std::cerr << "only arity 3 implemented so far, closing"  << std::endl;
                    }
                    
                    //construct table entry for this particular fact
                    for (int i = 0; i < input_string.length(); i++){
                        //std::cout << i <<": " << input_string[i] << std::endl;
                        this_fact.push_back(input_string[i]);
                    }
                    
                    //push this fact into table
                    facts.push_back(this_fact);
                    this_fact.clear();
                }
                
                //get next line
                getline(input_file, input_string);

            }
            
        } //end fact handling
        
        
        if(input_string == "end"){
            
            //std::cout << "\nfound end\n" << std::endl;
            
            input_file.close();
            
            //done file operations, call constructor
            
            Algebraic_structure *alg = new Algebraic_structure(elements, relation_name, facts);
            return alg;

            //finshing reading file
        }
    } //end main while loop 

    //we should only get here if file is malformed.

    return nullptr;
    
}


