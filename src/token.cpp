#include <string>
#include <vector>
#include <iostream>

#include "token.hpp"

void print_tokenlines(const std::vector<TokenLine> &tok_lns) {
    for (const TokenLine &tok_ln : tok_lns) {
        std::cout << "| Line Number in src: " << tok_ln.line_no << "\n";
        for (const Token &tok : tok_ln.tokens)
            std::cout << "* Column: " << tok.col_no   << "\n"
                      << "* Type..: " << (int)tok.type << "\n"
                      << "* Value.: " << tok.value     << "\n"
                      << "+-------+\n";
    }
}
