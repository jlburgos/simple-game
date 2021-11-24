/**
 * @file path.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "path.hpp"

#ifdef __linux__
#include <unistd.h>
#elif defined(__WIN32)
#include <windows.h>
#endif

#include <stdexcept>

std::string PathNS::get_exe_path()
{
    const int max_size = 256;
    char buffer[max_size];
#ifdef __linux__
    ssize_t length = sizeof(buffer);
    ssize_t ret_length = readlink("/proc/self/exe", buffer, length);
    ssize_t num_bytes = std::min(ret_length, length - 1);
#elif defined(__WIN32)
    /* Not calling GetModuleFileName(..) since it resolves to GetModuleFIleNameW since UNICODE is defined.
     * that that would include UNICODE filename support which I don't want to deal with. So calling
     * GetModuleFIleNameA(..) directly.
     */
    DWORD length = sizeof(buffer);
    int num_bytes = GetModuleFileNameA(NULL, buffer, length);
#else
#error "Expected either Linux or Windows platform!"
#endif
    if (num_bytes == -1)
    {
        throw PathException("Failed in GetModuleFileNameA call!");
    }
    if (num_bytes >= max_size)
    {
        throw PathException("Path to executable is longer than expected!");
    }
    buffer[num_bytes] = '\0';
    return std::string(buffer);
}

std::string PathNS::get_exe_path_no_ext()
{
    std::string exe_path = PathNS::get_exe_path();
    return exe_path.erase(exe_path.rfind(".exe"));
}

std::string PathNS::get_root_path()
{
    std::string exe_path = PathNS::get_exe_path();
    return exe_path.erase(exe_path.rfind("/"));
}

std::string PathNS::get_bin_path()
{
    return PathNS::get_root_path() + "/bin";
}

std::string PathNS::get_assets_path()
{
    return PathNS::get_root_path() + "/assets";
}