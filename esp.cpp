#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <variant>
#include <cctype>
#include <stdexcept>
#include <vector>
#include <stack>
#include <memory>

// Define variable types with proper type enforcement
using VarType = std::variant<int, float, bool, std::string, std::monostate>;

// Variable storage with type information
struct Variable {
    VarType value;
    std::string declaredType;
};

std::unordered_map<std::string, Variable> variables;
int currentLine = 0; // Track line numbers for better error messages

// Utility function to trim spaces
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Remove quotes from string literals
std::string removeQuotes(const std::string &str) {
    std::string s = trim(str);
    if (s.length() >= 2 && ((s.front() == '"' && s.back() == '"') || 
                             (s.front() == '\'' && s.back() == '\''))) {
        return s.substr(1, s.length() - 2);
    }
    return s;
}

// Enhanced tokenizer for math expressions
std::vector<std::string> tokenize(const std::string &expr) {
    std::vector<std::string> tokens;
    std::string current;
    bool inNumber = false;
    
    for (size_t i = 0; i < expr.length(); ++i) {
        char c = expr[i];
        
        if (std::isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
                inNumber = false;
            }
            continue;
        }
        
        if (std::isdigit(c) || c == '.') {
            current += c;
            inNumber = true;
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            // Handle negative numbers
            if (c == '-' && (tokens.empty() || 
                tokens.back() == "(" || tokens.back() == "+" || 
                tokens.back() == "-" || tokens.back() == "*" || tokens.back() == "/")) {
                current += c;
                continue;
            }
            
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1, c));
            inNumber = false;
        } else {
            current += c;
            inNumber = false;
        }
    }
    
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
}

// Get operator precedence
int getPrecedence(const std::string &op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

// Shunting Yard algorithm for proper operator precedence
std::vector<std::string> infixToPostfix(const std::vector<std::string> &tokens) {
    std::vector<std::string> output;
    std::stack<std::string> operators;
    
    for (const auto &token : tokens) {
        if (token.empty()) continue;
        
        // Number or variable
        if (std::isdigit(token[0]) || (token[0] == '-' && token.length() > 1) ||
            (std::isalpha(token[0]) && variables.find(token) != variables.end())) {
            output.push_back(token);
        }
        // Left parenthesis
        else if (token == "(") {
            operators.push(token);
        }
        // Right parenthesis
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove '('
        }
        // Operator
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            while (!operators.empty() && operators.top() != "(" &&
                   getPrecedence(operators.top()) >= getPrecedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }
    
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    
    return output;
}

// Evaluate postfix expression
float evaluatePostfix(const std::vector<std::string> &postfix) {
    std::stack<float> values;
    
    for (const auto &token : postfix) {
        if (token.empty()) continue;
        
        // Check if it's a variable
        if (variables.find(token) != variables.end()) {
            const auto &var = variables[token];
            if (std::holds_alternative<int>(var.value)) {
                values.push(static_cast<float>(std::get<int>(var.value)));
            } else if (std::holds_alternative<float>(var.value)) {
                values.push(std::get<float>(var.value));
            } else {
                throw std::runtime_error("Variable '" + token + "' is not numeric");
            }
        }
        // Check if it's a number
        else if (std::isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            values.push(std::stof(token));
        }
        // It's an operator
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (values.size() < 2) {
                throw std::runtime_error("Invalid expression");
            }
            
            float b = values.top(); values.pop();
            float a = values.top(); values.pop();
            
            if (token == "+") values.push(a + b);
            else if (token == "-") values.push(a - b);
            else if (token == "*") values.push(a * b);
            else if (token == "/") {
                if (b == 0) throw std::runtime_error("Division by zero at line " + std::to_string(currentLine));
                values.push(a / b);
            }
        }
    }
    
    if (values.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }
    
    return values.top();
}

// Enhanced mathematical expression evaluator with proper precedence
float evaluate_math_expression(const std::string &expr) {
    auto tokens = tokenize(expr);
    auto postfix = infixToPostfix(tokens);
    return evaluatePostfix(postfix);
}

// Expression evaluation logic
VarType evaluate_expression(const std::string &expr) {
    std::string trimmedExpr = trim(expr);
    
    // Check if it's a quoted string
    if ((trimmedExpr.front() == '"' && trimmedExpr.back() == '"') ||
        (trimmedExpr.front() == '\'' && trimmedExpr.back() == '\'')) {
        return removeQuotes(trimmedExpr);
    }
    
    // Direct variable fetch
    if (variables.find(trimmedExpr) != variables.end()) {
        return variables[trimmedExpr].value;
    }

    // Boolean literals
    if (trimmedExpr == "true") return true;
    if (trimmedExpr == "false") return false;

    // Try to parse as math expression
    if (trimmedExpr.find_first_of("+-*/()") != std::string::npos) {
        try {
            return evaluate_math_expression(trimmedExpr);
        } catch (...) {
            // If math evaluation fails, treat as string
            return trimmedExpr;
        }
    }

    // Try integer
    try {
        size_t pos;
        int intVal = std::stoi(trimmedExpr, &pos);
        if (pos == trimmedExpr.length()) {
            return intVal;
        }
    } catch (...) {}

    // Try float
    try {
        size_t pos;
        float floatVal = std::stof(trimmedExpr, &pos);
        if (pos == trimmedExpr.length()) {
            return floatVal;
        }
    } catch (...) {}

    // Default to string
    return trimmedExpr;
}

