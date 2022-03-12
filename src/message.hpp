/**
 * @file message.hpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-12-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __MESSAGE
#define __MESSAGE

#include <string>
#include <thread>

class Message
{
public:
    Message(std::string t, std::string f, std::thread::id id, std::string m);
    ~Message();
    std::string timestamp;
    std::string flag;
    std::thread::id threadid;
    std::string msg;
};

#endif /* __MESSAGE */