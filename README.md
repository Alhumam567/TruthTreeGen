# TTGenerator
A Truth Tree, aka Semantic Tableau or Analytical Tableaux, is a logic proof procedure which expands a formula into a tree structure. A branch in a truth tree represents a set of statements that hold true from the given formula. They can occur when using a decomposition rule on a statement that holds true under multiple different truth values. For example, the statement (PvQ) holds if P or Q are true, therefore the tree will split in such a case to represents the possible outcomes: 
    
       PvQ
       / \   
      P   Q
    
To prove that a logical formula is valid the tree must decompose fully into a contradiction, that is, every branch must be closed by a contradiction. If any branch is left open after fully decomposing, then the formula is invalid and thus a counterexample for it can be derived from that specific branch by starting at the top of the tree recovering all literals that are part of that branch. 

The purpose of this program is to generate a complete truth tree for a symbolic logical formula of the following types:
+ Propisitional Logic
+ Predicate Logic
  - Identity
  - Operations

In the case that the truth tree derives at an invalidity, it will provide you with a counterexample.

I will attempt to update this program to support Modal Logic once I formally learn it...
