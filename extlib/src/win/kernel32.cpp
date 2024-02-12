#include "win/kernel32.hpp"

#include <Psapi.h>

namespace extlib::win
{
    constexpr auto MAX_MODULE_COUNT = 512;
    constexpr auto MAX_PROCESSES_COUNT = 1024;

    result< std::vector< std::uint32_t > > enum_processes()
    {
        DWORD aProcesses[ MAX_PROCESSES_COUNT ], cbNeeded;

        // Try to call EnumProcesses. If it fails, we throw a new windows exception.
        if ( !EnumProcesses( aProcesses, sizeof( aProcesses ), &cbNeeded ) )
            return std::unexpected( error_t::get_last() );

        // Calculate the number of process identifiers returned
        std::size_t count = static_cast< std::size_t >( cbNeeded / sizeof( std::uint32_t ) );

        std::vector< std::uint32_t > ids;

        // Add all of our process identifiers into the vector. We don't want our own though.
        for ( std::size_t i = 0; i < count; ++i )
        {
            if ( aProcesses[ i ] )
                ids.push_back( aProcesses[ i ] );
        }

        return ids;
    }

    result< std::shared_ptr< handle_t > > open_process( std::uint32_t access, bool inherit, std::uint32_t id )
    {
        if ( HANDLE hProcess = OpenProcess( access, inherit, id ) )
            return std::make_shared< handle_t >( hProcess );

        return std::unexpected( error_t::get_last() );
    }

    result< void > close_handle( std::shared_ptr< handle_t > handle )
    {
        if ( !CloseHandle( handle->get() ) )
            return std::unexpected( error_t::get_last() );
    }
}  // namespace extlib::win