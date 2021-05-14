#ifndef TTMODEL_H
#define TTMODEL_H

#include <iostream>
#include <vector>

class TruthTreeBranch
{
    std::vector<std::string> literals;
    std::vector<std::string> decomposedStatements;
    std::vector<std::string> openStatements;

    TruthTreeBranch *parentBranch;

public:
    enum class Status { OPEN=0, CLOSED=1, COMPLETEOPEN=2 };

    TruthTreeBranch(const std::vector<std::string> &lines, TruthTreeBranch *parent);

    int update(const std::string &statement, const std::vector<std::string> &decomposition);

private:
    Status status;
};

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
    void applyDecompositionRule(TruthTreeBranch *branch, const std::string &statement);
};

#endif