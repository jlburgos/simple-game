#include "img_to_hpp.hpp"
#include "out_name.hpp"

std::vector<unsigned int> convert_in_file(const std::string src)
{
    std::vector<unsigned int> values;
    std::ifstream stream;
    stream.open(src, std::ios::in | std::ios::binary);
    if(!stream.is_open())
    {
        std::cout << "Failed to open src file " << src << std::endl;
        return values;
    }
    while(stream.good())
    {
        values.push_back(static_cast<unsigned int>(stream.get()));
    }
    stream.close();
    return values;
}

void write_out_file(const std::vector<unsigned int> &values, const struct out_name labels, const std::string dst)
{
    std::ofstream stream;
    stream.open(dst, std::ios::out | std::ios::trunc);
    if(!stream.is_open())
    {
        std::cout << "Failed to open dst file " << dst << std::endl;
        return;
    }

    std::stringstream ss;
    ss << "unsigned char " << labels.name << "_" << labels.ext << "[] = {\n";
    for(size_t i = 0; i < values.size(); ++i)
    {
        std::stringstream ss2;
        ss2 << std::setfill('0') << std::setw(2) << "0x" << std::hex << (0xff & values[i]);
        if(i + 1 != values.size())
        {
            ss2 << ", ";
        }
        if(i > 0 && i % 12 == 0)
        {
            ss2 << "\n";
        }
        ss << ss2.str();
    }
    ss << "\n};\nunsigned int " << labels.name << "_" << labels.ext << "_LEN = " << values.size() << ";" << std::endl;
    stream.write(ss.str().c_str(), ss.str().length());
    stream.close();
    std::cout << "Generated out file: " << dst << std::endl;
}