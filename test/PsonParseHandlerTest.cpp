#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Object.h>
#include <gtest/gtest.h>

#include <Pson/PsonParseHandler.hpp>

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

namespace Pson {
namespace {

class PsonParseHandlerTest : public ::testing::Test {

protected:
    PsonParseHandler d;

};

TEST_F(PsonParseHandlerTest, ShouldThrowException1){
    d.value(true);

    EXPECT_THROW({
        d.startArray();
    }, JSONException);
}

TEST_F(PsonParseHandlerTest, ShouldThrowException2){
    d.value(true);

    EXPECT_THROW({
        d.startObject();
    }, JSONException);
}

TEST_F(PsonParseHandlerTest, ShouldThrowException3){
    d.value(true);

    EXPECT_THROW({
        d.value(true);
    }, JSONException);
}

} // namespace
} // namespace Pson
