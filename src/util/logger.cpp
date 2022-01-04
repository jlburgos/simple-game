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
#include <chrono>
#include <iomanip>
#include <filesystem>

#include "logger.hpp"
#include "path.hpp"
#include "timer.hpp"

// Initialize static fields
std::once_flag Logger::init_flag;
std::unique_ptr<std::thread> Logger::thread;
const std::size_t Logger::FILE_SIZE_LIMIT = 1E4; // 10,000 bytes
bool Logger::health_status = false;
bool Logger::thread_needed = false;
MessageQueue Logger::message_queue;
std::string Logger::filename = "";
std::size_t Logger::rotation;


void manage_message_queue()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(Logger::is_thread_needed() || !Logger::has_messages())
        {
            Logger::write_message_buffers();
        }
        else
        {
            break;
        }
    }
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

bool Logger::is_thread_needed()
{
    return thread_needed;
}

bool Logger::get_health_status()
{
    return health_status;
}

bool Logger::has_messages()
{
    return !Logger::message_queue.empty();
}

void Logger::set_health_status(const bool status)
{
    health_status = status;
}

void Logger::cleanup()
{
    thread_needed = false;
    if(thread != nullptr)
    {
        thread->join();
    }
}

void Logger::init()
{
    std::call_once(Logger::init_flag, Logger::init_components);
}

void Logger::init_components()
{
    if (std::atexit(&Logger::cleanup) != 0)
    {
        throw Logger::LoggerException("Failed to call configure Logger cleanup call on program exit");
    }
    thread = nullptr;
#if !defined(__EMSCRIPTEN_major__)
    thread_needed = true;
#else
    thread_needed = false;
#endif
    rotation = 0;
    health_status = false;
    Logger::init_log();
    while (std::filesystem::file_size(Logger::get_filename_rotated()) > 0)
    {
        Logger::rotate_log();
    }
    if (thread_needed)
    {
        Logger::init_worker_thread();
    }
    health_status = true;
    SDL_LogSetOutputFunction(&Logger::SDL_Logger_Callback, NULL);
}

void Logger::SDL_Logger_Callback(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    (void) userdata;
    (void) category;

    /* Notes:
     * https://wiki.libsdl.org/SDL_Log
     * ^^^ Alias to https://wiki.libsdl.org/SDL_LogInfo
     * https://wiki.libsdl.org/SDL_LogError
     * https://wiki.libsdl.org/SDL_LOG_CATEGORY
     */
    switch(priority)
    {
        case SDL_LOG_PRIORITY_INFO:
            Logger::log(Logger::Flag::PINFO, message);
            break;
        case SDL_LOG_PRIORITY_WARN:
            Logger::log(Logger::Flag::PWARN, message);
            break;
        case SDL_LOG_PRIORITY_VERBOSE:
            Logger::log(Logger::Flag::PVERBOSE, message);
            break;
        case SDL_LOG_PRIORITY_DEBUG:
            Logger::log(Logger::Flag::PDEBUG, message);
            break;
        case SDL_LOG_PRIORITY_CRITICAL:
            Logger::log(Logger::Flag::PCRITICAL, message);
            break;
        case SDL_LOG_PRIORITY_ERROR:
            Logger::log(Logger::Flag::PERROR, message);
            break;
        case SDL_NUM_LOG_PRIORITIES:
            throw LoggerException("Invalid message priority \"SDL_NUM_LOG_PRIORITIES\" received");
        default:
            std::stringstream ss;
            ss << "Unknown message priority \"" << priority << "\" received";
            throw LoggerException(ss.str().c_str());
    }
}

void Logger::init_worker_thread()
{
    Logger::thread = std::unique_ptr<std::thread>(new std::thread(&manage_message_queue));
}

std::string Logger::get_filename_rotated()
{
    std::stringstream ss;
    ss << filename << "." << get_rotation() << ".log";
    return ss.str();
}

void Logger::init_log()
{
    filename = get_file_path();
    std::ofstream ofs;
    ofs.open(get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    ofs.close();
}

void Logger::rotate_log()
{
    set_rotation(rotation + 1);
    init_log();
}

std::size_t Logger::get_rotation()
{
    return rotation;
}

void Logger::set_rotation(std::size_t rot)
{
    rotation = rot;
}

std::string Logger::get_file_path()
{
    return PathNS::get_bin_logs_path() + "/" + PathNS::get_exe_name_no_path();
}

Message Logger::create_message(Flag flag, const std::string &str)
{
    std::string f;
    switch(flag)
    {
        case Logger::Flag::PINFO:
            f = "INFO";
            break;
        case Logger::Flag::PWARN:
            f = "WARN";
            break;
        case Logger::Flag::PERROR:
            f = "ERROR";
            break;
        case Logger::Flag::PVERBOSE:
            f = "CRITICAL";
            break;
        case Logger::Flag::PDEBUG:
            f = "DEBUG";
            break;
        case Logger::Flag::PCRITICAL:
            f = "CRITICAL";
            break;
    }
    return Message(TimerNS::current_time(), f, std::this_thread::get_id(), str);
}

void Logger::log(Logger::Flag flag, const std::string &str)
{
    Message msg = create_message(flag, str);
    std::clog << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}

/*
void Logger::info(const std::string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = create_message(Flag::PINFO, buffer);
    std::clog << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}

void Logger::warn(const std::string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = create_message(Flag::PWARN, buffer);
    std::clog << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}

void Logger::error(const std::string message, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = create_message(Flag::PERROR, buffer);
    std::cerr << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}
*/

void Logger::write_message_buffer(const std::string &message)
{
    while(std::filesystem::file_size(get_filename_rotated()) > FILE_SIZE_LIMIT)
    {
        rotate_log();
    }
    std::ofstream ofs;
    ofs.open(get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    ofs << message << std::endl;
    ofs.close();
}

void Logger::write_message_buffers()
{
    while(std::filesystem::file_size(get_filename_rotated()) > FILE_SIZE_LIMIT)
    {
        rotate_log();
    }
    std::ofstream ofs;
    ofs.open(get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    while(!message_queue.empty() && !ofs.fail()) // If fail, stop writing and close stream
    {
        ofs << fmt_message(message_queue.pop()) << "\n";
    }
    ofs.close();
}

std::string Logger::fmt_message(const Message &msg)
{
    std::stringstream ss;
    ss << "[" << msg.timestamp << "][THREAD " << msg.threadid << "] " << msg.flag << ": " << msg.msg;
    return ss.str();
}
