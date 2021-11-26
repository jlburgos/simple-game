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

// TODO :: Add std::mutex if parallel access to file is needed

class Logger
{
public:
    static Logger *get_logger();
    void info(const std::string message, ...);
    void warn(const std::string message, ...);
    void error(const std::string message, ...);

    std::string get_filename_raw();
    std::string get_filename_rotated();
    std::string get_file_path();
    unsigned int get_file_size();

    void initialize_log();
    void rotate_log();
    void set_rotation(unsigned int);
    unsigned int get_rotation();
    
private:
    static Logger *logger;

    std::string filename;
    unsigned int rotation;

    const std::string PINFO = "INFO";
    const std::string PWARN = "WARN";
    const std::string PERROR = "ERROR";
#if !defined(__EMSCRIPTEN_major__)
    const unsigned int FILE_SIZE_LIMIT = 1E3; // 1,000 bytes
#endif

    // Constructors and assignment operator are private because this is to be a singleton obj
    Logger();

    void write_message_buffer(const std::string message, const std::string flag);
    std::string message_prefix(const std::string flag);
}; 

// Define global logger
#define LOG (*Logger::get_logger())

#endif /* _LOGGER_HPP */
