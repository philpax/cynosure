#include "Utils.hpp"

std::string utils::EscapeCharacter(char input)
{
    switch (input)
    {
    case '\n':
        return "\\n";
    case '\t':
        return "\\t";
    case '\r':
        return "\\r";
    case 8:
        return "backspace";
    default:
        return std::string(1, input);
    };
}