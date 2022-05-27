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

#if defined(__linux__)
#include <unistd.h>
#define MAX_PATH 256
#elif defined(OSX)
#include <unistd.h>
//#include <mach-o/dyld.h>
#define MAX_PATH 256
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <iostream>
#include <stdexcept>

#include <SDL.h>

#include "path.hpp"

std::string PathNS::get_exe_path_filesystem2()
{
  return fs::current_path();
}

void PathNS::set_exe_path(const std::string_view path) {
    exe_path = std::filesystem::absolute(std::filesystem::path(path));
}

std::string PathNS::get_exe_path()
{
    if (exe_path.empty())
    {
       throw PathException("Failed to set up exe path!");
    }
    return exe_path;
}

/*
std::string PathNS::get_exe_path()
{
    if (exe_path.empty())
    {
        char buffer[MAX_PATH];
#if defined(__linux__)
        ssize_t num_bytes = readlink("/proc/self/exe", buffer, MAX_PATH);
        if (num_bytes < 0 || num_bytes >= MAX_PATH)
        {
            throw PathException("Failed in readlink call!");
        }
#elif defined(OSX)
        // Note: Of course, OSX had to do things differently ...
        // https://stackoverflow.com/questions/22675457/what-is-the-equivalent-of-proc-self-exe-on-macintosh-os-x-mavericks
        // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/dyld.3.html
        // _NSGetExecutablePath() copies the path of the main executable into the
        // buffer buf.  The bufsize parameter should initially be the size of the
        // buffer.  This function returns 0 if the path was successfully copied.  It
        // returns -1 if the buffer is not large enough, and * bufsize is set to the
        // size required.  Note that _NSGetExecutablePath() will return "a path" to
        // the executable not a "real path" to the executable.  That is, the path
        // may be a symbolic link and not the real file. With deep directories the
        // total bufsize needed could be more than MAXPATHLEN.
        uint32_t num_bytes = MAX_PATH;
        if (_NSGetExecutablePath(buffer, &num_bytes) != 0)
        {
            throw PathException("Failed in _NSGetExecutablePath() call!");
        }
#elif defined(_WIN32) || defined(_WIN64)
        // Not calling GetModuleFileName(..) since it resolves to GetModuleFIleNameW since UNICODE is defined.
        // that that would include UNICODE filename support which I don't want to deal with. So calling
        // GetModuleFIleNameA(..) directly.
        DWORD num_bytes = GetModuleFileNameA(NULL, buffer, MAX_PATH);
        if (num_bytes == 0)
        {
            throw PathException("Failed in GetModuleFileNameA call!");
        }
#else
#error "This is not a supported platform! Only support Windows, Linux, OSX"
#endif
        buffer[num_bytes] = '\0'; // Need this otherwise we're going to have a bad time
        std::string path(buffer);
        for (std::size_t i = 0; i < path.length(); ++i)
        {
            if (path[i] == '\\')
            {
                path[i] = '/';
            }
        }
        exe_path = path;
    }

    return exe_path;
}
*/

std::string PathNS::get_exe_path_no_ext()
{
    if (exe_path_no_ext.empty())
    {
        std::string path = PathNS::get_exe_path();
        std::size_t pos = path.rfind(".exe");
        if (pos != std::string::npos)
        {
            path.erase(path.rfind(".exe"));
        }
        exe_path_no_ext = path;
    }
    return exe_path_no_ext;
}

std::string PathNS::get_exe_name_no_path()
{
    if (exe_name_no_path.empty())
    {
        std::string path = PathNS::get_exe_path_no_ext();
        std::size_t pos = path.rfind("/");
        if (pos != std::string::npos)
        {
            path = path.substr(pos + 1);
        }
        exe_name_no_path = path;
    }

    return exe_name_no_path;
}

std::string PathNS::get_exe_dir_path()
{
    if (exe_dir_path.empty())
    {
        std::string path = PathNS::get_exe_path();
        std::size_t pos = path.rfind("/");

        // Remove executable name from path
        if (pos != std::string::npos)
        {
            path.erase(pos);
        }

        exe_dir_path = path;
    }

    return exe_dir_path;
}

std::string PathNS::get_root_path()
{
    if (root_path.empty())
    {
        std::string path = PathNS::get_exe_path();
        std::size_t pos = path.rfind("/");

        // Remove executable name from path
        if (pos != std::string::npos)
        {
            path.erase(pos);
        }

        // Remove "debug" or "release" directory name from path
        pos = path.rfind("/");
        if (pos != std::string::npos)
        {
            path.erase(pos);
        }

        // Remove "bin" directory name from path
        pos = path.rfind("/");
        if (pos != std::string::npos)
        {
            path.erase(pos);
        }

        root_path = path;
    }

    return root_path;
}

std::string PathNS::get_bin_path()
{
    return PathNS::get_root_path() + "/bin";
}

std::string PathNS::get_bin_logs_path()
{
    return PathNS::get_root_path() + "/logs";
}

std::string PathNS::get_assets_path()
{
    return PathNS::get_root_path() + "/assets";
}
