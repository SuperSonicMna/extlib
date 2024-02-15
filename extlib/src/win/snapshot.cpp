#include "win/snapshot.hpp"

namespace extlib::win
{
    void snapshot< snapshot_kind::process_t >::iterator::update_or_throw( result_t< PROCESSENTRY32 > result )
    {
        if ( !result.has_value() )
        {
            if ( result.error().code != ERROR_NO_MORE_FILES )
                throw std::runtime_error( std::format( "Failed to get next process entry: {}", result.error().what() ) );

            done = true;
        }
        else
            entry = result.value();
    }

    snapshot< snapshot_kind::process_t >::iterator::iterator( std::shared_ptr< handle_t > handle, bool end )
        : done( end ),
          handle( handle )
    {
        entry.dwSize = sizeof( PROCESSENTRY32 );

        if ( !end )
        {
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

    void snapshot< snapshot_kind::thread_t >::iterator::update_or_throw( result_t< THREADENTRY32 > result )
    {
        if ( !result.has_value() )
        {
            if ( result.error().code != ERROR_NO_MORE_FILES )
                throw std::runtime_error( std::format( "Failed to get next thread entry: {}", result.error().what() ) );

            done = true;
        }
        else
            entry = result.value();
    }

    snapshot< snapshot_kind::thread_t >::iterator::iterator( std::shared_ptr< handle_t > handle, bool end )
        : done( end ),
          handle( handle )
    {
        entry.dwSize = sizeof( THREADENTRY32 );

        if ( !end )
        {
            const auto result = thread32_first( handle );

            update_or_throw( result );
        }
    }

    THREADENTRY32& snapshot< snapshot_kind::thread_t >::iterator::operator*()
    {
        return entry;
    }

    THREADENTRY32* snapshot< snapshot_kind::thread_t >::iterator::operator->()
    {
        return &entry;
    }

    snapshot< snapshot_kind::thread_t >::iterator& snapshot< snapshot_kind::thread_t >::iterator::operator++()
    {
        const auto result = thread32_next( handle );

        update_or_throw( result );

        return *this;
    }

    bool snapshot< snapshot_kind::thread_t >::iterator::operator==( const iterator& other ) const
    {
        return done == other.done;
    }

    bool snapshot< snapshot_kind::thread_t >::iterator::operator!=( const iterator& other ) const
    {
        return !( *this == other );
    }

    template<>
    snapshot< snapshot_kind::thread_t >::iterator snapshot< snapshot_kind::thread_t >::begin()
    {
        return iterator( handle, false );
    }

    template<>
    snapshot< snapshot_kind::thread_t >::iterator snapshot< snapshot_kind::thread_t >::end()
    {
        return iterator( handle, true );
    }
}  // namespace extlib::win