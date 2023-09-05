#pragma once
#include <vector>
#include <string>

#include "token.hpp"
#include "instr.hpp"

enum class MatchType {
    ID,
    KEYWORD,
    VALUE,
    QUANTITY,
};

struct Match {
    MatchType type;
    std::string value;

    Match(MatchType type, std::string value = "");
    Match(std::string value);
};

class Parser {
public:
    Parser(std::vector<TokenLine> &p_tokenlines);
    std::vector<Instr> get_instrs();
private:
    std::vector<TokenLine> &m_tokenlines;
    std::vector<Instr> m_instrs;
    size_t m_tokenline_cursor = 0;
    TokenLine *m_curr_tokenline;

    void try_parse_instr(const std::vector<Match> &p_pattern, InstrType type);
};
