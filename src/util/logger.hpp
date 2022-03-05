/**
 * @file logger.hpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-11-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include <SDL.h>

#include <string>
#include <thread>
#include <memory>

#include "message_queue.hpp"
#include "message.hpp"

class Logger
{
public:
    static void init();
    static void cleanup();
    ~Logger(); // Needs to be public

    static bool is_thread_needed();
    static bool get_health_status();

    class LoggerException : public std::runtime_error
    {
    public:
        LoggerException(const char *msg) : std::runtime_error(msg)
        {
            // Nothing to do
        }
    };

private:
    // Constructor is private because this is to be a singleton obj
    Logger();
    
    static bool health_status;
    static bool thread_needed;
    static const std::size_t FILE_SIZE_LIMIT;
    static std::string filename;
    static std::size_t rotation;
    static MessageQueue message_queue;
    static std::once_flag init_flag;

    enum Flag
    {
        PINFO,
        PWARN,
        PERROR,
        PVERBOSE,
        PDEBUG,
        PCRITICAL
    };

    // Function matching the callbackSignature (SDL_LogOutputFunction)
    static void SDL_Logger_Callback(void *userdata, int category, SDL_LogPriority priority, const char *message);
    static void log(Logger::Flag flag, const std::string &message);
    /*
    static void info(const std::string message, ...);
    static void warn(const std::string message, ...);
    static void error(const std::string message, ...);
    */

    static void set_health_status(const bool status);
    static bool has_messages();

    static void init_log();
    static void init_components();
    static void init_worker_thread();
    friend void manage_message_queue();

    static std::unique_ptr<std::thread> thread;

    static Message create_message(Flag flag, const std::string &str);
    static std::string fmt_message(const Message &msg);
    static void write_message_buffer(const std::string &message);
    static void write_message_buffers();

    static std::string get_filename_rotated();
    static std::string get_file_path();
    static void rotate_log();
    static void set_rotation(std::size_t rot);
    static std::size_t get_rotation();
}; 

#endif /* _LOGGER_HPP */
