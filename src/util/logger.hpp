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

#include <string>
#include <thread>
#include <memory>

#include "concurrent_queue.hpp"
#include "message.hpp"

class Logger
{
public:
    static const std::unique_ptr<Logger> &get_logger();
    static void init();

    void info(const std::string message, ...);
    void warn(const std::string message, ...);
    void error(const std::string message, ...);
    const bool &is_thread_needed();
    const bool &get_health_status();
    bool has_messages();
    friend void manage_message_queue();

    class LoggerException : public std::runtime_error
    {
    public:
        LoggerException(const char *msg) : std::runtime_error(msg)
        {
            // Nothing to do
        }
    };

    ~Logger();

private:
    // Constructor is private because this is to be a singleton obj
    Logger();

    static void init_components();
    static std::once_flag init_flag;
    static const std::unique_ptr<Logger> logger;

    bool health_status;
    void set_health_status(const bool status);

    bool thread_needed;
    void initialize_worker_thread();

    std::unique_ptr<std::thread> thread;

    enum Flag
    {
        PINFO,
        PWARN,
        PERROR
    };

    Message create_message(Flag flag, const std::string &str);
    std::string fmt_message(const Message &msg);
    void write_message_buffer(const std::string &message);
    void write_message_buffers();
    ConcurrentQueue<Message> message_queue;

    std::string get_filename_rotated();
    std::string get_file_path();
    void initialize_log();
    void rotate_log();
    void set_rotation(unsigned int rot);
    unsigned int get_rotation();
    
    const std::size_t FILE_SIZE_LIMIT = 1E4; // 10,000 bytes
    std::string filename;
    unsigned int rotation;
}; 

// Define global logger
#define LOG (*Logger::get_logger())

#endif /* _LOGGER_HPP */
