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
#include <mutex>
#include <queue>
#include <thread>
#include <memory>

class Logger
{
public:
    static Logger *get_logger();
    void info(const std::string message, ...);
    void warn(const std::string message, ...);
    void error(const std::string message, ...);
    bool is_logger_healthy();
    bool is_stay_alive();
    friend void manage_message_queue();
    void close_logger_thread();

    class LoggerException : public std::runtime_error
    {
    public:
        LoggerException(const char *msg) : std::runtime_error(msg)
        {
            // Nothing to do
        }
    };

private:
    // Constructors and assignment operator are private because this is to be a singleton obj
    Logger();
    ~Logger();

    bool logger_healthy;
    bool stay_alive;

    std::unique_ptr<std::thread> thread;

    class Message
    {
    public:
        Message(std::string t, std::string f, std::string m) : timestamp(t), flag(f), msg(m) {}
        ~Message() {}
        std::string timestamp, flag, msg;
    };
    std::string fmt_message(const Logger::Message &msg);
    std::queue<Message> q1,q2;
    std::queue<Message> *active_q = &q1; // Initialize to this default
    void initialize_worker_thread();

    std::string get_filename_raw();
    std::string get_filename_rotated();
    std::string get_file_path();
    std::size_t get_file_size();

    void initialize_log();
    void rotate_log();
    void set_rotation(unsigned int rot);
    unsigned int get_rotation();
    
    static Logger *logger;

    std::string filename;
    unsigned int rotation;

    void write_message_buffer(const std::string message);

    const std::string PINFO = "INFO";
    const std::string PWARN = "WARN";
    const std::string PERROR = "ERROR";

#if !defined(__EMSCRIPTEN_major__)
    const unsigned int FILE_SIZE_LIMIT = 1E3; // 1,000 bytes
    std::mutex logger_mutex;
#endif
}; 

// Define global logger
#define LOG (*Logger::get_logger())

#endif /* _LOGGER_HPP */
