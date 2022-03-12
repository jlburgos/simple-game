#ifndef _HEADERS_HPP
#define _HEADERS_HPP

/**
 * @file headers.hpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// Workaround for not using SDL_main() as entrypoint
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>

#if defined(__EMSCRIPTEN_major__)
#include <emscripten.h>
#endif

#if defined(__linux__)
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

#include "path.hpp"
#include "timer.hpp"
#include "logger.hpp"
#include "window_manager.hpp"

#endif /* _HEADERS_HPP */
