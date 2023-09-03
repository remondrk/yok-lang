#pragma once
#include <sstream>
#include <string>

std::string make_err_msg(size_t ln, size_t col, const std::string &msg);