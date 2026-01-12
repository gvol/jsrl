/**
 * Copyright 2025 Adobe. All rights reserved.
 * This file is licensed to you under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
 * OF ANY KIND, either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */

#include "jsrl_test_helpers.hpp"
#include "../src/jsrlpp.hpp"
#include "../src/jsrl.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace jsrl::literals;
using jsrl::Json;
using std::ostringstream;
using std::vector;
using std::string;

namespace {
    auto stock_json_object() -> Json const & {
        static auto const json = R"JSON(
                    {
                        "null":   null,
                        "false":  false,
                        "true":   true,
                        "number": -1234,
                        "string": "Hello\n\tworld",
                        "array":  [ "", [], {} ],
                        "object": {
                            "array": [],
                            "object": {}
                        }
                    }
                )JSON"_Json;
        return json;
    }
}

TEST(Jsrl,PrettyPrint_Default) {
    Json const json = stock_json_object();
    string result=(ostringstream()<<pretty_print(json)).str();
    string const expectation =
            "{\n"
            "  \"array\": [\n"
            "    \"\",\n"
            "    [ ],\n"
            "    { }\n"
            "  ],\n"
            "  \"false\": false,\n"
            "  \"null\": null,\n"
            "  \"number\": -1234,\n"
            "  \"object\": {\n"
            "    \"array\": [ ],\n"
            "    \"object\": { }\n"
            "  },\n"
            "  \"string\": \"Hello\\n\\tworld\",\n"
            "  \"true\": true\n"
            "}"
            ;
    EXPECT_EQ( expectation, result );
}
TEST(Jsrl,PrettypPrint_Base) {
    Json const json = stock_json_object();
    string result=(ostringstream()<<pretty_print(json).base("/**/")).str();
    string const expectation =
R"JSON({
/**/  "array": [
/**/    "",
/**/    [ ],
/**/    { }
/**/  ],
/**/  "false": false,
/**/  "null": null,
/**/  "number": -1234,
/**/  "object": {
/**/    "array": [ ],
/**/    "object": { }
/**/  },
/**/  "string": "Hello\n\tworld",
/**/  "true": true
/**/})JSON"
            ;
    EXPECT_EQ( expectation, result );
}
TEST(Jsrl,PrettypPrint_FirstKeys_IB) {
    Json const json = stock_json_object();
    ostringstream oss;
    oss << pretty_print(json)
            .indent("    ")
            .first_keys( {"null","object"} )
            .base("        ")
            ;
    string const expectation =
R"JSON({
            "null": null,
            "object": {
                "object": { },
                "array": [ ]
            },
            "array": [
                "",
                [ ],
                { }
            ],
            "false": false,
            "number": -1234,
            "string": "Hello\n\tworld",
            "true": true
        })JSON";
    EXPECT_EQ( expectation, oss.str() );
}
TEST(Jsrl,PrettypPrint_FirstKeys_I) {
    Json const json = stock_json_object();
    string result = (ostringstream()
            << pretty_print(json)
                .first_keys({"null","object"})
                .indent("    ")
            ).str();
    string const expectation =
R"JSON({
    "null": null,
    "object": {
        "object": { },
        "array": [ ]
    },
    "array": [
        "",
        [ ],
        { }
    ],
    "false": false,
    "number": -1234,
    "string": "Hello\n\tworld",
    "true": true
})JSON";
    EXPECT_EQ( expectation, result );
}
TEST(Jsrl,PrettypPrint_FirstKeys_I_Commute) {
    Json const json = stock_json_object();
    string result = (ostringstream()
            << pretty_print(json)
                .indent("    ")
                .first_keys({"null","object"})
            ).str();
    string const expectation =
R"JSON({
    "null": null,
    "object": {
        "object": { },
        "array": [ ]
    },
    "array": [
        "",
        [ ],
        { }
    ],
    "false": false,
    "number": -1234,
    "string": "Hello\n\tworld",
    "true": true
})JSON";
    EXPECT_EQ( expectation, result );
}

TEST(Jsrl,PrettyPrint_NumericKeys) {
    Json const json = R"JSON(
                {
                    "-3": -3,
                    "-2": -2,
                    "-20": -20,
                    "-100": -100,
                    "0": 0,
                    "1": 1,
                    "2.5": 2.5,
                    "5.25": 5.25,
                    "3": 3,
                    "500": 500,
                    "7": 7
                }
            )JSON"_Json;
    string result=(
                ostringstream() << pretty_print(json)
                                        .numeric_key_order()
                                        .indent("    ")
            ).str();
    string const expectation =
R"JSON({
    "-100": -100,
    "-20": -20,
    "-3": -3,
    "-2": -2,
    "0": 0,
    "1": 1,
    "2.5": 2.5,
    "3": 3,
    "5.25": 5.25,
    "7": 7,
    "500": 500
})JSON"
            ;
    EXPECT_EQ( expectation, result );
}

