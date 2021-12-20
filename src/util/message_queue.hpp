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

template <class T>
class MessageQueue
{
public:
    MessageQueue();
    ~MessageQueue();

    std::size_t size();
    void push(const T value);
    T pop();

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
};

#endif /* __MESSAGE_QUEUE */
