#include "win/snapshot.hpp"

namespace extlib::win
{
    void snapshot< snapshot_kind::process_t >::iterator::update_or_throw( result_t< PROCESSENTRY32 > result )
    {
        if ( !result.has_value() )
        {
            if ( result.error().code != ERROR_NO_MORE_FILES )
                throw std::runtime_error( std::format( "Failed to get next entry: {}", result.error().what() ) );

            done = true;
        }
        else
            entry = result.value();
    }

    extlib::win::snapshot< snapshot_kind::process_t >::iterator::iterator( std::shared_ptr< handle_t > handle, bool end )
        : done( end ),
          handle( handle )
    {
        if ( !end )
        {
            entry.dwSize = sizeof( PROCESSENTRY32 );

            const auto result = process32_first( handle );

            update_or_throw( result );
        }
    }

    PROCESSENTRY32& snapshot< snapshot_kind::process_t >::iterator::operator*()
    {
        return entry;
    }

    PROCESSENTRY32* snapshot< snapshot_kind::process_t >::iterator::operator->()
    {
        return &entry;
    }

    snapshot< snapshot_kind::process_t >::iterator& snapshot< snapshot_kind::process_t >::iterator::operator++()
    {
        const auto result = process32_next( handle );

        update_or_throw( result );

        return *this;
    }

    bool snapshot< snapshot_kind::process_t >::iterator::operator==( const iterator& other ) const
    {
        return done == other.done;
    }

    bool snapshot< snapshot_kind::process_t >::iterator::operator!=( const iterator& other ) const
    {
        return !( *this == other );
    }

    template<>
    snapshot< snapshot_kind::process_t >::iterator snapshot< snapshot_kind::process_t >::begin()
    {
        return iterator( handle, false );
    }

    template<>
    snapshot< snapshot_kind::process_t >::iterator snapshot< snapshot_kind::process_t >::end()
    {
        return iterator( handle, true );
    }
}  // namespace extlib::win