TEST(Jsrl,PrettyPrint_LooseFloats) {
    vector<long double> testcases{ 0.05L, 0.01L, -123.456L,
            1.05L, 2.05L, 4.05L, 8.05L, 16.05L, 32.05L,
            1.01L, 2.01L, 4.01L, 8.01L, 16.01L, 32.01L };
    string expectation = "[0.05,0.01,-123.456,"
            "1.05,2.05,4.05,8.05,16.05,32.05,"
            "1.01,2.01,4.01,8.01,16.01,32.01]";
    if ( test_precision_check<long double>() ) {
        vector<Json> v;
        for ( long double num : testcases )
            v.push_back( Json(num) );
        Json json(std::move(v));
        string result=(
                    ostringstream() << pretty_print(json)
                                        .one_line()
                                        .loose_long_doubles()
                ).str();
        EXPECT_EQ( expectation, result );
    }
    if ( test_precision_check<double>() ) {
        vector<Json> v;
        for ( double num : testcases )
            v.push_back( Json(num) );
        Json json(std::move(v));
        string result=(
                    ostringstream() << pretty_print(json)
                                        .one_line()
                                        .loose_doubles()
                ).str();
        EXPECT_EQ( expectation, result );
    }
    if ( test_precision_check<float>() ) {
        vector<Json> v;
        for ( float num : testcases )
            v.push_back( Json(num) );
        Json json(std::move(v));
        string result=(
                    ostringstream() << pretty_print(json)
                                        .one_line()
                                        .loose_floats()
                ).str();
        EXPECT_EQ( expectation, result );
    }
}

TEST(Jsrl,PrettyPrint_OneLine) {
    Json const json = stock_json_object();
    string result = (
                ostringstream() << pretty_print(json).one_line()
            ).str();
    string const expectation =
            R"JSON({)JSON"
            R"JSON("array":["",[],{}],)JSON"
            R"JSON("false":false,)JSON"
            R"JSON("null":null,)JSON"
            R"JSON("number":-1234,)JSON"
            R"JSON("object":{"array":[],"object":{}},)JSON"
            R"JSON("string":"Hello\n\tworld",)JSON"
            R"JSON("true":true)JSON"
            R"JSON(})JSON"
            ;
    EXPECT_EQ( expectation, result );
}

TEST(Jsrl,PrettyPrint_OneLine_Comma) {
    Json const json = stock_json_object();
    string result = (
                ostringstream() << pretty_print(json)
                                    .one_line()
                                    .set_comma_spacing(" ","  ")
            ).str();
    string const expectation =
            R"JSON({)JSON"
            R"JSON("array":["" ,  [] ,  {}] ,  )JSON"
            R"JSON("false":false ,  )JSON"
            R"JSON("null":null ,  )JSON"
            R"JSON("number":-1234 ,  )JSON"
            R"JSON("object":{"array":[] ,  "object":{}} ,  )JSON"
            R"JSON("string":"Hello\n\tworld" ,  )JSON"
            R"JSON("true":true)JSON"
            R"JSON(})JSON"
            ;
    EXPECT_EQ( expectation, result );
}

TEST(Jsrl,PrettyPrint_OneLine_Colon) {
    Json const json = stock_json_object();
    string result = (
                ostringstream() << pretty_print(json)
                                    .one_line()
                                    .set_colon_spacing(" ","  ")
            ).str();
    string const expectation =
            R"JSON({)JSON"
            R"JSON("array" :  ["",[],{}],)JSON"
            R"JSON("false" :  false,)JSON"
            R"JSON("null" :  null,)JSON"
            R"JSON("number" :  -1234,)JSON"
            R"JSON("object" :  {"array" :  [],"object" :  {}},)JSON"
            R"JSON("string" :  "Hello\n\tworld",)JSON"
            R"JSON("true" :  true)JSON"
            R"JSON(})JSON"
            ;
    EXPECT_EQ( expectation, result );
}

TEST(Jsrl,PrettyPrint_OneLine_Thorough) {
    Json const json = stock_json_object();
    string result = (
                ostringstream() << pretty_print(json)
                                    .first_keys({
                                        "null", "false", "true",
                                        "number", "string",
                                        "array", "object" })
                                    .one_line()
                                    .set_comma_spacing(" ")
                                    .set_colon_spacing("  ")
            ).str();
    string const expectation =
            R"JSON({)JSON"
            R"JSON("null":  null, )JSON"
            R"JSON("false":  false, )JSON"
            R"JSON("true":  true, )JSON"
            R"JSON("number":  -1234, )JSON"
            R"JSON("string":  "Hello\n\tworld", )JSON"
            R"JSON("array":  ["", [], {}], )JSON"
            R"JSON("object":  {"array":  [], "object":  {}})JSON"
            R"JSON(})JSON"
            ;
    EXPECT_EQ( expectation, result );
}
// vi: et ts=4 sts=4 sw=4
