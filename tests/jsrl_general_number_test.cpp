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

#include "../src/jsrl_general_number.hpp"
#include <gtest/gtest.h>
#include <tuple>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <climits>

namespace {
    using jsrl::GeneralNumber;
    using std::tuple;
    using std::get;
    using std::begin;
    using std::end;
    using std::istringstream;
    using std::ostringstream;
    using std::vector;
    using std::string;
    using std::pair;

    template<typename T>
    auto makeString( T const &t ) -> string
    {
        auto oss = ostringstream();
        oss << t;
        return oss.str();
    }

    auto makeGN( string const &s ) -> GeneralNumber
    {
        auto iss = istringstream(s);
        GeneralNumber gn;
        iss >> gn;
        return gn;
    }

    auto stringMagIncr( string num ) -> string
    {
        auto stopDigit = begin(num);
        if ( *stopDigit == '-' )
            ++stopDigit;
        auto curDigit = end(num);
        --curDigit;
        for (;;) {
            if ( *curDigit == '9' ) {
                *curDigit = '0';
                if ( curDigit == stopDigit ) {
                    num.insert( curDigit, 1, '1' );
                    break;
                }
                --curDigit;
            } else {
                ++*curDigit;
                break;
            }
        }
        return num;
    }

    auto is_int( GeneralNumber const &gn ) -> bool
    {
        return gn.is_long_long();
    }

    auto is_uint( GeneralNumber const &gn ) -> bool
    {
        return gn.is_long_long_unsigned();
    }

    auto isnt_int( GeneralNumber const &gn ) -> bool
    {
        return ! is_int( gn );
    }

    auto isnt_uint( GeneralNumber const &gn ) -> bool
    {
        return ! is_uint( gn );
    }

}

