#include "message_queue.hpp"

template<class T>
MessageQueue<T>::MessageQueue()
{
    // Nothing to do
}

template<class T>
MessageQueue<T>::~MessageQueue()
{
    // Nothing to do
}

template<class T>
std::size_t size()
{
    return queue.size();
}

template<class T>
void push(const T value)
{
    const std::lock_guard<std::mutex> lock(LOG.logger_mutex);
    queue.push(value);
}

template<class T>
T pop()
{
    const std::lock_guard<std::mutex> lock(LOG.logger_mutex);
    T value = queue.front();
    queue.pop();
    return value;
}