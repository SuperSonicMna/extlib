#include "win/psapi.hpp"

#include "win/win_exception.hpp"

namespace extlib::win
{
    std::vector< std::uint64_t > psapi::enum_processes()
    {
        DWORD aProcesses[ MAX_PROCESSES_COUNT ], cbNeeded;

        // Try to call EnumProcesses. If it fails, we throw a new windows exception.
        if ( !EnumProcesses( aProcesses, sizeof( aProcesses ), &cbNeeded ) )
            throw win_exception::from_last_error( "EnumProcesses" );

        // Calculate the number of process identifiers returned
        std::size_t count = static_cast< std::size_t >( cbNeeded / sizeof( DWORD ) );

        std::vector< std::uint64_t > ids;

        // Add all of our process identifiers into the vector. We don't want our own though.
        for ( std::size_t i = 0; i < count; ++i )
        {
            if ( aProcesses[ i ] )
                ids.push_back( aProcesses[ i ] );
        }

        return ids;
    }

    std::vector< module_t > psapi::enum_process_modules( std::unique_ptr< handle_t >& handle, module_filter_flag filter )
    {
        const auto modules = try_enum_process_modules( handle, filter );

        if ( modules.empty() )
            throw win_exception::from_last_error( "EnumProcessModulesEx" );

        return modules;
    }

    std::vector< module_t > psapi::try_enum_process_modules( std::unique_ptr< handle_t >& handle, module_filter_flag filter )
    {
        HMODULE hModules[ MAX_MODULE_COUNT ];
        DWORD cbNeeded;

        std::vector< module_t > modules;

        if ( !EnumProcessModulesEx(
                 handle->handle, hModules, sizeof( hModules ), &cbNeeded, static_cast< DWORD >( filter ) ) )
            return modules;

        std::size_t count = static_cast< std::size_t >( cbNeeded / sizeof( HMODULE ) );

        // Add all of our modules into the vector. We don't want our own though.
        for ( std::size_t i = 0; i < count; ++i )
            modules.emplace_back( handle->handle, hModules[ i ] );

        return modules;
    }

    std::string psapi::get_module_base_name( std::unique_ptr< handle_t >& handle, const module_t& module )
    {
        TCHAR szProcessName[ MAX_PATH ] = TEXT( "<unknown>" );

        if ( !GetModuleBaseName( handle->handle, module.module, szProcessName, sizeof( szProcessName ) / sizeof( TCHAR ) ) )
            throw win_exception::from_last_error( "GetModuleBaseName" );

        return szProcessName;
    }

    std::string psapi::get_module_file_name( const handle_t& handle, const module_t& module )
    {
        TCHAR szProcessName[ MAX_PATH ];

        if ( !GetModuleFileNameEx( handle.handle, module.module, szProcessName, MAX_PATH ) )
        {
            if ( GetLastError() == ERROR_ACCESS_DENIED )
                return {};

            throw win_exception::from_last_error( "GetModuleFileNameEx" );
        }

        return szProcessName;
    }

    MODULEINFO psapi::get_module_information( const handle_t& handle, const module_t& module )
    {
        MODULEINFO Info;

        if ( !GetModuleInformation( handle.handle, module.module, &Info, sizeof( Info ) ) )
            throw win_exception::from_last_error( "GetModuleInformation" );

        return Info;
    }
}  // namespace extlib::win