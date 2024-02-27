#include <string_view>
#include <vector>

#include "TruthTreeModel.h"
#include "LogicStr.h"

/** Branch constructor, evaluates status after lines are processed:
 *      lines  : All statements that will be placed in this segment
 *      parent : Pointer to the parent TruthTreeBranch 
 */
TruthTreeBranch::TruthTreeBranch(const std::vector<Statement> &lines, TruthTreeBranch *parent) :
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
        if (s.isLiteral) 
            literals.push_back(s.value);
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
std::vector<std::string> TruthTreeBranch::update(const Statement &statement, const std::vector<Statement> &decomposition) {
    std::vector<std::string> newLiterals {};

    // Adds any new statements to the branch
    for (auto &s : decomposition) {
        if (s.isLiteral) {
            literals.push_back(s.value);
            newLiterals.push_back(s.value);
        }
        else
            openStatements.push_back(s);
    }
    
    // Update call wants to mark statement in this branch as decomposed
    if (statement.value != "")
        decomposedStatements.push_back(statement);

    // Branch must be leaf and there must've been new statements added for a re-evaluation of status
    if (!decomposition.empty() && status != Status::INT) 
        this->evaluateBranch(newLiterals);
    return newLiterals;
}

/** Given a set of new literals, this function will check for any contradiction in the branch or
 *  if there are no open statements left in the full branch and will update the status of this branch
 *  accordingly. Only called on leaf branches:
 *      newLiterals : Any literals that were just decomposed within this branch 
 */
