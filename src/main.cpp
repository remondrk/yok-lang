#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <string>

#include "token.hpp"
#include "instr.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "runtime.hpp"
#include "error.hpp"

std::string read_file_and_check_ext(const std::filesystem::path &path, const std::string &extension) {
    std::ifstream file(path);
    if (!file || path.extension() != extension)
        throw std::invalid_argument(make_err_msg(0, 0, "provided .yok file path is invalid"));

    std::stringstream content_ss;
    content_ss << file.rdbuf();
    
    return content_ss.str();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "USAGE: yok <input.yok>\n";
        return 1;
    }

    try {
        std::filesystem::path src_path = argv[1];
        std::string src = read_file_and_check_ext(src_path, ".yok");

        Lexer lexer(src);
        std::vector<TokenLine> tok_lns = lexer.get_tokenlines();

        Parser parser(tok_lns);
        std::vector<Instr> instrs = parser.get_instrs();

        Runtime runtime(instrs);
        runtime.run();
    }
    catch (const std::invalid_argument &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
