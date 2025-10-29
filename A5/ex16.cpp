#include <bits/stdc++.h>
using namespace std;

// Trim whitespace
string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Integer helpers
bool isInteger(const string &s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '-' || s[0] == '+') i = 1;
    for (; i < s.size(); ++i)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

long long toInt(const string &s) { return stoll(s); }

// IR instruction representation
struct Instr {
    string raw;
    string lhs, op1, op, op2;
    bool isBinary = false;
    bool isAssign = false;
    bool other = false;
};

// Parse one line of code into an Instr
Instr parseLine(const string &ln) {
    Instr ins;
    ins.raw = ln;
    string s = trim(ln);
    if (s.empty()) { ins.other = true; return ins; }
    if (s.back() == ':') { ins.other = true; return ins; }

    vector<string> keywords = {
        "if ", "goto ", "call ", "param ", "return ", "print ", "read "
    };
    for (auto &k : keywords)
        if (s.rfind(k, 0) == 0) { ins.other = true; return ins; }

    auto eq = s.find('=');
    if (eq == string::npos) { ins.other = true; return ins; }

    ins.lhs = trim(s.substr(0, eq));
    string rhs = trim(s.substr(eq + 1));

    vector<string> tokens;
    {
        string t;
        istringstream iss(rhs);
        while (iss >> t) tokens.push_back(t);
    }

    if (tokens.size() == 3) {
        ins.isBinary = true;
        ins.op1 = tokens[0];
        ins.op = tokens[1];
        ins.op2 = tokens[2];
    } else if (tokens.size() == 1) {
        ins.isAssign = true;
        ins.op1 = tokens[0];
    } else {
        ins.other = true;
    }
    return ins;
}