TEST(JsrlGeneralNumber,IntConvertBounds) {
    auto const gnMaxInt = makeGN( makeString(LLONG_MAX) );
    EXPECT_PRED1( is_int, gnMaxInt );
    EXPECT_PRED1( is_uint, gnMaxInt );
    auto const gnMinInt = makeGN( makeString(LLONG_MIN) );
    EXPECT_PRED1( is_int, gnMinInt );
    EXPECT_PRED1( isnt_uint, gnMinInt );
    auto const gnMaxUInt = makeGN( makeString(ULLONG_MAX) );
    EXPECT_PRED1( isnt_int, gnMaxUInt );
    EXPECT_PRED1( is_uint, gnMaxUInt );
    auto const gnZero = makeGN( "0" );
    EXPECT_PRED1( is_int, gnZero );
    EXPECT_PRED1( is_uint, gnZero );
    auto const gnMaxIntP1 = makeGN( stringMagIncr( makeString(LLONG_MAX) ) );
    EXPECT_PRED1( isnt_int, gnMaxIntP1 );
    EXPECT_PRED1( is_uint, gnMaxIntP1 );
    auto const gnMinIntP1 = makeGN( stringMagIncr( makeString(LLONG_MIN) ) );
    EXPECT_PRED1( isnt_int, gnMinIntP1 );
    EXPECT_PRED1( isnt_uint, gnMinIntP1 );
    auto const gnMaxUIntP1 = makeGN( stringMagIncr( makeString(ULLONG_MAX) ) );
    EXPECT_PRED1( isnt_int, gnMaxUIntP1 );
    EXPECT_PRED1( isnt_uint, gnMaxUIntP1 );

    auto const gnFPZero1 = makeGN( "0.0" );
    EXPECT_PRED1( isnt_int, gnFPZero1 );
    EXPECT_PRED1( isnt_uint, gnFPZero1 );
    auto const gnFPZero2 = makeGN( "0e0" );
    EXPECT_PRED1( isnt_int, gnFPZero2 );
    EXPECT_PRED1( isnt_uint, gnFPZero2 );
    auto const gnFPOne1 = makeGN( "1.0" );
    EXPECT_PRED1( isnt_int, gnFPOne1 );
    EXPECT_PRED1( isnt_uint, gnFPOne1 );
    auto const gnFPOne2 = makeGN( "1e0" );
    EXPECT_PRED1( isnt_int, gnFPOne2 );
    EXPECT_PRED1( isnt_uint, gnFPOne2 );
    auto const gnFPNeg1= makeGN( "-9.9" );
    EXPECT_PRED1( isnt_int, gnFPNeg1 );
    EXPECT_PRED1( isnt_uint, gnFPNeg1 );
    auto const gnFPNeg2= makeGN( "-123.45" );
    EXPECT_PRED1( isnt_int, gnFPNeg2 );
    EXPECT_PRED1( isnt_uint, gnFPNeg2 );
    auto const gnFPNeg3= makeGN( "-123.45e-1" );
    EXPECT_PRED1( isnt_int, gnFPNeg3 );
    EXPECT_PRED1( isnt_uint, gnFPNeg3 );
    auto const gnFPNeg4= makeGN( "-1234.5e2" );
    EXPECT_PRED1( isnt_int, gnFPNeg4 );
    EXPECT_PRED1( isnt_uint, gnFPNeg4 );
    auto const gnFPNeg5= makeGN( "-12.345e-4" );
    EXPECT_PRED1( isnt_int, gnFPNeg5 );
    EXPECT_PRED1( isnt_uint, gnFPNeg5 );
    auto const gnFPPos1 = makeGN( "9.9" );
    EXPECT_PRED1( isnt_int, gnFPPos1 );
    EXPECT_PRED1( isnt_uint, gnFPPos1 );
    auto const gnFPPos2 = makeGN( "123.45" );
    EXPECT_PRED1( isnt_int, gnFPPos2 );
    EXPECT_PRED1( isnt_uint, gnFPPos2 );
    auto const gnFPPos3 = makeGN( "123.45e-1" );
    EXPECT_PRED1( isnt_int, gnFPPos3 );
    EXPECT_PRED1( isnt_uint, gnFPPos3 );
    auto const gnFPPos4 = makeGN( "1234.5e2" );
    EXPECT_PRED1( isnt_int, gnFPPos4 );
    EXPECT_PRED1( isnt_uint, gnFPPos4 );
    auto const gnFPPos5 = makeGN( "12.345e-4" );
    EXPECT_PRED1( isnt_int, gnFPPos5 );
    EXPECT_PRED1( isnt_uint, gnFPPos5 );
}

