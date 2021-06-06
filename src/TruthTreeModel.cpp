#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

/** Branch constructor, evaluates status after lines are processed:
 *      lines  : All statements that will be placed in this segment
 *      parent : Pointer to the parent TruthTreeBranch 
 */
TruthTreeBranch::TruthTreeBranch(const std::vector<std::string> &lines, TruthTreeBranch *parent) :
    status {Status::OPEN},                     
    parentBranch {parent},
    // New branches are leaf segments
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

/** Called to update a branch when a statement in it was decomposed or simply to mark a statement in this branch as
 *  decomposed:
 *      statement     : The statement that was removed from this branch and decomposed, if it is an empty string
 *                      the statement will not be marked as decomposed
 *      decomposition : Vector of new statements created from decomposition of <statement>, if empty vector then
 *                      no update to status will be called
 */
TruthTreeBranch::Status TruthTreeBranch::update(const std::string &statement, const std::vector<std::string> &decomposition) {
    std::vector<std::string> newLiterals {};

    // Adds any new statements to the branch
    for (auto &newS : decomposition) {
        if (DecompositionUtil::isLiteral(newS)) { 
            newLiterals.push_back(newS);
            literals.push_back(newS);
        }
        else
            openStatements.push_back(newS);
    }
    
    // Update call wants to mark statement in this branch as decomposed
    if (statement != "")
        decomposedStatements.push_back(statement);

    // Branch must be leaf and there must've been new statements added for a re-evaluation of status
    if (!decomposition.empty() && status != Status::INT)
        return this->evaluateBranch(newLiterals);
    return this->status;
}

/** Given a set of new literals, this function will check for any contradiction in the branch or
 *  if there are no open statements left in the full branch and will update the status of this branch
 *  accordingly. Only called on leaf branches:
 *      newLiterals : Any literals that were just decomposed within this branch 
 */
TruthTreeBranch::Status TruthTreeBranch::evaluateBranch(const std::vector<std::string> &newLiterals) {
    bool completeOpen {true};

    // // Compare new literals to all literals in branch to find contradiction
    // for (int i {0}; i < newLiterals.size(); i++) {
    //     for (int j {0}; j < this->literals.size(); j++) {
    //         if (DecompositionUtil::isNegations(newLiterals.at(i), this->literals.at(j))) {
    //             std::cout << this << ", closed1\n"; 
    //             this->status = Status::CLOSED;
    //             return this->status;
    //         }
    //     }
    // }
    // if (this->openStatements.size() > 0) completeOpen = false;

    // Compare new literals to every other literal in branch to find contradiction
    // Also check if branch has any open statements, if so mark completeOpen false
    TruthTreeBranch *currBranch = this;

    while (currBranch != NULL) { // Loop over all TT segments
        for (int i {0}; i < newLiterals.size(); i++) { // Loop over all new literals
            for (int j {0}; j < currBranch->literals.size(); j++) { // Loop over all literals in currBranch
                if (DecompositionUtil::isNegations(newLiterals[i], currBranch->literals[j])) {
                    std::cout << this << ", closed2\n"; 
                    this->status = Status::CLOSED;
                    return this->status;
                }
            }
        }

        // If \u2203 atleast one openStatement in this branch, then it cannot be complete open
        if (currBranch->openStatements.size() > 0) completeOpen = false; 

        currBranch = currBranch->parentBranch;
    }

    if (completeOpen == true) {
        std::cout << this << ", copen\n"; 
        this->status = Status::COMPLETEOPEN;
    }

    return this->status;
}

/** Pops an open statement from branch, keeps checking parent branches when no open statements are found. 
 *  Only called on leaf branches:
 *      string : Ref to save an open statement
 *  Returns pointer to branch that open statement was found in, if no open statement found then NULL
 */
