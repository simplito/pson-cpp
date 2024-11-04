#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <gtest/gtest.h>

#include <Pson/Decoder.hpp>
#include <Pson/BinaryString.hpp>
#include <Pson/Type.hpp>

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

namespace Pson {
namespace {

class DecoderTest : public ::testing::Test {

protected:
    Decoder d;

};

TEST_F(DecoderTest, BoolTrue){
    const bool x = true;
    const string x_pson("\xF1", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(bool));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, BoolFalse){
    const bool x = false;
    const string x_pson("\xF2", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(bool));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_0){
    const Int32 x = 0;
    const string x_pson("\x00", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_m1){
    const Int32 x = -1;
    const string x_pson("\x01", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_1){
    const Int32 x = 1;
    const string x_pson("\x02", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_119){
    const Int32 x = 119;
    const string x_pson("\xEE", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_m120){
    const Int32 x = -120;
    const string x_pson("\xEF", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_120){
    const Int32 x = 120;
    const string x_pson("\xF8\xF0\x01", 3);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_min){
    const Int32 x = 0x80000000;
    const string x_pson("\xF8\xFF\xFF\xFF\xFF\x0F", 6);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int32_max){
    const Int32 x = 0x7FFFFFFF;
    const string x_pson("\xF8\xFE\xFF\xFF\xFF\x0F", 6);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

/**
 * If typeof number is int64 and number can be store using int32, then Pson cast number to int32
*/

TEST_F(DecoderTest, Int64_0){
    const Int64 x = 0;
    const string x_pson("\x00", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_m1){
    const Int64 x = -1;
    const string x_pson("\x01", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_1){
    const Int64 x = 1;
    const string x_pson("\x02", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_119){
    const Int64 x = 119;
    const string x_pson("\xEE", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_m120){
    const Int64 x = -120;
    const string x_pson("\xEF", 1);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_120){
    const Int64 x = 120;
    const string x_pson("\xF8\xF0\x01", 3);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int32));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_min){
    const Int64 x = 0x8000000000000000;
    const string x_pson("\xF9\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01", 11);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int64));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Int64_max){
    const Int64 x = 0x7FFFFFFFFFFFFFFF;
    const string x_pson("\xF9\xFE\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01", 11);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Int64));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Float){
    const float x = 1234.56;
    const string x_pson = "\xFA" + string((char*)&x, 4);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(float));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Double){
    const double x = 1234.56;
    const string x_pson = "\xFB" + string((char*)&x, 8);
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(double));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, ArrayEmpty){
    Array::Ptr x = new Array();
    const string x_pson = "\xF4";
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Array::Ptr));
    EXPECT_EQ(Var(x).toString(), x_decoded.toString());
}

TEST_F(DecoderTest, Array){
    Array::Ptr x = new Array();
    x->add(2);
    x->add(3);
    const string x_pson = "\xF7\x02\x04\x06";
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Array::Ptr));
    EXPECT_EQ(Var(x).toString(), x_decoded.toString());
}

TEST_F(DecoderTest, ObjectEmpty){
    Object::Ptr x = new Object();
    const string x_pson = "\xF3";
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Object::Ptr));
    EXPECT_EQ(Var(x).toString(), x_decoded.toString());
}

TEST_F(DecoderTest, Object){
    Object::Ptr x = new Object();
    x->set("a", 2);
    x->set("b", 3);
    const string x_pson = string("\xF6\x02\xFC\x01") + string("a\x04\xFC\x01") + string("b\x06");
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(Object::Ptr));
    EXPECT_EQ(Var(x).toString(), x_decoded.toString());
}

TEST_F(DecoderTest, StringEmpty){
    const string x = "";
    const string x_pson = "\xF5";
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(string));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, String){
    const string x = "test";
    const string x_pson = "\xFC\x04" + x;
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(string));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, BinaryData){
    const string data = "test";
    const BinaryString x(data);
    const string x_pson = "\xFF\x04" + x;
    Var x_decoded = d.decode(x_pson);
    EXPECT_EQ(x_decoded.type(), typeid(BinaryString));
    EXPECT_EQ(x, x_decoded);
}

TEST_F(DecoderTest, Null){
    Var x;
    const string x_pson = "\xF0";
    Var x_decoded = d.decode(x_pson);
    EXPECT_TRUE(x_decoded.isEmpty());
    EXPECT_EQ(x, x_decoded);
}

} // namespace
} // namespace Pson