TEST(JsrlGeneralNumber,IntConvertValue) {
    auto const gnMaxInt = makeGN( makeString(LLONG_MAX) );
    EXPECT_EQ( LLONG_MAX, gnMaxInt.as_long_long() );
    EXPECT_EQ( LLONG_MAX, gnMaxInt.as_long_long_unsigned() );
    auto const gnMinInt = makeGN( makeString(LLONG_MIN) );
    EXPECT_EQ( LLONG_MIN, gnMinInt.as_long_long() );
    EXPECT_EQ( 0, gnMinInt.as_long_long_unsigned() );
    auto const gnMaxUInt = makeGN( makeString(ULLONG_MAX) );
    EXPECT_EQ( LLONG_MAX, gnMaxUInt.as_long_long() );
    EXPECT_EQ( ULLONG_MAX, gnMaxUInt.as_long_long_unsigned() );
    auto const gnZero = makeGN( "0" );
    EXPECT_EQ( 0, gnZero.as_long_long() );
    EXPECT_EQ( 0, gnZero.as_long_long_unsigned() );
    auto const gnMaxIntP1 = makeGN( stringMagIncr( makeString(LLONG_MAX) ) );
    EXPECT_EQ( LLONG_MAX, gnMaxIntP1.as_long_long() );
    EXPECT_EQ( static_cast<long long unsigned>(LLONG_MAX)+1,
            gnMaxIntP1.as_long_long_unsigned() );
    auto const gnMinIntP1 = makeGN( stringMagIncr( makeString(LLONG_MIN) ) );
    EXPECT_EQ( LLONG_MIN, gnMinIntP1.as_long_long() );
    EXPECT_EQ( 0, gnMinIntP1.as_long_long_unsigned() );
    auto const gnMaxUIntP1 = makeGN( stringMagIncr( makeString(ULLONG_MAX) ) );
    EXPECT_EQ( LLONG_MAX, gnMaxUIntP1.as_long_long() );
    EXPECT_EQ( ULLONG_MAX, gnMaxUIntP1.as_long_long_unsigned() );

    auto const gnFPZero1 = makeGN( "0.0" );
    EXPECT_EQ( 0, gnFPZero1.as_long_long() );
    EXPECT_EQ( 0, gnFPZero1.as_long_long_unsigned() );
    auto const gnFPZero2 = makeGN( "0e0" );
    EXPECT_EQ( 0, gnFPZero2.as_long_long() );
    EXPECT_EQ( 0, gnFPZero2.as_long_long_unsigned() );
    auto const gnFPOne1 = makeGN( "1.0" );
    EXPECT_EQ( 1, gnFPOne1.as_long_long() );
    EXPECT_EQ( 1, gnFPOne1.as_long_long_unsigned() );
    auto const gnFPOne2 = makeGN( "1e0" );
    EXPECT_EQ( 1, gnFPOne2.as_long_long() );
    EXPECT_EQ( 1, gnFPOne2.as_long_long_unsigned() );
    auto const gnFPNeg1= makeGN( "-9.9" );
    EXPECT_EQ( -9, gnFPNeg1.as_long_long() );
    EXPECT_EQ( 0, gnFPNeg1.as_long_long_unsigned() );
    auto const gnFPNeg2= makeGN( "-123.45" );
    EXPECT_EQ( -123, gnFPNeg2.as_long_long() );
    EXPECT_EQ( 0, gnFPNeg2.as_long_long_unsigned() );
    auto const gnFPNeg3= makeGN( "-123.45e-1" );
    EXPECT_EQ( -12, gnFPNeg3.as_long_long() );
    EXPECT_EQ( 0, gnFPNeg3.as_long_long_unsigned() );
    auto const gnFPNeg4= makeGN( "-1234.5e2" );
    EXPECT_EQ( -123450, gnFPNeg4.as_long_long() );
    EXPECT_EQ( 0, gnFPNeg4.as_long_long_unsigned() );
    auto const gnFPNeg5= makeGN( "-12.345e-4" );
    EXPECT_EQ( 0, gnFPNeg5.as_long_long() );
    EXPECT_EQ( 0, gnFPNeg5.as_long_long_unsigned() );
    auto const gnFPPos1 = makeGN( "9.9" );
    EXPECT_EQ( 9, gnFPPos1.as_long_long() );
    EXPECT_EQ( 9, gnFPPos1.as_long_long_unsigned() );
    auto const gnFPPos2 = makeGN( "123.45" );
    EXPECT_EQ( 123, gnFPPos2.as_long_long() );
    EXPECT_EQ( 123, gnFPPos2.as_long_long_unsigned() );
    auto const gnFPPos3 = makeGN( "123.45e-1" );
    EXPECT_EQ( 12, gnFPPos3.as_long_long() );
    EXPECT_EQ( 12, gnFPPos3.as_long_long_unsigned() );
    auto const gnFPPos4 = makeGN( "1234.5e2" );
    EXPECT_EQ( 123450, gnFPPos4.as_long_long() );
    EXPECT_EQ( 123450, gnFPPos4.as_long_long_unsigned() );
    auto const gnFPPos5 = makeGN( "12.345e-4" );
    EXPECT_EQ( 0, gnFPPos5.as_long_long() );
    EXPECT_EQ( 0, gnFPPos5.as_long_long_unsigned() );
}