// Handle dynamic echo command with proper string parsing
void handle_echo(const std::string &content) {
    std::string trimmedContent = trim(content);
    
    // Remove surrounding quotes if present
    if ((trimmedContent.front() == '"' && trimmedContent.back() == '"') ||
        (trimmedContent.front() == '\'' && trimmedContent.back() == '\'')) {
        trimmedContent = trimmedContent.substr(1, trimmedContent.length() - 2);
    }
    
    size_t start = 0;
    
    while (true) {
        size_t open_brace = trimmedContent.find("{", start);
        if (open_brace == std::string::npos) {
            std::cout << trimmedContent.substr(start);
            break;
        }

        size_t close_brace = trimmedContent.find("}", open_brace);
        if (close_brace == std::string::npos) {
            std::cerr << "Error at line " << currentLine << ": Malformed expression - missing closing brace" << std::endl;
            return;
        }

        // Print text before opening brace
        std::cout << trimmedContent.substr(start, open_brace - start);

        // Evaluate the expression
        std::string expr = trimmedContent.substr(open_brace + 1, close_brace - open_brace - 1);
        
        try {
            VarType value = evaluate_expression(expr);

            // Dynamically resolve type and print
            if (std::holds_alternative<int>(value)) {
                std::cout << std::get<int>(value);
            } else if (std::holds_alternative<float>(value)) {
                std::cout << std::get<float>(value);
            } else if (std::holds_alternative<std::string>(value)) {
                std::cout << std::get<std::string>(value);
            } else if (std::holds_alternative<bool>(value)) {
                std::cout << (std::get<bool>(value) ? "true" : "false");
            }
        } catch (const std::exception &e) {
            std::cerr << "\nError at line " << currentLine << ": " << e.what() << std::endl;
            return;
        }

        start = close_brace + 1;
    }
    std::cout << std::endl;
}

// Enhanced variable assignment parser
void handle_variable_assignment(const std::string &line) {
    // Find the keyword (let, var, const)
    size_t keywordEnd = line.find(' ');
    if (keywordEnd == std::string::npos) {
        throw std::runtime_error("Invalid variable declaration at line " + std::to_string(currentLine));
    }
    
    std::string keyword = line.substr(0, keywordEnd);
    std::string rest = trim(line.substr(keywordEnd + 1));
    
    // Find the type
    size_t typeEnd = rest.find(' ');
    if (typeEnd == std::string::npos) {
        throw std::runtime_error("Missing type declaration at line " + std::to_string(currentLine));
    }
    
    std::string varType = rest.substr(0, typeEnd);
    rest = trim(rest.substr(typeEnd + 1));
    
    // Find the identifier and equals sign
    size_t equalsPos = rest.find('=');
    if (equalsPos == std::string::npos) {
        throw std::runtime_error("Missing assignment operator at line " + std::to_string(currentLine));
    }
    
    std::string identifier = trim(rest.substr(0, equalsPos));
    std::string valueStr = trim(rest.substr(equalsPos + 1));
    
    // Validate identifier
    if (identifier.empty() || !std::isalpha(identifier[0])) {
        throw std::runtime_error("Invalid identifier '" + identifier + "' at line " + std::to_string(currentLine));
    }
    
    // Evaluate the value
    VarType value;
    
    try {
        if (varType == "int") {
            auto evalResult = evaluate_expression(valueStr);
            if (std::holds_alternative<int>(evalResult)) {
                value = std::get<int>(evalResult);
            } else if (std::holds_alternative<float>(evalResult)) {
                value = static_cast<int>(std::get<float>(evalResult));
            } else {
                throw std::runtime_error("Cannot convert to int");
            }
        } else if (varType == "float") {
            auto evalResult = evaluate_expression(valueStr);
            if (std::holds_alternative<float>(evalResult)) {
                value = std::get<float>(evalResult);
            } else if (std::holds_alternative<int>(evalResult)) {
                value = static_cast<float>(std::get<int>(evalResult));
            } else {
                throw std::runtime_error("Cannot convert to float");
            }
        } else if (varType == "string") {
            value = removeQuotes(valueStr);
        } else if (varType == "bool" || varType == "boolean") {
            auto evalResult = evaluate_expression(valueStr);
            if (std::holds_alternative<bool>(evalResult)) {
                value = std::get<bool>(evalResult);
            } else {
                throw std::runtime_error("Cannot convert to boolean");
            }
        } else {
            throw std::runtime_error("Unknown type '" + varType + "'");
        }
        
        variables[identifier] = {value, varType};
        
    } catch (const std::exception &e) {
        throw std::runtime_error("Error assigning variable '" + identifier + "': " + e.what());
    }
}

// Command Executor
void execute_line(const std::string &line) {
    std::string trimmedLine = trim(line);
    
    // Skip empty lines and comments
    if (trimmedLine.empty() || trimmedLine[0] == '#' || trimmedLine.substr(0, 2) == "//") {
        return;
    }
    
    try {
        if (trimmedLine.find("echo") == 0) {
            std::string content = trim(trimmedLine.substr(4));
            handle_echo(content);
        } else if (trimmedLine.find("let") == 0 || trimmedLine.find("var") == 0 || trimmedLine.find("const") == 0) {
            handle_variable_assignment(trimmedLine);
        } else {
            std::cerr << "Warning at line " << currentLine << ": Unknown command '" << trimmedLine << "'" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error at line " << currentLine << ": " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.esp>" << std::endl;
        std::cerr << "Espira Programming Language Interpreter v0.1.0" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    std::string line;
    currentLine = 0;
    
    while (std::getline(file, line)) {
        currentLine++;
        execute_line(line);
    }

    file.close();
    variables.clear();
    return 0;
}