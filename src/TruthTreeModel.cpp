#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

TruthTreeBranch::TruthTreeBranch(const std::vector<std::string> &lines, TruthTreeBranch *parent) :
    status {Status::OPEN},
    parentBranch {parent},
    leftBranch {NULL},
    rightBranch {NULL},
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
    
    if (statement != "")
        decomposedStatements.push_back(statement); // Mark statement as decomposed

    return this->evaluateBranch(newLiterals);
}

TruthTreeBranch::Status TruthTreeBranch::evaluateBranch(const std::vector<std::string> &newLiterals) {
    bool completeOpen {true};

    // Compare new literals to all literals in branch to find contradiction
    for (int i {0}; i < newLiterals.size(); i++) {
        
        for (int j {0}; j < this->literals.size(); j++) {
            if (DecompositionUtil::isNegations(newLiterals.at(i), this->literals.at(j))) {
                std::cout << this << ", closed1\n"; 
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
            for (int j {0}; j < currBranch->literals.size(); j++) {
                if (DecompositionUtil::isNegations(newLiterals[i], currBranch->literals[j])) {
                    std::cout << this << ", closed2\n"; 
                    this->status = Status::CLOSED;
                    return this->status;
                }
            }
        }

        if (currBranch->openStatements.size() > 0) completeOpen = false; 
        currBranch = currBranch->parentBranch;
    }

    if (completeOpen == true) {
        std::cout << this << ", copen\n"; 
        this->status = Status::COMPLETEOPEN;
    }
    return this->status;
}

TruthTreeBranch *TruthTreeBranch::popOpenStatement(std::string &string) {
    TruthTreeBranch *b = this;

    while (b->openStatements.size() == 0) {
        b = b->parentBranch;

        if (b == NULL) { 
            string = ""; 
            return NULL; 
        }
    }

    string = b->openStatements.back();
    b->openStatements.pop_back();

    return b;
}

void TruthTreeBranch::printBranch() {
    for (auto s: literals) {
        std::cout << "\t\t" << s << "\n";
    }
    for (auto s: openStatements) {
        std::cout << "\t\t" << s << " o\n";
    }
    for (auto s: decomposedStatements) {
        std::cout << "\t\t" << s << " d\n";
    }
}

TruthTreeModel::TruthTreeModel(const std::vector<std::string> &arguments, const std::string &conclusion) : 
    complete {false},
    conclusion {conclusion},
    arguments {arguments}
{
    std::string negConc = conclusion;
    DecompositionUtil::addNegation(negConc);

    std::vector<std::string> allLines = arguments;
    allLines.push_back(negConc);
    openBranches = { new TruthTreeBranch(allLines, NULL) };

    std::cout << "\nStarting truth tree generation...\n\n";
    
    this->generateTree();
}

TruthTreeModel::~TruthTreeModel() {
    // deletes all truth tree branches
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
        
        std::cout << "\tBranch: " << currBranch << "\n";
        for (auto s: currBranch->openStatements) {
            std::cout << "\t" << s << " o\n";
        }
        for (auto s: currBranch->decomposedStatements) {
            std::cout << "\t" << s << " d\n\n";
        }

        if (currBranch->status == TruthTreeBranch::Status::OPEN) {  
            std::cout << "\t\tStatus: open\n";  
            std::string s {};
            TruthTreeBranch *br {currBranch->popOpenStatement(s)};
            std::cout << "\t\tPopped statement " << s << ", from branch: " << br << "\n";

            while (br != NULL) {
                bool split = this->applyDecompositionRule(br, s);
                
                br = currBranch->popOpenStatement(s);
                std::cout << "\t\tPopped statement " << s << ", from branch: " << br << "\n";
            }   
            std::cout << "\t\tStopped decomposing in currBranch: " << currBranch << "\n";
        }

        if (currBranch->status == TruthTreeBranch::Status::COMPLETEOPEN) { std::cout << "\t\t Status: copen\n";  this->completeOpenBranches.push_back(currBranch); }
        else if (currBranch->status == TruthTreeBranch::Status::CLOSED) { std::cout << "\t\t Status: closed\n";  this->closedBranches.push_back(currBranch); }
        else { std::cout << "INT branch detected\n"; }
    }

    std::cout << "\nEnding Truth Tree Generation...\n\n";
    std::cout << "=======================================\n";
    std::cout << "=======================================\n";
    std::cout << "Model info: \n";
    std::cout << "\t COpen Branches #:" << completeOpenBranches.size() << "\n";
    std::cout << "\t Closed Branches #:" << closedBranches.size() << "\n";
    std::cout << "\t Open Branches #:" << openBranches.size() << "\n";
    std::cout << "---------------------------------------\n";

    for (int i {}; i<this->closedBranches.size(); i++) {
        TruthTreeBranch *b {this->closedBranches[i]};
        std::stack<TruthTreeBranch *> bs {};
        bs.push(b);

        std::cout << "\t\t Closed Branch " << i + 1 << ":\n";
        while (1) {
            b = b->parentBranch;
            if (b != NULL) bs.push(b);
            else break; 
        }

        while (!bs.empty()) {
            bs.top()->printBranch(); 
            bs.pop();
            if (bs.size() != 0) std::cout << "\t\t\n";
        }

        std::cout << "\t\t~~~~~~~~~~~~~~~~~~~~\n";
    }

    for (int i {}; i<this->completeOpenBranches.size(); i++) {
        TruthTreeBranch *b {this->completeOpenBranches[i]};
        std::stack<TruthTreeBranch *> bs {};
        bs.push(b);

        std::cout << "\t\t COpen Branch " << i + 1 << ":\n";
        while (1) {
            b = b->parentBranch;
            if (b != NULL) bs.push(b);
            else break; 
        }

        while (!bs.empty()) {
            bs.top()->printBranch(); 
            bs.pop();
            if (bs.size() != 0) std::cout << "\t\t\n";
        }

        std::cout << "\t\t~~~~~~~~~~~~~~~~~~~~\n";
    }

    std::cout << "=======================================\n";
    std::cout << "=======================================\n";
}

