#include "process.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>

namespace extlib
{
    std::vector< std::unique_ptr< process > > process::get_all_by_name( const std::string_view name )
    {
        std::vector< std::unique_ptr< process > > processes;

        for ( const auto id : get_ids_from_name( name ) )
        {
            auto handle = win::ptapi::open_process( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, id );

            const auto old_size = processes.size();

            for ( const auto& mod : win::psapi::enum_process_modules( handle, win::module_filter_flag::list_all ) )
            {
                const auto& base_name = win::psapi::get_module_base_name( handle, mod );

                if ( base_name == name.data() )
                {
                    processes.emplace_back( new process{ std::move( handle ), mod, name.data() } );
                    goto after_close;
                }
            }

            win::ptapi::close_handle( std::move( handle ) );

        // Adding labels is terrible practice, but it was the best I could do for optimization.
        after_close:
        }

        return processes;
    }

    std::vector< std::uint64_t > process::get_ids_from_name( const std::string_view name )
    {
        std::vector< std::uint64_t > ids;

        for ( const auto id : win::psapi::enum_processes() )
        {
            std::unique_ptr< win::handle_t > handle;

            if ( !win::ptapi::try_open_process( handle, PROCESS_QUERY_INFORMATION, false, id ) )
                continue;

            if ( handle->path().filename().string() == name.data() )
                ids.push_back( id );

            win::ptapi::close_handle( std::move( handle ) );
        }

        return ids;
    }

    bool process::is_64bit() const
    {
        return main_module.kind == win::pe_kind_t::pe64;
    }

    win::module_t process::get_module_by_name( std::unique_ptr< win::handle_t >& handle, const std::string_view name )
    {
        for ( const auto& mod : win::psapi::enum_process_modules( handle, win::module_filter_flag::list_all ) )
        {
            const auto& base_name = win::psapi::get_module_base_name( handle, mod );

            if ( base_name == name.data() )
                return mod;
        }

        std::stringstream msg;
        msg << "Failed to find module with name " << name.data();

        throw std::runtime_error( msg.str() );
    }

    void process::close()
    {
        if ( !is_dead && handle->is_valid() )
            win::ptapi::close_handle( std::move( handle ) );

        is_dead = true;
    }

    std::vector< win::module_t > process::get_modules() const
    {
        return win::psapi::enum_process_modules( handle, win::module_filter_flag::list_all );
    }
}  // namespace extlib