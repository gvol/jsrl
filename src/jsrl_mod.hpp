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
#ifndef JSRL_MOD_HPP_25C1317D252EE5C5A9DBCC7F3EC0FF26
#define JSRL_MOD_HPP_25C1317D252EE5C5A9DBCC7F3EC0FF26

#include "jsrl.hpp"

#include <cassert>
#include <cstddef>
#include <set>
#include <string>
#include <string_view>
#include <utility>

namespace jsrl::jmod_impl {
    using std::set;
    using std::string_view;
    using std::string;
    using std::size_t;

    template<typename P>
    struct JModArrayPoint;
    template<typename P>
    struct JModObjectPoint;

    /*! @brief  Json modification proxy (base class).
     *
     *  This class and its derived classes provide proxies
     *  for emulating mutability to Json objects.
     *
     *  @c JMod expressions (including derived classes) give a formula
     *  for emulating modification of Json objects
     *  by assigning over the original Json variable
     *  with a new value computed to reflect the modification requested.
     *
     *  Example:
     *  @code
     *      auto json = R"({"a":[1,2,3],"o":{}})"_Json;
     *      mod(json)["a"][2]=123;
     *      mod(json)["o"]["foo"]="bar";
     *      cout << json << "\n";
     *      //  Expected output:
     *      //  {"a":[1,2,123],"o":{"foo":"bar"}}
     *  @endcode
     *
     *  @sa mod
     *  @sa JModVariable
     *  @sa JModArrayPoint
     *  @sa JModObjectPoint
     */
    template<
            typename D //!< CRTP parameter.
            >
    struct JMod {
    protected:
        ~JMod()=default;
        JMod(JMod const&)=default;
        JMod()=default;
        // These objects are immutable proxies without value semantics:
        JMod&operator=(JMod const&)=delete;
    public:

        /*! @brief  Do the emulated assignment.
         *
         *  This function replaces the original Json value
         *  to reflect the requested alteration.
         */
        auto operator=( Json newValue ) const -> JMod const &
        {
            p_derived().assign( std::move(newValue) );
            return *this;
        }

        /*! @brief  Get a proxy referring to an object-key JSON sub-component.
         *
         *  It is valid to use a @c key that doesn't exist
         *  in the currently referred object.
         *  If it's assigned to, the key will be created.
         *  However, if the currently referred JSON component isn't an object,
         *  attempting to perform an assignment will fail with an exception.
         */
        auto operator[]( string_view key ) const -> JModObjectPoint<D>;

        /*! @brief  Get a proxy referring to an array-index JSON sub-component.
         *
         *  It is valid to use an @c index that is beyond the end of the array.
         *  If it is assigned to, the array will be extended
         *  (with @c null elements).
         *  However, if the currently referred JSON component isn't an array,
         *  attempting to perform an assignment will fail with an exception.
         */
        auto operator[]( size_t index ) const -> JModArrayPoint<D>;

        /*! @brief  Append one item to the array (virtually).
         *
         *  The currently referrenced JSON component must be an array,
         *  or an exception will be thrown.
         */
        void push_back( Json value )
        {
            operator[]( p_derived().currentValue().as_array().size() )
                    = std::move(value);
        }

        /*! @brief  Remove items at the given indexes from the array.
         *
         *  The currently referrenced JSON component must be an array,
         *  or an exception will be thrown.
         */
        void erase_indexes( set<size_t> const &indexes ) const
        {
            auto curIndex = begin(indexes);
            auto const endIndex = end(indexes);
            return erase_indexes_if( [&]( auto index, auto ) {
                if ( curIndex == endIndex )
                    return false;
                if ( *curIndex == index ) {
                    ++curIndex;
                    return true;
                }
                assert( index < *curIndex );
                return false;
            } );
        }

        /*! @brief  Remove items with the given keys from the object.
         *
         *  The currently referrenced JSON component must be an object,
         *  or an exception will be thrown.
         */
        void erase_keys( set<string> const &keys ) const
        {
            auto curKey = begin(keys);
            auto const endKey = end(keys);
            return erase_keys_if( [&]( auto const &key, auto ) {
                while ( curKey != endKey && *curKey < key )
                    ++curKey;
                return curKey != endKey && *curKey == key;
            } );
        }

        /*! @brief  Remove items from an array based on a predicate.
         *
         *  Each index and element in the array
         *  is passed to the given predicate function.
         *  If the function returns @c true,
         *  the element is removed from the array.
         *
         *  The currently referrenced JSON component must be an array,
         *  or an exception will be thrown.
         */
        template<typename F>
        void erase_indexes_if( F &&f ) const
        {
            using jsrl::push_back;
            auto newBody = Json::ArrayBody{};
            auto &&oldValue = p_derived().currentValue().as_array();
            for ( auto i = size_t{}; i != oldValue.size(); ++i ) {
                auto &&e = oldValue[i];
                if ( ! f( i, e ) )
                    push_back( newBody, e );
            }
            p_derived().assign( std::move(newBody) );
        }

        /*! @brief  Remove items from an object based on a predicate.
         *
         *  Each key and value in the object
         *  is passed to the given predicate function.
         *  If the function returns @c true,
         *  the pair is removed from the object.
         *
         *  The currently referrenced JSON component must be an object,
         *  or an exception will be thrown.
         */
        template<typename F>
        void erase_keys_if( F &&f ) const
        {
            using jsrl::insert;
            auto newBody = Json::ObjectBody{};
            for ( auto &&[k, v] : p_derived().currentValue().as_object() ) {
                if ( ! f( k, v ) )
                    insert( newBody, k, v );
            }
            p_derived().assign( std::move(newBody) );
        }

