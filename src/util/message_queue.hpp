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
    MessageQueue() {}
    ~MessageQueue() {}

    bool empty() const
    {
        return size() == 0;
    }

    std::size_t size() const
    {
        return queue.size();
    }

    void push(const T value)
    {
        const std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
    }

    T pop()
    {
        const std::lock_guard<std::mutex> lock(mutex);
        T value = queue.front();
        queue.pop();
        return value;
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
};

#endif /* __MESSAGE_QUEUE */
