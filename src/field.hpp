#pragma once
#include <string>
#include <variant>

class Field {
public:
    Field(std::variant<std::string, float> p_data);

    bool is_string();
    bool is_float();
    
    std::string get_string();
    float get_float();
private:
    std::variant<std::string, float> m_data;
};