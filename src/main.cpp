#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>

#include "token.hpp"
#include "instr.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "runtime.hpp"

#define EXTENSION ".yok"
#define USAGE_MSG "USAGE: yok <.yok file path>\n"
#define INVALID_FILE_PATH_MSG "ERROR: provided .yok file path is invalid\n"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << USAGE_MSG;
        return 1;
    }
    
    std::filesystem::path src_path = argv[1];
    std::ifstream src_file(src_path);
    if (!src_file || src_path.extension() != EXTENSION) {
        std::cout << INVALID_FILE_PATH_MSG << "     * " << src_path;
        return 1;
    }

    std::stringstream src_ss;
    src_ss << src_file.rdbuf();
    std::string src = src_ss.str();
    src_file.close();

    Lexer lexer(src);
    std::vector<TokenLine> tok_lns = lexer.get_tokenlines();

    Parser parser(tok_lns);
    std::vector<Instr> instrs = parser.get_instrs();

    Runtime runtime(instrs);
    runtime.run();

    return 0;
}