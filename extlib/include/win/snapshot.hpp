#pragma once

// clang-format off
#include "win/kernel32.hpp"
// clang-format on

#include <TlHelp32.h>

namespace extlib::win
{
    /// <summary>
    /// The kind of snapshot we can take.
    /// </summary>
    enum class snapshot_kind : std::uint32_t
    {
        /// <summary>
        /// Includes all heaps of the process specified in th32ProcessID in the snapshot.
        /// </summary>
        heaplist_t,

        /// <summary>
        /// Includes all modules of the process specified in th32ProcessID in the snapshot.
        /// </summary>
        module_t,

        /// <summary>
        /// Includes all 32-bit modules of the process specified in th32ProcessID in the snapshot when called from a 64-bit
        /// process.
        /// </summary>
        module32_t,

        /// <summary>
        /// Includes all processes in the system in the snapshot.
        /// </summary>
        process_t,

        /// <summary>
        /// Includes all threads in the system in the snapshot.
        /// </summary>
        thread_t
    };

    /// <summary>
    /// Represents a basic snapshot created by the tool help library.
    /// </summary>
    template< snapshot_kind T >
    class snapshot final
    {
        std::shared_ptr< handle_t > handle;

       public:
        /// <summary>
        /// The iterator class (implementation varies).
        /// </summary>
        class iterator;

        /// <summary>
        /// Creates a new snapshot using an optional process identifier.
        /// </summary>
        /// <param name="id">The identifier.</param>
        /// <returns>The snapshot</returns>
        static result_t< snapshot< T > > get( std::uint32_t id = 0 );

        /// <summary>
        /// Creates a new snapshot from a snapshot handle.
        /// </summary>
        /// <param name="handle">The handle of the snapshot.</param>
        snapshot( std::shared_ptr< handle_t > handle ) : handle( handle )
        {
        }

        /// <summary>
        /// Returns the iterator for the beginning of the snapshot.
        /// </summary>
        /// <returns>Iterator.</returns>
        iterator begin(){};

        /// <summary>
        /// Returns the end of the iterator.
        /// </summary>
        /// <returns>Iterator.</returns>
        iterator end(){};
    };

    template< snapshot_kind T >
    inline result_t< snapshot< T > > snapshot< T >::get( std::uint32_t id )
    {
        return create_toolhelp32_snapshot( static_cast< std::uint32_t >( T ), id );
    }

    /// <summary>
    /// Specialization of the snapshot class for processes.
    /// </summary>
    template<>
    class snapshot< snapshot_kind::process_t >::iterator
    {
        std::shared_ptr< handle_t > handle;
        PROCESSENTRY32 entry;

       public:
        /// <summary>
        /// Creates a new iterator for the snapshot calss.
        /// </summary>
        /// <param name="handle">The handle to the snapshot.</param>
        /// <param name="first">If this is the first call.</param>
        explicit iterator( std::shared_ptr< handle_t > handle, bool first );
    };

}  // namespace extlib::win