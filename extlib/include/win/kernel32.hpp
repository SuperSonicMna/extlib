#pragma once

#include "win/windows.hpp"

namespace extlib::win
{
    /// <summary>
    /// Retrieves the process identifier for each process object in the system.
    /// </summary>
    /// <returns>A pointer to an array that receives the list of process identifiers.</returns>
    result< std::vector< std::uint32_t > > enum_processes();

    /// <summary>
    /// Opens an existing local process object.
    /// </summary>
    /// <param name="desired_access">The access to the process object.</param>
    /// <param name="inherit">If this value is true, processes created by this process will inherit the handle.</param>
    /// <param name="id">The identifier of the local process to be opened.</param>
    /// <returns>An open handle to the specified process.</returns>
    result< std::shared_ptr< handle_t > > open_process( std::uint32_t access, bool inherit, std::uint32_t id );

    /// <summary>
    /// Closes an open object handle.
    /// </summary>
    /// <param name="handle">A valid handle to an open object.</param>
    result< void > close_handle( std::shared_ptr< handle_t > handle );
}  // namespace extlib::win