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

#include "timer.hpp"

// TODO :: Add std::mutex if parallel access to file is needed

class Logger
{
public:
    static Logger *get_logger();
    void info(const std::string);
    void warn(const std::string);
    void error(const std::string);
    
private:
    std::string filename;
    unsigned int rotation;
    static Logger *logger;

    const std::string INFO = "INFO";
    const std::string WARN = "WARN";
    const std::string ERROR = "ERROR";
#if !defined(__EMSCRIPTEN_major__)
    const unsigned int FILE_SIZE_LIMIT = 1E4; // 10,000 bytes
#endif

    // Constructors and assignment operator are private because this is to be a singleton obj
    Logger();
    Logger(const std::string &_filename);
    Logger(const Logger &);
    Logger &operator=(const Logger &);

    void set_filename(const std::string &);
    std::string get_filename_raw() const;
    std::string get_filename_rotated() const;
    unsigned int get_file_size();
    void rotate_log();
    unsigned int get_rotation() const;
    void set_rotation(unsigned int);
    void write_message_buffer(const std::string, const std::string flag);
}; 

// Define global logger
#define LOG (*Logger::get_logger())

#endif /* _LOGGER_HPP */