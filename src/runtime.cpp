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

#define ERR_MSG_NO_LABEL "can't find the label to teleport to"
#define ERR_MSG_VAR_NOT_DEF "variable is not defined"
#define ERR_MSG_VAR_ALREADY_DEF "variable is already defined"
#define ERR_MSG_NOWHERE_TO_RET "no previous return points"
#define ERR_MSG_NOT_IMPLEMENTED "not implemented"
#define ERR_MSG_INVALID_CMP "invalid comparison type"
#define ERR_MSG_CANT_SKIP_NON_INT "cannot skip non-integer amount of lines"
#define ERR_MSG_INVALID_COMB_OF_ARG_TYPES "invalid combination of types"
#define ERR_MSG_STR_NOT_LONG_ENOUGH "string is not long enough"

#define CMP_IS "shares-the-same-value-with"
#define CMP_ISNT "is-in-no-way-identical-to"
#define CMP_GT "holds-a-greater-value-compared-to"
#define CMP_LT "holds-a-lesser-value-compared-to"
#define CMP_GTE "shares-the-same-value-with-or-holds-a-greater-value-compared-to"
#define CMP_LTE "shares-the-same-value-with-or-holds-a-lesser-value-compared-to"

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
            case InstrType::LABEL: /* run_sign(); */ break;
            case InstrType::GO: run_go(); break;
            case InstrType::GO_UP: run_go_up(); break;
            case InstrType::GO_DOWN: run_go_down(); break;
            case InstrType::CALL: run_call(); break;
            case InstrType::RETURN: run_return(); break;
            case InstrType::VAR_DECL: run_declare_var(); break;
            case InstrType::VAR_ASSIGN: run_assign_var(); break;
            case InstrType::INPUT: run_input(); break;
            case InstrType::WRITE_LN: run_write_ln(); break;
            case InstrType::WRITE: run_write(); break;
            case InstrType::UNLESS_SKIP: run_unless_skip(); break;
            case InstrType::IF_SKIP: run_if_skip(); break;
            case InstrType::DIVIDE: run_divide(); break;
            case InstrType::MULTIPLY: run_multiply(); break;
            case InstrType::ADD: run_add(); break;
            case InstrType::SUBTRACT: run_subtract(); break;
            default: run_not_implemented();
        }
    }
}

void Runtime::run_go() {
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
            const bool is_sign = m_instrs[above_cursor].type == InstrType::LABEL;
            const bool is_match = is_sign && m_instrs[above_cursor].args[0].value.get_string() == target_text;
            if (is_match) {
                m_instr_cursor = above_cursor;
                return;
            }
        }

        if (is_below_safe) {
            const bool is_sign = m_instrs[below_cursor].type == InstrType::LABEL;
            const bool is_match = is_sign && m_instrs[below_cursor].args[0].value.get_string() == target_text;
            if (is_match) {
                m_instr_cursor = below_cursor;
                return;
            }
        }
    }

    panic(ERR_MSG_NO_LABEL);
}

// TODO: check above one
// Note: above and below are according to the source .yok
// file, meaning above will check previous instructions
void Runtime::run_go_up() {
    const std::string &target_text = m_curr_instr->args[0].value.get_string();
    for (size_t cursor = m_instr_cursor; cursor > 0; cursor--) {
        const bool is_sign = m_instrs[cursor].type == InstrType::LABEL;
        const bool is_match = is_sign && m_instrs[cursor].args[0].value.get_string() == target_text;
        if (is_match) {
            m_instr_cursor = cursor;
            return;
        }
    }

    panic(ERR_MSG_NO_LABEL);
}

void Runtime::run_go_down() {
    const std::string &target_text = m_curr_instr->args[0].value.get_string();
    for (size_t cursor = m_instr_cursor + 1; cursor < m_instrs.size(); cursor++) {
        const bool is_sign = m_instrs[cursor].type == InstrType::LABEL;
        const bool is_match = is_sign && m_instrs[cursor].args[0].value.get_string() == target_text;
        if (is_match) {
            m_instr_cursor = cursor;
            return;
        }
    }

    panic(ERR_MSG_NO_LABEL);
}

void Runtime::run_call() {
    m_return_stack.push_back(m_instr_cursor);
    run_go();
}

void Runtime::run_return() {
    if (m_return_stack.empty())
        panic(ERR_MSG_NOWHERE_TO_RET);
    m_instr_cursor = m_return_stack.back();
    m_return_stack.pop_back();
}

void Runtime::run_input() {
    std::string input;
    std::getline(std::cin, input);
    m_vars[INPUT_STR_VAR_NAME].set_data(input);

    try {
        m_vars[INPUT_NUM_VAR_NAME].set_data(std::stof(input));
    }
    catch(const std::exception& e) {
        m_vars[INPUT_NUM_VAR_NAME].set_data(0.0f);
    }
}

void Runtime::run_write_ln() {
    const Field value = eval_arg(m_curr_instr->args[0]);
    if (value.is_float())
        std::cout << value.get_float() << "\n";
    else
        std::cout << value.get_string() << "\n";
}

