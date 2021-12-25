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
        Logger::init();
        LOG.info("Starting program...");

        SDL_SetMainReady();
        LOG.info("Initializing window manager...");

        WindowManager mgr;
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

            LOG.info("Program completed with status '%d'!", EXIT_SUCCESS);
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
        std::string msg = "Std Exception: " + std::string(ex.what());
        if(LOG.get_health_status())
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
        if(LOG.get_health_status())
        {
            LOG.error(msg);
        }
        else
        {
            std::cerr << msg << std::endl;
        }
    }

    return rc;
}
