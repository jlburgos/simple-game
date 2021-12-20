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