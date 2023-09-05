#include <string>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "lexer.hpp"
#include "token.hpp"
#include "error.hpp"

#define COMMENT_WORD "by-the-way"
#define SEPERATOR_WORD "and-then"

#define CHAR_EOF 0
#define CHAR_DEC_SEP ','

Lexer::Lexer(const std::string &p_src)
    : m_src(p_src) {};

std::vector<TokenLine> Lexer::get_tokenlines() {
    while (get_ch() != CHAR_EOF) {
        const size_t prev_cursor = m_cursor;

        skip_spaces();
        skip_inline_comments();
        try_lex_word();
        try_lex_str_lit();
        try_lex_num_lit();
        try_lex_comma();

        // We must ensure we use EOF before it breaks
        // the loop so this must be called last
        terminate_tokenline();

        if (prev_cursor == m_cursor)
            throw std::invalid_argument(make_err_msg(m_curr_ln, m_curr_col, "undefined token"));
    };

    return m_tokenlines;
}

inline bool Lexer::cmp_last_tok_val(const std::string p_cmp) {
    return !m_curr_tokenline.tokens.empty() && m_curr_tokenline.tokens.back().value == p_cmp;
}

void Lexer::calc_ln_and_col(char p_ch) {
    m_curr_col++;
    if (p_ch == '\n') {
        m_curr_ln++;
        m_curr_col = 1;
    }
}

char Lexer::get_ch() {
    if (m_cursor >= m_src.size())
        return CHAR_EOF;
    return m_src[m_cursor];
}

char Lexer::eat_ch() {
    if (m_cursor >= m_src.size())
        return CHAR_EOF;
    const char ch = m_src[m_cursor];
    calc_ln_and_col(ch);
    m_cursor++;
    return ch;
}

char Lexer::peek_ch(size_t dist) {
    if (m_cursor + dist >= m_src.size())
        return CHAR_EOF;
    return m_src[m_cursor + dist];
}

void Lexer::push_tok_to_line() {
    m_curr_tokenline.tokens.push_back(m_curr_tok);
    m_curr_tok = {};
}

void Lexer::push_tokenline() {
    if (!m_curr_tokenline.tokens.empty()) {
        m_tokenlines.push_back(m_curr_tokenline);
        m_curr_tokenline = {};
    }
    m_curr_tokenline.line_no = m_curr_ln;
}

void Lexer::skip_spaces() {
    while (get_ch() != '\n' && std::isspace(get_ch()))
        eat_ch();
}

void Lexer::skip_inline_comments() {
    if (cmp_last_tok_val(COMMENT_WORD)) {
        m_curr_tokenline.tokens.pop_back();
        if (cmp_last_tok_val(","))
            m_curr_tokenline.tokens.pop_back();
        while (get_ch() != '\n' && get_ch() != CHAR_EOF)
            eat_ch();
    }
}

void Lexer::terminate_tokenline() {
    if (cmp_last_tok_val(SEPERATOR_WORD)) {
        m_curr_tokenline.tokens.pop_back();
        push_tokenline();
    }
    if (get_ch() == '\n' || get_ch() == CHAR_EOF) {
        eat_ch();
        push_tokenline();
    }
}

void Lexer::try_lex_word() {
    if (!std::isalpha(get_ch()))
        return;

    m_curr_tok = {"", TokenType::WORD, m_curr_col};
    while (std::isalpha(get_ch()) || get_ch() == '-' || get_ch() == '\'')
        m_curr_tok.value += eat_ch();

    push_tok_to_line();
}

void Lexer::try_lex_str_lit() {
    if (get_ch() != '"')
        return;

    m_curr_tok = {"", TokenType::LIT_STR, m_curr_col};
    eat_ch(); // skip opening quote
    while (get_ch() != '"')
        m_curr_tok.value += eat_ch();
    eat_ch(); // skip closing quote

    push_tok_to_line();
}

void Lexer::try_lex_num_lit() {
    if (!std::isdigit(get_ch()))
        return;

    m_curr_tok = {"", TokenType::LIT_NUM, m_curr_col};
    bool found_dec_sep = false;
    while (std::isdigit(get_ch()) || get_ch() == CHAR_DEC_SEP) {
        if (get_ch() == CHAR_DEC_SEP) {
            if (found_dec_sep || !std::isdigit(peek_ch()))
                break;
            found_dec_sep = true;
        }
        m_curr_tok.value += eat_ch();
    }

    push_tok_to_line();
}

void Lexer::try_lex_comma() {
    if (get_ch() == ',') {
        m_curr_tok = {",", TokenType::COMMA, m_curr_col};
        eat_ch();
        push_tok_to_line();
    }
}
