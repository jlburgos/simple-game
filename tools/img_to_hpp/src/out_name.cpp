#include "out_name.hpp"

struct out_name get_out_name(const std::string src)
{
    std::string src2 = src;
    for(std::size_t i = 0; i < src2.length(); ++i)
    {
        src2[i] == '\\' ? src2[i] = '/' : src2[i];
    }
    std::size_t pos1 = src2.find_last_of("/");
    std::size_t pos2 = src2.find_last_of(".");

    std::string name = src2.substr(pos1+1,pos2-(pos1+1));
    std::string ext = src2.substr(pos2+1,src.length()-(pos2+1));
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char ch){return std::toupper(ch);});
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch){return std::toupper(ch);});
    return {name,ext};
}