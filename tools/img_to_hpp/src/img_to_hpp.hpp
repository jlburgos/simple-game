#ifndef __IMG_TO_HPP_HPP
#define __IMG_TO_HPP_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

/**
 * @file methods.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

std::vector<unsigned int> convert_file(const std::string src);
void write_file(const std::vector<unsigned int> &values, const struct out_name labels, const std::string dst);


#endif /* __IMG_TO_HPP_HPP */