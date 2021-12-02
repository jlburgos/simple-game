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

#ifdef __linux__
#include <unistd.h>
#elif defined(__WIN32)
#include <windows.h>
#endif
#include <iostream>
#include <stdexcept>

#include "path.hpp"

std::string PathNS::get_exe_path()
{
    char buffer[MAX_PATH];
#ifdef __linux__
    ssize_t length = sizeof(buffer);
    ssize_t ret_length = readlink("/proc/self/exe", buffer, length);
    ssize_t num_bytes = std::min(ret_length, length - 1);
    if (num_bytes == -1)
    {
        throw PathException("Failed in readlink call!");
    }
#elif defined(__WIN32)
    /* Not calling GetModuleFileName(..) since it resolves to GetModuleFIleNameW since UNICODE is defined.
     * that that would include UNICODE filename support which I don't want to deal with. So calling
     * GetModuleFIleNameA(..) directly.
     */
    DWORD length = sizeof(buffer);
    DWORD num_bytes = GetModuleFileNameA(NULL, buffer, length);
    if (num_bytes == static_cast<DWORD>(0))
    {
        throw PathException("Failed in GetModuleFileNameA call!");
    }
#else
#error "Expected either Linux or Windows platform!"
#endif
    buffer[num_bytes] = '\0'; // Need this otherwise we're going to have a bad time
    std::string path(buffer);
    for(std::size_t i = 0; i < path.length(); ++i)
    {
        path[i] == '\\' ? path[i] = '/' : path[i];
    }
    return path;
}

std::string PathNS::get_exe_path_no_ext()
{
    std::string path = PathNS::get_exe_path();
    std::size_t pos = path.rfind(".exe");
    if(pos != std::string::npos)
    {
        path.erase(path.rfind(".exe"));
    }
    return path;
}

std::string PathNS::get_exe_name_no_path()
{
    std::string path = PathNS::get_exe_path_no_ext();
    std::size_t pos = path.rfind("/");
    if(pos != std::string::npos)
    {
        path = path.substr(pos + 1);
    }

    return path;
}

std::string PathNS::get_root_path()
{
    std::string path = PathNS::get_exe_path();
    std::size_t pos = path.rfind("/");

    // Remove executable name from path
    if(pos != std::string::npos)
    {
        path.erase(pos);
    }

    // Remove "bin" directory name from path
    pos = path.rfind("/");
    if(pos != std::string::npos)
    {
        path.erase(pos);
    }

    return path;
}

std::string PathNS::get_bin_path()
{
    return PathNS::get_root_path() + "/bin";
}

std::string PathNS::get_bin_logs_path()
{
    return PathNS::get_root_path() + "/bin/logs";
}

std::string PathNS::get_assets_path()
{
    return PathNS::get_root_path() + "/assets";
}