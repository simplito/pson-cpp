#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <gtest/gtest.h>

#include <Pson/Encoder.hpp>
#include <Pson/BinaryString.hpp>
#include <Pson/Type.hpp>
#include <Pson/CannotEncodeException.hpp>

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

namespace Pson {
namespace {

class EncoderTest : public ::testing::Test {

protected:
    Encoder e;

};

TEST_F(EncoderTest, BoolTrue){
    const bool x = true;
    const string x_pson("\xF1", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, BoolFalse){
    const bool x = false;
    const string x_pson("\xF2", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_0){
    const Int32 x = 0;
    const string x_pson("\x00", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_m1){
    const Int32 x = -1;
    const string x_pson("\x01", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_1){
    const Int32 x = 1;
    const string x_pson("\x02", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_119){
    const Int32 x = 119;
    const string x_pson("\xEE", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_m120){
    const Int32 x = -120;
    const string x_pson("\xEF", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_120){
    const Int32 x = 120;
    const string x_pson("\xF8\xF0\x01", 3);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_min){
    const Int32 x = 0x80000000;
    const string x_pson("\xF8\xFF\xFF\xFF\xFF\x0F", 6);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int32_max){
    const Int32 x = 0x7FFFFFFF;
    const string x_pson("\xF8\xFE\xFF\xFF\xFF\x0F", 6);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_0){
    const Int64 x = 0;
    const string x_pson("\x00", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_m1){
    const Int64 x = -1;
    const string x_pson("\x01", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_1){
    const Int64 x = 1;
    const string x_pson("\x02", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_119){
    const Int64 x = 119;
    const string x_pson("\xEE", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_m120){
    const Int64 x = -120;
    const string x_pson("\xEF", 1);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_120){
    const Int64 x = 120;
    const string x_pson("\xF8\xF0\x01", 3);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_min){
    const Int64 x = 0x8000000000000000;
    const string x_pson("\xF9\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01", 11);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Int64_max){
    const Int64 x = 0x7FFFFFFFFFFFFFFF;
    const string x_pson("\xF9\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01", 11);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Float){
    const float x = 1234.56;
    const string x_pson = "\xFA" + string((char*)&x, 4);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Double){
    const double x = 1234.56;
    const string x_pson = "\xFB" + string((char*)&x, 8);
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, ArrayEmpty){
    Array::Ptr x = new Array();
    const string x_pson = "\xF4";
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Array){
    Array::Ptr x = new Array();
    x->add(2);
    x->add(3);
    const string x_pson = "\xF7\x02\x04\x06";
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, ObjectEmpty){
    Object::Ptr x = new Object();
    const string x_pson = "\xF3";
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Object){
    Object::Ptr x = new Object();
    x->set("a", 2);
    x->set("b", 3);
    const string x_pson = string("\xF6\x02\xFC\x01") + string("a\x04\xFC\x01") + string("b\x06");
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, ObjectWithEmptyKey){
    Object::Ptr x = new Object();
    x->set("", 2);
    x->set("b", 3);
    const string x_pson = string("\xF6\x02\xF5") + string("\x04\xFC\x01") + string("b\x06");
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, StringEmpty){
    const string x = "";
    const string x_pson = "\xF5";
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, String){
    const string x = "test";
    const string x_pson = "\xFC\x04" + x;
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, BinaryData){
    const string data = "test";
    const BinaryString x(data);
    const string x_pson = "\xFF\x04" + x;
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, Null){
    Var x;
    const string x_pson = "\xF0";
    string x_encoded = e.encode(x);
    EXPECT_EQ(x_pson, x_encoded);
}

TEST_F(EncoderTest, DefaultValue){
    class UncommonClass {};
    const UncommonClass x;

    EXPECT_THROW({
        string x_encoded = e.encode(x);
    }, CannotEncodeException);
}

} // namespace
} // namespace Pson