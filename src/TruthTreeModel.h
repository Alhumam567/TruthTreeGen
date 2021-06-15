#ifndef TTMODEL_H
#define TTMODEL_H

#include <iostream>
#include <vector>

#include "StatementUtil.h"

/** A section of the truth tree containing all statements occurring after the end of previous 
 *  parent branch, up to but not including any children branches. This is not a full branch of 
 *  the truth tree, it only contains a section of a branch, but given a leaf branch you can trace 
 *  the tree up to the root to obtain a full branch of the truth tree.
 **/
class TruthTreeBranch
{
    std::vector<std::string> literals;              // atomics, or negation of atomics: P, ~P, F(ab), ~G(x), ...
    
public:
    TruthTreeBranch *parentBranch;
    TruthTreeBranch *leftBranch, *rightBranch;
    enum class Status { OPEN=0, CLOSED=1, COMPLETEOPEN=2, INT=-1 };
    Status status;

    //Public for testing purposes
    std::vector<Statement> decomposedStatements;  // all statements that were broken down using their main connective rule
    std::vector<Statement> openStatements;        // all statements yet to be broken down

    TruthTreeBranch(const std::vector<Statement> &lines, TruthTreeBranch *parent); 

    void printBranch();
    std::vector<std::string> update(const Statement &statement, const std::vector<Statement> &decomposition);
    Status evaluateBranch(const std::vector<std::string> &newLiterals);
    TruthTreeBranch *popOpenStatement(Statement &string);
};

/** Entire truth tree held as a list of TruthTreeBranches. 
 *  Arguments and conclusion are the original inputs to the model.
 **/
class TruthTreeModel
{
    std::vector<TruthTreeBranch *> openBranches;
    std::vector<TruthTreeBranch *> closedBranches;
    std::vector<TruthTreeBranch *> completeOpenBranches;

    std::vector<std::string> premises;
    std::string conclusion;
    bool complete;

public:
    TruthTreeModel(const std::vector<std::string> &arguments, const std::string &conclusion);
    ~TruthTreeModel();

    int generateTree();
    bool applyDecompositionRule(TruthTreeBranch *branch, const Statement &statement);
    void printModel();
};

#endif