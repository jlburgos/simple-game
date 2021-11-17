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

#include "headers.hpp"
#include <string>
//#include <memory>

class WindowManager
{
private:
    const std::string DEFAULT_SCREEN_TITLE = "Game Window";
    const int DEFAULT_SCREEN_WIDTH = 720;
    const int DEFAULT_SCREEN_HEIGHT = 480;

    // std::unique_ptr<SDL_Window> window;
    // std::unique_ptr<SDL_Surface> surface;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_RWops *rw_apple;
    SDL_RWops *rw_plant;
    SDL_Surface *surface_apple;
    SDL_Surface *surface_plant;
    SDL_Texture *texture_apple;
    SDL_Texture *texture_plant;

public:
    WindowManager();
    WindowManager(const WindowManager &);
    WindowManager &operator=(const WindowManager &);
    ~WindowManager();
    int init();
    void start();
    void close();
};

#endif /* _WINDOW_MANAGER_HPP */