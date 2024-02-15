#include "win/kernel32.hpp"

#include <Psapi.h>

namespace extlib::win
{
    result_t< std::shared_ptr< handle_t > > open_process( std::uint32_t access, bool inherit, std::uint32_t id )
    {
        if ( HANDLE hProcess = OpenProcess( access, inherit, id ); hProcess != INVALID_HANDLE_VALUE )
            return handle_t::get( hProcess );

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
            return handle_t::get( hSnapshot );

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

    result_t< THREADENTRY32 > thread32_first( std::shared_ptr< handle_t > handle )
    {
        THREADENTRY32 Entry{};

        Entry.dwSize = sizeof( THREADENTRY32 );

        if ( Thread32First( handle->raw(), &Entry ) )
            return Entry;

        return std::unexpected( error_t::get_last() );
    }

    result_t< THREADENTRY32 > thread32_next( std::shared_ptr< handle_t > handle )
    {
        THREADENTRY32 Entry{};

        Entry.dwSize = sizeof( THREADENTRY32 );

        if ( Thread32Next( handle->raw(), &Entry ) )
            return Entry;

        return std::unexpected( error_t::get_last() );
    }
}  // namespace extlib::win