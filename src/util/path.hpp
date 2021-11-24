/**
 * @file path.hpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __PATH_HPP
#define __PATH_HPP

#include <string>
#include <stdexcept>

namespace PathNS
{
    class PathException : public std::runtime_error
    {
    public:
        PathException(const char *msg) : std::runtime_error(msg)
        {
            // Nothing to do
        }
    };

    std::string get_exe_path();
    std::string get_exe_path_no_ext();
    std::string get_bin_path();
    std::string get_root_path();
    std::string get_assets_path();
}

#endif /* __PATH_HPP */