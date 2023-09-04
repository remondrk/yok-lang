#include <vector>
#include <variant>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <map>

#include "instr.hpp"
#include "runtime.hpp"
#include "field.hpp"
#include "error.hpp"

#define ERR_MSG_NO_WAYPOINT "can't find the sign to hop to"
#define ERR_MSG_VAR_NOT_DEF "variable is not defined"
#define ERR_MSG_VAR_ALREADY_DEF "variable is already defined"
#define ERR_MSG_NOWHERE_TO_RET "no previous return points"
#define ERR_MSG_NOT_IMPLEMENTED "not implemented"
#define ERR_MSG_INVALID_CMP "invalid comparison type"
#define ERR_MSG_CANT_SKIP_NON_INT "cannot skip non-integer amount of lines"
#define ERR_MSG_INVALID_COMB_OF_ARG_TYPES "invalid combination of types"

#define CMP_IS "is"
#define CMP_ISNT "isn't"
#define CMP_GT "is-greater-than"
#define CMP_LT "is-less-than"
#define CMP_GTE "is-greater-than-or-equal-to"
#define CMP_LTE "is-less-than-or-equal-to"

Runtime::Runtime(std::vector<Instr> &p_instrs)
    : m_instrs(p_instrs) {}

Field Runtime::eval_arg(const Argument &p_arg) {
    Field value = p_arg.value;
    if (p_arg.type == ArgType::ID) {
        if (m_vars.find(p_arg.value.get_string()) == m_vars.end())
            panic(ERR_MSG_VAR_NOT_DEF);
        value = m_vars[p_arg.value.get_string()];
    }
    return value;
}

void Runtime::panic(const std::string &p_msg) {
    throw std::invalid_argument(make_err_msg(
        m_curr_instr->line_no,
        m_curr_instr->col_no,
        p_msg
    ));
}

void Runtime::run() {
    for (m_instr_cursor = 0; m_instr_cursor < m_instrs.size(); m_instr_cursor++) {
        m_curr_instr = &m_instrs[m_instr_cursor];

        switch (m_curr_instr->type) {
            case InstrType::SIGN: /* run_sign(); */ break;
            case InstrType::HOP: run_hop(); break;
            case InstrType::HOP_ABOVE: run_hop_above(); break;
            case InstrType::HOP_BELOW: run_hop_below(); break;
            case InstrType::HOP_BUT_RETUN: run_hop_but_ret(); break;
            case InstrType::RETURN: run_return(); break;
            case InstrType::CREATE_VAR: run_create_var(); break;
            case InstrType::ASSIGN_VAR: run_assign_var(); break;
            case InstrType::INPUT: run_input(); break;
            case InstrType::OUTPUT: run_output(); break;
            case InstrType::OUTPUT_NO_NEWL: run_output_no_newl(); break;
            case InstrType::IF_SKIP: run_if_skip(); break;
            case InstrType::DIVIDE: run_divide(); break;
            case InstrType::MULTIPLY: run_multiply(); break;
            case InstrType::ADD: run_add(); break;
            case InstrType::SUBTRACT: run_subtract(); break;
            case InstrType::CONCAT: run_concat(); break;
            default: run_not_implemented();
        }
    }
}

// void Runtime::run_sign() {}

void Runtime::run_hop() {
    const std::string &target_text = m_curr_instr->args[0].value.get_string();
    size_t above_cursor = m_instr_cursor;
    size_t below_cursor = m_instr_cursor;

    bool is_above_safe = true;
    bool is_below_safe = true;

    while (is_above_safe || is_below_safe) {
        is_above_safe = above_cursor == 0 ? false : is_above_safe;
        is_below_safe = below_cursor >= m_instrs.size() - 1 ? false : is_below_safe;

        above_cursor--;
        below_cursor++;

        if (is_above_safe) {
            const bool is_sign = m_instrs[above_cursor].type == InstrType::SIGN;
            const bool is_match = is_sign && m_instrs[above_cursor].args[0].value.get_string() == target_text;
            if (is_match) {
                m_instr_cursor = above_cursor;
                return;
            }
        }

        if (is_below_safe) {
            const bool is_sign = m_instrs[below_cursor].type == InstrType::SIGN;
            const bool is_match = is_sign && m_instrs[below_cursor].args[0].value.get_string() == target_text;
            if (is_match) {
                m_instr_cursor = below_cursor;
                return;
            }
        }
    }

    panic(ERR_MSG_NO_WAYPOINT);
}

// TODO: check above one
// Note: above and below are according to the source .yok
// file, meaning above will check previous instructions
void Runtime::run_hop_above() {
    const std::string &target_text = m_curr_instr->args[0].value.get_string();
    for (size_t cursor = m_instr_cursor; cursor > 0; cursor--) {
        const bool is_sign = m_instrs[cursor].type == InstrType::SIGN;
        const bool is_match = is_sign && m_instrs[cursor].args[0].value.get_string() == target_text;
        if (is_match) {
            m_instr_cursor = cursor;
            return;
        }
    }

    panic(ERR_MSG_NO_WAYPOINT);
}

