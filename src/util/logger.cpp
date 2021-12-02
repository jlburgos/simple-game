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
#include <filesystem>

#include "path.hpp"
#include "logger.hpp"
#include "timer.hpp"

// Initialize static fields
Logger *Logger::logger = nullptr;                 // Initialize singleton pointer

void manage_message_queue()
{
    std::queue<Logger::Message> *frozen_q = nullptr;
    std::cout << "Beginning message queue log worker..." << std::endl;
    while(LOG.is_stay_alive())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Create a scope for the lock to protect queue assignment
        {
            const std::lock_guard<std::mutex> lock(LOG.logger_mutex);
            if(LOG.active_q == &LOG.q1)
            {
                LOG.active_q = &LOG.q2;
                frozen_q = &LOG.q1;
            }
            else
            {
                LOG.active_q = &LOG.q1;
                frozen_q = &LOG.q2;
            }
        }

        while(!frozen_q->empty())
        {
            std::string msg = LOG.fmt_message(frozen_q->front());
            std::cout << msg << std::endl;
            LOG.write_message_buffer(msg);
            frozen_q->pop();
        }
    }
    std::cout << "Exiting message queue log worker..." << std::endl;
}

Logger::Logger()
{
    this->logger_healthy = false;
    this->filename = get_file_path();
    this->rotation = 0;
    this->initialize_log();
    this->logger_healthy = true;
#if !defined(__EMSCRIPTEN_major__) // Do not write to file if we build as web app
    this->stay_alive = true;
    this->initialize_worker_thread();
#else
    this->thread = nullptr;
    this->stay_alive = false;
#endif
}

Logger::~Logger()
{
    this->close_logger_thread();
}

void Logger::close_logger_thread()
{
    std::cout << "Closing logger worker thread" << std::endl;
    this->stay_alive = false;
    if(this->thread != nullptr)
    {
        this->thread->join();
    }
    std::cout << "Logger thread decomissioned" << std::endl;
}

bool Logger::is_logger_healthy()
{
    return this->logger_healthy;
}

bool Logger::is_stay_alive()
{
    return this->stay_alive;
}

Logger *Logger::get_logger()
{
    if(logger == nullptr)
    {
        logger = new Logger();
    }
    return logger;
}

void Logger::initialize_worker_thread()
{
    this->thread = std::unique_ptr<std::thread>(new std::thread(manage_message_queue));
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

void Logger::set_rotation(unsigned int rot)
{
    this->rotation = rot;
}

std::string Logger::get_file_path()
{
    return PathNS::get_bin_logs_path() + "/" + PathNS::get_exe_name_no_path();
}

void Logger::info(std::string message, ...)
{
    std::string time = TimerNS::current_time();
    char buffer[256];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message.c_str(), args);
    va_end(args);
    Message msg = {time, this->PINFO, std::string(buffer)};
    if(this->stay_alive)
    {
        const std::lock_guard<std::mutex> lock(LOG.logger_mutex);
        active_q->push(msg);
    }
    else
    {
        std::cout << this->fmt_message(msg) << std::endl;
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
    Message msg = {time, this->PWARN, std::string(buffer)};
    if(this->stay_alive)
    {
        const std::lock_guard<std::mutex> lock(LOG.logger_mutex);
        active_q->push(msg);
    }
    else
    {
        std::cout << this->fmt_message(msg) << std::endl;
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
    Message msg = {time, this->PERROR, std::string(buffer)};
    if(this->stay_alive)
    {
        const std::lock_guard<std::mutex> lock(LOG.logger_mutex);
        this->active_q->push(msg);
    }
    else
    {
        std::cout << this->fmt_message(msg) << std::endl;
    }
}

void Logger::write_message_buffer(const std::string message)
{
    while(std::filesystem::file_size(this->get_filename_rotated()) > this->FILE_SIZE_LIMIT)
    {
        this->rotate_log();
    }
    std::ofstream ofs;
    ofs.open(this->get_filename_rotated(), std::ofstream::out | std::ofstream::app);
    ofs << message << std::endl;
    ofs.close();
}

std::string Logger::fmt_message(const Logger::Message &msg)
{
    std::stringstream ss;
    ss << "[" << msg.timestamp << "][THREAD " << std::this_thread::get_id() << "] " << msg.flag << ": " << msg.msg;
    return ss.str();
}
