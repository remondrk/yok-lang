#include <string>
#include <variant>
#include "field.hpp"

Field::Field() {
    m_data = {};
}

Field::Field(std::variant<std::string, float> p_data)
    : m_data(p_data) {}

void Field::set_data(std::variant<std::string, float> p_data) {
    m_data = p_data;
}

bool Field::is_string() const {
    return std::holds_alternative<std::string>(m_data);
}

bool Field::is_float() const {
    return std::holds_alternative<float>(m_data);
}

std::string Field::get_string() const {
    if (is_string())
        return std::get<std::string>(m_data);
    return std::to_string(std::get<float>(m_data));
}

float Field::get_float() const {
    if (is_float())
        return std::get<float>(m_data);
    return 0.0f;
}
