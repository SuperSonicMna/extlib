#include "process.hpp"

#include "win/snapshot.hpp"

namespace extlib
{
    std::unique_ptr< win::module_t > process::get_module( std::string_view name )
    {
        for ( const auto& entry : win::snapshot< win::snapshot_kind::module_t >::get( id ) )
        {
            if ( entry.szModule == name )
                return std::make_unique< win::module_t >( handle, entry );
        }

        throw win::error( "Failed to locate module by name: {}", name );
    }

    std::vector< std::unique_ptr< process > > process::get_all( std::string_view name )
    {
        std::vector< std::unique_ptr< process > > processes;

        for ( const auto& entry : win::snapshot< win::snapshot_kind::process_t >::get() )
        {
            if ( entry.szExeFile == name )
            {
                const auto result =
                    win::open_process( PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, entry.th32ProcessID );

                if ( !result.has_value() )
                    throw win::error( "OpenProcess failed: {}", result.error().what() );

                processes.emplace_back( new process{ result.value(), entry } );
            }
        }

        return processes;
    }

    std::unique_ptr< process > process::get( std::string_view name )
    {
        if ( auto processes = get_all( name ); !processes.empty() )
            return std::move( processes.front() );

        return nullptr;
    }

    process::process( std::shared_ptr< win::handle_t > handle, const PROCESSENTRY32& entry )
        : name( entry.szExeFile ),
          id( entry.th32ProcessID ),
          handle( handle ),
          main_module( get_module( entry.szExeFile ) )
    {
    }
}  // namespace extlib