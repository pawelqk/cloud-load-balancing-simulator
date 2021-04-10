#include "Stdout.hpp"

#include <iostream>

namespace logger
{

void Stdout::log(const char *message)
{
    std::cout << message << '\n';
}

} // namespace logger
