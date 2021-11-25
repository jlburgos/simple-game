/**
 * @file logger.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-11-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>
#include <sstream>
#include <fstream>

#include "path.hpp"
#include "logger.hpp"
#include "timer.hpp"

// Initialize static fields
Logger *Logger::logger = nullptr;                 // Initialize singleton pointer

Logger::Logger()
{
    filename = get_file_path();
    rotation = 0;
}

Logger *Logger::get_logger()
{
    if(logger == nullptr)
    {
        logger = new Logger();
    }
    return logger;
}

void Logger::set_filename(const std::string &_filename)
{
    this->filename = _filename;
}

std::string Logger::get_filename_raw()
{
    return this->filename;
}

std::string Logger::get_filename_rotated()
{
    std::stringstream ss;
    ss << this->get_filename_raw() << "." << static_cast<int>(this->get_rotation()) << ".log";
    return ss.str();
}

void Logger::rotate_log()
{
    this->set_rotation(this->rotation + 1);
}

unsigned int Logger::get_rotation()
{
    return this->rotation;
}

void Logger::set_rotation(unsigned int _rotation)
{
    this->rotation = _rotation;
}

unsigned int Logger::get_file_size()
{
    std::ifstream is;
    is.open(this->get_filename_rotated(), std::ifstream::binary);
    unsigned int file_size = 0;
    if(is)
    {
        is.seekg(0, is.end);
        file_size = static_cast<unsigned int>(is.tellg());
        is.close();
    }
    else
    {
        std::stringstream ss;
        ss << "Failed to read file \"" << this->get_filename_rotated() << "\" to determine file size. Returning size value of \"0\".";
        this->error(ss.str());
    }
    return file_size;
}

std::string Logger::get_file_path()
{
    return PathNS::get_exe_path_no_ext(); // Using game binary (minus extension) as base for log file name
}

void Logger::info(const std::string _message)
{
    this->write_message_buffer(_message, this->PINFO);
}

void Logger::warn(const std::string _message)
{
    this->write_message_buffer(_message, this->PWARN);
}

void Logger::error(const std::string _message)
{
    this->write_message_buffer(_message, this->PERROR);
}

void Logger::write_message_buffer(const std::string _message, const std::string _flag)
{
    std::cout << this->message_prefix(_flag) << _message << std::endl;

// Do not write to file if we build as web app
#if !defined(__EMSCRIPTEN_major__)

    std::ofstream stream;
    stream.open(this->get_filename_rotated(), std::ios::out | std::ios::app);
    stream << this->message_prefix(_flag) << _message << std::endl;
    stream.flush();
    stream.close();

    if(this->get_file_size() > this->FILE_SIZE_LIMIT)
    {
        std::stringstream ss;
        ss << this->message_prefix(_flag) << ": Rotated log to rotation " << this->get_filename_rotated();
        std::cout << ss.str() << std::endl;

        this->rotate_log();
        stream.open(this->get_filename_rotated(), std::ios::out | std::ios::app);
        stream << ss.str() << std::endl;
        stream.flush();
        stream.close();
    }
#endif
}

std::string Logger::message_prefix(const std::string _flag)
{
    return "[" + TimerNS::current_time() + "] " + _flag + ": ";
}
