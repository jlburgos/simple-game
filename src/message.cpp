#include "message.hpp"

Message::Message(std::string t, std::string f, std::thread::id id, std::string m) : timestamp(t), flag(f), threadid(id), msg(m)
{
    // Nothing to do
}
Message::~Message()
{
    // Nothing to do
}