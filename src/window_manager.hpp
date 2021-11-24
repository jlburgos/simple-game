#ifndef _MY_WINDOW_MANAGER_HPP
#define _MY_WINDOW_MANAGER_HPP

/**
 * @file window_manager.hpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-11-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <string>
//#include <memory>

#ifdef __linux__
#include <unistd.h>
#elif defined(__WIN32)
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

#include "headers.hpp"

class WindowManager
{
private:
    static const std::string DEFAULT_SCREEN_TITLE;
    static const int DEFAULT_SCREEN_WIDTH;
    static const int DEFAULT_SCREEN_HEIGHT;
    // std::unique_ptr<SDL_Window> window;
    // std::unique_ptr<SDL_Surface> surface;
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_RWops *rw_apple;
    static SDL_RWops *rw_plant;
    static SDL_Surface *surface_apple;
    static SDL_Surface *surface_plant;
    static SDL_Texture *texture_apple;
    static SDL_Texture *texture_plant;

public:
    WindowManager();
    WindowManager(const WindowManager &);
    WindowManager &operator=(const WindowManager &);
    ~WindowManager();
    static int init();
    static void start();
    static void close();
};

#endif /* _WINDOW_MANAGER_HPP */