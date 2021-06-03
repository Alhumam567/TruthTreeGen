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

    evaluateBranch(this->literals);
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
    
    decomposedStatements.push_back(statement); // Mark statement as decomposed

    return this->evaluateBranch(newLiterals);
}

TruthTreeBranch::Status TruthTreeBranch::evaluateBranch(const std::vector<std::string> &newLiterals) {
    bool completeOpen {true};

    // Compare new literals to all literals in branch to find contradiction
    for (int i {0}; i < newLiterals.size(); i++) {
        for (int j {0}; j < this->literals.size(); j++) {
            if (DecompositionUtil::isNegations(newLiterals.at(i), this->literals.at(j))) {
                this->status = Status::CLOSED;
                return this->status;
            }
        }
    }
    if (this->openStatements.size() > 0) completeOpen = false;

    // Compare new literals to rest of literals in full branch to find contradiction
    // Also check if branch has any open statements, if so mark completeOpen false
    TruthTreeBranch *currBranch = this->parentBranch;
    while (currBranch != NULL) {
        for (int i {0}; i < newLiterals.size(); i++) {
            for (int j {i + 1}; j < currBranch->literals.size(); j++) {
                if (DecompositionUtil::isNegations(newLiterals.at(i), currBranch->literals.at(j))) {
                    this->status = Status::CLOSED;
                    return this->status;
                }
            }
        }

        if (currBranch->openStatements.size() > 0) completeOpen = false;
        currBranch = currBranch->parentBranch;
    }

    if (completeOpen == true) 
        this->status = Status::COMPLETEOPEN;
    return this->status;
}

std::string TruthTreeBranch::popOpenStatement() {
    if (this->openStatements.size() == 0) return "";
    std::string s {this->openStatements.back()};
    this->openStatements.pop_back();

    return s;
}

void TruthTreeBranch::printBranch() {
    for (auto s: literals) {
        std::cout << s << "\n";
    }
    for (auto s: openStatements) {
        std::cout << s << "\n";
    }
    for (auto s: decomposedStatements) {
        std::cout << s << "\n";
    }
}

TruthTreeModel::TruthTreeModel(const std::vector<std::string> &arguments, const std::string &conclusion) : 
    complete {false},
    conclusion {conclusion},
    arguments {arguments}
{
    std::string negConc;
    if (!DecompositionUtil::isLiteral(conclusion)) {
        negConc = "\uFFE2(" + conclusion + ")";
    } else {
        negConc = "\uFFE2" + conclusion;
    }

    std::vector<std::string> allLines = arguments;
    allLines.push_back(negConc);
    openBranches = { new TruthTreeBranch(allLines, NULL) };

    std::cout << "\nStarting truth tree generation...\n\n";
    
    this->generateTree();
}

TruthTreeModel::~TruthTreeModel() {
    // delete all truth tree branches
    std::vector<TruthTreeBranch *> upperBranches {};
    std::vector<TruthTreeBranch *> currBranches {closedBranches};
    currBranches.insert(currBranches.end(), completeOpenBranches.begin(), completeOpenBranches.end()); //currBranches holds every branch

    while (!upperBranches.empty() || !currBranches.empty()) {
        for (int i {0}; i < currBranches.size(); i++) {
            if (currBranches[i]->parentBranch != NULL && // If upper segment was already deleted
                std::count(currBranches.begin(), currBranches.end(), currBranches[i]->parentBranch) == 0 && // If its about to be deleted this loop
                std::count(upperBranches.begin(), upperBranches.end(), currBranches[i]->parentBranch) == 0) // If its already appended to next deletion list
                upperBranches.push_back(currBranches[i]->parentBranch);
            
            delete currBranches[i];

            currBranches[i] = NULL; // Mark branch as NULL for later iterations
        }

        currBranches = upperBranches;
        upperBranches = {};
    }
}

int TruthTreeModel::generateTree() {
    TruthTreeBranch *currBranch;

    // Fully decompose all branches:
    while (openBranches.size() > 0) {
        currBranch = this->openBranches.back();  
        this->openBranches.pop_back();  

        if (currBranch->status == TruthTreeBranch::Status::OPEN) {    
            std::string s {currBranch->popOpenStatement()};

            while (s != "" && currBranch->status == TruthTreeBranch::Status::OPEN) {
                // std::cout << "Applying Decomp...\n";
                // std::cout << "Statement: " << s.length() << "\n";
                bool b = this->applyDecompositionRule(currBranch, s);
                
                // std::cout << "bool: " << b << "\n";
                s = currBranch->popOpenStatement();
            }   
            // std::cout << "Stopped\n";
        }
        else {
            if (currBranch->status == TruthTreeBranch::Status::COMPLETEOPEN) this->completeOpenBranches.push_back(currBranch);
            else if (currBranch->status == TruthTreeBranch::Status::CLOSED) this->closedBranches.push_back(currBranch);
            else { std::cout << "Error: INT branch detected\n"; break;}
        }
    }

    for (int i {}; i<this->closedBranches.size(); i++) {
        TruthTreeBranch *b {this->closedBranches[i]};

        std::cout << "Closed Branch " << i << ":\n";
        while (b != NULL) {
            b->printBranch();
            b = b->parentBranch;
            std::cout << "|\n";
        }

        std::cout << "-------------------\n";
    }

    for (int i {}; i<this->completeOpenBranches.size(); i++) {
        TruthTreeBranch *b {this->completeOpenBranches[i]};

        std::cout << "CO Branch " << i << ":\n";
        while (b != NULL) {
            b->printBranch();
            b = b->parentBranch;
            std::cout << "|\n";
        }

        std::cout << "-------------------\n";
    }
}

/** Takes a branch and a statement in that branch and attempt to decompose it. If the branch splits
 *  then two new branches connected to the original are initialized. Otherwise, statements get added to
 *  the given branch.
 **/
bool TruthTreeModel::applyDecompositionRule(TruthTreeBranch *branch, const std::string &statement) {
    std::vector<std::string> decomposition {};
    std::cout << "DecompUtil decompose started on " << statement << " ...\n";
    bool split = DecompositionUtil::decompose(statement, &decomposition);
    std::cout << "DecompUtil decompose done on " << statement << " ...\n";
    // Decomposition caused split in branch
    if (split) {
        std::cout << "split true\n";
        std::vector<std::string> lLines, rLines;

        // Split decomposition into left and right branches
        for (int i {0}; i < decomposition.size(); i++) {
            if (i < decomposition.size()/2) lLines.push_back(decomposition.at(i));
            else rLines.push_back(decomposition.at(i));
        }
        std::cout << "Creating new branches ...\n";
        TruthTreeBranch *lB = new TruthTreeBranch(lLines, branch);
        TruthTreeBranch *rB = new TruthTreeBranch(rLines, branch);
        
        this->openBranches.push_back(lB);
        this->openBranches.push_back(rB);

        branch->status = TruthTreeBranch::Status::INT;
        std::cout << "Updating branch ...\n";
        branch->update(statement, {}); // Mark statement as decomposed
        std::cout << "Done applying decomposition...\n";
    } 
    // No splitting, append new statements to same branch
    else {
        branch->update(statement, decomposition);
    }

    return split;
}