#pragma once
#include <vector>
#include <string>
#include <variant>

enum class InstrType {
    SIGN,
    HOP,
    HOP_BUT_RETUN,
    HOP_BELOW,
    HOP_ABOVE,
    RETURN,

    CREATE_VAR,
    ASSIGN_VAR,

    INPUT,
    OUTPUT,
    OUTPUT_NO_NEWL,

    IF_SKIP,

    DIVIDE,
    MULTIPLY,
    ADD,
    SUBTRACT,
    CONCAT,
};

enum class ArgType {
    ID,
    VAL
};

typedef std::variant<std::string, float> ArgValue;

struct Argument {
    ArgValue value;
    ArgType type;

    Argument(ArgValue p_value, ArgType p_type);
    std::string str_val() const;
    float float_val() const;
};

struct Instr {
    InstrType type;
    size_t line_no;
    size_t col_no;
    std::vector<Argument> args;

    Instr(InstrType p_type, size_t p_line_no, size_t p_col_no, std::vector<Argument> p_args = {});
};

void print_instrs(std::vector<Instr> instrs);