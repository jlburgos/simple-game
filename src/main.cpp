/**
 * @file main.cpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-11-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "headers.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc == 0) {
        std::cerr << "Something has gone terribly wrong! There's no arg 0! This is usually the path to binary...";
        return 1;
    }

    int rc = EXIT_FAILURE;
    try
    {
        // Workaround for not using SDL_main() as entrypoint
        SDL_SetMainReady();

        // Using std::filesystem we avoid needing to implement OS dependent way to finding exe path
        PathNS::set_exe_path(argv[0]);

        // Initialize SDL
        if (!Custom_SDL_Wrapper::init())
        {
            throw std::runtime_error("Ending program due to previous error!");
        }
        std::atexit(Custom_SDL_Wrapper::unload);

        // Initialize custom logger
        Logger::init();

        WindowManager mgr;
        rc = mgr.init();
        if (rc != 0)
        {
            throw std::runtime_error("Failed to initialize window manager");
        }
        mgr.start();
        rc = EXIT_SUCCESS;
    }
    catch (const PathNS::PathException &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Path Exception: %s", ex.what());
    }
    catch (const Logger::LoggerException &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Logger Exception: %s", ex.what());
    }
    catch (const std::runtime_error &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Runtime Error: %s", ex.what());
    }
    catch (const std::exception &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Exception: %s", ex.what());
    }
    catch (...)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Unknown Exception Occurred");
    }

    SDL_Log("Closing program!");

    return rc;
}