void Runtime::run_hop_below() {
    const std::string &target_text = m_curr_instr->args[0].value.get_string();
    for (size_t cursor = m_instr_cursor + 1; cursor < m_instrs.size(); cursor++) {
        const bool is_sign = m_instrs[cursor].type == InstrType::SIGN;
        const bool is_match = is_sign && m_instrs[cursor].args[0].value.get_string() == target_text;
        if (is_match) {
            m_instr_cursor = cursor;
            return;
        }
    }

    panic(ERR_MSG_NO_WAYPOINT);
}

void Runtime::run_hop_but_ret() {
    m_return_stack.push_back(m_instr_cursor);
    run_hop();
}

void Runtime::run_return() {
    if (m_return_stack.empty())
        panic(ERR_MSG_NOWHERE_TO_RET);
    m_instr_cursor = m_return_stack.back();
    m_return_stack.pop_back();
}

void Runtime::run_input() {
    std::string input;
    std::cin >> input;
    m_vars["the-inputted-string"].set_data(input);

    try {
        m_vars["the-inputted-number"].set_data(std::stof(input));
    }
    catch(const std::exception& e) {
        m_vars["the-inputted-number"].set_data(0.0f);
    }
}

void Runtime::run_output() {
    const Field value = eval_arg(m_curr_instr->args[0]);
    if (value.is_float())
        std::cout << value.get_float() << "\n";
    else
        std::cout << value.get_string() << "\n";
}

void Runtime::run_output_no_newl() {
    const Field value = eval_arg(m_curr_instr->args[0]);
    if (value.is_float())
        std::cout << value.get_float();
    else
        std::cout << value.get_string();
}

void Runtime::run_create_var() {
    const std::string name = m_curr_instr->args[0].value.get_string();
    if (m_vars.find(name) != m_vars.end())
        panic(ERR_MSG_VAR_ALREADY_DEF);
    
    m_vars.insert({name, {}});
}

void Runtime::run_assign_var() {
    const Field value = eval_arg(m_curr_instr->args[0]);
    const std::string name = m_curr_instr->args[1].value.get_string();

    if (m_vars.find(name) == m_vars.end())
        panic(ERR_MSG_VAR_NOT_DEF);
    
    m_vars[name] = value;
}

void Runtime::run_if_skip() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[2]);

    const float n1 = field1.get_float();
    const float n2 = field2.get_float();

    const std::string &cmp = m_curr_instr->args[1].value.get_string();

    // instead of checking if cmp is not equal to every
    // possible comparison type to verify it is valid
    if (cmp == CMP_IS)        { if (field1.get_string() != field2.get_string()) return; }
    else if (cmp == CMP_ISNT) { if (field1.get_string() == field2.get_string()) return; }
    else if (cmp == CMP_GT)   { if (n1     <= n2    ) return; }
    else if (cmp == CMP_LT)   { if (n1     >= n2    ) return; }
    else if (cmp == CMP_GTE)  { if (n1      < n2    ) return; }
    else if (cmp == CMP_LTE)  { if (n1      > n2    ) return; }
    else                      { panic(ERR_MSG_INVALID_CMP);   }

    const size_t ln_count = m_curr_instr->args[3].value.get_float();
    if (ln_count != m_curr_instr->args[3].value.get_float())
        panic(ERR_MSG_CANT_SKIP_NON_INT);

    for (size_t cursor = m_instr_cursor; cursor < m_instrs.size(); cursor++) {
        if (m_instrs[cursor].line_no > m_curr_instr->line_no + ln_count) {
            m_instr_cursor = cursor - 1;
            return;
        }
    }
}

void Runtime::run_divide() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() / field2.get_float();
        m_vars["the-resulting-number"].set_data(result);
        m_vars["the-resulting-string"].set_data(std::to_string(result));
        return;
    }

    if (field1.is_string() && field2.is_float()) {
        m_vars["the-resulting-number"].set_data(0.0f);
        m_vars["the-resulting-string"].set_data(field1.get_string().substr(0, field2.get_float()));
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_multiply() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() * field2.get_float();
        m_vars["the-resulting-number"].set_data(result);
        m_vars["the-resulting-string"].set_data(std::to_string(result));
        return;
    }

    if (field1.is_string() && field2.is_float()) {
        std::string result = "";
        for (size_t i = 0; i < field2.get_float(); i++)
            result += field1.get_string();
        m_vars["the-resulting-number"].set_data(0.0f);
        m_vars["the-resulting-string"].set_data(result);
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_add() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() + field2.get_float();
        m_vars["the-resulting-number"].set_data(result);
        m_vars["the-resulting-string"].set_data(std::to_string(result));
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_subtract() {
    // reversed because it is "subtract arg0 from arg1"
    const Field field1 = eval_arg(m_curr_instr->args[1]);
    const Field field2 = eval_arg(m_curr_instr->args[0]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() - field2.get_float();
        m_vars["the-resulting-number"].set_data(result);
        m_vars["the-resulting-string"].set_data(std::to_string(result));
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_concat() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_string() && field2.is_string()) {
        m_vars["the-resulting-number"].set_data(0.0f);
        m_vars["the-resulting-string"].set_data(field1.get_string() + field2.get_string());
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_not_implemented() {
    panic(ERR_MSG_NOT_IMPLEMENTED);
}