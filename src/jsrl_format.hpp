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
#ifndef JSRL_FORMAT_HPP_9A4E7C3B2D1F8E6A5B9C0D3F4E7A2B1C
#define JSRL_FORMAT_HPP_9A4E7C3B2D1F8E6A5B9C0D3F4E7A2B1C

#include "jsrl.hpp"
#include "jsrlpp.hpp"
#include "jsrl_general_number.hpp"

#include <format>
#include <sstream>

/*! @file jsrl_format.hpp
 *  @brief std::formatter specializations for jsrl types
 *
 *  This header provides std::formatter specializations for jsrl JSON types,
 *  enabling them to be used with std::format() and related formatting functions.
 *  All formatters produce output identical to their corresponding operator<< implementations.
 *
 *  Requires C++20 or later for std::format support.
 */

namespace std {

    /*! @brief std::formatter specialization for jsrl::Json
     *
     *  Formats Json values using their operator<< implementation,
     *  producing compact JSON output.
     */
    template <>
    struct formatter<jsrl::Json> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(jsrl::Json const& val, format_context& ctx) const {
            std::ostringstream oss;
            oss << val;
            return std::format_to(ctx.out(), "{}", oss.str());
        }
    };

    /*! @brief std::formatter specialization for jsrl::Json::OptionedWrite
     *
     *  Formats Json values with custom encoding options (loose floats, UTF-8 handling, etc.).
     */
    template <>
    struct formatter<jsrl::Json::OptionedWrite> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(jsrl::Json::OptionedWrite const& val, format_context& ctx) const {
            std::ostringstream oss;
            oss << val;
            return std::format_to(ctx.out(), "{}", oss.str());
        }
    };

    /*! @brief std::formatter specialization for jsrl::JsonPrettyPrint
     *
     *  Formats Json values with pretty-printing (indentation, newlines).
     */
    template <>
    struct formatter<jsrl::JsonPrettyPrint> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(jsrl::JsonPrettyPrint const& val, format_context& ctx) const {
            std::ostringstream oss;
            oss << val;
            return std::format_to(ctx.out(), "{}", oss.str());
        }
    };

    /*! @brief std::formatter specialization for jsrl::BoundJsonPrettyPrint
     *
     *  Formats Json values bound to a specific pretty-print configuration.
     */
    template <>
    struct formatter<jsrl::BoundJsonPrettyPrint> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(jsrl::BoundJsonPrettyPrint const& val, format_context& ctx) const {
            std::ostringstream oss;
            oss << val;
            return std::format_to(ctx.out(), "{}", oss.str());
        }
    };

    /*! @brief std::formatter specialization for jsrl::Json::Error
     *
     *  Formats Json error objects with their error tag and message.
     */
    template <>
    struct formatter<jsrl::Json::Error> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(jsrl::Json::Error const& val, format_context& ctx) const {
            std::ostringstream oss;
            oss << val;
            return std::format_to(ctx.out(), "{}", oss.str());
        }
    };

    /*! @brief std::formatter specialization for jsrl::GeneralNumber
     *
     *  Formats GeneralNumber values using their operator<< implementation.
     */
    template <>
    struct formatter<jsrl::GeneralNumber> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(jsrl::GeneralNumber const& val, format_context& ctx) const {
            std::ostringstream oss;
            oss << val;
            return std::format_to(ctx.out(), "{}", oss.str());
        }
    };

} // namespace std

#endif
