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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#if defined(__EMSCRIPTEN_major__)
#include <emscripten.h>
#endif

#if defined(__WIN32)
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

#include "util/path.hpp"
#include "util/timer.hpp"
#include "util/logger.hpp"
#include "window_manager.hpp"

#endif /* _HEADERS_HPP */
