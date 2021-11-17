#include "img_to_hpp.hpp"
#include "out_name.hpp"

std::vector<unsigned int> convert_in_file(const std::string src)
{
    std::vector<unsigned int> values;
    std::ifstream ifs;
    ifs.open(src, std::ios::in | std::ios::binary);
    if(!ifs.is_open())
    {
        std::cout << "Failed to open src file " << src << std::endl;
        return values;
    }
    while(ifs.good())
    {
        values.push_back(static_cast<unsigned int>(ifs.get()));
    }
    ifs.close();
    return values;
}

void write_out_file(const std::vector<unsigned int> &values, const struct out_name labels, const std::string dst)
{
    std::ofstream ofs;
    ofs.open(dst, std::ios::out | std::ios::trunc);
    if(!ofs.is_open())
    {
        std::cout << "Failed to open dst file " << dst << std::endl;
        return;
    }

    std::stringstream ss;
    ss << "#ifndef __" << labels.name << "_" << labels.ext << "_HPP\n";
    ss << "#define __" << labels.name << "_" << labels.ext << "_HPP\n\n";
    ss << "unsigned char " << labels.name << "_" << labels.ext << "[] = {\n\t";
    for(std::size_t i = 0; i < values.size(); ++i)
    {
        std::stringstream ss2;
        ss2 << std::setfill('0') << std::setw(2) << "0x" << std::hex << (0xff & values[i]);
        if(i + 1 != values.size())
        {
            ss2 << ", ";
        }
        if(i > 0 && i % 12 == 0)
        {
            ss2 << "\n\t";
        }
        ss << ss2.str();
    }
    ss << "\n};\n\nunsigned int " << labels.name << "_" << labels.ext << "_LEN = " << values.size() << ";\n\n";
    ss << "#endif /* __" << labels.name << "_" << labels.ext << "_HPP */" << std::endl;
    ofs.write(ss.str().c_str(), ss.str().length());
    ofs.close();
    std::cout << "Generated out file: " << dst << std::endl;
}