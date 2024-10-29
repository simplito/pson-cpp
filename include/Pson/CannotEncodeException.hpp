#ifndef __PSON__CANNOTENCODEEXCEPTION_HPP__
#define __PSON__CANNOTENCODEEXCEPTION_HPP__

#include <exception>
#include <string>

namespace Pson {
    class CannotEncodeException : public std::invalid_argument
    {
    public:
        CannotEncodeException(const std::string& type)
            : std::invalid_argument(std::string("Cannot encode ").append(type)) {}
        
    };
}

#endif // __PSON__CANNOTENCODEEXCEPTION_HPP__
