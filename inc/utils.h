#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>

class Utils final
{
public:
    static std::string readFile(const std::string &filepath);
};

#endif // UTILS_H