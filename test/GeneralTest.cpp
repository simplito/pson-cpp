#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <gtest/gtest.h>

#include <Pson/Encoder.hpp>
#include <Pson/Decoder.hpp>
#include <Pson/BinaryString.hpp>
#include <Pson/Type.hpp>

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

namespace Pson {
namespace {

TEST(GeneralTest, Main){
    Encoder e;
    Decoder d;
    
    Array::Ptr a = new Array();
    Array::Ptr b2 = new Array();

    a->add(Var());
    a->add(Array::Ptr(new Array()));
    a->add(Object::Ptr(new Object()));
    a->add("test");
    a->add(1234567.89);
    a->add(12345);

    Object::Ptr o = new Object(true);
    o->set("", "test3");
    o->set("p1", Var());
    o->set("p2", Array::Ptr(new Array()));
    o->set("p3", Object::Ptr(new Object()));
    o->set("p4", "test2");
    o->set("p5", 123.456789);
    o->set("p6", 12345678);
    o->set("p7", a);
    o->set("p8", "");

    string x_pson = Var(o).toString();

    string x_encoded = e.encode(o);
    string x_decoded = d.decode(x_encoded).toString();


    EXPECT_EQ(x_pson, x_decoded);
}

} // namespace
} // namespace Pson
