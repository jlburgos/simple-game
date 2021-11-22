/**
 * @file main.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include "img_to_hpp.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "./<prog> [src] [target]" << std::endl;
        return 1;
    }

    std::string src = argv[1];
    std::string dst = argv[2];

    std::vector<unsigned int> values = convert_in_file(src);
    if(values.empty())
    {
        return 1;
    }
    struct out_name name = get_out_name(src);
    return write_out_file(values, name, dst);
}

