#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

TruthTreeBranch::TruthTreeBranch(std::vector<std::string> lines) :
    status {Status::OPEN}
{
    for (auto &s : lines) {
        if (DecompositionUtil::isLiteral(s)) 
            literals.push_back(s);
        else
            openStatements.push_back(s);
    }
}

TruthTreeModel::TruthTreeModel(std::vector<std::string> arguments, std::string conclusion) : 
    complete {false}
{
    if (!DecompositionUtil::isLiteral(conclusion)) {
        conclusion = "\uFFE2(" + conclusion + ")";
    } else {
        conclusion = "\uFFE2" + conclusion ;
    }
}