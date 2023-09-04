#include <sstream>
#include <string>
#include "error.hpp"

std::string make_err_msg(size_t ln, size_t col, const std::string &msg) {
    std::stringstream err_ss;
    err_ss << "[" << ln << ", " << col << "] ERROR: " << msg;
    return err_ss.str();
}
