cmake_minimum_required(VERSION 3.11.4)

project(Pson)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
message(STATUS "Build using shared libraries - BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")

option(PSON_BUILD_PROGRAMS "Build pson2json and json2pson programs" OFF)
message(STATUS "Build pson2json and json2pson programs - PSON_BUILD_PROGRAMS=${PSON_BUILD_PROGRAMS}")

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/Decoder.cpp
    ${PROJECT_SOURCE_DIR}/src/Encoder.cpp
    ${PROJECT_SOURCE_DIR}/src/PsonParseHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/PsonCInterface.cpp
)
set(INCLUDES
    ${PROJECT_SOURCE_DIR}/include/Pson/Decoder.hpp
    ${PROJECT_SOURCE_DIR}/include/Pson/Encoder.hpp
    ${PROJECT_SOURCE_DIR}/include/Pson/Type.hpp
    ${PROJECT_SOURCE_DIR}/include/Pson/BinaryString.hpp
    ${PROJECT_SOURCE_DIR}/include/Pson/PsonParseHandler.hpp
    ${PROJECT_SOURCE_DIR}/include/Pson/pson.h
    ${PROJECT_SOURCE_DIR}/include/Pson/CannotEncodeException.hpp
)

option(PSON_IMPORTED_LIBRARIES "Use imported libraries" OFF)
message(STATUS "Use imported libraries - PSON_IMPORTED_LIBRARIES=${PSON_IMPORTED_LIBRARIES}")

function(PSON_IMPORT_LIB ALIAS_NAME LIB_PATH IMPLIB_PATH INCLUDES_PATH)
    add_library("${ALIAS_NAME}" UNKNOWN IMPORTED GLOBAL)
    set_property(TARGET "${ALIAS_NAME}" PROPERTY IMPORTED_LOCATION "${${LIB_PATH}}")
    if(${IMPLIB_PATH})
        set_property(TARGET "${ALIAS_NAME}" PROPERTY IMPORTED_IMPLIB "${${IMPLIB_PATH}}")
    endif(${IMPLIB_PATH})
    target_include_directories("${ALIAS_NAME}" INTERFACE "${${INCLUDES_PATH}}")
endfunction(PSON_IMPORT_LIB ALIAS_NAME LIB_PATH IMPLIB INCLUDES_PATH)

if(PSON_IMPORTED_LIBRARIES)
    pson_import_lib(Poco::Foundation PSON_POCO_FOUNDATION PSON_POCO_FOUNDATION_IMPLIB PSON_POCO_INCLUDES)
    pson_import_lib(Poco::JSON PSON_POCO_JSON PSON_POCO_JSON_IMPLIB PSON_POCO_INCLUDES)
else(PSON_IMPORTED_LIBRARIES)
    find_package(Poco REQUIRED COMPONENTS Foundation JSON)
endif(PSON_IMPORTED_LIBRARIES)

add_library(Pson ${SOURCES})
target_include_directories(Pson PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Pson PUBLIC Poco::Foundation Poco::JSON)
install(TARGETS Pson)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/Pson DESTINATION include)

if(PSON_BUILD_PROGRAMS)
    add_executable(json2pson ${PROJECT_SOURCE_DIR}/src/Json2Pson.cpp)
    add_executable(pson2json ${PROJECT_SOURCE_DIR}/src/Pson2Json.cpp)
    target_link_libraries(json2pson PUBLIC Poco::Foundation Poco::JSON Pson)
    target_link_libraries(pson2json PUBLIC Poco::Foundation Poco::JSON Pson)
    install(TARGETS json2pson)
    install(TARGETS pson2json)
endif(PSON_BUILD_PROGRAMS)

option(PSON_ENABLE_TESTS "Building tests" OFF)
message(STATUS "Building test - PSON_ENABLE_TESTS=${PSON_ENABLE_TESTS}")
if(PSON_ENABLE_TESTS)
    include(FindGTest)
    include(GoogleTest)
    enable_testing()
    find_package(GTest REQUIRED)
    add_executable(PsonCppTest test/main.cpp
        test/DecoderTest.cpp
        test/EncoderTest.cpp
        test/GeneralTest.cpp
        test/PsonParseHandlerTest.cpp
        test/PsonCInterfaceTest.cpp)
    target_link_libraries(PsonCppTest PUBLIC Poco::Foundation Poco::JSON Pson GTest::GTest GTest::Main)
    gtest_add_tests(TARGET PsonCppTest)
endif(PSON_ENABLE_TESTS)
