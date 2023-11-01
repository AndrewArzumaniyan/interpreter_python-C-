#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "syntaxer.h"
#include "visitor.h"

int main() {
    std::ifstream file("example.txt");

    std::stringstream buffer;
    buffer << file.rdbuf();
    auto input = buffer.str();
    std::cout << input << std::endl;
    auto tokens = Lexer::parse(input);
    // for(auto it: tokens){
    //     std::cout << it.type << " " << it.value << std::endl; 
    // }
    auto res = Syntaxer::parse(tokens);

    std::cout << std::endl << "---------------------------" << std::endl;

    Printer printer;

    for (const auto& it : res) {
        it -> accept(printer);
    }

    std::cout << std::endl << "---------------------------" << std::endl;

    Analyzer analyzer;

    analyzer.scope = std::make_shared<Scope>("example.txt", 0);

    for (const auto& it : res) {
        it->accept(analyzer);
    }
}