TEST(JsrlGeneralNumber,FloatConvertValue) {
    auto const gnMaxInt = makeGN( makeString(LLONG_MAX) );
    EXPECT_DOUBLE_EQ( static_cast<long double>(LLONG_MAX), gnMaxInt.as_long_double() );
    auto const gnMinInt = makeGN( makeString(LLONG_MIN) );
    EXPECT_DOUBLE_EQ( LLONG_MIN, gnMinInt.as_long_double() );
    auto const gnMaxUInt = makeGN( makeString(ULLONG_MAX) );
    EXPECT_DOUBLE_EQ( static_cast<long double>(ULLONG_MAX), gnMaxUInt.as_long_double() );
    auto const gnZero = makeGN( "0" );
    EXPECT_DOUBLE_EQ( 0, gnZero.as_long_double() );
    auto const gnMaxIntP1 = makeGN( stringMagIncr( makeString(LLONG_MAX) ) );
    EXPECT_DOUBLE_EQ( static_cast<long double>(LLONG_MAX)+1,
            gnMaxIntP1.as_long_double() );
    auto const gnMinIntP1 = makeGN( stringMagIncr( makeString(LLONG_MIN) ) );
    EXPECT_DOUBLE_EQ( static_cast<long double>(LLONG_MIN)-1,
            gnMinIntP1.as_long_double() );
    auto const gnMaxUIntP1 = makeGN( stringMagIncr( makeString(ULLONG_MAX) ) );
    EXPECT_DOUBLE_EQ( static_cast<long double>(ULLONG_MAX)+1,
            gnMaxUIntP1.as_long_double() );

    auto const gnFPZero1 = makeGN( "0.0" );
    EXPECT_DOUBLE_EQ( 0, gnFPZero1.as_long_double() );
    auto const gnFPZero2 = makeGN( "0e0" );
    EXPECT_DOUBLE_EQ( 0, gnFPZero2.as_long_double() );
    auto const gnFPOne1 = makeGN( "1.0" );
    EXPECT_DOUBLE_EQ( 1, gnFPOne1.as_long_double() );
    auto const gnFPOne2 = makeGN( "1e0" );
    EXPECT_DOUBLE_EQ( 1, gnFPOne2.as_long_double() );
    auto const gnFPNeg1 = makeGN( "-9.9" );
    EXPECT_DOUBLE_EQ( -9.9, gnFPNeg1.as_long_double() );
    auto const gnFPNeg2 = makeGN( "-123.45" );
    EXPECT_DOUBLE_EQ( -123.45, gnFPNeg2.as_long_double() );
    auto const gnFPNeg3 = makeGN( "-123.45e-1" );
    EXPECT_DOUBLE_EQ( -123.45e-1, gnFPNeg3.as_long_double() );
    auto const gnFPNeg4 = makeGN( "-1234.5e2" );
    EXPECT_DOUBLE_EQ( -1234.5e2, gnFPNeg4.as_long_double() );
    auto const gnFPNeg5 = makeGN( "-12.345e-4" );
    EXPECT_DOUBLE_EQ( -12.345e-4, gnFPNeg5.as_long_double() );
    auto const gnFPPos1 = makeGN( "9.9" );
    EXPECT_DOUBLE_EQ( 9.9, gnFPPos1.as_long_double() );
    auto const gnFPPos2 = makeGN( "123.45" );
    EXPECT_DOUBLE_EQ( 123.45, gnFPPos2.as_long_double() );
    auto const gnFPPos3 = makeGN( "123.45e-1" );
    EXPECT_DOUBLE_EQ( 123.45e-1, gnFPPos3.as_long_double() );
    auto const gnFPPos4 = makeGN( "1234.5e2" );
    EXPECT_DOUBLE_EQ( 1234.5e2, gnFPPos4.as_long_double() );
    auto const gnFPPos5 = makeGN( "12.345e-4" );
    EXPECT_DOUBLE_EQ( 12.345e-4, gnFPPos5.as_long_double() );
}