        /*! @brief  Insert or replace multiple keys.
         */
        void assign_keys( Json::ObjectBody const &newKeys )
        {
            using jsrl::insert;
            auto newBody = p_derived().currentValue().as_object();
            for ( auto &&[k, v] : newKeys )
                insert( newBody, k, v );
            p_derived().assign( std::move(newBody) );
        }

    private:

        auto p_derived() const -> D const &
        {
            return *static_cast<D const*>(this);
        }

    };

    /*! @brief  Proxy for a top-level JSON entity.
     */
    struct JModVariable : JMod<JModVariable> {

        using JMod::operator=;

        /*! @brief  Create proxy for a top-level JSON entity.
         *
         *  The passed-in entity is the JSON that will be changed
         *  when the alteration operation is ultimately performed
         *  (even through sub-proxies obtained by operator[]).
         */
        explicit JModVariable( Json &variable )
            : m_v(variable)
        { }

        /*! @brief  Set the original Json.
         */
        void assign( Json &&newValue ) const
        {
            m_v = std::move(newValue);
        }

        /*! @brief  Access the current value of the referenced JSON element.
         */
        auto currentValue() const -> Json const &
        {
            return m_v;
        }

    private:

        Json &m_v;

    };

    /*! @brief  Proxy referring to an index point in a sub-array JSON entity.
     */
    template<
            typename P //!< Parent expression type.
            >
    struct JModArrayPoint : JMod< JModArrayPoint<P> > {

        using JMod<JModArrayPoint>::operator=;

        /*! @brief  Refer to an array index point inside a parent object.
         */
        explicit
        JModArrayPoint( P const &parent, size_t index )
            : m_parent( parent )
            , m_index( index )
        { }

        /*! @brief  Remove the element from the referenced array at this point.
         */
        void erase() const
        {
            m_parent.erase_indexes_if( [this]( auto k, auto && ) {
                return k == m_index;
            } );
        }

        /*! @brief  Remove multiple elements from the referenced array.
         */
        void erase_count( size_t count = size_t(-1) ) const
        {
            return m_parent.erase_indexes_if( [&]( auto index, auto && ) {
                return index >= m_index && index - m_index < count;
            } );
        }

        /*! @brief  Splice a value into the referenced array at this point.
         */
        void insert_at( Json value ) const
        {
            return insert_all_at( { value } );
        }

        /*! @brief  Splice multiple values into the referenced array.
         */
        void insert_all_at( Json::ArrayBody const &elements ) const
        {
            using jsrl::push_back;
            auto &&oldBody = m_parent.currentValue().as_array();
            auto newBody = Json::ArrayBody{};
            if ( m_index < oldBody.size() ) {
                newBody.reserve( oldBody.size() + newBody.size() );
                for ( auto i = size_t{} ; i != m_index ; ++i )
                    push_back( newBody, oldBody[i] );
                for ( auto &&e : elements )
                    push_back( newBody, e );
                for ( auto i = m_index ; i != oldBody.size() ; ++i )
                    push_back( newBody, oldBody[i] );
            } else {
                newBody = oldBody;
                newBody.resize( m_index );
                for ( auto &&e : elements )
                    push_back( newBody, e );
            }
            m_parent.assign( std::move(newBody) );
        }

        /*! @brief  Set the referenced array element in the original Json.
         */
        void assign( Json &&newValue ) const
        {
            auto newBody = m_parent.currentValue().as_array();
            if ( m_index < newBody.size() ) {
                newBody[m_index] = std::move(newValue);
            } else {
                newBody.resize( m_index );
                newBody.emplace_back( std::move(newValue) );
            }
            m_parent.assign( std::move(newBody) );
        }

        /*! @brief  Access the current value of the referenced JSON element.
         */
        auto currentValue() const -> Json const &
        {
            return m_parent.currentValue()[ m_index ];
        }

    private:

        P const m_parent;
        size_t const m_index;

    };

    /*! @brief  Proxy referring to a key in a sub-object JSON entity.
     */
    template<
            typename P //!< Parent expression type.
            >
    struct JModObjectPoint : JMod< JModObjectPoint<P> > {

        using JMod<JModObjectPoint>::operator=;

        /*! @brief  Refer to an object key in the object parent entity.
         */
        explicit
        JModObjectPoint( P const &parent, string_view key )
            : m_parent( parent )
            , m_key( key )
        { }

        /*! @brief  Remove this key association.
         */
        void erase() const
        {
            m_parent.erase_keys_if( [this]( auto &&k, auto && ) {
                return k == m_key;
            } );
        }

        /*! @brief  Set the referenced object element in the original Json.
         */
        void assign( Json &&newValue ) const
        {
            using jsrl::insert;
            auto newBody = m_parent.currentValue().as_object();
            insert( newBody, string(m_key), newValue );
            m_parent.assign( std::move(newBody) );
        }

        /*! @brief  Access the current value of the referenced JSON element.
         */
        auto currentValue() const -> Json const &
        {
            return m_parent.currentValue()[ string(m_key) ];
        }

    private:

        P const m_parent;
        string_view const m_key;

    };

    template<typename D>
    auto JMod<D>::operator[]( string_view key ) const -> JModObjectPoint<D>
    {
        return JModObjectPoint<D>( p_derived(), key );
    }

    template<typename D>
    auto JMod<D>::operator[]( size_t index ) const -> JModArrayPoint<D>
    {
        return JModArrayPoint<D>( p_derived(), index );
    }

    /*! @brief  Get a modification emulation proxy.
     */
    inline
    auto mod(
            Json &variable  //!<[in,out] Variable for proxy to modify.
            ) -> JModVariable
    {
        return JModVariable( variable );
    }

}
namespace jsrl {
    using jmod_impl::mod;
}
#endif
// vi: et ts=4 sts=4 sw=4
