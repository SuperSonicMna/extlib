#pragma once

// clang-format off
#include "win/windows.hpp"
// clang-format on

#include <TlHelp32.h>

namespace extlib::win
{
    /// <summary>
    /// Opens an existing local process object.
    /// </summary>
    /// <param name="desired_access">The access to the process object.</param>
    /// <param name="inherit">If this value is true, processes created by this process will inherit the handle.</param>
    /// <param name="id">The identifier of the local process to be opened.</param>
    /// <returns>An open handle to the specified process.</returns>
    result_t< std::shared_ptr< handle_t > > open_process( std::uint32_t access, bool inherit, std::uint32_t id ) noexcept;

    /// <summary>
    /// Closes an open object handle.
    /// </summary>
    /// <param name="handle">A valid handle to an open object.</param>
    result_t< void > close_handle( handle_t* handle ) noexcept; 

    /// <summary>
    /// Takes a snapshot of the specified processes, as well as the heaps, modules, and threads used by these processes.
    /// </summary>
    /// <param name="flags">The portions of the system to be included in the snapshot.</param>
    /// <param name="id">The process identifier of the process to be included in the snapshot.</param>
    /// <returns>If the function succeeds, it returns an open handle to the specified snapshot.</returns>
    result_t< std::shared_ptr< handle_t > > create_toolhelp32_snapshot( std::uint32_t flags, std::uint32_t id ) noexcept;

    /// <summary>
    /// Retrieves information about the first process encountered in a system snapshot.
    /// </summary>
    /// <param name="handle">A handle to the snapshot returned from a previous call to the CreateToolhelp32Snapshot
    /// function.</param>
    /// <returns>Process information such as the name of the executable file, the process identifier, and
    /// the process identifier of the parent process.</returns>
    result_t< PROCESSENTRY32 > process32_first( std::shared_ptr< handle_t > handle ) noexcept;

    /// <summary>
    /// Retrieves information about the next process recorded in a system snapshot.
    /// </summary>
    /// <param name="handle">A handle to the snapshot returned from a previous call to the CreateToolhelp32Snapshot
    /// function.</param> <returns>Process information such as the name of the executable file, the process identifier, and
    /// the process identifier of the parent process.</returns>
    result_t< PROCESSENTRY32 > process32_next( std::shared_ptr< handle_t > handle ) noexcept;

    /// <summary>
    /// Retrieves information about the first thread encountered in a system snapshot.
    /// </summary>
    /// <param name="handle">A handle to the snapshot returned from a previous call to the CreateToolhelp32Snapshot
    /// function.</param>
    /// <returns>A thread entry.</returns>
    result_t< THREADENTRY32 > thread32_first( std::shared_ptr< handle_t > handle ) noexcept;

    /// <summary>
    /// Retrieves information about the next thread recorded in a system snapshot.
    /// </summary>
    /// <param name="handle">A handle to the snapshot returned from a previous call to the CreateToolhelp32Snapshot
    /// function.</param>
    /// <returns>A thread entry.</returns>
    result_t< THREADENTRY32 > thread32_next( std::shared_ptr< handle_t > handle ) noexcept;

    //result_t<MODULEENTRY32

}  // namespace extlib::win