/** Takes a branch and a statement in that branch and attempt to decompose it. If the branch splits
 *  then two new branches connected to the original are initialized. Otherwise, statements get added to
 *  the given branch.
 **/
bool TruthTreeModel::applyDecompositionRule(TruthTreeBranch *branch, const std::string &statement) {
    std::vector<std::string> decomposition {};
    std::cout << "\t\t\tdecompose started on " << statement << " ...\n";
    bool split = DecompositionUtil::decompose(statement, &decomposition);
    std::cout << "\t\t\tdecompose done on " << statement << " ...\n";
    // Decomposition caused split in branch
    if (split) {
        std::cout << "\t\t\tsplit true\n";
        std::vector<std::string> lLines, rLines;

        // Split decomposition into left and right branches
        for (int i {0}; i < decomposition.size(); i++) {
            if (i < decomposition.size()/2) lLines.push_back(decomposition.at(i));
            else rLines.push_back(decomposition.at(i));
        }

        std::cout << "\t\t\tCreating new branches ...\n";

        TruthTreeBranch *lB, *rB;
        std::stack<TruthTreeBranch*> bs {};
        bs.push(branch);

        while (!bs.empty()) {
            TruthTreeBranch *cB = bs.top();
            bs.pop();

            if (cB->status == TruthTreeBranch::Status::OPEN) {
                lB = new TruthTreeBranch(lLines, cB);
                rB = new TruthTreeBranch(rLines, cB);

                cB->leftBranch = lB;
                cB->rightBranch = rB;
                cB->status = TruthTreeBranch::Status::INT;
                
                for (int i {}; i < this->openBranches.size(); i++) {
                    if (this->openBranches[i] == cB) {
                        this->openBranches.erase(this->openBranches.begin() + i);
                        break;
                    }
                }
                
                this->openBranches.push_back(lB);
                this->openBranches.push_back(rB);
            } else if (cB->status == TruthTreeBranch::Status::INT) {
                if (cB->leftBranch != NULL)
                    bs.push(cB->leftBranch);
                if (cB->rightBranch != NULL)
                    bs.push(cB->rightBranch);
            }
        }

        branch->update(statement, {}); // Mark statement as decomposed
        branch->status = TruthTreeBranch::Status::INT;
    } 
    // No splitting, append new statements to same branch
    else {
        branch->update(statement, decomposition);

        std::stack<TruthTreeBranch*> bs {};
        bs.push(branch);

        while (!bs.empty()) {
            TruthTreeBranch *cB = bs.top();
            bs.pop();

            if (cB->leftBranch != NULL) {
                if (cB->leftBranch->status == TruthTreeBranch::Status::OPEN) 
                    cB->update("", decomposition);
                else if (cB->leftBranch->status == TruthTreeBranch::Status::INT) 
                    bs.push(cB->leftBranch);
            }
            if (cB->rightBranch != NULL) {
                if (cB->rightBranch->status == TruthTreeBranch::Status::OPEN) 
                    cB->update("", decomposition);
                else if (cB->rightBranch->status == TruthTreeBranch::Status::INT) 
                    bs.push(cB->leftBranch);
            }
        }
    }

    return split;
}