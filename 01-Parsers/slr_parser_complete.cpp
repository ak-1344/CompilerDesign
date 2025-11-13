// FOLLOW set calculator for context-free grammars. Compile: g++ ex4.cpp -o ex4 && ./ex4
#include <bits/stdc++.h>
using namespace std;

struct Production {
    string left;
    string right;
};

struct Item {
    string left;
    string right;
    int dotPos;
};

bool operator==(const Item& a, const Item& b) {
    return a.left == b.left && a.right == b.right && a.dotPos == b.dotPos;
}

bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

vector<Production> grammar;
map<string, set<char>> FIRST;
map<string, set<char>> FOLLOW;
map<int, map<char, string>> ACTION;
map<int, map<char, int>> GOTO;
vector<vector<Item>> states;

// Helper: print set<char> nicely
string setToString(const set<char>& s) {
    string out = "{";
    bool first = true;
    for (char c : s) {
        if (!first) out += ", ";
        first = false;
        if (c == '#') out += "ε";
        else out.push_back(c);
    }
    out += "}";
    return out;
}

// -------------------------
// Compute closure(I)
// -------------------------
vector<Item> closure(vector<Item> I) {
    bool changed = true;
    while (changed) {
        changed = false;
        vector<Item> newItems = I;
        for (auto& item : I) {
            if (item.dotPos < (int)item.right.size()) {
                char next = item.right[item.dotPos];
                if (isNonTerminal(next)) {
                    for (auto& prod : grammar) {
                        if (prod.left.size() > 0 && prod.left[0] == next) {
                            Item newItem = {prod.left, prod.right, 0};
                            if (find(newItems.begin(), newItems.end(), newItem) == newItems.end()) {
                                newItems.push_back(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        I = newItems;
    }
    return I;
}

// -------------------------
// GOTO(I, X)
// -------------------------
vector<Item> GOTO_func(const vector<Item>& I, char X) {
    vector<Item> J;
    for (auto& item : I) {
        if (item.dotPos < (int)item.right.size() && item.right[item.dotPos] == X) {
            J.push_back({item.left, item.right, item.dotPos + 1});
        }
    }
    return closure(J);
}

// Compare two item sets
bool sameItemSet(const vector<Item>& a, const vector<Item>& b) {
    if (a.size() != b.size()) return false;
    for (auto& it : a) {
        if (find(b.begin(), b.end(), it) == b.end()) return false;
    }
    return true;
}

// find state index equal to set, or -1
int findState(const vector<Item>& s) {
    for (int i = 0; i < (int)states.size(); ++i) {
        if (sameItemSet(states[i], s)) return i;
    }
    return -1;
}

// -------------------------
// Build canonical collection of LR(0) items
// -------------------------
void build_LR0_items(const string& startSym) {
    // start item is the augmented production at grammar[0]
    vector<Item> start = { {grammar[0].left, grammar[0].right, 0} };
    start = closure(start);
    states.clear();
    states.push_back(start);

    bool added = true;
    while (added) {
        added = false;
        int n = states.size();
        for (int i = 0; i < n; i++) {
            // try all ASCII symbols that might appear: terminals and nonterminals are discovered from grammar
            // We'll attempt A..Z and common punctuation/identifiers to ensure completeness
            for (char X = 'A'; X <= 'Z'; X++) {
                vector<Item> g = GOTO_func(states[i], X);
                if (!g.empty() && findState(g) == -1) {
                    states.push_back(g);
                    added = true;
                }
            }
            string terminals = "+-*()/i$";
            for (char X : terminals) {
                vector<Item> g = GOTO_func(states[i], X);
                if (!g.empty() && findState(g) == -1) {
                    states.push_back(g);
                    added = true;
                }
            }
        }
    }
}

// -------------------------
// FIRST and FOLLOW computation
// -------------------------
set<char> FIRST_of_string(const string& s) {
    // returns set of chars possibly containing epsilon '#'
    set<char> result;
    if (s.empty()) {
        result.insert('#');
        return result;
    }
    for (int i = 0; i < (int)s.size(); ++i) {
        char X = s[i];
        if (!isNonTerminal(X)) {
            result.insert(X);
            return result;
        } else {
            string sym(1, X);
            for (char c : FIRST[sym]) {
                if (c != '#') result.insert(c);
            }
            if (FIRST[sym].count('#') == 0) {
                return result;
            }
            // else epsilon in FIRST(sym) -> continue to next symbol
        }
    }
    // if all nullable
    result.insert('#');
    return result;
}

void compute_FIRST() {
    FIRST.clear();
    // initialize FIRST for non-terminals and terminals encountered
    for (auto &p : grammar) {
        string A = p.left;
        if (A.size() > 0) {
            if (FIRST.find(A) == FIRST.end()) FIRST[A] = {};
        }
        // also ensure nonterminals for RHS chars
        for (char c : p.right) {
            if (isNonTerminal(c)) {
                string s(1, c);
                if (FIRST.find(s) == FIRST.end()) FIRST[s] = {};
            } else {
                // terminals' FIRST is themselves when referenced
                string s(1, c);
                if (FIRST.find(s) == FIRST.end()) FIRST[s] = {c};
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            string A = p.left;            // left nonterminal (string)
            string alpha = p.right;       // RHS (string of chars)
            // compute FIRST(alpha)
            set<char> firstAlpha = FIRST_of_string(alpha);
            for (char c : firstAlpha) {
                if (FIRST[A].count(c) == 0) {
                    FIRST[A].insert(c);
                    changed = true;
                }
            }
        }
    }
}

void compute_FOLLOW(const string& startSym) {
    FOLLOW.clear();
    // initialize all nonterminals
    for (auto &p : grammar) {
        FOLLOW[p.left]; // default constructed set
    }

    // start symbol contains $
    FOLLOW[startSym].insert('$');

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            string A = p.left;
            string beta = p.right;
            for (int i = 0; i < (int)beta.size(); ++i) {
                char Bc = beta[i];
                if (!isNonTerminal(Bc)) continue;
                string B(1, Bc);
                string rest = beta.substr(i+1);
                set<char> firstRest = FIRST_of_string(rest);
                // add FIRST(rest) - epsilon to FOLLOW(B)
                for (char c : firstRest) {
                    if (c != '#') {
                        if (FOLLOW[B].count(c) == 0) {
                            FOLLOW[B].insert(c);
                            changed = true;
                        }
                    }
                }
                // if FIRST(rest) contains epsilon or rest is empty => add FOLLOW(A) to FOLLOW(B)
                if (rest.empty() || firstRest.count('#')) {
                    for (char c : FOLLOW[A]) {
                        if (FOLLOW[B].count(c) == 0) {
                            FOLLOW[B].insert(c);
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

// -------------------------
// Build ACTION and GOTO tables
// -------------------------
void build_SLR_table() {
    ACTION.clear();
    GOTO.clear();
    int numStates = states.size();
    for (int i = 0; i < numStates; i++) {
        for (auto& item : states[i]) {
            // if item is A -> α . a β
            if (item.dotPos < (int)item.right.size()) {
                char a = item.right[item.dotPos];
                vector<Item> g = GOTO_func(states[i], a);
                int j = findState(g);
                if (j != -1) {
                    if (!isNonTerminal(a)) {
                        ACTION[i][a] = "s" + to_string(j);
                    } else {
                        GOTO[i][a] = j;
                    }
                }
            } else {
                // item is A -> α .
                if (item.left == grammar[0].left) { // augmented start: S' -> S .
                    ACTION[i]['$'] = "acc";
                } else {
                    // reduce by A->α for all a in FOLLOW(A)
                    string rhs = item.right.empty() ? "ε" : item.right;
                    string red = "r(" + item.left + "->" + rhs + ")";
                    for (char a : FOLLOW[item.left]) {
                        // if conflict occurs, this simple implementation overwrites; SLR may have conflicts for some grammars
                        ACTION[i][a] = red;
                    }
                }
            }
        }
    }
}

// -------------------------
// LR Parsing Simulation
// -------------------------
void parseString(string input) {
    input += "$";
    stack<int> stateStack;
    stack<char> symbolStack;
    stateStack.push(0);

    int i = 0;
    cout << "\nStep-by-step Parsing Process:\n";
    cout << "--------------------------------------------\n";
    cout << "Stack\t\tInput\t\tAction\n";
    cout << "--------------------------------------------\n";

    while (true) {
        int s = stateStack.top();
        char a = input[i];
        string action = "";
        if (ACTION.count(s) && ACTION[s].count(a)) action = ACTION[s][a];

        // Print current step
        stack<int> temp = stateStack;
        vector<int> st;
        while (!temp.empty()) { st.push_back(temp.top()); temp.pop(); }
        reverse(st.begin(), st.end());
        for (size_t idx = 0; idx < st.size(); ++idx) {
            if (idx) cout << " ";
            cout << st[idx];
        }
        cout << "\t\t" << input.substr(i) << "\t\t";
        if (action.empty()) cout << "err";
        else cout << action;
        cout << "\n";

        if (action.size() > 0 && action[0] == 's') {
            int t = stoi(action.substr(1));
            symbolStack.push(a);
            stateStack.push(t);
            i++;
        } else if (action.size() > 0 && action[0] == 'r') {
            // format r(A->alpha) or r(A->ε)
            string prod = action.substr(2, action.size() - 3); // A->alpha
            string left = prod.substr(0, prod.find("->"));
            string right = prod.substr(prod.find("->") + 2);
            if (right != "ε") {
                for (int k = 0; k < (int)right.size(); k++) {
                    if (!symbolStack.empty()) symbolStack.pop();
                    if (!stateStack.empty()) stateStack.pop();
                }
            }
            // push left
            symbolStack.push(left[0]);
            int topState = stateStack.top();
            if (GOTO.count(topState) && GOTO[topState].count(left[0])) {
                int t = GOTO[topState][left[0]];
                stateStack.push(t);
            } else {
                cout << "\n❌ ERROR: no GOTO entry after reduction. Rejected.\n";
                return;
            }
        } else if (!action.empty() && action == "acc") {
            cout << "\n✅ Input string ACCEPTED!\n";
            return;
        } else {
            cout << "\n❌ ERROR: Input string REJECTED!\n";
            return;
        }
    }
}

int main() {
    cout << "Enter number of productions: ";
    int n; cin >> n;
    cout << "Enter productions (one per line, e.g., E->E+T). Use single character non-terminals (A-Z) and single-character terminals (e.g. +,i,*,(,)):\n";
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        string left = s.substr(0, s.find("->"));
        string right = s.substr(s.find("->") + 2);
        grammar.push_back({left, right});
    }

    // Augment grammar: insert S' -> S where S is original start (grammar[0].left)
    string originalStart = grammar[0].left;
    grammar.insert(grammar.begin(), {"S'", originalStart});

    // Compute FIRST & FOLLOW
    compute_FIRST();
    compute_FOLLOW(originalStart);

    // Build LR(0) items and SLR table
    build_LR0_items(originalStart);
    build_SLR_table();

    // Print augmented grammar
    cout << "\nAugmented Grammar:\n";
    for (size_t i = 0; i < grammar.size(); ++i) {
        cout << i << ": " << grammar[i].left << " -> ";
        if (grammar[i].right.empty()) cout << "ε";
        else cout << grammar[i].right;
        cout << "\n";
    }

    // Print FIRST sets
    cout << "\nFIRST sets:\n";
    for (auto &pr : FIRST) {
        string key = pr.first;
        if (!key.empty() && isNonTerminal(key[0])) {
            cout << "FIRST(" << key << ") = " << setToString(pr.second) << "\n";
        }
    }

    // Print FOLLOW sets
    cout << "\nFOLLOW sets:\n";
    for (auto &pr : FOLLOW) {
        cout << "FOLLOW(" << pr.first << ") = " << setToString(pr.second) << "\n";
    }

    // -----------------------------
    // Collect symbols and print unified SLR table
    // -----------------------------
    // Collect terminals (characters appearing in RHS that are not nonterminals), plus '$'
    set<char> terminals_set;
    set<char> nonterms_set;
    for (auto &p : grammar) {
        if (!p.left.empty() && isNonTerminal(p.left[0])) nonterms_set.insert(p.left[0]);
        for (char c : p.right) {
            if (isNonTerminal(c)) nonterms_set.insert(c);
            else {
                if (c != '#') terminals_set.insert(c); // ignore epsilon marker if any
            }
        }
    }
    terminals_set.insert('$');

    // create vectors preserving order: terminals first, then nonterminals
    vector<char> terminals(terminals_set.begin(), terminals_set.end());
    vector<char> nonterms(nonterms_set.begin(), nonterms_set.end());

    // Print header
    cout << "\nSLR(1) Parsing Table (unified):\n";
    cout << setw(6) << "State";
    for (char t : terminals) cout << setw(10) << t;
    for (char N : nonterms) cout << setw(10) << N;
    cout << "\n";
    cout << string(6 + 10 * (terminals.size() + nonterms.size()), '-') << "\n";

    // Print rows
    for (int i = 0; i < (int)states.size(); ++i) {
        cout << setw(6) << i;
        // terminals => ACTION entries (shift / reduce / acc)
        for (char t : terminals) {
            string cell = "";
            if (ACTION.count(i) && ACTION[i].count(t)) cell = ACTION[i][t];
            cout << setw(10) << cell;
        }
        // nonterminals => GOTO entries (state numbers)
        for (char N : nonterms) {
            string cell = "";
            if (GOTO.count(i) && GOTO[i].count(N)) cell = to_string(GOTO[i][N]);
            cout << setw(10) << cell;
        }
        cout << "\n";
    }

    // Print states (items) for debugging
    cout << "\nStates (LR(0) items):\n";
    for (int i = 0; i < (int)states.size(); ++i) {
        cout << "I" << i << ":\n";
        for (auto &it : states[i]) {
            cout << "  " << it.left << " -> ";
            for (int k = 0; k < (int)it.right.size(); ++k) {
                if (k == it.dotPos) cout << ".";
                cout << it.right[k];
            }
            if (it.dotPos == (int)it.right.size()) cout << ".";
            cout << "\n";
        }
    }

    string input;
    cout << "\nEnter input string (use i for id): ";
    cin >> input;

    parseString(input);
    return 0;
}
