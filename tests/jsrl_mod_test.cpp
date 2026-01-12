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

#include "../src/jsrl_mod.hpp"
#include <gtest/gtest.h>

namespace {
    using namespace jsrl::literals;
    using jsrl::Json;
}

TEST( JsrlMod,ModTopAssignWithNull ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = Json{};
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(null)JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithNumber ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = 123;
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(123)JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithString ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = "bar";
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON("bar")JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithBool ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = false;
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(false)JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithSelfAsArrayComponent ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = R"JSON(false)JSON"_Json;
    mod(jmod) = Json::ArrayBody{ jmod, jref };
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([false,"foo"])JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithSelfAsObjectComponent ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = R"JSON([false,"foo"])JSON"_Json;
    mod(jmod) = Json::ObjectBody{ { "old", jmod }, { "orig", jref } };
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({"old":[false,"foo"],"orig":"foo"})JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithSelfAsArrayComponentCompound ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = R"JSON({"old":[false,"foo"],"orig":"foo"})JSON"_Json;
    mod(jmod) = Json::ArrayBody{ jmod, jref };
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                {"old":[false,"foo"],"orig":"foo"},
                "foo"
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModTopAssignWithSelfAsObjectComponentCompound ) {
    auto const jref = Json( "foo" );
    auto jmod = jref;

    mod(jmod) = R"JSON([
                {"old":[false,"foo"],"orig":"foo"},
                "foo"
            ])JSON"_Json;
    mod(jmod) = Json::ObjectBody{ { "old", jmod }, { "orig", jref } };
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({
                "old": [
                    {"old":[false,"foo"],"orig":"foo"},
                    "foo"
                ],
                "orig": "foo"
            })JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayReplaceWithObject ) {
    auto const jref = R"JSON([ "abc", 123, null, true ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1] = Json::ObjectBody{};
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([ "abc", {}, null, true ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayReplaceWithString ) {
    auto const jref = R"JSON([ "abc", {}, null, true ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[0] = "ABCDE";
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([ "ABCDE", {}, null, true ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayReplaceExtendWithBool ) {
    auto const jref = R"JSON([ "ABCDE", {}, null, true ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[6] = true;
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(["ABCDE",{},null,true,null,null,true])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayEraseOne ) {
    auto const jref = R"JSON(["ABCDE",{},null,true,null,null,true])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[3].erase();
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(["ABCDE",{},null,null,null,true])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayInsertOne ) {
    auto const jref = R"JSON(["ABCDE",{},null,null,null,true])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[3].insert_at( "bar" );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(["ABCDE",{},null,"bar",null,null,true])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayAppendOne ) {
    auto const jref = R"JSON(["ABCDE",{},null,"bar",null,null,true])JSON"_Json;
    auto jmod = jref;

    mod(jmod).push_back( Json::ArrayBody{} );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ(R"JSON(["ABCDE",{},null,"bar",null,null,true,[]])JSON"_Json,jmod);
}

TEST( JsrlMod,ModArrayArrayEraseSet ) {
    auto const jref=R"JSON(["ABCDE",{},null,"bar",null,null,true,[]])JSON"_Json;
    auto jmod = jref;

    mod(jmod).erase_indexes( { 1, 3, 5, 7, 9 } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(["ABCDE",null,null,true])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayInsertMulti ) {
    auto const jref = R"JSON(["ABCDE",null,null,true])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[2].insert_all_at( { Json(true), Json(0), Json(1), Json(false) } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ(R"JSON(["ABCDE",null,true,0,1,false,null,true])JSON"_Json,jmod);
}

TEST( JsrlMod,ModArrayArrayEraseRange ) {
    auto const jref = R"JSON(["ABCDE",null,true,0,1,false,null,true])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1].erase_count( 3 );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(["ABCDE",1,false,null,true])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayEraseRangeOpen ) {
    auto const jref = R"JSON(["ABCDE",1,false,null,true])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[2].erase_count();
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON(["ABCDE",1])JSON"_Json, jmod );
}

TEST( JsrlMod,ModArrayArrayErasePred ) {
    auto const jref = R"JSON(["ABCDE",1])JSON"_Json;
    auto jmod = jref;

    mod(jmod).erase_indexes_if( []( auto &&i, auto &&v ) {
        return i == 0 && v.is_string();
    } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([1])JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectReplaceWithObject ) {
    auto const jref = R"JSON({
        "0":"abc",
        "one":123,
        "2":null,
        "three":true
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod)["one"] = Json::ObjectBody{};
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({
                "0":"abc",
                "one":{},
                "2":null,
                "three":true
            })JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectReplaceWithString ) {
    auto const jref = R"JSON({
        "0":"abc",
        "one":{},
        "2":null,
        "three":true
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod)["0"] = "ABCDE";
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({
                "0":"ABCDE",
                "one":{},
                "2":null,
                "three":true
            })JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectReplaceInsertWithBool ) {
    auto const jref = R"JSON({
        "0":"ABCDE",
        "one":{},
        "2":null,
        "three":true
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod)["6"] = true;
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({
                "0":"ABCDE",
                "one":{},
                "2":null,
                "three":true,
                "6":true
            })JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectEraseOne ) {
    auto const jref = R"JSON({
        "0":"ABCDE",
        "one":{},
        "2":null,
        "three":true,
        "6":true
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod)["three"].erase();
    EXPECT_NE( jref, jmod );
    EXPECT_EQ(R"JSON({"0":"ABCDE","one":{},"2":null,"6":true})JSON"_Json,jmod);
}

TEST( JsrlMod,ModObjectObjectInsertReviveKeyWithString ) {
    auto const jref=R"JSON({"0":"ABCDE","one":{},"2":null,"6":true})JSON"_Json;
    auto jmod = jref;

    mod(jmod)["three"] = "bar";
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({
                "0":"ABCDE",
                "one":{},
                "2":null,
                "three":"bar",
                "6":true
            })JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectInsertMulti ) {
    auto const jref = R"JSON({
        "0":"ABCDE",
        "one":{},
        "2":null,
        "three":"bar",
        "6":true
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod).assign_keys( { { "five", Json::ArrayBody{} },
            { "seven", Json::ObjectBody{} } } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ(
            R"JSON({
        "0":"ABCDE",
        "one":{},
        "2":null,
        "three":"bar",
        "five":[],
        "6":true,
        "seven":{}
    })JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectErasePred ) {
    auto const jref = R"JSON({
        "0":"ABCDE",
        "one":{},
        "2":null,
        "three":"bar",
        "five":[],
        "6":true,
        "seven":{}
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod).erase_keys_if( []( auto &&key, auto &&value ) {
        return key.size() == 3
                || ( value.is_string()
                    && value.as_string().size() == 3 )
                ;
    } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ(
            R"JSON({
        "0":"ABCDE",
        "2":null,
        "five":[],
        "6":true,
        "seven":{}
    })JSON"_Json, jmod );
}

