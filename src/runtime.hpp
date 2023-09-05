#pragma once
#include <vector>
#include <variant>
#include <map>

#include "instr.hpp"

#define INPUT_STR_VAR_NAME "the-ingressed-string"
#define INPUT_NUM_VAR_NAME "the-ingressed-number"
#define RESULT_STR_VAR_NAME "the-resulting-string"
#define RESULT_NUM_VAR_NAME "the-resulting-number"

class Runtime {
public:
    Runtime(std::vector<Instr> &p_instr);
    void run();
private:
    size_t m_instr_cursor = 0;
    std::map<std::string, Field> m_vars = {
        {INPUT_STR_VAR_NAME , {}},
        {INPUT_NUM_VAR_NAME , {}},
        {RESULT_STR_VAR_NAME, {}},
        {RESULT_NUM_VAR_NAME, {}},
    };
    std::vector<Instr> &m_instrs;
    std::vector<size_t> m_return_stack;
    Instr *m_curr_instr;

    Field eval_arg(const Argument &p_arg);

    bool compare_fields(const Field &p_field1, const Field &p_field2, const std::string &p_cmp);
    void skip_n_lines(size_t p_n);

    void panic(const std::string &p_msg);
    void run_not_implemented();
    void run_go();
    void run_go_up();
    void run_go_down();
    void run_call();
    void run_return();
    void run_declare_var();
    void run_assign_var();
    void run_input();
    void run_write_ln();
    void run_write();
    void run_unless_skip();
    void run_if_skip();
    void run_divide();
    void run_multiply();
    void run_add();
    void run_subtract();
};