TruthTreeBranch::Status TruthTreeBranch::evaluateBranch(const std::vector<std::string> &newLiterals) {
    bool completeOpen {true};

    // Compare new literals to every other literal in branch to find contradiction
    // Also check if branch has any open statements, if so mark completeOpen false
    TruthTreeBranch *currBranch = this;

    while (currBranch != NULL) { // Loop over all TT segments
        for (int i {0}; i < newLiterals.size(); i++) { // Loop over all new literals
            for (int j {0}; j < currBranch->literals.size(); j++) { // Loop over all literals in currBranch
                if (StatementUtil::isNegations(newLiterals[i], currBranch->literals[j])) {
                    std::cout << "\t\t\tEvaluated branch: " << this << ", status: closed\n"; 
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
        std::cout << "\t\t\tEvaluated branch: " << this << ", status: copen\n"; 
        this->status = Status::COMPLETEOPEN;
    }

    return this->status;
}

/** Pops an open statement from branch, keeps checking parent branches when no open statements are found. 
 *  Only called on leaf branches:
 *      string : Ref to save an open statement
 *  Returns pointer to branch that open statement was found in, if no open statement found then NULL
 */
TruthTreeBranch *TruthTreeBranch::popOpenStatement(Statement &s) {
    TruthTreeBranch *b = this;

    // Loop until a branch with open statements is found
    while (b->openStatements.size() == 0) {
        b = b->parentBranch;

        if (b == NULL) { 
            s = {""}; 
            return NULL; 
        }
    }

    s = b->openStatements.back();
    b->openStatements.pop_back();

    return b;
}

void TruthTreeBranch::printBranch() {
    for (auto s: literals) {
        std::cout << "\t\t" << s << "\n";
    }
    for (auto s: openStatements) {
        std::cout << "\t\t" << s.value << " o\n";
    }
    for (auto s: decomposedStatements) {
        std::cout << "\t\t" << s.value << " d\n";
    }
}

/** Model constructor, creates the root branch given the premises and conclusion and then generates
 *  a full truth tree from the arguments:
 *      premises   : Vector of premises as strings
 *      conclusion : Conclusion to the argument
 */
TruthTreeModel::TruthTreeModel(const std::vector<std::string> &premises, const std::string &conclusion) : 
    complete {false},
    premises {} {
    for (auto &premise : premises) this->premises.push_back(LogicStr(premise));
    this->conclusion { LogicStr(conclusion) };

    Statement negConc {StatementUtil::initializeStatement(conclusion, true)};
    StatementUtil::addNegation(negConc);

    // Create root branch with all premises and negation of the conclusion
    std::vector<Statement> allLines;

    for (auto &str : premises) 
        allLines.push_back(StatementUtil::initializeStatement(str, true));
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
            if (currBranches[i]->parentBranch != NULL &&                                                    // If upper segment was already deleted
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

/**
 * 
 */
void TruthTreeModel::parselang() {
    std::vector<LogicStr> formulae = this->premises;
    formulae.push_back(this->conclusion);
    
    const std::string pred_letters {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    const std::string lowercase_letters {"abcdefghijklmnopqrstuvwxyz"};
    const std::string quantifiers {"∀∃"};
    bool prev_quant = false;

    for (auto &formula : formulae) {
        for (auto it = formula.begin(); it != formula.end(); it++) {
            uchar *uc = *it;
            if (uc->len != 1) {
                if (quantifiers.find(uc->val) != std::string::npos) {
                    prev_quant = true;
                    is_decision_procedure = false;
                }
                else prev_quant = false;
                continue;
            }

            char ch = *(uc->val);
            if (pred_letters.find(ch) != std::string::npos){
                int places = 0;
                auto it2 = it;
                it2++;
                while (it2 != formula.end() && lowercase_letters.find(*((*it2)->val)) != std::string::npos) { 
                    it2++;
                    places++;
                }

                if (predicates.find(ch) != predicates.end()) 
                    predicates[ch].insert(places);
                else
                    predicates.insert({ch,std::unordered_set<int>{places}});
            }
            else if (lowercase_letters.find(ch) != std::string::npos && prev_quant && nameletters.find(ch) == nameletters.end())
                variables.insert(ch);
            else if (lowercase_letters.find(ch) != std::string::npos && variables.find(ch) == variables.end())
                nameletters.insert(ch);   
            prev_quant = false;
        }
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
            std::cout << "\t" << s.value << " o\n";
        }
        for (auto s: currBranch->decomposedStatements) {
            std::cout << "\t" << s.value << " d\n\n";
        }

        if (currBranch->status == TruthTreeBranch::Status::OPEN) {  
            std::cout << "\t\tStatus: open\n";  
            Statement s {};
            TruthTreeBranch *br {currBranch->popOpenStatement(s)};
            std::cout << "\t\tPopped statement " << s.value << ", from branch: " << br << "\n";

            while (br != NULL) {
                bool split = this->applyDecompositionRule(br, s);
                
                br = currBranch->popOpenStatement(s);
                std::cout << "\t\tPopped statement " << s.value << ", from branch: " << br << "\n";
            }   
            std::cout << "\t\tStopped decomposing in currBranch: " << currBranch << "\n";
        }

        if (currBranch->status == TruthTreeBranch::Status::COMPLETEOPEN) { std::cout << "\t\tStatus: COpen\n";  this->completeOpenBranches.push_back(currBranch); }
        else if (currBranch->status == TruthTreeBranch::Status::CLOSED) { std::cout << "\t\tStatus: Closed\n";  this->closedBranches.push_back(currBranch); }
        else { std::cout << "\t\tStatus: INT\n"; }
    }
    
    this->complete = true;
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
bool TruthTreeModel::applyDecompositionRule(TruthTreeBranch *branch, const Statement &statement) {
    std::vector<Statement> decomposition {};

    std::cout << "\t\t\tdecompose started on " << statement.value << " ...\n";
    bool split = StatementUtil::decompose(statement, &decomposition);
    std::cout << "\t\t\tdecompose done on " << statement.value << " ...\n";

    // Decomposition caused split in branch
    if (split) {
        std::cout << "\t\t\tsplit true\n";
        std::vector<Statement> lLines, rLines;

        // Split decomposition into left and right branches
        for (int i {0}; i < decomposition.size(); i++) {
            if (i < decomposition.size()/2) lLines.push_back(decomposition.at(i));
            else rLines.push_back(decomposition.at(i));
        }

        std::cout << "\t\t\tCreating new branches ...\n";

        TruthTreeBranch *lB, *rB;
        std::stack<TruthTreeBranch*> bs {};
        bs.push(branch);
        int i{0};

        while (!bs.empty()) {
            TruthTreeBranch *cB = bs.top();
            bs.pop();

            // If current branch is leaf
            if (cB->status == TruthTreeBranch::Status::OPEN) {
                lB = new TruthTreeBranch(lLines, cB);
                rB = new TruthTreeBranch(rLines, cB);
                i += 2;

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
        std::cout << "\t\t\tBranches created: " << i << "\n";
        for (auto &s : lLines) std::cout << "\t\t\t\t" << s.value << "\n";
        for (auto &s : rLines) std::cout << "\t\t\t\t" << s.value << "\n";

        branch->update(statement, {}); // Mark statement as decomposed
        branch->status = TruthTreeBranch::Status::INT;
    } 
    // No splitting, append new statements to branch
    else {
        std::cout << "\t\t\tsplit false\n";

        std::vector<std::string> literals {branch->update(statement, decomposition)}; // Mark statement and append new statements

        std::cout << "\t\t\tDecomp created:\n";
        for (auto &s : decomposition) std::cout << "\t\t\t\t" << s.value << "\n";

        std::stack<TruthTreeBranch*> bs {};
        bs.push(branch);

        if (!literals.empty()) {
            // Update all child leaf branches, or just self branch
            while (!bs.empty()) {
                TruthTreeBranch *cB = bs.top();
                bs.pop();

                if (cB->leftBranch != NULL) {
                    if (cB->leftBranch->status == TruthTreeBranch::Status::OPEN) 
                        cB->leftBranch->evaluateBranch(literals);
                    else if (cB->leftBranch->status == TruthTreeBranch::Status::INT) 
                        bs.push(cB->leftBranch);
                }
                if (cB->rightBranch != NULL) {
                    if (cB->rightBranch->status == TruthTreeBranch::Status::OPEN) 
                        cB->rightBranch->evaluateBranch(literals);
                    else if (cB->rightBranch->status == TruthTreeBranch::Status::INT) 
                        bs.push(cB->leftBranch);
                }
            }
        }
    }

    return split;
}