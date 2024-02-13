#pragma once

// clang-format off
#include "win/windows.hpp"
// clang-format on

#include <TlHelp32.h>

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
    result< void > close_handle( handle_t* handle );

    /// <summary>
    /// Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
    /// </summary>
    /// <param name="flags">The portions of the system to be included in the snapshot.</param>
    /// <param name="id">The process identifier of the process to be included in the snapshot.</param>
    /// <returns>If the function succeeds, it returns an open handle to the specified snapshot.</returns>
    result< std::shared_ptr< handle_t > > create_toolhelp32_snapshot( std::uint32_t flags, std::uint32_t id );

}  // namespace extlib::win
