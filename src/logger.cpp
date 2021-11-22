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

#include "logger.hpp"
#include "timer.hpp"

// Initialize static fields
Logger *Logger::logger = nullptr;                 // Initialize singleton pointer

/**
 * @brief Construct a new Logger:: Logger object
 * 
 */
Logger::Logger()
{
    filename = get_file_path();
    rotation = 0;
}

/**
 * @brief Construct a new Logger:: Logger object
 * 
 */
Logger::Logger(const std::string &)
{
    /* Do nothing since this is private */
}

/**
 * @brief Construct a new Logger:: Logger object
 * /
 */
Logger::Logger(const Logger &)
{
    /* Do nothing since this is private */
}

/**
 * @brief 
 * 
 * @return Logger& 
 */
Logger &Logger::operator=(const Logger &)
{
    return *this;
}

/**
 * @brief 
 * 
 * @return Logger* 
 */
Logger *Logger::get_logger()
{
    if(logger == nullptr)
    {
        logger = new Logger();
    }
    return logger;
}

/**
 * @brief 
 * 
 * @param _filename 
 */
void Logger::set_filename(const std::string &_filename)
{
    this->filename = _filename;
}

/**
 * @brief 
 * 
 * @return std::string 
 */
std::string Logger::get_filename_raw() const
{
    return this->filename;
}

/**
 * @brief 
 * 
 * @return std::string 
 */
std::string Logger::get_filename_rotated() const
{
    std::stringstream ss;
    ss << this->get_filename_raw() << "." << static_cast<int>(this->get_rotation()) << ".log";
    return ss.str();
}

/**
 * @brief 
 * 
 */
void Logger::rotate_log()
{
    this->set_rotation(this->rotation + 1);
}

/**
 * @brief 
 * 
 * @return unsigned int 
 */
unsigned int Logger::get_rotation() const
{
    return this->rotation;
}

/**
 * @brief 
 * 
 * @param _rotation 
 */
void Logger::set_rotation(unsigned int _rotation)
{
    this->rotation = _rotation;
}

/**
 * @brief 
 * 
 * @return unsigned int 
 */
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

/**
 * @brief
 *
 * @return std::string
 */
std::string Logger::get_file_path()
{
  char buffer[256];
  ssize_t length = sizeof(buffer);
#ifdef __linux__
  ssize_t ret_length = readlink("/proc/self/exe", buffer, length);
  ssize_t num_bytes = std::min(ret_length, length-1);
#elif __WIN32
  int num_bytes = GetModuleFileName(NULL, buffer, length);
#else
#error "Expected either Linux or Windows platform!"
#endif
  if(num_bytes != -1)
  {
    buffer[num_bytes] = '\0';
  }
  return std::string(buffer);
}

/**
 * @brief 
 * 
 * @param _message 
 */
void Logger::info(const std::string _message)
{
    this->write_message_buffer(_message, this->INFO);
}

/**
 * @brief 
 * 
 * @param _message 
 */
void Logger::warn(const std::string _message)
{
    this->write_message_buffer(_message, this->WARN);
}

/**
 * @brief 
 * 
 * @param _message 
 */
void Logger::error(const std::string _message)
{
    this->write_message_buffer(_message, this->ERROR);
}

/**
 * @brief 
 * 
 * @param _message 
 * @param _flag 
 */
void Logger::write_message_buffer(const std::string _message, const std::string _flag)
{
    std::cout << this->message_prefix(_flag) << _message << std::endl;

// Do not write to file if we build as web app
#if !defined(__EMSCRIPTEN_major__)

    std::cout << this->message_prefix(_flag) << "Writing logs to file..." << std::endl;
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
    return "[" + Timer::current_time() + "] " + _flag + ": ";
}