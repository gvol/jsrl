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
#ifndef JSRL_TEST_HELPERS_HPP_730BC38B9D1F03857AE585B92C013600
#define JSRL_TEST_HELPERS_HPP_730BC38B9D1F03857AE585B92C013600

#include <sstream>
#include <iostream>
#include <string>
#include <limits>
#include <typeinfo>

namespace jsrl_test_helpers_impl {
    using std::stringstream;
    using std::cerr;
    using std::string;
    using std::numeric_limits;

    template<typename F>
    auto add_epsilon( F f ) -> F
    {
        auto const epsilon = F(numeric_limits<F>::epsilon());
        f += epsilon;
        return f;
    }

    template<typename F>
    auto through_stream( F f ) -> F
    {
        stringstream ss;
        ss.precision( numeric_limits<F>::max_digits10 );
        ss << f;
        ss >> f;
        return f;
    }

    template<typename F>
    auto test_precision_check() -> bool
    {
        auto const unity = F(1);
        auto const probe = add_epsilon( unity );
        if ( through_stream(unity) == through_stream(probe) ) {
            cerr << "\x1B[93;40mWarning:\x1B[39;1m"
                    " Floating point precision of"
                    " \"" << typeid(F).name() << "\""
                    " insufficient for test, skipping"
                    "\x1B[m\n"
                    ;
            return false;
        }
        return true;
    }
}
using jsrl_test_helpers_impl::test_precision_check;

#endif
// vi: et ts=4 sts=4 sw=4