// Power-of-two test
bool isPowerOfTwo(long long n, int &k) {
    if (n <= 0) return false;
    if ((n & (n - 1)) != 0) return false;
#if defined(__GNUC__)
    k = __builtin_ctzll(n);
#else
    k = (int)log2(n);
#endif
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> lines;
    string line;
    while (getline(cin, line))
        lines.push_back(line);

    vector<Instr> ins;
    for (auto &ln : lines)
        ins.push_back(parseLine(ln));

    unordered_map<string, optional<long long>> cval;

    auto getConst = [&](const string &s) -> optional<long long> {
        if (isInteger(s)) return toInt(s);
        auto it = cval.find(s);
        if (it != cval.end()) return it->second;
        return nullopt;
    };

    bool changed = true;
    int passes = 0;

    while (changed && passes < 10) {
        changed = false;
        ++passes;

        for (auto &I : ins) {
            if (I.isBinary) {
                auto c1 = getConst(I.op1);
                auto c2 = getConst(I.op2);

                // Constant folding
                if (c1 && c2) {
                    long long a = *c1, b = *c2, res = 0;
                    bool foldable = true;
                    if (I.op == "+") res = a + b;
                    else if (I.op == "-") res = a - b;
                    else if (I.op == "*") res = a * b;
                    else if (I.op == "/") { if (b == 0) foldable = false; else res = a / b; }
                    else if (I.op == "%") { if (b == 0) foldable = false; else res = a % b; }
                    else if (I.op == "<<") res = a << b;
                    else if (I.op == ">>") res = a >> b;
                    else foldable = false;

                    if (foldable) {
                        string newraw = I.lhs + " = " + to_string(res);
                        if (I.raw != newraw) { I.raw = newraw; changed = true; }
                        I.isBinary = false;
                        I.isAssign = true;
                        I.op1 = to_string(res);
                        I.op = "";
                        I.op2 = "";
                        cval[I.lhs] = res;
                        continue;
                    }
                }

                // Simplifications
                if (I.op == "+" && ((c2 && *c2 == 0) || (c1 && *c1 == 0))) {
                    string keep = (c2 && *c2 == 0) ? I.op1 : I.op2;
                    string newraw = I.lhs + " = " + keep;
                    if (I.raw != newraw) { I.raw = newraw; changed = true; }
                    I.isBinary = false;
                    I.isAssign = true;
                    I.op1 = keep;
                    cval[I.lhs] = isInteger(keep) ? optional<long long>(toInt(keep)) : nullopt;
                    continue;
                }

                if (I.op == "-" && (c2 && *c2 == 0)) {
                    string newraw = I.lhs + " = " + I.op1;
                    if (I.raw != newraw) { I.raw = newraw; changed = true; }
                    I.isBinary = false;
                    I.isAssign = true;
                    I.op1 = I.op1;
                    cval[I.lhs] = isInteger(I.op1) ? optional<long long>(toInt(I.op1)) : nullopt;
                    continue;
                }

                if (I.op == "*") {
                    if ((c2 && *c2 == 1) || (c1 && *c1 == 1)) {
                        string keep = (c2 && *c2 == 1) ? I.op1 : I.op2;
                        string newraw = I.lhs + " = " + keep;
                        if (I.raw != newraw) { I.raw = newraw; changed = true; }
                        I.isBinary = false;
                        I.isAssign = true;
                        I.op1 = keep;
                        cval[I.lhs] = isInteger(keep) ? optional<long long>(toInt(keep)) : nullopt;
                        continue;
                    }
                    if ((c2 && *c2 == 0) || (c1 && *c1 == 0)) {
                        string newraw = I.lhs + " = 0";
                        if (I.raw != newraw) { I.raw = newraw; changed = true; }
                        I.isBinary = false;
                        I.isAssign = true;
                        I.op1 = "0";
                        cval[I.lhs] = 0;
                        continue;
                    }
                    if (c2 && *c2 > 0) {
                        int k;
                        if (isPowerOfTwo(*c2, k)) {
                            string newraw = I.lhs + " = " + I.op1 + " << " + to_string(k);
                            if (I.raw != newraw) { I.raw = newraw; changed = true; }
                            I.op = "<<"; I.op2 = to_string(k);
                            continue;
                        }
                    }
                    if (c1 && *c1 > 0) {
                        int k;
                        if (isPowerOfTwo(*c1, k)) {
                            string newraw = I.lhs + " = " + I.op2 + " << " + to_string(k);
                            if (I.raw != newraw) { I.raw = newraw; changed = true; }
                            I.op1 = I.op2; I.op = "<<"; I.op2 = to_string(k);
                            continue;
                        }
                    }
                }

                if (I.op == "-" && I.op1 == I.op2) {
                    string newraw = I.lhs + " = 0";
                    if (I.raw != newraw) { I.raw = newraw; changed = true; }
                    I.isBinary = false;
                    I.isAssign = true;
                    I.op1 = "0";
                    cval[I.lhs] = 0;
                    continue;
                }

                // Substitute known constants
                string left = I.op1, right = I.op2;
                bool replaced = false;
                if (!isInteger(left)) {
                    auto c = getConst(left);
                    if (c) { left = to_string(*c); replaced = true; }
                }
                if (!isInteger(right)) {
                    auto c = getConst(right);
                    if (c) { right = to_string(*c); replaced = true; }
                }
                if (replaced) {
                    string newrhs = left + " " + I.op + " " + right;
                    string newraw = I.lhs + " = " + newrhs;
                    if (I.raw != newraw) { I.raw = newraw; changed = true; }
                    I.op1 = left; I.op2 = right;
                }
                cval.erase(I.lhs);
            } else if (I.isAssign) {
                if (isInteger(I.op1)) {
                    long long v = toInt(I.op1);
                    if (!cval.count(I.lhs) || cval[I.lhs] != optional<long long>(v)) {
                        cval[I.lhs] = v;
                        changed = true;
                    }
                } else {
                    auto c = cval.find(I.op1);
                    if (c != cval.end() && c->second.has_value()) {
                        cval[I.lhs] = c->second;
                        string newraw = I.lhs + " = " + to_string(*c->second);
                        if (I.raw != newraw) { I.raw = newraw; changed = true; }
                    } else {
                        if (cval.count(I.lhs)) { cval.erase(I.lhs); changed = true; }
                    }
                }
            }
        }
    }

    // Final output
    for (auto &I : ins)
        cout << I.raw << "\n";
}
