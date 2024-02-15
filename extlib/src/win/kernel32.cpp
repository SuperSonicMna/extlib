#include "win/kernel32.hpp"

#include <Psapi.h>

namespace extlib::win
{
    constexpr auto MAX_MODULE_COUNT = 512;
    constexpr auto MAX_PROCESSES_COUNT = 1024;

    result_t< std::vector< std::uint32_t > > enum_processes()
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

    result_t< std::shared_ptr< handle_t > > open_process( std::uint32_t access, bool inherit, std::uint32_t id )
    {
        if ( HANDLE hProcess = OpenProcess( access, inherit, id ); hProcess != INVALID_HANDLE_VALUE )
            return std::make_shared< handle_t >( hProcess );

        return std::unexpected( error_t::get_last() );
    }

    result_t< void > close_handle( handle_t* handle )
    {
        if ( !CloseHandle( handle->raw() ) )
            return std::unexpected( error_t::get_last() );
    }

    result_t< std::shared_ptr< handle_t > > create_toolhelp32_snapshot( std::uint32_t flags, std::uint32_t id )
    {
        if ( HANDLE hSnapshot = CreateToolhelp32Snapshot( flags, id ); hSnapshot != INVALID_HANDLE_VALUE )
            return std::make_shared< handle_t >( hSnapshot );

        return std::unexpected( error_t::get_last() );
    }
    result_t< PROCESSENTRY32 > process32_first( std::shared_ptr< handle_t > handle )
    {
        PROCESSENTRY32 Entry;
        if ( Process32First( handle->raw(), &Entry ) )
            return Entry;

        return std::unexpected( error_t::get_last() );
    }

    result_t< PROCESSENTRY32 > process32_next( std::shared_ptr< handle_t > handle )
    {
        PROCESSENTRY32 Entry;
        if ( Process32Next( handle->raw(), &Entry ) )
            return Entry;

        return std::unexpected( error_t::get_last() );
    }
}  // namespace extlib::win