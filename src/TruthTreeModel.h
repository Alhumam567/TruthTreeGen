#ifndef TTMODEL_H
#define TTMODEL_H

#include <iostream>
#include <vector>

class TruthTreeBranch
{
    enum class Status { OPEN=0, CLOSED=1, COMPLETEOPEN=2 };
    Status status;
    
    std::vector<std::string> literals;
    std::vector<std::string> decomposedStatements;
    std::vector<std::string> openStatements;

public:
    TruthTreeBranch(std::vector<std::string> lines);

    int update(std::string *newStatements);
};

class TruthTreeModel
{
    std::vector<TruthTreeBranch *> branches;
    std::vector<std::string> arguments;
    std::string conclusion;
    bool complete;

public:
    TruthTreeModel(std::vector<std::string> arguments, std::string conclusion);
    ~TruthTreeModel();

    int generateTree();
    void applyDecompositionRule(TruthTreeBranch branch, std::string statement, std::string mainConnective);
};

#endif