#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    if (s[0] == '-' && s.length() > 1) {
        return std::all_of(s.begin() + 1, s.end(), ::isdigit);
    }
    return std::all_of(s.begin(), s.end(), ::isdigit);
}

std::string getJumpInstruction(const std::string& op) {
    if (op == "==") return "JE";
    if (op == "!=") return "JNE";
    if (op == "<")  return "JL";
    if (op == "<=") return "JLE";
    if (op == ">")  return "JG";
    if (op == ">=") return "JGE";
    return "";
}

void generateAssembly(const std::vector<std::string>& tac) {
    std::set<std::string> variables;

    // Collect variables used in TAC
    for (const auto& line : tac) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (ss >> token) tokens.push_back(token);
        if (tokens.empty()) continue;

        if (tokens[0] == "if") {
            if (!isNumber(tokens[1])) variables.insert(tokens[1]);
            if (tokens.size() > 3 && !isNumber(tokens[3])) variables.insert(tokens[3]);
        } else if (tokens.size() > 1 && tokens[1] == "=") {
            if (!isNumber(tokens[0])) variables.insert(tokens[0]);
            if (!isNumber(tokens[2])) variables.insert(tokens[2]);
            if (tokens.size() > 4 && !isNumber(tokens[4])) variables.insert(tokens[4]);
        }
    }

    // Print data section
    std::cout << "; Generated 8086 Assembly Code" << std::endl;
    std::cout << "MODEL SMALL" << std::endl;
    std::cout << "DATA" << std::endl;
    for (const auto& var : variables) {
        std::cout << " " << var << " DW 0" << std::endl;
    }

    // Code section
    std::cout << "CODE" << std::endl;
    std::cout << "MAIN PROC" << std::endl;
    std::cout << " MOV AX, @DATA" << std::endl;
    std::cout << " MOV DS, AX" << std::endl << std::endl;

    for (const auto& line : tac) {
        std::cout << "; TAC: " << line << std::endl;
        std::stringstream ss(line);
        std::string first_word;
        ss >> first_word;

        // Label
        if (first_word.back() == ':') {
            std::cout << first_word << std::endl;
            continue;
        }

        // Goto
        if (first_word == "goto") {
            std::string label;
            ss >> label;
            std::cout << " JMP " << label << std::endl << std::endl;
            continue;
        }

        // Conditional jump
        if (first_word == "if") {
            std::string src1, op, src2, go, label;
            ss >> src1 >> op >> src2 >> go >> label;
            std::cout << " MOV AX, " << src1 << std::endl;
            std::cout << " CMP AX, " << src2 << std::endl;
            std::string jmp_inst = getJumpInstruction(op);
            std::cout << " " << jmp_inst << " " << label << std::endl << std::endl;
            continue;
        }

        // Assignment or arithmetic
        std::stringstream line_ss(line);
        std::string dest, eq_op, src1, op, src2;
        line_ss >> dest >> eq_op >> src1;

        if (line_ss >> op >> src2) {
            // Binary operation
            if (isNumber(src1)) std::cout << " MOV AX, " << src1 << std::endl;
            else std::cout << " MOV AX, " << src1 << " ; Move value of " << src1 << " to AX" << std::endl;

            if (op == "+") {
                std::cout << " ADD AX, " << src2 << std::endl;
            } else if (op == "-") {
                std::cout << " SUB AX, " << src2 << std::endl;
            } else if (op == "*") {
                if (isNumber(src2)) {
                    std::cout << " MOV BX, " << src2 << std::endl;
                    std::cout << " MUL BX ; Multiply AX by BX" << std::endl;
                } else {
                    std::cout << " MUL " << src2 << " ; Multiply AX by " << src2 << std::endl;
                }
            } else if (op == "/") {
                std::cout << " MOV DX, 0 ; Clear DX for division" << std::endl;
                if (isNumber(src2)) {
                    std::cout << " MOV BX, " << src2 << std::endl;
                    std::cout << " DIV BX ; Divide DX:AX by BX" << std::endl;
                } else {
                    std::cout << " DIV " << src2 << " ; Divide DX:AX by " << src2 << std::endl;
                }
            }

            std::cout << " MOV " << dest << ", AX ; Store result in " << dest << std::endl;
        } else {
            // Simple assignment
            if (isNumber(src1)) std::cout << " MOV AX, " << src1 << std::endl;
            else std::cout << " MOV AX, " << src1 << " ; Move value of " << src1 << " to AX" << std::endl;

            std::cout << " MOV " << dest << ", AX ; Store result in " << dest << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << " MOV AH, 4CH" << std::endl;
    std::cout << " INT 21H" << std::endl;
    std::cout << "MAIN ENDP" << std::endl;
    std::cout << "END MAIN" << std::endl;
}

int main() {
    std::vector<std::string> three_address_code;
    std::string line;

    std::cout << "Enter Three-Address Code (one instruction per line)." << std::endl;
    std::cout << "Press Ctrl+D (Linux/macOS) or Ctrl+Z then Enter (Windows) to generate code." << std::endl;
    std::cout << std::endl;

    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            three_address_code.push_back(line);
        }
    }

    std::cout << "\n--- Generating 8086 Assembly Code ---\n" << std::endl;
    generateAssembly(three_address_code);
    return 0;
}
