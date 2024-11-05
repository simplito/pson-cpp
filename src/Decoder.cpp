#include <sstream>
#include <Pson/Decoder.hpp>
#include <Pson/Type.hpp>

using namespace std;
using namespace Poco;
using namespace Pson;
using Poco::Dynamic::Var;
using Pson::PsonParseHandler;

Var Decoder::decode(const string &data){
    stringstream stream(data);
    return decode(stream);
}

Var Decoder::decode(istream &stream){
    handler = PsonParseHandler();
    BinaryReader breader(stream, BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
    decodeType(breader);
    return handler.asVar();
}

void Decoder::decodeType(BinaryReader &breader, bool key){
    UInt8 t;
    Int32 n32;
    UInt32 un32;
    UInt64 un64;
    float f32;
    double f64;
    string str;
    
    breader >> t;
    if(t <= Type::MAX){
        n32 = t;
        n32 = n32 & 1 ? ((n32 >> 1) ^ -1) : (n32 >> 1);
        handler.value(n32);
        return;
    }
    switch(t){
        case Type::PNULL:
            handler.null();
            return;
        case Type::PTRUE:
            handler.value(true);
            return;
        case Type::PFALSE:
            handler.value(false);
            return;
        case Type::EOBJECT:
            handler.startObject();
            handler.endObject();
            return;
        case Type::EARRAY:
            handler.startArray();
            handler.endArray();
            return;
        case Type::ESTRING:
            handler.value(string());
            return;
        case Type::OBJECT:
            breader.read7BitEncoded(un32);
            handler.startObject();
            while(un32 > 0){
                decodeType(breader, true);
                decodeType(breader);
                --un32;
            }
            handler.endObject();
            return;
        case Type::ARRAY:
            breader.read7BitEncoded(un32);
            handler.startArray();
            while(un32 > 0){
                decodeType(breader);
                --un32;
            }
            handler.endArray();
            return;
        case Type::INTEGER:
            breader.read7BitEncoded(un32);
            un32 = un32 & 1 ? ((un32 >> 1) ^ -1) : (un32 >> 1);
            handler.value(*((Int32 *)&un32));
            return;
        case Type::LONG:
            breader.read7BitEncoded(un64);
            un64 = un64 & 1 ? ((un64 >> 1) ^ -1) : (un64 >> 1);
            handler.value(*((Int64 *)&un64));
            return;
        case Type::FLOAT:
            breader >> f32;
            handler.value(f32);
            return;
        case Type::DOUBLE:
            breader >> f64;
            handler.value(f64);
            return;
        case Type::BINARY:
            breader.read7BitEncoded(un32);
            breader.readRaw(un32, str);
            handler.value(BinaryString(str));
            return;
        case Type::STRING:
            breader.read7BitEncoded(un32);
            breader.readRaw(un32, str);
            if(key)
                handler.key(str);
            else
                handler.value(str);
            return;
        case Type::STRING_ADD:
            breader.read7BitEncoded(un32);
            breader.readRaw(un32, str);
            dict.push_back(str);
            if(key)
                handler.key(str);
            else
                handler.value(str);
            return;
        case Type::STRING_GET:
            breader.read7BitEncoded(un32);
            str = dict[un32];
            if(key)
                handler.key(str);
            else
                handler.value(str);
            return;
    }
}
