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
const std::unique_ptr<Logger> Logger::logger = std::unique_ptr<Logger>(new Logger());
std::once_flag Logger::init_flag;


void manage_message_queue()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(LOG.thread_needed || !LOG.message_queue.empty())
        {
            LOG.write_message_buffers(LOG.message_queue);
        }
        else
        {
            break;
        }
    }
}

Logger::Logger()
{
    filename = get_file_path();
    rotation = 0;
    thread = nullptr;
#if !defined(__EMSCRIPTEN_major__) // Do not write to file if we build as web app
    thread_needed = true;
#else
    thread_needed = false;
#endif
}

Logger::~Logger()
{
    thread_needed = false;
    if(thread != nullptr)
    {
        thread->join();
    }
}

bool Logger::is_thread_needed()
{
    return thread_needed;
}

const std::unique_ptr<Logger> &Logger::get_logger()
{
    return logger;
}

void Logger::init()
{
    std::call_once(Logger::init_flag, Logger::init_components);
}

void Logger::init_components()
{
    logger->initialize_log();
    while (std::filesystem::file_size(logger->get_filename_rotated()) > 0)
    {
        logger->rotate_log();
    }
    if (logger->is_thread_needed())
    {
        logger->initialize_worker_thread();
    }
}

void Logger::initialize_worker_thread()
{
    thread = std::unique_ptr<std::thread>(new std::thread(manage_message_queue));
}

std::string Logger::get_filename_rotated()
{
    std::stringstream ss;
    ss << filename << "." << static_cast<int>(get_rotation()) << ".log";
    return ss.str();
}

void Logger::initialize_log()
{
    std::ofstream ofs;
    ofs.open(get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    ofs.close();
}

void Logger::rotate_log()
{
    set_rotation(rotation + 1);
    initialize_log();
}

unsigned int Logger::get_rotation()
{
    return rotation;
}

void Logger::set_rotation(unsigned int rot)
{
    rotation = rot;
}

std::string Logger::get_file_path()
{
    /*
    std::stringstream ss;
    ss << std::quoted(PathNS::get_bin_logs_path() + "/" + PathNS::get_exe_name_no_path());
    return ss.str();
    */
    return PathNS::get_bin_logs_path() + "/" + PathNS::get_exe_name_no_path();
}

Message Logger::create_message(Flag flag, const std::string &str)
{
    std::string f;
    switch(flag)
    {
        case Flag::PINFO:
            f = "INFO";
            break;
        case Flag::PWARN:
            f = "WARN";
            break;
        case Flag::PERROR:
            f = "ERROR";
            break;
    }
    return Message(TimerNS::current_time(), f, std::this_thread::get_id(), str);
}

void Logger::info(const std::string message, ...)
{
    std::string time = TimerNS::current_time();
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = create_message(Flag::PINFO, buffer);
    std::cout << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}

void Logger::warn(const std::string message, ...)
{
    std::string time = TimerNS::current_time();
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = create_message(Flag::PWARN, buffer);
    std::cout << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}

void Logger::error(const std::string message, ...)
{
    std::string time = TimerNS::current_time();
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = create_message(Flag::PERROR, buffer);
    std::cout << fmt_message(msg) << std::endl;
    if(thread_needed)
    {
        message_queue.push(msg);
    }
}

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

void Logger::write_message_buffers(ConcurrentQueue<Message> &messages)
{
    while(std::filesystem::file_size(get_filename_rotated()) > FILE_SIZE_LIMIT)
    {
        rotate_log();
    }
    std::ofstream ofs;
    ofs.open(get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    while(!messages.empty() && !ofs.fail()) // If fail, stop writing and close stream
    {
        ofs << fmt_message(messages.pop()) << "\n";
    }
    ofs.close();
}

std::string Logger::fmt_message(const Message &msg)
{
    std::stringstream ss;
    ss << "[" << msg.timestamp << "][THREAD " << msg.threadid << "] " << msg.flag << ": " << msg.msg;
    return ss.str();
}
