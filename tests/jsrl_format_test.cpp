/**
 * Copyright 2026 Adobe. All rights reserved.
 * This file is licensed to you under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
 * OF ANY KIND, either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */

#include "../src/jsrl_format.hpp"
#include <gtest/gtest.h>
#include <format>
#include <sstream>
#include <string>

namespace {
    using jsrl::Json;
    using jsrl::GeneralNumber;
    using jsrl::JsonPrettyPrint;
    using jsrl::pretty_print;
    using std::string;
    using std::ostringstream;

    /*! @brief Helper function to compare std::format output with operator<< output
     */
    template<typename T>
    void verify_format_matches_ostream(T const& value) {
        // Get output from operator<<
        ostringstream oss;
        oss << value;
        string ostream_result = oss.str();

        // Get output from std::format
        string format_result = std::format("{}", value);

        // They should be identical
        EXPECT_EQ(format_result, ostream_result);
    }

    // Tests for jsrl::Json formatting
    TEST(JsonFormatTest, NullValue) {
        Json json;
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "null");
    }

    TEST(JsonFormatTest, BooleanTrue) {
        Json json(true);
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "true");
    }

    TEST(JsonFormatTest, BooleanFalse) {
        Json json(false);
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "false");
    }

    TEST(JsonFormatTest, IntegerValue) {
        Json json(42);
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "42");
    }

    TEST(JsonFormatTest, NegativeInteger) {
        Json json(-123);
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "-123");
    }

    TEST(JsonFormatTest, FloatingPointValue) {
        Json json(3.14);
        verify_format_matches_ostream(json);
        // Note: exact output may vary based on encoding options
    }

    TEST(JsonFormatTest, StringValue) {
        Json json("hello world");
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "\"hello world\"");
    }

    TEST(JsonFormatTest, StringWithEscapes) {
        Json json("line1\nline2\ttab");
        verify_format_matches_ostream(json);
        // Should have escaped newline and tab
    }

    TEST(JsonFormatTest, EmptyArray) {
        Json json(Json::ArrayBody{});
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "[]");
    }

    TEST(JsonFormatTest, SimpleArray) {
        Json json(Json::ArrayBody{Json(1), Json(2), Json(3)});
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "[1,2,3]");
    }

    TEST(JsonFormatTest, EmptyObject) {
        Json json(Json::ObjectBody{});
        verify_format_matches_ostream(json);
        EXPECT_EQ(std::format("{}", json), "{}");
    }

    TEST(JsonFormatTest, SimpleObject) {
        Json json(Json::ObjectBody{
            {"name", Json("John")},
            {"age", Json(30)}
        });
        verify_format_matches_ostream(json);
        // Note: object key order may vary
    }

    TEST(JsonFormatTest, NestedStructure) {
        Json json(Json::ObjectBody{
            {"user", Json(Json::ObjectBody{
                {"name", Json("Alice")},
                {"id", Json(123)}
            })},
            {"items", Json(Json::ArrayBody{Json(1), Json(2), Json(3)})}
        });
        verify_format_matches_ostream(json);
    }

    TEST(JsonFormatTest, ParsedJson) {
        string json_str = R"({"key": ["value1", true, 42]})";
        Json json = Json::parse(json_str);
        verify_format_matches_ostream(json);
    }

    // Tests for jsrl::Json::OptionedWrite formatting
    TEST(JsonOptionedWriteFormatTest, LooseFloats) {
        Json json(3.14159);
        auto optioned = loose_floats(Json::OptionedWrite(json));
        verify_format_matches_ostream(optioned);
    }

    TEST(JsonOptionedWriteFormatTest, ExactNumbers) {
        Json json(1.23456789);
        auto optioned = exact_numbers(Json::OptionedWrite(json));
        verify_format_matches_ostream(optioned);
    }

    // Tests for jsrl::GeneralNumber formatting
    TEST(GeneralNumberFormatTest, PositiveInteger) {
        GeneralNumber num(42LL);
        verify_format_matches_ostream(num);
        EXPECT_EQ(std::format("{}", num), "42");
    }

    TEST(GeneralNumberFormatTest, NegativeInteger) {
        GeneralNumber num(-123LL);
        verify_format_matches_ostream(num);
        EXPECT_EQ(std::format("{}", num), "-123");
    }

    TEST(GeneralNumberFormatTest, Zero) {
        GeneralNumber num(0LL);
        verify_format_matches_ostream(num);
        EXPECT_EQ(std::format("{}", num), "0");
    }

    TEST(GeneralNumberFormatTest, LargeInteger) {
        GeneralNumber num(9223372036854775807LL);
        verify_format_matches_ostream(num);
    }

    TEST(GeneralNumberFormatTest, FloatingPoint) {
        GeneralNumber num(3.14159L);
        verify_format_matches_ostream(num);
    }

    TEST(GeneralNumberFormatTest, ParsedNumber) {
        GeneralNumber num = GeneralNumber::parse("123.456");
        verify_format_matches_ostream(num);
        // GeneralNumber may format differently than the input string
        // Just verify the formatter matches operator<<
    }

    TEST(GeneralNumberFormatTest, ScientificNotation) {
        GeneralNumber num = GeneralNumber::parse("1.23e10");
        verify_format_matches_ostream(num);
    }

    // Tests for jsrl::JsonPrettyPrint formatting
    TEST(JsonPrettyPrintFormatTest, SimpleObject) {
        Json json(Json::ObjectBody{
            {"name", Json("John")},
            {"age", Json(30)}
        });
        auto pretty = pretty_print(json);
        verify_format_matches_ostream(pretty);
    }

    TEST(JsonPrettyPrintFormatTest, NestedStructure) {
        Json json(Json::ObjectBody{
            {"user", Json(Json::ObjectBody{
                {"name", Json("Alice")},
                {"id", Json(123)}
            })},
            {"items", Json(Json::ArrayBody{Json(1), Json(2), Json(3)})}
        });
        auto pretty = pretty_print(json);
        verify_format_matches_ostream(pretty);

        // Verify it contains newlines (pretty-printed)
        string formatted = std::format("{}", pretty);
        EXPECT_NE(formatted.find('\n'), string::npos);
    }

    TEST(JsonPrettyPrintFormatTest, OneLine) {
        Json json(Json::ObjectBody{
            {"a", Json(1)},
            {"b", Json(2)}
        });
        auto pretty = pretty_print(json).one_line();
        verify_format_matches_ostream(pretty);

        // Verify it doesn't contain newlines
        string formatted = std::format("{}", pretty);
        EXPECT_EQ(formatted.find('\n'), string::npos);
    }

    TEST(JsonPrettyPrintFormatTest, CustomIndent) {
        Json json(Json::ObjectBody{
            {"items", Json(Json::ArrayBody{Json(1), Json(2)})}
        });
        auto pretty = pretty_print(json).indent("    ");
        verify_format_matches_ostream(pretty);
    }

    TEST(JsonPrettyPrintFormatTest, KeyOrdering) {
        // Keys inserted in non-alphabetical order; jsrl sorts them lexicographically
        Json json(Json::ObjectBody{
            {"zebra", Json(1)},
            {"apple", Json(2)},
            {"mango", Json(3)}
        });
        auto pretty = pretty_print(json);
        verify_format_matches_ostream(pretty);

        // Keys must appear in alphabetical order: apple, mango, zebra
        string formatted = std::format("{}", pretty);
        auto apple_pos = formatted.find("\"apple\"");
        auto mango_pos = formatted.find("\"mango\"");
        auto zebra_pos = formatted.find("\"zebra\"");
        EXPECT_LT(apple_pos, mango_pos);
        EXPECT_LT(mango_pos, zebra_pos);
    }

    // Tests for jsrl::Json::Error formatting
    TEST(JsonErrorFormatTest, BasicError) {
        try {
            Json json;
            // Trigger a type error by trying to access it as an array
            json.as_array();
            FAIL() << "Expected TypeError to be thrown";
        } catch (Json::TypeError const& e) {
            // Cast to base class for formatting since only Json::Error has a formatter
            Json::Error const& base_error = e;
            verify_format_matches_ostream(base_error);

            // Verify the formatted output contains expected content
            string formatted = std::format("{}", base_error);
            EXPECT_NE(formatted.find("Type Error"), string::npos);
        }
    }

    TEST(JsonErrorFormatTest, KeyError) {
        try {
            Json json(Json::ObjectBody{{"key1", Json(42)}});
            // Trigger an ObjectKeyError
            json["nonexistent"];
            FAIL() << "Expected ObjectKeyError to be thrown";
        } catch (Json::ObjectKeyError const& e) {
            // Cast to base class for formatting
            Json::Error const& base_error = e;
            verify_format_matches_ostream(base_error);

            string formatted = std::format("{}", base_error);
            EXPECT_NE(formatted.find("Key Error"), string::npos);
        }
    }

    TEST(JsonErrorFormatTest, ArrayKeyError) {
        try {
            Json json(Json::ArrayBody{Json(1), Json(2)});
            // Trigger an ArrayKeyError
            json[10];
            FAIL() << "Expected ArrayKeyError to be thrown";
        } catch (Json::ArrayKeyError const& e) {
            // Cast to base class for formatting
            Json::Error const& base_error = e;
            verify_format_matches_ostream(base_error);

            string formatted = std::format("{}", base_error);
            EXPECT_NE(formatted.find("Key Error"), string::npos);
        }
    }

    TEST(JsonErrorFormatTest, ParseError) {
        try {
            Json::parse("{invalid json");
            FAIL() << "Expected ParseError to be thrown";
        } catch (Json::ParseError const& e) {
            // Cast to base class for formatting
            Json::Error const& base_error = e;
            verify_format_matches_ostream(base_error);

            string formatted = std::format("{}", base_error);
            EXPECT_NE(formatted.find("Parsing Error"), string::npos);
        }
    }

    // Integration tests - verify formatters can be used in practical scenarios
    TEST(JsonFormatIntegrationTest, ErrorMessages) {
        Json num(42);
        string msg = std::format("Invalid value: {}", num);
        EXPECT_EQ(msg, "Invalid value: 42");
    }

    TEST(JsonFormatIntegrationTest, Serialization) {
        Json json(Json::ObjectBody{
            {"status", Json("ok")},
            {"code", Json(200)}
        });
        string output = std::format("Response: {}", json);
        EXPECT_NE(output.find("\"status\""), string::npos);
        EXPECT_NE(output.find("\"ok\""), string::npos);
    }

    TEST(JsonFormatIntegrationTest, InlineConversion) {
        Json value("test");
        string result = std::format("{}", value);
        EXPECT_EQ(result, "\"test\"");
    }

    TEST(JsonFormatIntegrationTest, MultipleValues) {
        Json name("Alice");
        Json age(30);
        string msg = std::format("User: {}, Age: {}", name, age);
        EXPECT_EQ(msg, "User: \"Alice\", Age: 30");
    }

} // anonymous namespace
