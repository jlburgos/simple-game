#ifndef _MY_WINDOW_MANAGER_HPP
#define _MY_WINDOW_MANAGER_HPP

#include <string>
#include <memory>

#include "headers.hpp"
#include "custom_sdl_wrapper.hpp"
#include "entity.hpp"
#include "asset_manager.hpp"
#include "layer.hpp"

class WindowManager
{
private:
    const char* DEFAULT_SCREEN_TITLE = "Game Window";
    const int DEFAULT_SCREEN_WIDTH = 720;
    const int DEFAULT_SCREEN_HEIGHT = 480;

    unique_window_ptr screen_window = nullptr;
    shared_renderer_ptr renderer = nullptr;
    
    std::vector<Layer> layers;

    std::vector<Entity> entities;

public:
    WindowManager() = default;
    ~WindowManager() = default;

    int init();
    void start();

    class WindowManagerException : public std::runtime_error
    {
    public:
        explicit WindowManagerException(const char* msg);
    };
};

#endif /* _WINDOW_MANAGER_HPP */