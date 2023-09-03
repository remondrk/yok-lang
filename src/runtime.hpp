#pragma once
#include <vector>
#include <variant>
#include <map>

#include "instr.hpp"

class Runtime {
public:
    Runtime(std::vector<Instr> &p_instr);
    void run();
private:
    size_t m_instr_cursor = 0;
    std::map<std::string, Field> m_vars = {
        {"the-inputted-string" , {}},
        {"the-inputted-number" , {}},
        {"the-resulting-string", {}},
        {"the-resulting-number", {}},
    };
    std::vector<Instr> &m_instrs;
    std::vector<size_t> m_return_stack;
    Instr *m_curr_instr;

    Field eval_arg(const Argument &p_arg);

    void panic(const std::string &p_msg);
    void run_not_implemented();
    // void run_sign();
    void run_hop();
    void run_hop_above();
    void run_hop_below();
    void run_hop_but_ret();
    void run_return();
    void run_create_var();
    void run_assign_var();
    void run_input();
    void run_output();
    void run_output_no_newl();
    void run_if_skip();
    void run_divide();
    void run_multiply();
    void run_add();
    void run_subtract();
    void run_concat();
};