#include "process.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>

namespace extlib
{
    std::vector< std::shared_ptr< process > > process::get_all_by_name( const std::string_view name )
    {
        std::vector< std::shared_ptr< process > > processes;

        for ( const auto id : win::psapi::enum_processes() )
        {
            win::handle_t handle;

            if ( !win::ptapi::try_open_process( &handle, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, id ) )
                continue;

            if ( handle.path().filename().string() != name.data() )
            {
                win::ptapi::close_handle( handle );
                continue;
            }

            const auto old_size = processes.size();

            for ( const auto& mod : win::psapi::enum_process_modules( handle, win::module_filter_flag::list_all ) )
            {
                const auto& base_name = win::psapi::get_module_base_name( handle, mod );

                if ( base_name == name.data() )
                {
                    processes.emplace_back( new process{ handle, mod, name.data() } );
                    break;
                }
            }

            if ( old_size == processes.size() )
                win::ptapi::close_handle( handle );
        }

        return processes;
    }

    bool process::is_64bit() const
    {
        return main_module.kind == win::pe_kind_t::pe64;
    }

    win::module_t process::get_module_by_name( const win::handle_t& handle, const std::string_view name )
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
        if ( !is_dead && handle.is_valid() )
            win::ptapi::close_handle( handle );

        is_dead = true;
    }

    std::vector< win::module_t > process::get_modules() const
    {
        return win::psapi::enum_process_modules( handle, win::module_filter_flag::list_all );
    }
}  // namespace extlib