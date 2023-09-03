#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include "instr.hpp"

Argument::Argument(ArgValue p_value, ArgType p_type)
    : value(p_value), type(p_type) {}

std::string Argument::str_val() const {
    if (std::holds_alternative<std::string>(this->value))
        return std::get<std::string>(this->value);
    return "";
}

float Argument::float_val() const {
    if (std::holds_alternative<float>(this->value))
        return std::get<float>(this->value);
    return 0;
}

Instr::Instr(InstrType p_type, size_t p_line_no, size_t p_col_no, std::vector<Argument> p_args)
    : type(p_type), line_no(p_line_no), col_no(p_col_no), args(p_args) {}

void print_instrs(std::vector<Instr> instrs) {
    std::cout << "INSTRUCTIONS:\n";
    for (const Instr &instr : instrs) {
        std::cout << "| Line Number in src: " << instr.line_no << "\n"
                  << "* Type: " << (int)instr.type << "\n"
                  << "* Args:\n";
        for (const Argument &arg : instr.args) {
            std::cout << "| - ArgType.: " << (int)arg.type << "\n"
                      << "| - ArgValue: ";
            if (std::holds_alternative<float>(arg.value))
                std::cout << arg.float_val() << "\n";
            else
                std::cout << arg.str_val() << "\n";
        }
        std::cout << "+-------------------+\n";
    }
}