/*
PrivMX Endpoint.
Copyright © 2024 Simplito sp. z o.o.

This file is part of the PrivMX Platform (https://privmx.dev).
This software is Licensed under the PrivMX Free License.

See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __PSON__DECODER_HPP__
#define __PSON__DECODER_HPP__

#include <istream>
#include <vector>
#include <Poco/BinaryReader.h>
#include <Poco/Dynamic/Var.h>
#include <Pson/PsonParseHandler.hpp>

namespace Pson {
    class Decoder
    {
    public:
        Poco::Dynamic::Var decode(const std::string &data);
        Poco::Dynamic::Var decode(std::istream &stream);

        std::vector<std::string> dict;

    private:
        void decodeType(Poco::BinaryReader &breader, bool key = false);

        Pson::PsonParseHandler handler;
    };
}

#endif // __PSON__DECODER_HPP__
