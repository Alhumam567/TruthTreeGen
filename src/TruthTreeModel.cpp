#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

TruthTreeBranch::TruthTreeBranch(const std::vector<std::string> &lines, TruthTreeBranch *parent) :
    status {Status::OPEN},
    parentBranch {parent},
    decomposedStatements {},
    openStatements {},
    literals {}
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

TruthTreeModel::TruthTreeModel(const std::vector<std::string> &arguments, const std::string &conclusion) : 
    complete {false}
{
    this->arguments = arguments;
    if (!DecompositionUtil::isLiteral(conclusion)) {
        this->conclusion = "\uFFE2(" + conclusion + ")";
    } else {
        this->conclusion = "\uFFE2" + conclusion;
    }

    std::vector<std::string> allLines = arguments;
    allLines.push_back(this->conclusion);
    branches = {new TruthTreeBranch(arguments, NULL)}; 
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

void TruthTreeModel::applyDecompositionRule(TruthTreeBranch *branch, const std::string &statement) {
    std::vector<std::string> *decomposition = new std::vector<std::string>{};

    bool split = DecompositionUtil::decompose(statement, decomposition);

    if (split) {
        std::vector<std::string> lLines, rLines;

        for (int i {0}; i < decomposition->size(); i++) {
            if (i < decomposition->size()/2) lLines.push_back(decomposition->at(i));
            else rLines.push_back(decomposition->at(i));
        }

        TruthTreeBranch *lB = new TruthTreeBranch(lLines, branch);
        TruthTreeBranch *rB = new TruthTreeBranch(rLines, branch);

        branch->update(statement, {}); // Mark statement as decomposed
    } else {
        branch->update(statement, *decomposition);
    }
}