TEST(JsrlGeneralNumber,Comparisons) {
#define BV( S ) { __LINE__, S, makeGN(S) }
    auto valMx
            = vector<vector<tuple<unsigned, string, GeneralNumber> > >{ {
                    BV( stringMagIncr( makeString(LLONG_MIN) ) ),
                }, {
                    BV( makeString(LLONG_MIN) ),
                }, {
                    BV( "-1234.5e2" ),
                }, {
                    BV( "-123.45" ),
                }, {
                    BV( "-123.45e-1" ),
                }, {
                    BV( "-9.9" ),
                }, {
                    BV( "-12.345e-4" ),
                }, {
                    BV( "0" ),
                }, {
                    BV( "0.0" ),
                    BV( "0e0" ),
                }, {
                    BV( "12.345e-4" ),
                }, {
                    BV( "1.0" ),
                    BV( "1e0" ),
                }, {
                    BV( "9.9" ),
                }, {
                    BV( "123.45e-1" ),
                }, {
                    BV( "123.45" ),
                }, {
                    BV( "1234.5e2" ),
                }, {
                    BV( makeString(LLONG_MAX) ),
                }, {
                    BV( stringMagIncr( makeString(LLONG_MAX) ) ),
                }, {
                    BV( makeString(ULLONG_MAX) ),
                }, {
                    BV( stringMagIncr( makeString(ULLONG_MAX) ) ),
                } };
#undef BV
    for ( auto list1 = begin(valMx); list1!=end(valMx); ++list1 ) {
        for ( auto list2 = begin(valMx); list2!=end(valMx); ++list2 ) {
            for ( auto &&el1 : *list1 ) {
                SCOPED_TRACE( "Left operand from line "
                        + makeString(get<0>(el1))
                        + ": \"" + makeString(get<1>(el1)) + "\" ("
                        + makeString(get<2>(el1)) + ")" );
                for ( auto &&el2 : *list2 ) {
                    SCOPED_TRACE( "Right operand from line "
                            + makeString(get<0>(el2))
                            + ": \"" + makeString(get<1>(el2)) + "\" ("
                            + makeString(get<2>(el2)) + ")" );
                    if ( list1 < list2 ) {
                        EXPECT_TRUE( get<2>(el1) != get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) == get<2>(el2) );
                        EXPECT_TRUE( get<2>(el1) < get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) >= get<2>(el2) );
                        EXPECT_TRUE( get<2>(el1) <= get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) > get<2>(el2) );
                    } else if ( list2 < list1 ) {
                        EXPECT_TRUE( get<2>(el1) != get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) == get<2>(el2) );
                        EXPECT_TRUE( get<2>(el1) >= get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) < get<2>(el2) );
                        EXPECT_TRUE( get<2>(el1) > get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) <= get<2>(el2) );
                    } else {
                        EXPECT_TRUE( get<2>(el1) == get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) != get<2>(el2) );
                        EXPECT_TRUE( get<2>(el1) >= get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) < get<2>(el2) );
                        EXPECT_TRUE( get<2>(el1) <= get<2>(el2) );
                        EXPECT_FALSE( get<2>(el1) > get<2>(el2) );
                    }
                }
            }
        }
    }
}

namespace {
    template<typename R>
    auto range2string( R r ) -> string {
        return string( begin(r), end(r) );
    }
}

TEST(JsrlGeneralNumber,Properties) {
    {
        auto gn = makeGN( "123450" );
        EXPECT_FALSE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_FALSE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "-123450" );
        EXPECT_FALSE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_TRUE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "1.2345e5" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_FALSE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "-1.2345e5" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_TRUE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "12345e1" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_FALSE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "-12345e1" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_TRUE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "1234500e-1" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_FALSE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "-1234500e-1" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_TRUE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( 6, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "12345000e-20" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_FALSE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( -12, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
    {
        auto gn = makeGN( "-12345000e-20" );
        EXPECT_TRUE( gn.is_decimal() ) << "gn: " << gn;
        EXPECT_TRUE( gn.is_negative() ) << "gn: " << gn;
        EXPECT_EQ( -12, gn.exponent() ) << "gn: " << gn;
        EXPECT_EQ( "12345", range2string( gn.digits() ) ) << "gn: " << gn;
    }
}

// vi: et ts=4 sts=4 sw=4
