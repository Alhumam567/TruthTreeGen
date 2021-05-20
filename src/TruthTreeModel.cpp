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

/** Marks statement in branch as decomposed and adds any new statements from the decomposition if there are any.
 *  Returns the status of the branch after updating. 
 */
TruthTreeBranch::Status TruthTreeBranch::update(const std::string &statement, const std::vector<std::string> &decomposition) {
    std::vector<std::string> newLiterals {};

    for (auto &newS : decomposition) {
        if (DecompositionUtil::isLiteral(newS)) { 
            newLiterals.push_back(newS);
            literals.push_back(newS);
        }
        else
            openStatements.push_back(newS);
    }

    for (std::vector<std::string>::iterator iterator = openStatements.begin(); 
            iterator != openStatements.end(); ++iterator) {
        if (*iterator == statement) {
            openStatements.erase(iterator);
            break;
        }
    }

    return this->evaluateBranch(newLiterals);
}

TruthTreeBranch::Status TruthTreeBranch::evaluateBranch(const std::vector<std::string> &newLiterals) {
    // Finding contradiction:
    // - compare new literals to themselves to find contradiction
    for (int i {0}; i < newLiterals.size(); i++) {
        for (int j {i + 1}; j < newLiterals.size(); j++) {
            
        }
    }
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

    this->generateTree();
}

TruthTreeModel::~TruthTreeModel() {
    // delete all truth tree branches
    std::vector<TruthTreeBranch *> upperBranches {};
    std::vector<TruthTreeBranch *> currBranches = branches;

    while (!upperBranches.empty() || !currBranches.empty()) {
        for (int i {0}; i < currBranches.size(); i++) {
            if (i % 2 == 0)
                upperBranches.push_back(currBranches[i]->parentBranch);
            delete currBranches[i];
        }
        currBranches = upperBranches;
        upperBranches = {};
    }
}

int TruthTreeModel::generateTree() {
    
}

/** Takes a branch and a statement in that branch and attempt to decompose it. If the branch splits
 *  then two new branches connected to the original are initialized. Otherwise, statements get added to
 *  the given branch.
 **/
bool TruthTreeModel::applyDecompositionRule(TruthTreeBranch *branch, const std::string &statement) {
    std::vector<std::string> *decomposition = new std::vector<std::string>{};

    bool split = DecompositionUtil::decompose(statement, decomposition);

    // Decomposition caused split in branch
    if (split) {
        std::vector<std::string> lLines, rLines;

        // Split decomposition into left and right branches
        for (int i {0}; i < decomposition->size(); i++) {
            if (i < decomposition->size()/2) lLines.push_back(decomposition->at(i));
            else rLines.push_back(decomposition->at(i));
        }

        TruthTreeBranch *lB = new TruthTreeBranch(lLines, branch);
        TruthTreeBranch *rB = new TruthTreeBranch(rLines, branch);

        branch->update(statement, {}); // Mark statement as decomposed
    } 
    // No splitting, append new statements to same branch
    else {
        branch->update(statement, *decomposition);
    }

    return split;
}