TruthTreeBranch *TruthTreeBranch::popOpenStatement(std::string &string) {
    TruthTreeBranch *b = this;

    // Loop until a branch with open statements is found
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

/** Model constructor, creates the root branch given the premises and conclusion and then generates
 *  a full truth tree from the arguments:
 *      premises   : Vector of premises as strings
 *      conclusion : Conclusion to the argument
 */
TruthTreeModel::TruthTreeModel(const std::vector<std::string> &premises, const std::string &conclusion) : 
    complete {false},
    conclusion {conclusion},
    premises {premises}
{
    std::string negConc = conclusion;
    DecompositionUtil::addNegation(negConc);

    // Create root branch with all premises and negation of the conclusion
    std::vector<std::string> allLines = premises;
    allLines.push_back(negConc);
    openBranches = { new TruthTreeBranch(allLines, NULL) }; 

    std::cout << "\nGenerating truth tree...\n\n";
    
    this->generateTree();
}


/** Model destructor, has to delete TruthTreeBranch pointers created by this truth tree. Works by
 *  starting from the bottom of the tree (leafs), adding parents and then deleting TTBranches. 
 */
TruthTreeModel::~TruthTreeModel() {
    std::vector<TruthTreeBranch *> upperBranches {};               // List of TruthTreeBranch pointers to check through on next loop
    std::vector<TruthTreeBranch *> currBranches {closedBranches};  // Current list of TruthTreeBranches to delete
    currBranches.insert(currBranches.end(), completeOpenBranches.begin(), completeOpenBranches.end()); // Adds all leaf branches

    while (!upperBranches.empty() || !currBranches.empty()) { // While there are still TTB pointers
        for (int i {0}; i < currBranches.size(); i++) {
            if (currBranches[i]->parentBranch != NULL && // If upper segment was already deleted
                std::count(currBranches.begin(), currBranches.end(), currBranches[i]->parentBranch) == 0 && // If its about to be deleted this loop
                std::count(upperBranches.begin(), upperBranches.end(), currBranches[i]->parentBranch) == 0) // If its already appended to next deletion list
                upperBranches.push_back(currBranches[i]->parentBranch);
            
            delete currBranches[i];

            currBranches[i] = NULL; // Mark branch as NULL for later iterations
        }

        // Reset lists for next loop
        currBranches = upperBranches;
        upperBranches = {};
    }
}

/** Main TruthTreeModel method, this will generate the entire Truth Tree, stopping only when every branch is either
 *  complete open or closed.
 */
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
    
    this->complete = true;

    this->printModel();
}

void TruthTreeModel::printModel() {
    std::cout << "\nTruth Tree Generation done...\n\n";
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

            // If current branch is leaf
            if (cB->status == TruthTreeBranch::Status::OPEN) {
                lB = new TruthTreeBranch(lLines, cB);
                rB = new TruthTreeBranch(rLines, cB);

                // Split the branch given the decomposition and set status of branch to intermediate
                cB->leftBranch = lB;
                cB->rightBranch = rB;
                cB->status = TruthTreeBranch::Status::INT;
                
                // Find current branch is remove it from the model (its now intermediate)
                for (int i {}; i < this->openBranches.size(); i++) {
                    if (this->openBranches[i] == cB) {
                        this->openBranches.erase(this->openBranches.begin() + i);
                        break;
                    }
                }
                
                // Add new leaf branches to the model
                this->openBranches.push_back(lB);
                this->openBranches.push_back(rB);
            } 
            else if (cB->status == TruthTreeBranch::Status::INT) {
                if (cB->leftBranch != NULL)
                    bs.push(cB->leftBranch);
                if (cB->rightBranch != NULL)
                    bs.push(cB->rightBranch);
            }
        }

        branch->update(statement, {}); // Mark statement as decomposed
        branch->status = TruthTreeBranch::Status::INT;
    } 
    // No splitting, append new statements to branch
    else {
        branch->update(statement, decomposition); // Mark statement and append new statements

        std::vector<std::string> literals {};
        for (auto s : decomposition) {
            if (DecompositionUtil::isLiteral(s)) literals.push_back(s);
        }

        std::stack<TruthTreeBranch*> bs {};
        bs.push(branch);

        if (!literals.empty()) {
            // Update all child leaf branches, or just self branch
            while (!bs.empty()) {
                TruthTreeBranch *cB = bs.top();
                bs.pop();

                if (cB->leftBranch != NULL) {
                    if (cB->leftBranch->status == TruthTreeBranch::Status::OPEN) 
                        cB->evaluateBranch(literals);
                    else if (cB->leftBranch->status == TruthTreeBranch::Status::INT) 
                        bs.push(cB->leftBranch);
                }
                if (cB->rightBranch != NULL) {
                    if (cB->rightBranch->status == TruthTreeBranch::Status::OPEN) 
                        cB->evaluateBranch(literals);
                    else if (cB->rightBranch->status == TruthTreeBranch::Status::INT) 
                        bs.push(cB->leftBranch);
                }
            }
        }
    }

    return split;
}