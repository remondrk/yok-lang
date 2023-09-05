#include <vector>
#include <string>
#include <stdexcept>

#include "generated/parser_patterns.hpp"
#include "token.hpp"
#include "instr.hpp"
#include "parser.hpp"
#include "field.hpp"
#include "error.hpp"

Match::Match(MatchType type, std::string value) {
    this->type = type;
    this->value = value;
}

Match::Match(std::string value) {
    this->type = MatchType::KEYWORD;
    this->value = value;
}

Parser::Parser(std::vector<TokenLine> &p_tokenlines)
    : m_tokenlines(p_tokenlines) {}

#define M Match
std::vector<Instr> Parser::get_instrs() {
    for (size_t i = 0; i < m_tokenlines.size(); i++) {
        m_curr_tokenline = &m_tokenlines[i];
        const size_t prev_instrs_len = m_instrs.size();

        try_parse_instr(PATTERN_LABEL, InstrType::WAYPOINT);
        try_parse_instr(PATTERN_GO, InstrType::TP);
        try_parse_instr(PATTERN_CALL, InstrType::TP_BUT_RETURN);
        try_parse_instr(PATTERN_GO_DOWN, InstrType::TP_BELOW);
        try_parse_instr(PATTERN_GO_UP, InstrType::TP_ABOVE);
        try_parse_instr(PATTERN_RET, InstrType::RETURN);
        try_parse_instr(PATTERN_VAR_DECL, InstrType::CREATE_VAR);
        try_parse_instr(PATTERN_VAR_ASSIGN, InstrType::ASSIGN_VAR);
        try_parse_instr(PATTERN_INPUT, InstrType::INPUT);
        try_parse_instr(PATTERN_WRITE_LN, InstrType::SAY);
        try_parse_instr(PATTERN_WRITE, InstrType::WHISPER);
        try_parse_instr(PATTERN_UNLESS_SKIP, InstrType::UNLESS_SKIP);
        try_parse_instr(PATTERN_IF_SKIP, InstrType::IF_SKIP);
        try_parse_instr(PATTERN_DIVIDE, InstrType::DIVIDE);
        try_parse_instr(PATTERN_MULTIPLY, InstrType::MULTIPLY);
        try_parse_instr(PATTERN_ADD, InstrType::ADD);
        try_parse_instr(PATTERN_SUBTRACT, InstrType::SUBTRACT);

        if (m_instrs.size() <= prev_instrs_len)
            throw std::invalid_argument(make_err_msg(
                m_curr_tokenline->line_no,
                m_curr_tokenline->tokens[0].col_no,
                "undefined instruction"));
    }

    return m_instrs;
}
#undef M

void Parser::try_parse_instr(const std::vector<Match> &p_pattern, InstrType type) {
    if (p_pattern.size() != m_curr_tokenline->tokens.size())
        return;

    Instr instr(type, m_curr_tokenline->line_no, m_curr_tokenline->tokens[0].col_no);
    bool must_end_with_s = false;

    for (size_t i = 0; i < p_pattern.size(); i++) {
        Token &tok = m_curr_tokenline->tokens[i];
        const Match &match = p_pattern[i];

        if (must_end_with_s) {
            if (tok.value.back() != 's')
                throw(make_err_msg(
                    m_curr_tokenline->line_no,
                    tok.col_no,
                    "plural suffix must be used correctly"
                ));
            tok.value.pop_back();
        }

        if (match.type == MatchType::KEYWORD && tok.type == TokenType::WORD && tok.value == match.value)
            continue;

        if (match.type == MatchType::ID && tok.type == TokenType::WORD) {
            instr.args.push_back(Argument(Field(tok.value), ArgType::ID));
            continue;
        }

        if (match.type == MatchType::VALUE) {
            if (tok.type == TokenType::WORD) {
                instr.args.push_back(Argument(Field(tok.value), ArgType::ID));
                continue;
            }

            if (tok.type != TokenType::LIT_NUM && tok.type != TokenType::LIT_STR)
                return;

            Argument arg(Field(tok.value), ArgType::VAL);
            if (tok.type == TokenType::LIT_NUM)
                arg.value = Field(std::stof(tok.value));

            instr.args.push_back(arg);
            continue;
        }

        if (match.type == MatchType::QUANTITY && tok.type == TokenType::LIT_NUM) {
            if (std::stof(tok.value) != 1)
                must_end_with_s = true;
            instr.args.push_back(Argument(Field(std::stof(tok.value)), ArgType::VAL));
            continue;
        }

        return;
    }

    m_instrs.push_back(instr);
}
