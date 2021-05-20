#ifndef TTMODEL_H
#define TTMODEL_H

#include <iostream>
#include <vector>

/** A section of the truth tree containing all statements occurring after the end of previous 
 *  parent branch, up to but not including any children branches. This is not a full branch of 
 *  the truth tree, it only contains a section of a branch, but given a leaf branch you can trace 
 *  the tree up to the root to obtain a full branch of the truth tree.
 **/
class TruthTreeBranch
{
    std::vector<std::string> literals;              // atomics, or negation of atomics: P, ~P, F(ab), ~G(x), ...
    std::vector<std::string> decomposedStatements;  // all statements that were broken down using their main connective rule
    std::vector<std::string> openStatements;        // all statements yet to be broken down

public:
    TruthTreeBranch *parentBranch;
    enum class Status { OPEN=0, CLOSED=1, COMPLETEOPEN=2 };

    TruthTreeBranch(const std::vector<std::string> &lines, TruthTreeBranch *parent);

    Status update(const std::string &statement, const std::vector<std::string> &decomposition);

private:
    Status status;

    Status evaluateBranch(const std::vector<std::string> &newLiterals);
};

/** Entire truth tree 
 * 
 **/
class TruthTreeModel
{
    std::vector<TruthTreeBranch *> branches;   
    std::vector<std::string> arguments;
    std::string conclusion;
    bool complete;

public:
    TruthTreeModel(const std::vector<std::string> &arguments, const std::string &conclusion);
    ~TruthTreeModel();

    int generateTree();
    bool applyDecompositionRule(TruthTreeBranch *branch, const std::string &statement);
};

#endif