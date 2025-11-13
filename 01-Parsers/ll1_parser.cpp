// Predictive LL(1) parser: computes FIRST/FOLLOW sets and builds parse table. Compile: g++ ex1.cpp -o ex1 && ./ex1
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;
class PredictiveParser
{
private:
    set<char> terminals;
    set<char> non_terminals;
    vector<pair<char, string>> productions;
    map<char, set<char>> first_sets;
    map<char, set<char>> follow_sets;
    map<pair<char, char>, string> parse_table;
    char start_symbol;

public:
    void inputGrammar()
    {
        int n_terminals, n_non_terminals, n_productions;
        // Input terminals
        cout << "Enter number of terminals: ";
        cin >> n_terminals;
        cout << "Enter terminals (single characters, space separated): ";
        for (int i = 0; i < n_terminals; i++)
        {
            char t;
            cin >> t;
            terminals.insert(t);
        }
        // Input non-terminalscout << "Enter number of non-terminals: ";
        cin >> n_non_terminals;
        cout << "Enter non-terminals (single characters, space separated): ";
        for (int i = 0; i < n_non_terminals; i++)
        {
            char nt;
            cin >> nt;
            non_terminals.insert(nt);
        }
        // Set start symbol as first non-terminal
        start_symbol = *non_terminals.begin();
        // Input productions
        cout << "Enter number of productions: ";
        cin >> n_productions;
        cout << "Enter productions in format 'A->alpha' (use 'e' for epsilon):\n";
        cin.ignore(); // Clear buffer
        for (int i = 0; i < n_productions; i++)
        {
            string production;
            cout << "Production " << (i + 1) << ": ";
            getline(cin, production);
            // Parse production A->alpha
            char lhs = production[0];
            string rhs = production.substr(3); // Skip 'A->'
            productions.push_back({lhs, rhs});
        }
        cout << "\nGrammar input complete!\n";
        displayGrammar();
    }
    void displayGrammar()
    {
        cout << "Terminals: { ";
        for (char t : terminals)
            cout << t << " ";
        cout << "}\n";
        cout << "Non-terminals: { ";
        for (char nt : non_terminals)
            cout << nt << " ";
        cout << "}\n";
        cout << "Start Symbol: " << start_symbol << "\n";
        cout << "Productions:\n";
        for (auto &prod : productions)
        {
            cout << " " << prod.first << " -> " << prod.second << "\n";
        }
    }
    bool isTerminal(char c) { return terminals.find(c) != terminals.end(); }
    bool isNonTerminal(char c)
    {
        return non_terminals.find(c) != non_terminals.end();
    }
    bool isEpsilon(char c) { return c == 'e'; }
    void computeFirst()
    {
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto &prod : productions)
            {
                char lhs = prod.first;
                string rhs = prod.second;
                set<char> old_first = first_sets[lhs];
                // Compute FIRST for this production
                for (int i = 0; i < rhs.length(); i++)
                {
                    char symbol = rhs[i];
                    if (isTerminal(symbol) || isEpsilon(symbol))
                    {
                        first_sets[lhs].insert(symbol);
                        break;
                    }
                    else if (isNonTerminal(symbol))
                    {
                        // Add FIRST(symbol) - {epsilon} to FIRST(lhs)
                        for (char c : first_sets[symbol])
                        {
                            if (!isEpsilon(c))
                            {
                                first_sets[lhs].insert(c);
                            }
                        }
                        // If epsilon not in FIRST(symbol), break
                        if (first_sets[symbol].find('e') == first_sets[symbol].end())
                        {
                            break;
                        } // If we've processed all symbols and all had epsilon
                        if (i == rhs.length() - 1)
                        {
                            first_sets[lhs].insert('e');
                        }
                    }
                }
                if (first_sets[lhs] != old_first)
                {
                    changed = true;
                }
            }
        }
        displayFirstSets();
    }
    void computeFollow()
    {
        // Add $ to FOLLOW of start symbol
        follow_sets[start_symbol].insert('$');
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto &prod : productions)
            {
                char lhs = prod.first;
                string rhs = prod.second;
                for (int i = 0; i < rhs.length(); i++)
                {
                    char symbol = rhs[i];
                    if (isNonTerminal(symbol))
                    {
                        set<char> old_follow = follow_sets[symbol];
                        // Look at symbols after current symbol
                        bool all_epsilon = true;
                        for (int j = i + 1; j < rhs.length(); j++)
                        {
                            char next_symbol = rhs[j];
                            if (isTerminal(next_symbol))
                            {
                                follow_sets[symbol].insert(next_symbol);
                                all_epsilon = false;
                                break;
                            }
                            else if (isNonTerminal(next_symbol))
                            { // Add FIRST(next_symbol) - {epsilon} to FOLLOW(symbol)
                                for (char c : first_sets[next_symbol])
                                {
                                    if (!isEpsilon(c))
                                    {
                                        follow_sets[symbol].insert(c);
                                    }
                                }
                                // If epsilon not in FIRST(next_symbol), break
                                if (first_sets[next_symbol].find('e') ==
                                    first_sets[next_symbol].end())
                                {
                                    all_epsilon = false;
                                    break;
                                }
                            }
                        }
                        // If all symbols after have epsilon or no symbols after
                        if (all_epsilon)
                        {
                            // Add FOLLOW(lhs) to FOLLOW(symbol)
                            for (char c : follow_sets[lhs])
                            {
                                follow_sets[symbol].insert(c);
                            }
                        }
                        if (follow_sets[symbol] != old_follow)
                        {
                            changed = true;
                        }
                    }
                }
            }
        }
        displayFollowSets();
    }
    void displayFirstSets()
    {
        cout << "\nFIRST Sets:\n";
        for (char nt : non_terminals)
        {
            cout << "FIRST(" << nt << ") = { ";
            for (char c : first_sets[nt])
            {
                cout << c << " ";
            }
            cout << "}\n";
        }
    }
    void displayFollowSets()
    {
        cout << "\nFOLLOW Sets:\n";
        for (char nt : non_terminals)
        {
            cout << "FOLLOW(" << nt << ") = { ";
            for (char c : follow_sets[nt])
            {
                cout << c << " ";
            }
            cout << "}\n";
        }
    }
    void constructParseTable()
    {
        for (int i = 0; i < productions.size(); i++)
        {
            char lhs = productions[i].first;
            string rhs = productions[i].second;
            // Compute FIRST of RHS
            set<char> first_rhs;
            bool epsilon_in_first = true;
            for (int j = 0; j < rhs.length(); j++)
            {
                char symbol = rhs[j];
                if (isTerminal(symbol) || isEpsilon(symbol))
                {
                    first_rhs.insert(symbol);
                    if (!isEpsilon(symbol))
                    {
                        epsilon_in_first = false;
                    }
                    break;
                }
                else if (isNonTerminal(symbol))
                {
                    for (char c : first_sets[symbol])
                    {
                        first_rhs.insert(c);
                    }
                    if (first_sets[symbol].find('e') == first_sets[symbol].end())
                    {
                        epsilon_in_first = false;
                        break;
                    }
                }
            }
            // Add entries to parse table for each terminal in FIRST(RHS)
            for (char terminal : first_rhs)
            {
                if (!isEpsilon(terminal))
                {
                    if (parse_table.find({lhs, terminal}) != parse_table.end())
                    {
                        cout << "WARNING: Grammar is not LL(1)! Conflict at [" << lhs << ","
                             << terminal << "]\n";
                    }
                    parse_table[{lhs, terminal}] = rhs;
                }
            }
            // If epsilon in FIRST(RHS), add entries for FOLLOW(LHS)
            if (first_rhs.find('e') != first_rhs.end())
            {
                for (char terminal : follow_sets[lhs])
                {
                    if (parse_table.find({lhs, terminal}) != parse_table.end())
                    {
                        cout << "WARNING: Grammar is not LL(1)! Conflict at [" << lhs << ","
                             << terminal << "]\n";
                    }
                    parse_table[{lhs, terminal}] = rhs;
                }
            }
        }
        displayParseTable();
    }
    void displayParseTable()
    {
        cout << "\n=== PREDICTIVE PARSE TABLE ===\n\n";
        // Create sorted vectors for consistent display
        vector<char> sorted_terminals(terminals.begin(), terminals.end());
        vector<char> sorted_non_terminals(non_terminals.begin(),
                                          non_terminals.end());
        sorted_terminals.push_back('$'); // Add $ to terminals for display
        sort(sorted_terminals.begin(), sorted_terminals.end());
        sort(sorted_non_terminals.begin(), sorted_non_terminals.end());
        // Print header
        cout << setw(8) << " ";
        for (char t : sorted_terminals)
        {
            cout << setw(15) << t;
        }
        cout << "\n";
        cout << setfill('-') << setw(8 + 15 * sorted_terminals.size()) << ""
             << setfill(' ') << "\n";
        // Print table rows
        for (char nt : sorted_non_terminals)
        {
            cout << setw(8) << nt;
            for (char t : sorted_terminals)
            {
                auto key = make_pair(nt, t);
                if (parse_table.find(key) != parse_table.end())
                {
                    string entry = string(1, nt) + "->" + parse_table[key];
                    cout << setw(15) << entry;
                }
                else
                {
                    cout << setw(15) << " ";
                }
            }
            cout << "\n";
        }
    }
    void run()
    {
        inputGrammar();
        computeFirst();
        computeFollow();
        constructParseTable();
        cout << "\n======\n";
    }
};
int main()
{
    PredictiveParser parser;
    parser.run();
    return 0;
}