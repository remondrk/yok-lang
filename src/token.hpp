#pragma once
#include <string>
#include <vector>

enum class TokenType {
    WORD,
    COMMA,
    LIT_STR,
    LIT_NUM,
};

struct Token {
    std::string value;
    TokenType type;
    size_t col_no;
};

struct TokenLine {
    size_t line_no;
    std::vector<Token> tokens;
};

void print_tokenlines(const std::vector<TokenLine> &tok_lns);
