Gordon Krieger
COMP527 project


The first iteration of the project will take groups or other finite algebraic structures as input, and output any axioms that apply to that structure. In particular it looks for claims of the form 

	Q1 x Q2 y P(v1, v2, v3)
	
	where: 
	
	the Qs are any of the universal, existential, or "exists uniquely" (∃!) quantifiers

	v1, v2, v3 can be either x or y 

	The predicate P represents claims of the form “v1 * v2 = v3”, where “*” is the operator for the group, or some binary operator for non-groups. 

Input should be from text files following roughly this format:


----------------

Elements: a, b
Relation: *

Facts:   
(0,0,0)
(0,1, 1)
(1, 1,0)
(1,0,1)
end

----------------


Output will be a set of axioms describing the structure. 







Implementation notes:

"parse.cpp" is the input file parser. This is largely finished although still brittle as parsers go. 

"algebraic_structure.h" is a class file for representing the input. This represents the domain we're quantifying over. 

The implementation for the class is given in algebraic_structure.cpp; most of the code there involves multiple iterations over the domain, checking conditions. For the two quantifier format above, this is done through a series of nested loops. The most straightforward example is for ∀x∀y P(x,y) for some fixed predicate P;

	for all x in the domain:
		for all y in the domain:
			if not P(x,y) return false
		endfor
	endfor
	return true


or for ∀x∃y P(x,y):

	for all x:		flag ← false		for all y:			check condition: P(x,y), if true, flag ← true		endfor		if flag == false, return false 
	endforreturn true



.... and so on for 9 cases in total. There is enough overlap between cases that we can double up on a few of them rather than mindlessly doing 9 sets of nested loops. This makes for more case analysis in the code but should help speed up the implementation. 


"condition.h" is the class file for predicates. This includes a representation of the predicate itself, along with a map that connects the nine different quantifier cases to boolean values for that predicate. (These values are filled in by "algebraic_structure.cpp". 


There is a brief main file "analogy.cpp", which is mostly notes and pseudocode. The code compiles but currently does nothing, so I haven't included a makefile. 



