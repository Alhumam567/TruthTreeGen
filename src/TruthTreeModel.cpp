#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

TruthTreeBranch::TruthTreeBranch(std::vector<std::string> lines) :
    status {Status::OPEN},
    decomposedStatements {},
    openStatements {},
    literals {},
    parentBranch {NULL}
{
    for (auto &s : lines) {
        if (DecompositionUtil::isLiteral(s)) 
            literals.push_back(s);
        else
            openStatements.push_back(s);
    }
}

int update(std::string statement, std::vector<std::string> newStatements) {

}

TruthTreeModel::TruthTreeModel(std::vector<std::string> arguments, std::string conclusion) : 
    complete {false}
{
    this->arguments = arguments;
    if (!DecompositionUtil::isLiteral(conclusion)) {
        this->conclusion = "\uFFE2(" + conclusion + ")";
    } else {
        this->conclusion = "\uFFE2" + conclusion ;
    }

    arguments.push_back(conclusion);
    branches = {new TruthTreeBranch(arguments)}; 
}

TruthTreeModel::~TruthTreeModel() {
    // delete all truth tree branches
    while (!branches.empty()) {
        delete branches.back();
        branches.pop_back();
    }
}

int TruthTreeModel::generateTree() {

}

void TruthTreeModel::applyDecompositionRule(TruthTreeBranch branch, std::string statement) {
    std::vector<std::string> *decomposition = new std::vector<std::string>{};

    bool split = DecompositionUtil::decompose(statement, decomposition);

    branch.update(statement, *decomposition);
}