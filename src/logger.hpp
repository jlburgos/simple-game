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

#include <cstdio>
#include <iostream>
#include <fstream>

#ifdef __linux__
#include <unistd.h>
#elif defined(__WIN32)
#include <windows.h>
#endif

#include "timer.hpp"

// TODO :: Add std::mutex if parallel access to file is needed

class Logger
{
public:
    static Logger *get_logger();
    void info(const std::string);
    void warn(const std::string);
    void error(const std::string);

    void set_filename(const std::string &_filename);
    std::string get_filename_raw();
    std::string get_filename_rotated();
    std::string get_file_path();
    unsigned int get_file_size();

    void rotate_log();
    void set_rotation(unsigned int _rotation);
    unsigned int get_rotation();
    
private:
    static Logger *logger;

    std::string filename;
    unsigned int rotation;

    const std::string PINFO = "INFO";
    const std::string PWARN = "WARN";
    const std::string PERROR = "ERROR";
#if !defined(__EMSCRIPTEN_major__)
    const unsigned int FILE_SIZE_LIMIT = 1E4; // 10,000 bytes
#endif

    // Constructors and assignment operator are private because this is to be a singleton obj
    Logger();
    Logger(const std::string &_filename);
    Logger(const Logger &);
    Logger &operator=(const Logger &_logger);

    void write_message_buffer(const std::string _message, const std::string _flag);
    std::string message_prefix(const std::string _flag);
}; 

// Define global logger
#define LOG (*Logger::get_logger())

#endif /* _LOGGER_HPP */
