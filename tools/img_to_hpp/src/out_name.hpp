#ifndef __OUT_NAME_HPP
#define __OUT_NAME_HPP

#include <string>
#include <algorithm>

/**
 * @file out_name.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

struct out_name {
    std::string name;
    std::string ext;
};

struct out_name get_out_name(const std::string src);

#endif /* __OUT_NAME_HPP */