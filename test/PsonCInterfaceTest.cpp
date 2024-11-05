#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include <gtest/gtest.h>

#include <Pson/pson.h>

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

namespace Pson {
namespace {

class PsonCInterfaceTest : public ::testing::Test {
public:
    void SetUp() override {
        pson_parse("\xf6\x01\xfc\x04\x6b\x65\x79\x31\xfc\x06\x76\x61\x6c\x75\x65\x31", 16, &value);
    }
    void TearDown() override {
        pson_free_result(value);
    }

protected:
    pson_value* value;

};

TEST_F(PsonCInterfaceTest, ShouldReturnString){
    pson_value* res;

    int status = pson_object_get_value_or_null_as_copy(value, "key1", &res);

    EXPECT_TRUE(status);

    EXPECT_FALSE(pson_is_null(res));
    EXPECT_STREQ("value1", pson_get_cstring(res));

    pson_free_result(res);
}

TEST_F(PsonCInterfaceTest, ShouldReturnNull){
    pson_value* res;

    int status = pson_object_get_value_or_null_as_copy(value, "key2", &res);

    EXPECT_TRUE(status);

    EXPECT_TRUE(pson_is_null(res));

    pson_free_result(res);
}

} // namespace
} // namespace Pson
