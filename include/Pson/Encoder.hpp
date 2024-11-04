/*
PrivMX Endpoint.
Copyright © 2024 Simplito sp. z o.o.

This file is part of the PrivMX Platform (https://privmx.dev).
This software is Licensed under the PrivMX Free License.

See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __PSON__ENCODER_HPP__
#define __PSON__ENCODER_HPP__

#include <ostream>
#include <Poco/BinaryWriter.h>
#include <Poco/Dynamic/Var.h>

namespace Pson {
    class Encoder
    {
    public:
        std::string encode(Poco::Dynamic::Var data);
        void encode(Poco::Dynamic::Var data, std::ostream &stream);
    private:
        void encodeType(Poco::Dynamic::Var data, Poco::BinaryWriter &bwriter);
    };
}

#endif // __PSON__ENCODER_HPP__