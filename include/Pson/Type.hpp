/*
PrivMX Endpoint.
Copyright © 2024 Simplito sp. z o.o.

This file is part of the PrivMX Platform (https://privmx.dev).
This software is Licensed under the PrivMX Free License.

See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __PSON__TYPE_HPP__
#define __PSON__TYPE_HPP__

#include <Poco/Types.h>

namespace Pson {
    class Type
    {
    public:
        static const Poco::UInt8 ZERO          = 0x00; // 0
        //                                       0x01; // -1
        //                                       0x02; // 1
        static const Poco::UInt8 MAX           = 0xEF; // -120, max. zig-zag encoded varint

        static const Poco::UInt8 PNULL         = 0xF0; // null
        static const Poco::UInt8 PTRUE          = 0xF1; // true
        static const Poco::UInt8 PFALSE         = 0xF2; // false
        static const Poco::UInt8 EOBJECT       = 0xF3; // {}
        static const Poco::UInt8 EARRAY        = 0xF4; // []
        static const Poco::UInt8 ESTRING       = 0xF5; // ""
        static const Poco::UInt8 OBJECT        = 0xF6; // {...}
        static const Poco::UInt8 ARRAY         = 0xF7; // [...]
        static const Poco::UInt8 INTEGER       = 0xF8; // number (zig-zag encoded varint32)
        static const Poco::UInt8 LONG          = 0xF9; // Long   (zig-zag encoded varint64)
        static const Poco::UInt8 FLOAT         = 0xFA; // number (float32)
        static const Poco::UInt8 DOUBLE        = 0xFB; // number (float64)
        static const Poco::UInt8 STRING        = 0xFC; // string (varint length + data)
        static const Poco::UInt8 STRING_ADD    = 0xFD; // string (varint length + data, add to dictionary)
        static const Poco::UInt8 STRING_GET    = 0xFE; // string (varint index to get from dictionary)
        static const Poco::UInt8 BINARY        = 0xFF; // bytes (varint length + data)
    };
}

#endif // __PSON__TYPE_HPP__
