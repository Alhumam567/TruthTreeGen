#include <iostream>
#include <vector>

class TruthTreeBranch
{
    int status;
    char *literals;
    std::string *decomposedStatements;
    std::string *openStatements;

public:
    TruthTreeBranch(std::string lines);

    int update(std::string *newStatements);
};

class TruthTreeModel
{
    std::vector<TruthTreeBranch> branches;
    std::string *arguments;
    std::string conclusion;
    bool complete;

public:
    TruthTreeModel(std::string arguments, std::string conclusion);
    ~TruthTreeModel();

    int generateTree();
    void applyDecompositionRule(TruthTreeBranch branch, std::string statement, std::string mainConnective);
};