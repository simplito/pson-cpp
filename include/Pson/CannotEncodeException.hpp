/*
PrivMX Endpoint.
Copyright © 2024 Simplito sp. z o.o.

This file is part of the PrivMX Platform (https://privmx.dev).
This software is Licensed under the PrivMX Free License.

See the License for the specific language governing permissions and
limitations under the License.
*/

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