TEST( JsrlMod,ModObjectObjectEraseSet ) {
    auto const jref = R"JSON({
        "0":"ABCDE",
        "2":null,
        "five":[],
        "6":true,
        "seven":{}
    })JSON"_Json;
    auto jmod = jref;

    mod(jmod).erase_keys( { "one", "three", "five", "seven", "nine" } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON({"0":"ABCDE","2":null,"6":true})JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayAppendOne ) {
    auto const jref = R"JSON([
        false,
        [ { } ],
        {
            "a": [ 0, 1, 2, 3 ],
            "o": { "k": "v" }
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1].push_back("foo");
    mod(jmod)[2]["a"].push_back("foo");
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { },
                    "foo"
                ],
                {
                    "a": [ 0, 1, 2, 3, "foo" ],
                    "o": { "k":"v" }
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayReplace ) {
    auto const jref = R"JSON([
        false,
        [
            { },
            "foo"
        ],
        {
            "a": [ 0, 1, 2, 3, "foo" ],
            "o": { "k":"v" }
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][1] = "bar";
    mod(jmod)[2]["a"][1] = "baz";
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { },
                    "bar"
                ],
                {
                    "a": [ 0, "baz", 2, 3, "foo" ],
                    "o": { "k":"v" }
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubObjectReplaceInsert ) {
    auto const jref = R"JSON([
        false,
        [
            { },
            "bar"
        ],
        {
            "a": [ 0, "baz", 2, 3, "foo" ],
            "o": { "k":"v" }
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0]["one"] = "qux";
    mod(jmod)[2]["two"] = "ABCDE";
    mod(jmod)[2]["o"]["three"] = 1234;
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { "one": "qux" },
                    "bar"
                ],
                {
                    "a": [ 0, "baz", 2, 3, "foo" ],
                    "o": { "k":"v", "three": 1234 },
                    "two": "ABCDE"
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubObjectReplace ) {
    auto const jref = R"JSON([
        false,
        [
            { "one": "qux" },
            "bar"
        ],
        {
            "a": [ 0, "baz", 2, 3, "foo" ],
            "o": { "k":"v", "three": 1234 },
            "two": "ABCDE"
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0]["one"] = 12345;
    mod(jmod)[2]["two"] = 6789;
    mod(jmod)[2]["o"]["k"] = "value";
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { "one": 12345 },
                    "bar"
                ],
                {
                    "a": [ 0, "baz", 2, 3, "foo" ],
                    "o": { "k":"value", "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayReplaceExtend ) {
    auto const jref = R"JSON([
        false,
        [
            { "one": 12345 },
            "bar"
        ],
        {
            "a": [ 0, "baz", 2, 3, "foo" ],
            "o": { "k":"value", "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][3] = true;
    mod(jmod)[2]["a"][8] = Json{};
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { "one": 12345 },
                    "bar",
                    null,
                    true
                ],
                {
                    "a": [ 0, "baz", 2, 3, "foo", null, null, null, null ],
                    "o": { "k":"value", "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayEraseOne ) {
    auto const jref = R"JSON([
        false,
        [
            { "one": 12345 },
            "bar",
            null,
            true
        ],
        {
            "a": [ 0, "baz", 2, 3, "foo", null, null, null, null ],
            "o": { "k":"value", "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][2].erase();
    mod(jmod)[2]["a"][3].erase();
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { "one": 12345 },
                    "bar",
                    true
                ],
                {
                    "a": [ 0, "baz", 2, "foo", null, null, null, null ],
                    "o": { "k":"value", "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubObjectEraseOne ) {
    auto const jref = R"JSON([
        false,
        [
            { "one": 12345 },
            "bar",
            true
        ],
        {
            "a": [ 0, "baz", 2, "foo", null, null, null, null ],
            "o": { "k":"value", "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0]["one"].erase();
    mod(jmod)[2]["o"]["k"].erase();
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    { },
                    "bar",
                    true
                ],
                {
                    "a": [ 0, "baz", 2, "foo", null, null, null, null ],
                    "o": { "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayInsertOne ) {
    auto const jref = R"JSON([
        false,
        [
            { },
            "bar",
            true
        ],
        {
            "a": [ 0, "baz", 2, "foo", null, null, null, null ],
            "o": { "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0].insert_at( Json::ArrayBody{ "alpha", "beta", "gamma" } );
    mod(jmod)[2]["a"][3].insert_at( 543 );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ "alpha", "beta", "gamma" ],
                    { },
                    "bar",
                    true
                ],
                {
                    "a": [ 0, "baz", 2, 543, "foo", null, null, null, null ],
                    "o": { "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayEraseRange ) {
    auto const jref = R"JSON([
        false,
        [
            [ "alpha", "beta", "gamma" ],
            { },
            "bar",
            true
        ],
        {
            "a": [ 0, "baz", 2, 543, "foo", null, null, null, null ],
            "o": { "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0][0].erase_count( 2 );
    mod(jmod)[2]["a"][1].erase_count( 2 );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ "gamma" ],
                    { },
                    "bar",
                    true
                ],
                {
                    "a": [ 0, 543, "foo", null, null, null, null ],
                    "o": { "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayEraseRangeOpen ) {
    auto const jref = R"JSON([
        false,
        [
            [ "gamma" ],
            { },
            "bar",
            null,
            true
        ],
        {
            "a": [ 0, 543, 3, "foo", null, null, null, null ],
            "o": { "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0][1].erase_count();
    mod(jmod)[1][4].erase_count();
    mod(jmod)[2]["a"][3].erase_count();
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ "gamma" ],
                    { },
                    "bar",
                    null
                ],
                {
                    "a": [ 0, 543, 3 ],
                    "o": { "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayInsertMulti ) {
    auto const jref = R"JSON([
        false,
        [
            [ "gamma" ],
            { },
            "bar",
            null
        ],
        {
            "a": [ 0, 543, 3 ],
            "o": { "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0][0].insert_all_at( { true, Json{}, 987 } );
    mod(jmod)[1][2].insert_all_at( { 654, false } );
    mod(jmod)[2]["a"][2].insert_all_at( { 12, 34, 56 } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ true, null, 987, "gamma" ],
                    { },
                    654,
                    false,
                    "bar",
                    null
                ],
                {
                    "a": [ 0, 543, 12, 34, 56, 3 ],
                    "o": { "three": 1234 },
                    "two": 6789
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubObjectInsertMulti ) {
    auto const jref = R"JSON([
        false,
        [
            [ true, null, 987, "gamma" ],
            { },
            654,
            false,
            "bar",
            null
        ],
        {
            "a": [ 0, 543, 12, 34, 56, 3 ],
            "o": { "three": 1234 },
            "two": 6789
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][1].assign_keys( {
            { "alpha", "a" },
            { "beta", "b" },
            { "gamma", "g" }, } );
    mod(jmod)[2].assign_keys( {
            { "a2", Json::ArrayBody{} },
            { "o2", Json::ObjectBody{} }, } );
    mod(jmod)[2]["o"].assign_keys( {
            { "nine", 9 },
            { "ten", 10 }, } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ true, null, 987, "gamma" ],
                    { "alpha": "a", "beta": "b", "gamma": "g" },
                    654,
                    false,
                    "bar",
                    null
                ],
                {
                    "a": [ 0, 543, 12, 34, 56, 3 ],
                    "o": { "three": 1234, "nine": 9, "ten": 10 },
                    "two": 6789,
                    "a2": [ ],
                    "o2": { }
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayEraseSet ) {
    auto const jref = R"JSON([
        false,
        [
            [ true, null, 987, "gamma" ],
            { "alpha": "a", "beta": "b", "gamma": "g" },
            654,
            false,
            "bar",
            null
        ],
        {
            "a": [ 0, 543, 12, 34, 56, 3 ],
            "o": { "three": 1234, "nine": 9, "ten": 10 },
            "two": 6789,
            "a2": [ ],
            "o2": { }
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0].erase_indexes( { 1, 3 } );
    mod(jmod)[1].erase_indexes( { 2, 5, 3 } );
    mod(jmod)[2]["a"].erase_indexes( { 0, 2, 4 } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ true, 987 ],
                    { "alpha": "a", "beta": "b", "gamma": "g" },
                    "bar"
                ],
                {
                    "a": [ 543, 34, 3 ],
                    "o": { "three": 1234, "nine": 9, "ten": 10 },
                    "two": 6789,
                    "a2": [ ],
                    "o2": { }
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubObjectEraseSet ) {
    auto const jref = R"JSON([
        false,
        [
            [ true, 987 ],
            { "alpha": "a", "beta": "b", "gamma": "g" },
            "bar"
        ],
        {
            "a": [ 543, 34, 3 ],
            "o": { "three": 1234, "nine": 9, "ten": 10 },
            "two": 6789,
            "a2": [ ],
            "o2": { }
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][1].erase_keys( { "gamma", "beta" } );
    mod(jmod)[2].erase_keys( { "o2", "two" } );
    mod(jmod)[2]["o"].erase_keys( { "ten", "three" } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ true, 987 ],
                    { "alpha": "a" },
                    "bar"
                ],
                {
                    "a": [ 543, 34, 3 ],
                    "o": { "nine": 9 },
                    "a2": [ ]
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubArrayErasePred ) {
    auto const jref = R"JSON([
        false,
        [
            [ true, 987 ],
            { "alpha": "a" },
            "bar"
        ],
        {
            "a": [ 543, 34, 3 ],
            "o": { "nine": 9 },
            "a2": [ ]
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[1][0].erase_indexes_if( []( auto &&i, auto &&v ) {
        return i == 0 && v.is_bool();
    } );
    mod(jmod)[2]["a"].erase_indexes_if( []( auto &&i, auto &&v ) {
        return i == 2
                || ( v.is_number_uint()
                    && v.as_number_uint() == 543U );
    } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ 987 ],
                    { "alpha": "a" },
                    "bar"
                ],
                {
                    "a": [ 34 ],
                    "o": { "nine": 9 },
                    "a2": [ ]
                },
                true
            ])JSON"_Json, jmod );
}

TEST( JsrlMod,ModSubObjectErasePred ) {
    auto const jref = R"JSON([
        false,
        [
            [ 987 ],
            { "alpha": "a" },
            "bar"
        ],
        {
            "a": [ 34 ],
            "o": { "nine": 9 },
            "a2": [ ]
        },
        true
    ])JSON"_Json;
    auto jmod = jref;

    mod(jmod)[2].erase_keys_if( []( auto &&k, auto &&v ) {
        return k.size() == 2 || v.is_object();
    } );
    EXPECT_NE( jref, jmod );
    EXPECT_EQ( R"JSON([
                false,
                [
                    [ 987 ],
                    { "alpha": "a" },
                    "bar"
                ],
                { "a": [ 34 ] },
                true
            ])JSON"_Json, jmod );
}

// vi:et ts=4 sts=4 sw=4
