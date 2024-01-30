#pragma once

// clang-format off
#include "win.hpp"
// clang-format on

#include <Psapi.h>

#include <vector>
#include <string>

namespace extlib::win
{
    // Filter flags for process module enumeration.
    enum class module_filter_flag : std::uint64_t;

    /// <summary>
    /// Wrapper for the windows process status API.
    /// </summary>
    class psapi
    {
       public:
        /// <summary>
        /// Retrieves the process identifier for each process object in the system.
        /// </summary>
        /// <returns>A pointer to an array that receives the list of process identifiers.</returns>
        static std::vector< std::uint64_t > enum_processes();

        /// <summary>
        /// Retrieves a handle for each module in the specified process that meets the specified filter criteria.
        /// </summary>
        /// <param name="handle">A handle to the process.</param>
        /// <param name="filter">The filter criteria.</param>
        /// <returns>An array that receives the list of modules.</returns>
        static std::vector< module_t > enum_process_modules( const handle_t& handle, module_filter_flag filter );

        /// <summary>
        /// Attempts to retrieve a handle for each module in the specified process that meets the specified filter criteria.
        /// </summary>
        /// <param name="handle">A handle to the process.</param>
        /// <param name="filter">The filter criteria.</param>
        /// <returns>An array that receives the list of modules.</returns>
        static std::vector< module_t > try_enum_process_modules( const handle_t& handle, module_filter_flag filter );

        /// <summary>
        /// Retrieves the base name of the specified module.
        /// </summary>
        /// <param name="handle">A handle to the process that contains the module.</param>
        /// <param name="module">A handle to the module.</param>
        /// <returns>The base name of the module.</returns>
        static std::string get_module_base_name( const handle_t& handle, const module_t& module );

        /// <summary>
        /// Retrieves the fully qualified path for the file containing the specified module.
        /// </summary>
        /// <param name="handle">A handle to the process that contains the module.</param>
        /// <param name="module">A handle to the module. If this parameter is NULL, GetModuleFileNameEx returns the path of the executable file of the process specified in hProcess.</param>
        /// <returns>Fully qualified path to the module.</returns>
        static std::string get_module_file_name( const handle_t& handle, const module_t& module );

        /// <summary>
        /// Retrieves information about the specified module in the MODULEINFO structure.
        /// </summary>
        /// <param name="handle">A handle to the process that contains the module.</param>
        /// <param name="module">A handle to the module.</param>
        /// <returns>The MODULEINFO structure that receives information about the module.</returns>
        static MODULEINFO get_module_information( const handle_t& handle, const module_t& module );

       private:
    };

    /// <summary>
    /// Filter flags for process module enumeration.
    /// </summary>
    enum class module_filter_flag : std::uint64_t
    {
        /// <summary>
        /// List the 32-bit modules.
        /// </summary>
        list_32bit = 0x01,

        /// <summary>
        /// List the 64-bit modules.
        /// </summary>
        list_64bit = 0x02,

        /// <summary>
        /// List all modules.
        /// </summary>
        list_all = 0x03,

        /// <summary>
        /// Use the default behavior.
        /// </summary>
        list_default = 0x0
    };

}  // namespace extlib::win