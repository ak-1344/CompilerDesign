// FIRST set calculator for context-free grammars. Compile: g++ ex3.cpp -o ex3 && ./ex3
#include <bits/stdc++.h>
using namespace std;
using Set = set<char>;
// helper: print set
string set_to_str(const Set &s) {
string out;
for (char c : s) {
out.push_back(c);
out.push_back(',');
}
if (!out.empty())
out.pop_back();
return out;
}
int idx_of(const vector<char> &v, char c) {
for (size_t i = 0; i < v.size(); ++i)
if (v[i] == c)
return (int)i;
return -1;
}
int main() {
cout << "Enter number of productions: ";
int n;
if (!(cin >> n))
return 0;
vector<string> prod(n);
cout << "Enter the productions (format A->aB etc). Terminals and "
"nonterminals should be single chars.\n";
for (int i = 0; i < n; ++i) {
cin >> prod[i];
// allow both A->... and A->
// we'll assume '->' at positions 1 and 2 or at index 1 for simplicity
// normalize to form "A->rhs"
if (prod[i].size() >= 2 && prod[i][1] != '-') {
// maybe format like "A=abc"? leave as-is
}
}
// collect nonterminals (left sides) and terminals (symbols appearing on RHS
// that are not nonterminals)
vector<char> NT; // nonterminals
vector<char> T; // terminals
for (int i = 0; i < n; ++i) {
char A = prod[i][0];if (idx_of(NT, A) == -1)
NT.push_back(A);
}
// find RHS characters (starting after '->' which we assume starts at index 2
// or 3).
for (int i = 0; i < n; ++i) {
string &p = prod[i];
// find start of RHS
size_t rhs_start = p.find("->");
if (rhs_start == string::npos)
rhs_start = 2;
else
rhs_start += 2;
for (size_t k = rhs_start; k < p.size(); ++k) {
char ch = p[k];
if (ch == ' ' || ch == '\t')
continue;
if (idx_of(NT, ch) == -1 && idx_of(T, ch) == -1) {
// if not a known nonterminal, treat as terminal for now
if (!isupper(ch)) // heuristic: uppercase = nonterminal
T.push_back(ch);
else {
// uppercase may be a nonterminal if it appears on LHS earlier; check:
if (idx_of(NT, ch) == -1) {
// treat uppercase not in NT as terminal (or possibly missing LHS)
T.push_back(ch);
}
}
}
}
}
// If "$" not in terminals, add it as end marker
if (idx_of(T, '$') == -1)
T.push_back('$');
// map nonterminal -> leading / trailing sets (terminals)
unordered_map<char, Set> LEAD, TRAIL;
// initialize empty sets
for (char A : NT) {
LEAD[A] = Set();
TRAIL[A] = Set();
}
// Helper lambdas to extract RHS and iterate
auto rhs_of = [&](int i) -> string {
size_t rhs_start = prod[i].find("->");
if (rhs_start == string::npos)
rhs_start = 2;else
rhs_start += 2;
return prod[i].substr(rhs_start);
};
// Compute LEADING
// Rule: If A -> a... then a ∈ LEADING(A) (if a is terminal)
// If A -> B... and B is nonterminal, then LEADING(B) subset of LEADING(A)
// We'll do a fixed-point iterative closure.
bool changed = true;
while (changed) {
changed = false;
for (int i = 0; i < n; ++i) {
char A = prod[i][0];
string rhs = rhs_of(i);
if (rhs.empty())
continue;
// first symbol
char x = rhs[0];
if (idx_of(T, x) != -1) {
if (LEAD[A].insert(x).second)
changed = true;
} else if (idx_of(NT, x) != -1) {
// add all LEAD[x] to LEAD[A]
for (char t : LEAD[x])
if (LEAD[A].insert(t).second)
changed = true;
// also if there is terminal after the nonterminal: A -> B a ...,
// collect a But the iterative approach will propagate via other rules.
}
// Also, if first symbol is nonterminal followed by terminal, include that
// terminal:
for (size_t k = 0; k + 1 < rhs.size(); ++k) {
char c1 = rhs[k], c2 = rhs[k + 1];
if (idx_of(NT, c1) != -1 && idx_of(T, c2) != -1) {
if (LEAD[A].insert(c2).second)
changed = true;
}
}
}
}
// Compute TRAILING similar but using last symbol
changed = true;
while (changed) {
changed = false;
for (int i = 0; i < n; ++i) {
char A = prod[i][0];
string rhs = rhs_of(i);
if (rhs.empty())
continue;char x = rhs[rhs.size() - 1];
if (idx_of(T, x) != -1) {
if (TRAIL[A].insert(x).second)
changed = true;
} else if (idx_of(NT, x) != -1) {
for (char t : TRAIL[x])
if (TRAIL[A].insert(t).second)
changed = true;
}
// also if terminal followed by nonterminal anywhere -> add that terminal
for (int k = (int)rhs.size() - 2; k >= 0; --k) {
char c1 = rhs[k], c2 = rhs[k + 1];
if (idx_of(T, c1) != -1 && idx_of(NT, c2) != -1) {
if (TRAIL[A].insert(c1).second)
changed = true;
}
}
}
}
// Print LEADING and TRAILING
cout << "\nLEADING sets:\n";
for (char A : NT) {
cout << "LEADING[" << A << "] = { ";
bool first = true;
for (char t : LEAD[A]) {
if (!first)
cout << ", ";
cout << t;
first = false;
}
cout << " }\n";
}
cout << "\nTRAILING sets:\n";
for (char A : NT) {
cout << "TRAILING[" << A << "] = { ";
bool first = true;
for (char t : TRAIL[A]) {
if (!first)
cout << ", ";
cout << t;
first = false;
}
cout << " }\n";
}
// Build operator-precedence table among terminals.
// Initialize table with ' ' (no relation). We'll use chars: '<', '>', '=', '
// '.
int Tn = (int)T.size();
vector<vector<char>> table(Tn, vector<char>(Tn, ' ')); // table[i][j] relation between T[i] and T[j]
// for each production, for adjacent symbols a b:
// if a and b are terminals: set a = b
// if a terminal followed by nonterminal B: for every t in LEADING(B): set a <
// t if nonterminal A followed by terminal b: for every t in TRAILING(A): set
// t > b
for (int i = 0; i < n; ++i) {
string rhs = rhs_of(i);
for (size_t k = 0; k + 1 < rhs.size(); ++k) {
char a = rhs[k], b = rhs[k + 1];
if (idx_of(T, a) != -1 && idx_of(T, b) != -1) {
int ia = idx_of(T, a), ib = idx_of(T, b);
table[ia][ib] = '=';
}
if (idx_of(T, a) != -1 && idx_of(NT, b) != -1) {
int ia = idx_of(T, a);
for (char t : LEAD[b]) {
int it = idx_of(T, t);
if (it != -1)
table[ia][it] = '<';
}
}
if (idx_of(NT, a) != -1 && idx_of(T, b) != -1) {
int ib = idx_of(T, b);
for (char t : TRAIL[a]) {
int it = idx_of(T, t);
if (it != -1)
table[it][ib] = '>';
}
}
// case A B C where a is terminal, b nonterminal, c terminal -> a < c and
// maybe a = b? already handled
if (k + 2 < rhs.size()) {
char c = rhs[k + 2];
if (idx_of(T, a) != -1 && idx_of(NT, b) != -1 && idx_of(T, c) != -1) {
int ia = idx_of(T, a), ic = idx_of(T, c);
table[ia][ic] = '=';
}
}
}
}
// Usually we also set $ relations: $ < leading(S) and trailing(S) > $
// find start symbol as left side of first production
char start = prod[0][0];
int id_dollar = idx_of(T, '$');
if (id_dollar != -1) {
for (char t : LEAD[start]) {
int it = idx_of(T, t);
if (it != -1)table[id_dollar][it] = '<';
}
for (char t : TRAIL[start]) {
int it = idx_of(T, t);
if (it != -1)
table[it][id_dollar] = '>';
}
// $ = $ maybe not needed, but set to blank
}
// Print precedence table
cout << "\nOperator-precedence table (rows = stack-top terminal, cols = "
"input terminal):\n ";
for (char t : T)
cout << t << " ";
cout << "\n +" << string(2 * Tn, '-') << "\n";
for (int i = 0; i < Tn; ++i) {
cout << " " << T[i] << " | ";
for (int j = 0; j < Tn; ++j) {
cout << (table[i][j] == ' ' ? '.' : table[i][j]) << " ";
}
cout << "\n";
}
// ---- Parser (operator-precedence) ----
cout << "\nEnter input string (use single-char terminals, end with $): ";
string input;
cin >> input;
// ensure input ends with $
if (input.back() != '$')
input.push_back('$');
// stack holds symbols (we'll store as chars). For parser we need to find the
// rightmost terminal on stack to compare precedence.
vector<char> stack_sym;
stack_sym.push_back('$'); // bottom marker
// for readability we show actions
auto print_state = [&](const vector<char> &stk, const string &inp,
const string &action) {
// print stack as string
cout << setw(20);
string s;
for (char c : stk)
s.push_back(c);
cout << s << " ";
cout << setw(20) << inp << " ";
cout << action << "\n";
};
cout << "\nParsing steps:\n";cout << setw(20) << "Stack" << setw(25) << "Input" << setw(15) << "Action"
<< "\n";
cout << string(60, '-') << "\n";
string action;
size_t ip = 0;
// helper: find index of rightmost terminal in stack
auto rightmost_terminal_pos = [&](const vector<char> &stk) -> int {
for (int i = (int)stk.size() - 1; i >= 0; --i) {
if (idx_of(T, stk[i]) != -1)
return i;
}
return -1;
};
bool accepted = false;
int safety = 0;
while (true) {
// produce readable input remnant
string rem = input.substr(ip);
print_state(stack_sym, rem, "");
if (safety++ > 1000) {
cout << "Parsing loop limit reached. Aborting.\n";
break;
}
// find topmost terminal on stack
int pos = rightmost_terminal_pos(stack_sym);
if (pos == -1) {
cout << "No terminal on stack (error)\n";
break;
}
char a = stack_sym[pos];
char b = input[ip];
int ia = idx_of(T, a), ib = idx_of(T, b);
char rel = ' ';
if (ia != -1 && ib != -1)
rel = table[ia][ib];
if (a == '$' && b == '$') {
action = "ACCEPT";
print_state(stack_sym, rem, action);
accepted = true;
break;
}
if (rel == '<' || rel == '=') {
// SHIFT
action = string("SHIFT ") + b;
// push the input symbol onto stack
stack_sym.push_back(b);ip++;
print_state(stack_sym, input.substr(ip), action);
continue;
} else if (rel == '>') {
// REDUCE: find handle between nearest terminal t (at pos) and the top of
// stack According to operator-precedence algorithm, find nearest terminal
// to left having '<' relation with the terminal at or to the right. We'll
// find the leftmost terminal index lpos such that relation
// table[lpos][ib_top] is '<' where ib_top is index of the terminal at
// topmost terminal pos For simplicity: find leftmost terminal index 'l'
// scanning back from pos-1 until either we find table[ idx(T,
// stack_sym[l]) ][ idx(T, stack_sym[pos]) ] == '<' OR l==0 Then the
// handle is everything after that terminal.
int top_term_pos = rightmost_terminal_pos(stack_sym); // pos
int lpos = -1;
for (int k = top_term_pos - 1; k >= 0; --k) {
if (idx_of(T, stack_sym[k]) != -1) {
int ik = idx_of(T, stack_sym[k]);
int itop = idx_of(T, stack_sym[top_term_pos]);
if (table[ik][itop] == '<') {
lpos = k;
break;
}
}
}
if (lpos == -1) {
// if none found, set lpos to 0 (bottom)
lpos = 0;
}
// handle is substring stack_sym[lpos+1 ... end]
int handle_start = lpos + 1;
int handle_len = (int)stack_sym.size() - handle_start;
string handle;
for (int k = handle_start; k < (int)stack_sym.size(); ++k)
handle.push_back(stack_sym[k]);
// try to match handle to some production RHS
bool reduced = false;
char reduce_to = 'N'; // default nonterminal placeholder
for (int p = 0; p < n; ++p) {
string rhs = rhs_of(p);
if (rhs == handle) {
// reduce by replacing handle with LHS
reduce_to = prod[p][0];
// pop handle symbols
for (int k = 0; k < handle_len; ++k)
stack_sym.pop_back();
stack_sym.push_back(reduce_to);
action = string("REDUCE by ") + prod[p];
reduced = true;
break;}
}
if (!reduced) {
// No exact match -> collapse handle to 'N' nonterminal (to continue)
for (int k = 0; k < handle_len; ++k)
stack_sym.pop_back();
stack_sym.push_back(reduce_to);
action = string("REDUCE (general) replace '") + handle + "' by N";
}
print_state(stack_sym, input.substr(ip), action);
continue;
} else {
// no relation defined -> error
action = "ERROR: no precedence relation between ";
action.push_back(a);
action.push_back('/');
action.push_back(b);
print_state(stack_sym, rem, action);
break;
}
} // end parsing loop
if (accepted)
cout << "\nInput accepted by operator-precedence parser.\n";
else
cout << "\nInput rejected (or error occurred).\n";
return 0;
}