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
        LOG.info("Starting program...");
        SDL_SetMainReady();
        WindowManager mgr;

        LOG.info("Initializing window manager...");
        rc = mgr.init();
        if (rc != 0)
        {
            LOG.error("Failed to initialize program!\n");
        }
        else
        {
            LOG.info("Initialization completed! Bringing up window now...");
            mgr.start();
            mgr.close();

            LOG.info("Program completed with status '%d'! Exiting in a couple seconds...", EXIT_SUCCESS);
            sleep(2);
            rc = EXIT_SUCCESS;
        }
    }
    catch (const PathNS::PathException &ex)
    {
        std::cerr << "Path Exception: " << ex.what() << std::endl;
    }
    catch (const Logger::LoggerException &ex)
    {
        std::cerr << "Logger Exception: " << ex.what() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::string msg = "Exception: " + std::string(ex.what());
        if(Logger::get_logger() != nullptr && LOG.is_logger_healthy())
        {
            LOG.error(msg);
        }
        else
        {
            std::cerr << msg << std::endl;
        }
    }
    catch (...)
    {
        std::string msg = "Exception: Unknown exception occurred!";
        if(Logger::get_logger() != nullptr && LOG.is_logger_healthy())
        {
            LOG.error(msg);
        }
        else
        {
            std::cerr << msg << std::endl;
        }
    }
    if (Logger::get_logger() != nullptr && LOG.is_logger_healthy())
    {
        LOG.close_logger_thread();
    }
    return rc;
}
