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
#ifndef JSRL_IMPL_UTIL_HPP_B8F82B7DC8889D7E8E015CDB9103EB8F
#define JSRL_IMPL_UTIL_HPP_B8F82B7DC8889D7E8E015CDB9103EB8F
#include <streambuf>
#include <vector>
#include <string>
#include <cassert>

namespace jsrl {
    using std::streambuf;
    using std::vector;
    using std::string;

    struct jsrl_streambuf : streambuf {
        explicit
        jsrl_streambuf( char const *start, char const *finish )
        {
            assert( start <= finish );
            char *const begin = const_cast<char*>(start),
                 *const end = begin + (finish-start);
            setg( begin, begin, end );
        }
    };

    /*! @brief  Reusable buffer for read_json_string_value to build a result. */
    struct StringPackager {
        struct Make {
            ~Make() {
                assert(m_sp);
                m_sp->m_buffer.clear();
            }
            explicit
            Make( StringPackager &sp )
                : m_sp( &sp )
            {
                assert( m_sp->m_buffer.empty() );
            }
            void add_byte( char c ) {
                m_sp->m_buffer.push_back( c );
            }
            string package() {
                return string(
                        m_sp->m_buffer.begin(),
                        m_sp->m_buffer.end() );
            }
        private:
            //Noncopyable:
            Make(Make const&);
            Make&operator=(Make const&);

            StringPackager *m_sp;
        };
    private:
        vector<char> m_buffer;
    };

    /*! @brief Read a json string from a streambuf.
     *
     *  @throw Json::UnexpectedByteParseError Input is not a valid json string.
     */
    string read_json_string_value(
            streambuf &sbuf, //!<[in] The buffer to read the json string out of.
            StringPackager &sp //!<[in] Reusable buffer
            );

    int jsrl_get_nonspace_byte( streambuf &sbuf );

}
#endif
// vi: et ts=4 sts=4 sw=4
