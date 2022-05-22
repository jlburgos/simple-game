#ifndef _CONTROLLER_HPP
#define _CONTROLLER_HPP

#include <SDL.h>

class Controller
{
private:
	// Key designations
	uint8_t up_key = SDL_SCANCODE_UP;
	uint8_t down_key = SDL_SCANCODE_DOWN;
	uint8_t left_key = SDL_SCANCODE_LEFT;
	uint8_t right_key = SDL_SCANCODE_RIGHT;
	uint8_t menu_key = SDL_SCANCODE_ESCAPE;
	uint8_t select_key = SDL_SCANCODE_SPACE;

	// Key press status -- Points to values in SDL_GetKeyboardState(nullptr)
	const uint8_t* keyboard_state = nullptr; // SDL_GetKeyboardState(nullptr);

public:
	Controller() = default;
	Controller(const Controller&) = default;
	~Controller() = default;

	void set_up_key(enum SDL_ScanCode v);
	void set_down_key(enum SDL_ScanCode v);
	void set_left_key(enum SDL_ScanCode v);
	void set_right_key(enum SDL_ScanCode v);
	void set_menu_key(enum SDL_ScanCode v);
	void set_select_key(enum SDL_ScanCode v);

	uint8_t get_up_key(enum SDL_ScanCode v);
	uint8_t get_down_key(enum SDL_ScanCode v);
	uint8_t get_left_key(enum SDL_ScanCode v);
	uint8_t get_right_key(enum SDL_ScanCode v);
	uint8_t get_menu_key(enum SDL_ScanCode v);
	uint8_t get_select_key(enum SDL_ScanCode v);

	void get_keyboard_state();

	uint8_t get_up_status();
	uint8_t get_down_status();
	uint8_t get_left_status();
	uint8_t get_right_status();
	uint8_t get_menu_status();
	uint8_t get_select_status();
};

#endif /* _CONTROLLER_HPP */