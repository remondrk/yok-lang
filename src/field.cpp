#include <string>
#include <variant>
#include "field.hpp"

Field::Field(std::variant<std::string, float> p_data)
    : m_data(p_data) {}

inline bool Field::is_string() {
    return std::holds_alternative<std::string>(m_data);
}

inline bool Field::is_float() {
    return std::holds_alternative<float>(m_data);
}

std::string Field::get_string() {
    if (is_string())
        return std::get<std::string>(m_data);
    return std::to_string(std::get<float>(m_data));
}

float Field::get_float() {
    if (is_float())
        return std::get<float>(m_data);
    return 0.0f;
}