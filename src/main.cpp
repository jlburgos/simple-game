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
    try
    {
        LOG.info("Starting program...");
        SDL_SetMainReady();
        WindowManager mgr;

        LOG.info("Initializing window manager...");
        int rc = mgr.init();
        if (rc != 0)
        {
            LOG.error("Failed to initialize program!\n");
            return rc;
        }
        LOG.info("Initialization completed! Bringing up window now...");
        mgr.start();
        mgr.close();

        LOG.info("Program completed! Exiting in a couple seconds...");
        sleep(2);
        return EXIT_SUCCESS;
    }
    catch (const PathNS::PathException &ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
