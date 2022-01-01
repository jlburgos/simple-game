/**
 * @file message_queue.hpp
 * @author Juan Burgos
 * @brief
 * @version 0.1
 * @date 2021-12-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "message_queue.hpp"

MessageQueue::MessageQueue() {}
MessageQueue::~MessageQueue() {}

bool MessageQueue::empty() const
{
    return size() == 0;
}

std::size_t MessageQueue::size() const
{
    return queue.size();
}

void MessageQueue::push(const Message message)
{
    const std::lock_guard<std::mutex> lock(mutex);
    queue.push(message);
}

Message MessageQueue::pop()
{
    const std::lock_guard<std::mutex> lock(mutex);
    Message message = queue.front();
    queue.pop();
    return message;
}