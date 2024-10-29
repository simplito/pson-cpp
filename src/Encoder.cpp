#include <sstream>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/JSON.h>
#include <Pson/Encoder.hpp>
#include <Pson/Type.hpp>
#include <Pson/BinaryString.hpp>
#include <Pson/CannotEncodeException.hpp>

using namespace std;
using namespace Poco;
using namespace Pson;
using Poco::Dynamic::Var;
using Poco::JSON::Object;
using Poco::JSON::Array;

string Encoder::encode(Var data){
    stringstream stream;
    encode(data, stream);
    return stream.str();
}

void Encoder::encode(Var data, ostream &stream){
    BinaryWriter bwriter(stream, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
    encodeType(data, bwriter);
}

void Encoder::encodeType(Var data, BinaryWriter &bwriter){
    if(data.type() == typeid(Object::Ptr)){
        Object::Ptr o = data.extract<Object::Ptr>();
        if(o->size() > 0){
            bwriter << Type::OBJECT;
            bwriter.write7BitEncoded((UInt64)o->size());
        } else {
            bwriter << Type::EOBJECT;
        }
        for(Object::Iterator it = o->begin(); it != o->end(); ++it){
            if(it->first.size() > 0){
                bwriter << Type::STRING;
                bwriter.write7BitEncoded((UInt64)it->first.size());
                bwriter.writeRaw(it->first);
            } else {
                bwriter << Type::ESTRING;
            }
            encodeType(it->second, bwriter);
        }
    } else if(data.type() == typeid(Array::Ptr)){
        Array::Ptr a = data.extract<Array::Ptr>();
        if(a->size() > 0){
            bwriter << Type::ARRAY;
            bwriter.write7BitEncoded((UInt64)a->size());
        } else {
            bwriter << Type::EARRAY;
        }
        for(size_t i = 0; i < a->size(); ++i){
            encodeType(a->get(i), bwriter);
        }
    } else if(data.type() == typeid(string)){
        string str = data.extract<string>();
        if(str.length() > 0){
            bwriter << Type::STRING;
            bwriter.write7BitEncoded((UInt64)str.size());
            bwriter.writeRaw(str);
        } else {
            bwriter << Type::ESTRING;
        }
    } else if(data.type() == typeid(BinaryString)){
        const BinaryString& bin = data.extract<BinaryString>();
        bwriter << Type::BINARY;
        bwriter.write7BitEncoded((UInt64)bin.size());
        bwriter.writeRaw(bin.data(), bin.size());
    } else if(data.type() == typeid(Int64)){
        Int64 n = data.extract<Int64>();
        n = ((n << 1) ^ (n >> 63));
        UInt64 *un = (UInt64 *)&n;
        if(*un <= Type::MAX){
            bwriter << (UInt8)*un;
        } else {
            if(*un < (((UInt64)1) << 32))
                bwriter << Type::INTEGER;
            else
                bwriter << Type::LONG;
            bwriter.write7BitEncoded(*un);
        }
    } else if(data.type() == typeid(Int32)){
        Int32 n = data.extract<Int32>();
        n = ((n << 1) ^ (n >> 31));
        UInt32 *un = (UInt32 *)&n;
        if(*un <= Type::MAX){
            bwriter << (UInt8)*un;
        } else {
            bwriter << Type::INTEGER;
            bwriter.write7BitEncoded(*un);
        }
    } else if(data.type() == typeid(double)){
        bwriter << Type::DOUBLE;
        bwriter << data.extract<double>();
    }  else if(data.type() == typeid(float)){
        bwriter << Type::FLOAT;
        bwriter << data.extract<float>();
    } else if(data.type() == typeid(bool)){
        if(data.extract<bool>() == true){
            bwriter << Type::PTRUE;
        } else {
            bwriter << Type::PFALSE;
        }
    } else if(data.type() == typeid(void)) {
        // case: data.isEmpty() (null or undefined)
        bwriter << Type::PNULL;
    } else {
        // case: uncommon type (as exception)
        throw CannotEncodeException(data.type().name());
    }
}
