#include <fstream>
#include <sstream>

#include "utils.h"

std::string read_file(const char* name)
{
    std::ifstream file(name);
    if (!file)
    {
        throw std::runtime_error(std::string("can't read ") + name);
    }

    std::stringstream reader;
    reader << file.rdbuf();

    return reader.str();
}
