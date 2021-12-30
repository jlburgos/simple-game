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

#include <iostream>
#include "headers.hpp"

int main(void)
{
    int rc = EXIT_FAILURE;
    try
    {
        SDL_SetMainReady();

        // Initialize custom logger
        Logger::init();
        if(atexit(&Logger::cleanup) != 0)
        {
            Logger::cleanup(); // Manually cleanup then throw exception
            throw Logger::LoggerException("Failed to call configure Logger cleanup call on program exit");
        }

        SDL_Log("Initializing window manager...");
        WindowManager mgr;
        rc = mgr.init();
        if (rc != 0)
        {
            SDL_Log("Failed to initialize program!\n");
        }
        else
        {
            SDL_Log("Initialization completed! Bringing up window now...");
            mgr.start();
            mgr.close();

            SDL_Log("Program completed with status '%d'!", EXIT_SUCCESS);
            rc = EXIT_SUCCESS;
        }
    }
    catch (const PathNS::PathException &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Path Exception: %s", ex.what());
    }
    catch (const Logger::LoggerException &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Logger Exception: %s", ex.what());
    }
    catch (const std::exception &ex)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Exception: %s", ex.what());
    }
    catch (...)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Unknown Exception Occurred");
    }

    return rc;
}
