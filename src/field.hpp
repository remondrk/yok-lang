#pragma once
#include <string>
#include <variant>

class Field {
public:
    Field();
    Field(std::variant<std::string, float> p_data);

    void set_data(std::variant<std::string, float> p_data);

    bool is_string() const;
    bool is_float() const;
    
    std::string get_string() const;
    float get_float() const;
private:
    std::variant<std::string, float> m_data;
};
