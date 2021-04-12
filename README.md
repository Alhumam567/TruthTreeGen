# TTGenerator
A Truth Tree, aka Semantic Tableau or Analytical Tableaux, is a logic proof procedure which expands a formula into a tree structure. A branch in a Truth Tree represents a set of statements deriving from the given formula that hold true. They can occur when applying a decomposition rule on a statement in the branch where multiple different truth values allow the statement to be true. For example, the statement (PvQ) holds when P or Q are true, therefore we split the current branch into two possible scenarios, one to represent the case where P is true, and one for if Q is true:
    
       PvQ
       / \   
      P   Q
    
Any previous statements that may occur before this decomposition will still hold in both branches, but any subsequent decompositions in either branch will be independent from the other.

To prove that a logical formula is valid, the tree must decompose fully into a contradiction, that is, every branch must be closed by a contradiction. If any branch is left open after fully decomposing every statement in the branch, then the formula is invalid and thus a counterexample for the argument can be derived from that specific branch by starting at the top of the tree and recovering all literals that are part of that branch. 

The purpose of this program is to generate a complete truth tree for symbolic logical formulas of the following types:
+ Propisitional Logic
+ Predicate Logic
  - Identity
  - Operations

In the case that the truth tree derives at an invalidity, the program will provide a counterexample.

I will attempt to update this program to support Modal Logic once I formally learn it...
