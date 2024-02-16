#include "process.hpp"

#include "win/snapshot.hpp"

namespace extlib
{
    std::vector< std::unique_ptr< process > > process::get_all( std::string_view name )
    {
        std::vector< std::unique_ptr< process > > processes;

        for ( const auto& entry : win::snapshot< win::snapshot_kind::process_t >::get() )
        {
            if ( entry.szExeFile == name )
                processes.emplace_back( new process{ entry } );
        }

        return processes;
    }

    std::unique_ptr< process > process::get( std::string_view name )
    {
        if ( auto processes = get_all( name ); !processes.empty() )
            return std::move( processes.front() );

        return nullptr;
    }

    process::process( const PROCESSENTRY32& entry ) : name( entry.szExeFile ), id( entry.th32ProcessID )
    {
        for ( const auto& entry : win::snapshot< win::snapshot_kind::module_t >::get( id ) )
        {
            std::cout << entry.szModule << '\n';
        }
    }
}  // namespace extlib