void Runtime::run_write() {
    const Field value = eval_arg(m_curr_instr->args[0]);
    if (value.is_float())
        std::cout << value.get_float();
    else
        std::cout << value.get_string();
}

void Runtime::run_declare_var() {
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

bool Runtime::compare_fields(const Field &p_field1, const Field &p_field2, const std::string &p_cmp) {
    const float n1 = p_field1.get_float();
    const float n2 = p_field2.get_float();

    if (p_cmp == CMP_IS)        { if (p_field1.get_string() == p_field2.get_string()) return true; }
    else if (p_cmp == CMP_ISNT) { if (p_field1.get_string() != p_field2.get_string()) return true; }
    else if (p_cmp == CMP_GT)   { if (n1      > n2    ) return true; }
    else if (p_cmp == CMP_LT)   { if (n1      < n2    ) return true; }
    else if (p_cmp == CMP_GTE)  { if (n1     >= n2    ) return true; }
    else if (p_cmp == CMP_LTE)  { if (n1     <= n2    ) return true; }
    else                        { panic(ERR_MSG_INVALID_CMP); }

    return false;
}

void Runtime::skip_n_lines(size_t p_n) {
    for (size_t cursor = m_instr_cursor; cursor <= m_instrs.size(); cursor++) {
        if (m_instrs[cursor].line_no > m_curr_instr->line_no + p_n) {
            m_instr_cursor = cursor - 1;
            return;
        }
    }
}

void Runtime::run_unless_skip() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[2]);
    const std::string &cmp = m_curr_instr->args[1].value.get_string();

    if (compare_fields(field1, field2, cmp))
        return;

    const size_t ln_count = m_curr_instr->args[3].value.get_float();
    if (ln_count != m_curr_instr->args[3].value.get_float())
        panic(ERR_MSG_CANT_SKIP_NON_INT);

    skip_n_lines(ln_count);
}

void Runtime::run_if_skip() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[2]);
    const std::string &cmp = m_curr_instr->args[1].value.get_string();

    if (!compare_fields(field1, field2, cmp))
        return;

    const size_t ln_count = m_curr_instr->args[3].value.get_float();
    if (ln_count != m_curr_instr->args[3].value.get_float())
        panic(ERR_MSG_CANT_SKIP_NON_INT);
    skip_n_lines(ln_count);
}

void Runtime::run_divide() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() / field2.get_float();
        m_vars[RESULT_NUM_VAR_NAME].set_data(result);
        m_vars[RESULT_STR_VAR_NAME].set_data(std::to_string(result));
        return;
    }

    if (field1.is_string() && field2.is_float()) {
        m_vars[RESULT_NUM_VAR_NAME].set_data(0.0f);
        m_vars[RESULT_STR_VAR_NAME].set_data(field1.get_string().substr(0, field2.get_float()));
        return;
    }

    if (field1.is_float() && field2.is_string()) {
        const size_t start_pos = field1.get_float();
        if (start_pos >= field2.get_string().size())
            panic(ERR_MSG_STR_NOT_LONG_ENOUGH);

        m_vars[RESULT_NUM_VAR_NAME].set_data(0.0f);
        m_vars[RESULT_STR_VAR_NAME].set_data(field2.get_string().substr(start_pos));
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_multiply() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() * field2.get_float();
        m_vars[RESULT_NUM_VAR_NAME].set_data(result);
        m_vars[RESULT_STR_VAR_NAME].set_data(std::to_string(result));
        return;
    }

    if (field1.is_string() && field2.is_float()) {
        std::string result = "";
        for (size_t i = 0; i < field2.get_float(); i++)
            result += field1.get_string();
        m_vars[RESULT_NUM_VAR_NAME].set_data(0.0f);
        m_vars[RESULT_STR_VAR_NAME].set_data(result);
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_add() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() + field2.get_float();
        m_vars[RESULT_NUM_VAR_NAME].set_data(result);
        m_vars[RESULT_STR_VAR_NAME].set_data(std::to_string(result));
        return;
    }

    m_vars[RESULT_NUM_VAR_NAME].set_data(0.0f);
    m_vars[RESULT_STR_VAR_NAME].set_data(field1.get_string() + field2.get_string());
}

void Runtime::run_subtract() {
    const Field field1 = eval_arg(m_curr_instr->args[0]);
    const Field field2 = eval_arg(m_curr_instr->args[1]);

    if (field1.is_float() && field2.is_float()) {
        const float result = field1.get_float() - field2.get_float();
        m_vars[RESULT_NUM_VAR_NAME].set_data(result);
        m_vars[RESULT_STR_VAR_NAME].set_data(std::to_string(result));
        return;
    }

    panic(ERR_MSG_INVALID_COMB_OF_ARG_TYPES);
}

void Runtime::run_not_implemented() {
    panic(ERR_MSG_NOT_IMPLEMENTED);
}
