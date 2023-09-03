#pragma once
#include <vector>

#include "token.hpp"

class Lexer {
public:
    Lexer(const std::string &p_src);
    std::vector<TokenLine> get_tokenlines();
private:
    const std::string &m_src;
    size_t m_cursor = 0;
    size_t m_curr_ln = 1, m_curr_col = 1;
    std::vector<TokenLine> m_tokenlines;
    TokenLine m_curr_tokenline = {1, {}};
    Token m_curr_tok;

    bool cmp_last_tok_val(const std::string p_cmp);
    void calc_ln_and_col(char p_ch);
    char get_ch();
    char eat_ch();
    char peek_ch(size_t p_dist = 1);

    void push_tok_to_line();
    void push_tokenline();

    void skip_spaces();
    void skip_inline_comments();
    void terminate_tokenline();
    void try_lex_word();
    void try_lex_str_lit();
    void try_lex_num_lit();
    void try_lex_comma();
};