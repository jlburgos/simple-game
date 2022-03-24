/**
 * @file window_manager.cpp
 * @author Juan Burgos
 * @brief 
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <cstdio>
#include <sstream>

#include "window_manager.hpp"
#include "path.hpp"
#include "logger.hpp"

int WindowManager::init()
{
    // Initialize screen window
    screen_window = unique_window_ptr(
        SDL_CreateWindow(
            DEFAULT_SCREEN_TITLE,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        )
    );
    if (screen_window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Window: %s", SDL_GetError());
        return 1;
    }

    // Create temporary window surface
    auto screen_surface = unique_surface_ptr(SDL_GetWindowSurface(screen_window.get()));
    if (screen_surface == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Surface: %s", SDL_GetError());
        return 1;
    }

    // Set window surface in renderer
    renderer = unique_renderer_ptr(SDL_CreateRenderer(screen_window.get(), -1, SDL_RENDERER_ACCELERATED));
    if(renderer == nullptr)
    {
        SDL_Log("Failed to create SDL_Renderer: %s", SDL_GetError());
        return 1;
    }

/*
    SDL_FillRect(screen_surface.get(), nullptr, SDL_MapRGB(screen_surface.get()->format, 0xaf, 0xaf, 0xff));
    SDL_UpdateWindowSurface(screen_window.get());
    SDL_Log("Showing a white screen...");
    SDL_Delay(2000);
    SDL_Log("Switching to a blue screen...");
*/
    
    Entity entity;
    if(!entity.set_entity(renderer, PathNS::get_assets_path() + "/test-imgs/blocky-sprite.png"))
    {
        SDL_Log("Failed to set entity");
        return 1;
    }

    Entity background;
    if (!background.set_background(renderer, PathNS::get_assets_path() + "/test-imgs/test-background.png"))
    {
        SDL_Log("Failed to set background");
        return 1;
    }

    entities.push_back(entity);
    entities.push_back(background);
    return 0;
}

void WindowManager::start()
{
    // Fill the surface white
    //SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    SDL_Event e;
    bool quit = false;
    uint8_t color = 0x0;
    Entity entity = entities[0];
    Entity background = entities[1];
    const uint8_t* key_states = SDL_GetKeyboardState(nullptr);
    const uint8_t* up = &key_states[SDL_SCANCODE_UP];
    const uint8_t* down = &key_states[SDL_SCANCODE_DOWN];
    const uint8_t* left = &key_states[SDL_SCANCODE_LEFT];
    const uint8_t* right = &key_states[SDL_SCANCODE_RIGHT];

    uint8_t alpha = 0;
    bool add = true;
    while(!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                SDL_Log("Breaking from loop!");
                quit = true;
            }
            /*if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
            {
                up    = key_states[SDL_SCANCODE_UP];
                down  = key_states[SDL_SCANCODE_DOWN];
                left  = key_states[SDL_SCANCODE_LEFT];
                right = key_states[SDL_SCANCODE_RIGHT];
            }*/
            /*if (e.type == SDL_KEYUP)
            {
                if (!key_states[SDL_SCANCODE_UP])
                {
                    up = 0;
                    SDL_Log("UNPressed UP");
                    SDL_Log("keystate: %d", key_states[SDL_SCANCODE_UP]);
                }
                if (!)
                {
                    down = 0;
                    SDL_Log("UNPressed DOWN");
                }
                if (!)
                {
                    left = 0;
                    SDL_Log("UNPressed LEFT");
                }
                if (!)
                {
                    right = 0;
                    SDL_Log("Pressed RIGHT");
                }
            }
            if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
            {
                if (key_states[SDL_SCANCODE_UP])
                {
                    up = 1;
                    SDL_Log("Pressed UP");
                    SDL_Log("keystate: %d", key_states[SDL_SCANCODE_UP]);
                }
                if (key_states[SDL_SCANCODE_DOWN])
                {
                    down = 1;
                    SDL_Log("Pressed DOWN");
                }
                if (key_states[SDL_SCANCODE_LEFT])
                {
                    left = 1;
                    SDL_Log("Pressed LEFT");
                }
                if (key_states[SDL_SCANCODE_RIGHT])
                {
                    right = 1;
                    SDL_Log("Pressed RIGHT");
                }
            }*/
        }
        if (*up)     entity.move(0, -10);
        if (*down)   entity.move(0, 10);
        if (*left)   entity.move(-10, 0);
        if (*right)  entity.move(10, 0);
        //entity.show();

        //SDL_SetRenderDrawColor(renderer.get(), 0x0, 0x0, color, 0xff);
        //color += 1;

        if (alpha == 255)
            add = false;
        else if (alpha == 0)
            add = true;

        if (add)
            alpha += 5;
        else
            alpha -= 5;
        SDL_SetTextureAlphaMod(entity.get_texture().get(), alpha);

        SDL_RenderClear(renderer.get());
        background.render_background();
        entity.render_entity();
        SDL_RenderPresent(renderer.get());

        SDL_Delay(10);
    }
}

