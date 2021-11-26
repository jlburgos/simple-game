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
#include <cstdio>
#include <sstream>
#include <fstream>
#include <cstdarg>
#include <thread>

#include "path.hpp"
#include "logger.hpp"
#include "timer.hpp"

// Initialize static fields
Logger *Logger::logger = nullptr;                 // Initialize singleton pointer

Logger::Logger()
{
    this->filename = get_file_path();
    this->rotation = 0;
    this->initialize_log();
}

Logger *Logger::get_logger()
{
    if(logger == nullptr)
    {
        logger = new Logger();
    }
    return logger;
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

void Logger::initialize_log()
{
    std::ofstream ofs;
    ofs.open(this->get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    ofs.close();
}

void Logger::rotate_log()
{
    this->set_rotation(this->rotation + 1);
    this->initialize_log();
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
    return PathNS::get_bin_logs_path() + "/" + PathNS::get_exe_name_no_path();
}

void Logger::info(const std::string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    this->write_message_buffer(std::string(buffer), this->PINFO);
}

void Logger::warn(const std::string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    this->write_message_buffer(std::string(buffer), this->PWARN);
}

void Logger::error(const std::string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    this->write_message_buffer(std::string(buffer), this->PERROR);
}

void Logger::write_message_buffer(const std::string message, const std::string flag)
{
    std::cout << this->message_prefix(flag) << message << std::endl;

// Do not write to file if we build as web app
#if !defined(__EMSCRIPTEN_major__)
    const std::lock_guard<std::mutex> lock(this->logger_mutex);
    while(this->get_file_size() > this->FILE_SIZE_LIMIT)
    {
        this->rotate_log();
    }
    std::ofstream ofs;
    ofs.open(this->get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    ofs << this->message_prefix(flag) << message << std::endl;
    ofs.flush();
    ofs.close();
#endif
}

std::string Logger::message_prefix(const std::string flag)
{
    std::stringstream ss;
    ss << "[" << TimerNS::current_time() << "][THREAD " << std::this_thread::get_id() << "] " << flag << ": ";
    return ss.str();
}
