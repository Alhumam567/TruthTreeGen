#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

TruthTreeBranch::TruthTreeBranch(std::vector<std::string> lines) :
    status {Status::OPEN}
{
    for (auto &s : lines) {
        if (DecompositionUtil::isLiteral(s))
            literals.push_back(s);
    }
}