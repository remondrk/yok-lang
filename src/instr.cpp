#include <vector>
#include <string>
#include <iostream>

#include "field.hpp"
#include "instr.hpp"

Argument::Argument(Field p_value, ArgType p_type)
    : value(p_value), type(p_type) {}

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
                      << "| - ArgValue: " << arg.value.get_string();
        }
        std::cout << "+-------------------+\n";
    }
}
