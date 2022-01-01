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

#ifndef __MESSAGE_QUEUE
#define __MESSAGE_QUEUE

#include <queue>
#include <mutex>
#include <cstddef>

#include "message.hpp"

class MessageQueue
{
public:
    MessageQueue();
    ~MessageQueue();

    bool empty() const;
    std::size_t size() const;
    void push(const Message message);
    Message pop();

private:
    std::queue<Message> queue;
    mutable std::mutex mutex;
};

#endif /* __MESSAGE_QUEUE */
